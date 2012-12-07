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

//class C2SHandler : public service_engine::RpcHandler
class C2SHandler : public base::packet::PacketHandler
{
public:
	C2SHandler(int fd, 
		const base::net::SockAddr& addr, 
		boost::shared_ptr<base::net::ReactorImpl> reactor_impl,
		RelayLine& line,
		boost::shared_ptr<RelayStubsScheduler> scheduler);
	virtual ~C2SHandler();

public:
	virtual int onOpen();
	virtual int onPacketArrive(const base::packet::Header& header, base::packet::Packet& body);
	virtual int onDisconnected();

	void sendFailedResponse(const rpc::MessageBody& request_body, int err_code, const char* err_msg);
public:
	virtual void relay(service_engine::rpc::MessageBody& body) ;

protected:
	uint64_t conn_id_;
	RelayLine line_;
private:
	boost::shared_ptr<RelayStubsScheduler> scheduler_;
};

} /* namespace c2s */
#endif /* C2S_HANDLER_H_ */
