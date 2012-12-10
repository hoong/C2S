#include "c2s_server_handler.h"
#include "relay_exception.h"


namespace c2s
{


C2SServerChannelFactory::C2SServerChannelFactory(Comunicator* communicator,RelayStubsScheduler* scheduler,RelayLine line)
{

	connector_.reset(new base::net::Connector(communicator->reactor(),
				new C2SHandlerCreatorStrategy(line,scheduler)));

}
C2SServerChannelFactory::~C2SServerChannelFactory()
{
}

boost::shared_ptr<C2SHandler> C2SServerChannelFactory::createHandler(const char* ip, int port)
{
	base::net::SockAddr sockaddr;
	if (-1 == sockaddr.set(ip, port))
		throw RelayCallException(rpc::EC_RPC_TRANSPORT_ERROR, "bad address!");
	return createHandler(sockaddr);
}

boost::shared_ptr<C2SHandler> C2SServerChannelFactory::createHandler(const base::net::SockAddr& sockaddr)
{
	boost::mutex::scoped_lock lock(mutex_);
	map_t::iterator it = handlers_map_.find(sockaddr);
	if (it != handlers_map_.end() && !it->second->isClosed())
		return it->second;

	boost::shared_ptr<base::net::RefHandler> outhandler;

	try
	{
		connector_->open(sockaddr, &outhandler);
	}
	catch (base::net::NetException& e)
	{
		THROW_RPC_CALL_EXCEPTION(rpc::EC_RPC_TRANSPORT_ERROR, e.what());
	}

	boost::shared_ptr<C2SHandler> spout = boost::dynamic_pointer_cast<C2SHandler>(outhandler);

	handlers_map_[sockaddr] = spout;
	return spout;
}

}
