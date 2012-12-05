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

namespace c2s
{

//class C2SHandler : public service_engine::RpcHandler
class C2SHandler : public base::packet::PacketHandler
{
public:
	C2SHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~C2SHandler();

public:
	virtual int onOpen();
	virtual int onPacketArrive(const base::packet::Header& header, base::packet::Packet& body);
	virtual int onDisconnected();

public:
	virtual void relay() = 0;

protected:
	uint64_t conn_id_;
	RelayLine line_;

};

} /* namespace c2s */
#endif /* C2S_HANDLER_H_ */
