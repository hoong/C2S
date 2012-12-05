/*
 * c2s_handler.cpp
 *
 *  Created on: 2012-11-5
 *      Author: root
 */

#include "c2s_handler.h"
#include "relay_target_manager.h"

namespace c2s
{

C2SHandler::C2SHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
: base::packet::PacketHandler(fd, addr, reactor_impl),conn_id_(0),uin_(0)
{
}

C2SHandler::~C2SHandler()
{
}

int C2SHandler::onOpen()
{
	LOG(info) << "c2s handler open: " << addr() << ENDL;

	static const std::string key = "CLIENT";
	from_ = RTM::instance().get(key);



	return 0;
}

int C2SHandler::onPacketArrive(const base::packet::Header& header, base::packet::Packet& body)
{
	LOG(trace) << "c2s packet arrive: " << header << ENDL;

	return 0;
}

int C2SHandler::onDisconnected()
{
	LOG(info) << "c2s handler disconnected: " << addr() << ENDL;
	return 0;
}


} /* namespace c2s */
