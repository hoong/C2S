#ifndef C2S_CLIENT_HANDLER_H_
#define C2S_CLIENT_HANDLER_H_

#include "c2s_handler.h"
#include "singleton.h"
#include "pch.h"
#include "base/net/acceptor.h"

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
		C2SHandler(fd,addr,reactor_impl,line,scheduler),uin_(0) {}

	virtual ~C2SClientHandler(){}

public:
	virtual void onOpen();
//	virtual int onPacketArrive(const base::packet::Header& header, base::packet::Packet& body);
	virtual void onDisconnected();

	uint64_t uin_;

};

/*
class C2SClientHandlerCreatorStrategy:public HandlerCreatorStrategyBase
{
public:
	C2SClientHandlerCreatorStrategy(RelayLine& line,boost::shared_ptr<RelayStubsScheduler> scheduler) :line_(line),scheduler_(scheduler) {}
	virtual ~C2SClientHandlerCreatorStrategy() {}
	virtual boost::shared_ptr<RefHandler> create(int fd, const SockAddr& addr, boost::shared_ptr<ReactorImpl> reactor_impl);

private:
	RelayLine line_;
	boost::shared_ptr<RelayStubsScheduler> scheduler_;
};
*/

class SE_API C2SClientAcceptor
{
public:
	C2SClientAcceptor(int listen_port,RelayLine line, RelayStubsScheduler* scheduler);
	virtual ~C2SClientAcceptor();

private:
	auto_ptr<base::net::Acceptor> acceptor_;
};



}


#endif
