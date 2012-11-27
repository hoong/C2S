#ifndef RELAY_CLIENT_H_
#define RELAY_CLIENT_H_
#include "relay_target.h"

namespace c2s
{

class RelayClient:public IRelayTarget
{
public:
	virtual ~RelayClient(){};
	virtual void send(uint64_t& business_id,service_engine::rpc::MessageBody& body);
	virtual boost::shared_ptr<C2SHandler> get(uint64_t& business_id);
};

}

#endif
