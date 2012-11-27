#ifndef RELAY_TARGET_H_
#define RELAY_TARGET_H_

#include <stdint.h>
#include "c2s_handler.h"
#include <boost/shared_ptr.hpp>
#include "rpc_proto/rpc.pb.h"

namespace c2s
{

class IRelayTarget
{
public:
	virtual ~IRelayTarget();
	virtual void send(uint64_t& business_id,service_engine::rpc::MessageBody& body) = 0;
	virtual boost::shared_ptr<C2SHandler> get(uint64_t& business_id) = 0;

};

}







#endif
