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
#ifndef _KETAMA_H_
#define _KETAMA_H_
#include <sys/sem.h>    /* semaphore functions and structs.     */


#define MC_SHMSIZE  524288  /*  512KB should be ample.*/
#define MAX_SERVERNAME_SIZE 22

union semun_mod
{
	int val;              /* used for SETVAL only */
	struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
	ushort *array;        /* used for GETALL and SETALL */
};

typedef int (*compfn)( const void*, const void* );

typedef struct
{
	unsigned int point;  /* point on circle*/
	char name[MAX_SERVERNAME_SIZE];
} mcs;

typedef struct
{
	char name[MAX_SERVERNAME_SIZE];
	unsigned long memory;
} serverinfo;

typedef struct
{
  serverinfo* serverinfos;   /* number of servers */
  unsigned int servernum;   /* number of servers */
  unsigned long totalmemory;  /* total of server memory */
  key_t key; /* key of ketama object */
} serverlist;

typedef struct
{
	int numpoints;
	void* data;
	void* array; /* array of mcs structs*/
} continuum;

typedef continuum* ketama_continuum;

#ifdef __cplusplus
extern "C" {
#endif

void ketama_print_continuum(ketama_continuum);

int ketama_compare(mcs*, mcs*);

char* ketama_error();

int ketama_serverlist_init(serverlist** ,key_t);

void ketama_serverlist_clear(serverlist* serverlistptr);

void ketama_serverlist_smoke(serverlist*);

int ketama_add_server(serverlist* ,const char*, unsigned int);

int ketama_roll(ketama_continuum* ,serverlist*);

mcs* ketama_get_server(ketama_continuum, const char*);

void ketama_smoke(ketama_continuum);

void ketama_smoke_serverlist(serverlist*);

#ifdef __cplusplus
}
#endif
#endif
