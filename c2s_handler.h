/*
 * c2s_handler.h
 *
 *  Created on: 2012-11-5
 *      Author: root
 */

#ifndef C2S_HANDLER_H_
#define C2S_HANDLER_H_

#include "base/packet/packet_handler.h"
#include "rpc_handler.h"
#include "relay_target.h"
#include "relay_stub.h"

namespace c2s
{

class C2SHandler : public base::packet::PacketHandler
{
public:
	C2SHandler(int fd, 
		const base::net::SockAddr& addr, 
		boost::shared_ptr<base::net::ReactorImpl> reactor_impl,
		boost::shared_ptr<RelayStubsScheduler> scheduler);
	virtual ~C2SHandler();

public:
	virtual void onOpen();
	virtual void onPacketArrive(const base::packet::Header& header, base::packet::Packet& body);
	virtual void onDisconnected();

	void sendFailedResponse(const rpc::MessageBody& request_body, int err_code, const char* err_msg);
	uint64_t conn_id()
	{
		return conn_id_;
	}
	RelayLine line()
	{
		return line_;
	}
public:
	virtual void relay(service_engine::rpc::MessageBody& body) ;

protected:
	uint64_t conn_id_;
	RelayLine line_;
private:
	boost::shared_ptr<RelayStubsScheduler> scheduler_;
};

template<typename HANDLER=C2SHandler>
class C2SHandlerCreatorStrategy:public HandlerCreatorStrategyBase
{
public:
	C2SClientHandlerCreatorStrategy(RelayLine& line,boost::shared_ptr<RelayStubsScheduler> scheduler) :line_(line),scheduler_(scheduler) {}
	virtual ~C2SClientHandlerCreatorStrategy() {}
	virtual boost::shared_ptr<RefHandler> create(int fd, const SockAddr& addr, boost::shared_ptr<ReactorImpl> reactor_impl)
	{
		return boost::shared_ptr<base::net::RefHandler>(new HANDLER(fd,addr,reactor_impl,line_,scheduler_));
	}

private:
	RelayLine line_;
	boost::shared_ptr<RelayStubsScheduler> scheduler_;
};



} /* namespace c2s */
#endif /* C2S_HANDLER_H_ */
