#ifndef RELAY_CLIENT_H_
#define RELAY_CLIENT_H_
#include "relay_target.h"
#include "tbb/concurrent_hash_map.h"
namespace c2s
{

class RelayClient:public RelayTarget
{
public:
	virtual ~RelayClient(){};
	virtual void relay(uint64_t& conn_id,service_engine::rpc::MessageBody& body);
	virtual boost::shared_ptr<C2SHandler> relayHandler(uint64_t& conn_id);

	void registerHandler(const uint64_t& conn_id,boost::shared_ptr<C2SHandler>& handler);
	void unregisterHandler(const uint64_t& conn_id);
	boost::shared_ptr<C2SHandler> getHandler(const uint64_t& conn_id);

private:
	tbb::concurrent_hash_map<uint64_t,boost::shared_ptr<C2SHandler>,tbb::tbb_hash_compare<uint64_t> conn_map_;
};

}

#endif
