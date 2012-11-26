/*
 * c2s_service.cpp
 *
 *  Created on: 2012-10-25
 *      Author: root
 */

#include "c2s_service.h"
#include "base/logger.h"
#include "service_engine/communicator.h"
#include "base/net/acceptor.h"
#include "c2s_handler.h"

namespace c2s
{

C2SService::C2SService(const service_engine::CommandLineParser& parser)
: service_engine::ServiceT<C2SService>(parser)
{
}

C2SService::~C2SService()
{
}

void C2SService::run()
{
	service_engine::Communicator communicator;

	base::net::Acceptor acceptor(communicator.reactor());
	acceptor.open(
			getCmdline().getIntOption("listen-port"),
			-1,
			new base::net::HandlerCreatorStrategy<C2SHandler>());



	communicator.wait();


}

} /* namespace c2s */
