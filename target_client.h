#ifndef TARGET_CLIENT_H_
#define TARGET_CLIENT_H_
#include "relay_target.h"
#include "tbb/concurrent_hash_map.h"
namespace c2s
{

class TargetClient:public RelayTarget
{
public:
	virtual ~TargetClient(){}
	virtual void relay(uint64_t& conn_id,service_engine::rpc::MessageBody& body);
	virtual boost::shared_ptr<C2SHandler> relayHandler(uint64_t& conn_id);
	/*
	virtual void registerHandler(std::string& ,boost::shared_ptr<C2SHandler>);
	virtual void unregisterHandler(std::string& ,boost::shared_ptr<C2SHandler>);
	*/


	void registerHandler(const uint64_t& conn_id,boost::shared_ptr<C2SHandler>& handler);
	void unregisterHandler(const uint64_t& conn_id);
	boost::shared_ptr<C2SHandler> getHandler(const uint64_t& conn_id);



	typedef tbb::concurrent_hash_map<uint64_t,boost::shared_ptr<C2SHandler> ,tbb::tbb_hash_compare<uint64_t> > map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::const_accessor const_accessor;
	typedef typename map_type::accessor accessor;
	typedef typename map_type::const_iterator const_iterator;
	typedef typename map_type::iterator iterator;


private:
	tbb::concurrent_hash_map<uint64_t,boost::shared_ptr<C2SHandler>,tbb::tbb_hash_compare<uint64_t> conn_map_;
};

}

#endif
