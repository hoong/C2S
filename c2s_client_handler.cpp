#include "c2s_client_handler.h"
#include "target_client.h"

namespace c2s
{

void C2SClientHandler::onOpen()
{
	CONNID_GENERATOR::instance().gen(conn_id_);
	if (line_.from_.get() )
	{
		boost::shared_ptr<C2SHandler> handler = boost::static_pointer_cast<C2SHandler>(this->shared_from_this());
		line_.from_->registerHandler(conn_id_,handler);
	}

}

void C2SClientHandler::onDisconnected()
{
	if (line_.from_.get())
	{
		line_.from_->unregisterHandler(conn_id_);
	}
}

/*
boost::shared_ptr<RefHandler> C2SClientHandlerCreatorStrategy::create(int fd, const SockAddr& addr, boost::shared_ptr<ReactorImpl> reactor_impl)
{
	return boost::shared_ptr<base::net::RefHandler>(new C2SClientHandler(fd,addr,reactor_impl,line_,scheduler_));
}
*/

C2SClientAcceptor::C2SClientAcceptor(int listen_port,RelayLine line, RelayStubsScheduler* scheduler)
{

	try
	{
		acceptor_->open(
			listen_port,
			-1,
			new C2SHandlerCreatorStrategy<C2SClientHandler>(line ,scheduler));
	}
	catch (base::net::NetException& e)
	{
		THROW_RPC_CALL_EXCEPTION(rpc::EC_RPC_TRANSPORT_ERROR, e.what());
	}
}

C2SClientAcceptor::~C2SClientAcceptor()
{
}



}
