#ifndef C2S_CLIENT_HANDLER_H_
#define C2S_CLIENT_HANDLER_H_

#include "c2s_handler.h"
#include "singleton.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <time.h>
namespace c2s
{

class IDGenerator
{
public:
	IDGenrator():count_(0);
	inline void gen(uint64_t& id)
	{
		boost::mutex::scoped_lock lock(mutex_);
		uint32_t now = time(NULL);
		id = now;
		id <<32;
		id += count_++;
	}
private:
	boost::mutex mutex_;
	uint32_t count_;
};

typedef base::Singleton<IDGenerator> CONNID_GENERATOR;




class C2SClientHandler:public C2SHandler
{
public:
	C2SClientHandler(int fd, 
		const base::net::SockAddr& addr, 
		boost::shared_ptr<base::net::ReactorImpl> reactor_impl,
		RelayLine& line,
		boost::shared_ptr<RelayStubsScheduler> scheduler):
		C2SHandler(fd,addr,reactor_impl,line,scheduler){}

	virtual ~C2SClientHandler(){}

public:
	virtual int onOpen();
//	virtual int onPacketArrive(const base::packet::Header& header, base::packet::Packet& body);
	virtual int onDisconnected();


};

}


#endif
