#ifndef C2S_SERVER_HANDLER_H_
#define C2S_SERVER_HANDLER_H_

#include "c2s_handler.h"



namespace c2s
{



class SE_API C2SServerChannelFactory
{
public:
	C2SServerChannelFactory(Comunicator* communicator,RelayStubsScheduler* scheduler,RelayLine line);
	virtual ~C2SServerChannelFactory();

	boost::shared_ptr<C2SHandler> createHandler(const char* ip, int port);
	boost::shared_ptr<C2SHandler> createHandler(const base::net::SockAddr& sockaddr);

private:
	typedef map<base::net::SockAddr, boost::shared_ptr<C2SHandler> > map_t;
	map_t handlers_map_;

	boost::mutex mutex_;
	auto_ptr<base::net::Connector> connector_;
};




}


#endif
