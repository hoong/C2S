/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 only.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>           /* for reading last time modification   */
#include <unistd.h>         /* needed for usleep                    */
#include <math.h>           /* floor & floorf                       */
#include <sys/stat.h>       /* various type definitions             */
#include <sys/shm.h>        /* shared memory functions and structs  */
#include "ketama.h"
#include "md5.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void free (void *__ptr);
extern void *malloc (size_t __size);
extern void qsort (void *__base, size_t __nmemb, size_t __size,__compar_fn_t __compar);

char k_error[255] = "";

/** \brief Locks the semaphore.
  * \param sem_set_id The semaphore handle that you want to lock. */
static void ketama_sem_lock( int sem_set_id )
{
    union semun_mod sem_val;
    sem_val.val = 2;
    semctl( sem_set_id, 0, SETVAL, sem_val );
}

/** \brief Unlocks the semaphore.
  * \param sem_set_id The semaphore handle that you want to unlock. */
static void ketama_sem_unlock( int sem_set_id )
{
    union semun_mod sem_val;
    sem_val.val = 1;
    semctl( sem_set_id, 0, SETVAL, sem_val );
}

/** \brief Initialize a semaphore.
  * \param key Semaphore key to use.
  * \return The freshly allocated semaphore handle. */
static int ketama_sem_init( key_t key )
{
	int sem_set_id;
	sem_set_id = semget( key, 1, 0 );
	if ( sem_set_id == -1 )
    {
		// create a semaphore set with ID SEM_ID
		sem_set_id = semget( key, 1, IPC_CREAT | 0666 );
		if ( sem_set_id == -1 )
        {
			strcpy( k_error, "Could not open semaphore!" );
			return 0;
        }
		ketama_sem_unlock( sem_set_id );
    }
	return sem_set_id;
}

void ketama_md5_digest(const char* inString, unsigned char md5pword[16] )
{
	md5_state_t md5state;

	md5_init( &md5state );
	md5_append( &md5state, (const md5_byte_t*)inString, strlen( inString ) );
	md5_finish( &md5state, md5pword );
}

unsigned int ketama_hashi(const char* inString )
{
	unsigned char digest[16];
	ketama_md5_digest(inString, digest);
	unsigned int ret = ( digest[3] << 24 )
			         | ( digest[2] << 16 )
			         | ( digest[1] <<  8 )
			         |   digest[0];

	return ret;
}

void ketama_print_continuum( ketama_continuum cont )
{
	int a;
	printf( "Numpoints in continuum: %d\n", cont->numpoints );

	if ( cont->array == 0 )
	{
		printf( "Continuum empty\n" );
	}
	else
	{
		mcs (*mcsarr)[cont->numpoints] = cont->array;
		for( a = 0; a < cont->numpoints; a++ )
		{
			printf( "%s (%u)\n", (*mcsarr)[a].name, (*mcsarr)[a].point );
		}
	}
}

int ketama_compare( mcs *a, mcs *b )
{
	if ( a->point < b->point )
		return -1;

	if ( a->point > b->point )
		return 1;

	return 0;
}

char* ketama_error()
{
	return k_error;
}

int ketama_serverlist_init(serverlist** serverlistptr,key_t key)
{
	strcpy( k_error, "" );

	*serverlistptr = (serverlist *)malloc(sizeof(serverlist));
	if(serverlistptr == NULL)
	{
		strcpy( k_error, "Ketama_serverlist_init failed  to malloc memory!" );
		return 0;
	}

	(*serverlistptr)->key = key;
	(*serverlistptr)->servernum = 0;
	(*serverlistptr)->totalmemory = 0;
	(*serverlistptr)->serverinfos = NULL;
	return 1;
}

void ketama_serverlist_smoke(serverlist* serverlistptr)
{
	if(serverlistptr != NULL)
	{
		free(serverlistptr);
	}
}

void ketama_serverlist_clear(serverlist* serverlistptr)
{
	if(serverlistptr != NULL)
	{
		if(serverlistptr->serverinfos != NULL)
		{
			free(serverlistptr->serverinfos);
		}
		serverlistptr->servernum = 0;
		serverlistptr->totalmemory = 0;
	}
}

int ketama_add_server(serverlist* serverlistptr,const char* servername, unsigned int memory)
{
	strcpy( k_error, "" );

	if(serverlistptr == NULL)
	{
		strcpy( k_error, "ketama_add_server failed: serverlistptr is null" );
		return 0;
	}

	serverinfo* serverinfoptr_old = serverlistptr->serverinfos;
	serverlistptr->serverinfos = (serverinfo *)malloc(sizeof(serverinfo)*(serverlistptr->servernum + 1));
	if(serverlistptr->serverinfos == NULL)
	{
		strcpy( k_error, "ketama_add_server failed: serverinfoptr_new malloc failue" );
		return 0;
	}

	if(serverlistptr->servernum > 0)
	{
		memcpy(serverlistptr->serverinfos ,serverinfoptr_old, sizeof(serverinfo)*(serverlistptr->servernum));
		free(serverinfoptr_old);
	}

	strncpy(serverlistptr->serverinfos[serverlistptr->servernum].name,servername,MAX_SERVERNAME_SIZE);
	serverlistptr->serverinfos[serverlistptr->servernum].memory = memory;

	serverlistptr->servernum += 1;
	serverlistptr->totalmemory += memory;
	return 1;
}


/** \brief Generates the continuum of servers (each server as many points on a circle).
  * \param key Shared memory key for storing the newly created continuum.
  * \param filename Server definition file, which will be parsed to create this continuum.
  * \return 0 on failure, 1 on success. */
int ketama_create_continuum( key_t key,serverinfo* slist,unsigned int numservers,unsigned long totalmemory)
{
	int shmid;
	int* data; // pointer to shmem location

	if ( numservers < 1 )
	{
		strcpy(k_error, "Can't open shmmem for writing,because numservers <1.");
		return 0;
	}

	// continuum will hold one mcs for each point on the circle:
	mcs continuum[ numservers * 160 ];
	unsigned int i;
	int k, cont = 0;

	for( i = 0; i < numservers; i++ )
	{
		float pct = (float)slist[i].memory / (float)totalmemory;
		int ks = floorf( pct * 40.0 * (float)numservers );

		for( k = 0; k < ks; k++ )
		{
			// 40 hashes, 4 numbers per hash = 160 points per server
			char ss[30];
			sprintf( ss, "%s-%d", slist[i].name, k );
			unsigned char digest[16];
			ketama_md5_digest( ss, digest );

			// use successive 4-bytes from hash as numbers
			// for the points on the circle:
			int h;
			for( h = 0; h < 4; h++ )
			{
				continuum[cont].point = ( digest[3+h*4] << 24 )
                            | ( digest[2+h*4] << 16 )
																    | ( digest[1+h*4] <<  8 )
						                   | ( digest[h*4] );
				memcpy( continuum[cont].name, slist[i].name, 22 );
				cont++;
			}
		}
	}
	// free(slist);  // 不用释放，由外部进行释放

	// sorts in ascending order of "point"
	qsort( (void*) &continuum, cont, sizeof( mcs ), (compfn)ketama_compare );

	// add data to shmmem
	shmid = shmget( key, MC_SHMSIZE, 0644 | IPC_CREAT );
	data = shmat( shmid, (void *)0, 0 );
	if ( data == (void *)(-1) )
	{
		//strcpy( k_error, "Can't open shmmem for writing." );
		snprintf( k_error,sizeof(k_error)-1, "Can't open shmmem for writing. key=0x%08x,msg=%s",key,strerror(errno) );
		return 0;
	}

	time_t modtime = 0;//file_modtime( filename );
	int nump = cont;
	memcpy( data, &nump, sizeof( int ) );
	memcpy( data + 1, &modtime, sizeof( time_t ) );
	memcpy( data + 1 + sizeof( void* ), &continuum, sizeof( mcs ) * nump );

	// we detatch here because we will re-attach in read-only
	// mode to actually use it.
	if ( shmdt( data ) == -1 )
	{
		strcpy( k_error, "Error detatching from shared memory!" );
	}

	return 1;
}

int ketama_roll(ketama_continuum* contptr,serverlist* serverlistptr)
{
	strcpy( k_error, "" );

	if(serverlistptr == NULL)
	{
		strcpy( k_error, "ketama_roll failed: serverlistptr is null" );
	}
	//key_t key;
	int shmid;
	int *data;
	int sem_set_id;

	if(contptr > (ketama_continuum*)0 && *contptr > (ketama_continuum)0)
	{
			shmdt( (*contptr)->data );
	}

  *contptr = malloc(sizeof(continuum));
	(*contptr)->numpoints = 0;
	(*contptr)->array = 0;
	(*contptr)->data = 0;

	sem_set_id = ketama_sem_init( serverlistptr->key );

	int sanity = 0;

	while ( semctl( sem_set_id, 0, GETVAL, 0 ) == 2 )
    {

		// wait for the continuum creator to finish, but don't block others
		usleep( 5 );

		// if we are waiting for > 1 second, take drastic action:
		if(++sanity > 1000000)
		{
			usleep( rand()%50000 );
			ketama_sem_unlock( sem_set_id );
			break;
        }
    }

	//shmid = shmget( serverlistptr->key, MC_SHMSIZE, 0 ); // read only attempt.
	//data = shmat( shmid, (void *)0, SHM_RDONLY );

	ketama_sem_lock( sem_set_id );
	if(!ketama_create_continuum(serverlistptr->key,serverlistptr->serverinfos,serverlistptr->servernum,serverlistptr->totalmemory) )
	{
		ketama_sem_unlock( sem_set_id );
		return 0;
	}
	ketama_sem_unlock( sem_set_id );

	shmid = shmget( serverlistptr->key, MC_SHMSIZE, 0 ); // read only attempt.
	data = shmat( shmid, (void *)0, SHM_RDONLY );

	if ( data == (void *)(-1) )
	{
		strcpy( k_error, "Failed miserably to get pointer to shmemdata!" );
		return 0;
	}

	(*contptr)->numpoints = *data;
	(*contptr)->data = data;  //data
	(*contptr)->array = data + 1 + sizeof( void* );

	return 1;
}

mcs* ketama_get_server(ketama_continuum cont ,const char* key)
{
	unsigned int h = ketama_hashi( key );
	int highp = cont->numpoints;
	mcs (*mcsarr)[cont->numpoints] = cont->array;
	int maxp = highp, lowp = 0, midp;
	unsigned int midval, midval1;

	// divide and conquer array search to find server with next biggest
	// point after what this key hashes to
	while ( 1 )
	{
		midp = (int)( ( lowp+highp ) / 2 );
		if ( midp == maxp )
        {
			if ( midp == cont->numpoints )
				midp = 1; // if at the end, roll back to zeroth

			return &( (*mcsarr)[midp-1] );
        }
		midval = (*mcsarr)[midp].point;
		midval1 = midp == 0 ? 0 : (*mcsarr)[midp-1].point;

		if ( h <= midval && h > midval1 )
			return &( (*mcsarr)[midp] );

		if ( midval < h )
			lowp = midp + 1;
		else
			highp = midp - 1;

		if ( lowp > highp )
			return &( (*mcsarr)[0] );
	}
}

void ketama_smoke( ketama_continuum contptr )
{
	if(contptr != NULL)
	{
		free( contptr );
	}
}

#ifdef __cplusplus
}
#endif
