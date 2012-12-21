#ifndef RELAY_HANDLER_H_
#define RELAY_HANDLER_H_

#include "rpc_handler.h"
namespace relay
{

class RelayHandler:public service_engine::RpcHandler
{
	typedef service_engine::RpcHandler super;
public:

	RelayHandler(int fd,
			const base::net::SockAddr& addr,
			boost::shared_ptr<base::net::ReactorImpl> reactor_impl,
			boost::shared_ptr<RpcStubsDispatcher> stub_dispatcher,
			boost::shared_ptr<RpcCallTimeoutQueue> call_timeout_queue);
	virtual ~RelayHandler();

	virtual void onPacketArrive(
			const base::packet::Header& header, 
			base::packet::Packet& body);

	/*
	virtual void onMessage(
			const rpc::Addr&     from,
			const rpc::AddrList& targets,
			rpc::MsgList&        msg_list);
	*/

	virtual void onRelay(
			const rpc::Addr&     from,
			const rpc::AddrList& targets,
			const rpc::MsgList&        msg_list);

protected:


private:
	//判断是否本地地址
	virtual bool isLocal(const rpc::Addr& addr) = 0;


};


}

#endif
