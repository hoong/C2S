#ifndef RELAY_TARGET_H_
#define RELAY_TARGET_H_

#include "c2s_handler.h"
#include "rpc_proto/rpc.pb.h"
#include "singleton.h"
#include <map>
#include <string>
#include <stdint.h>
#include <boost/shared_ptr.hpp>

namespace c2s
{

class RelayTarget
{
public:
	virtual ~RelayTarget();
	virtual void relay(uint64_t& ,service_engine::rpc::MessageBody& ) = 0;
	virtual boost::shared_ptr<C2SHandler> relayHandler(uint64_t& ) = 0;

};

class RelayLine
{
public:
	RelayLine(boost::shared_ptr<RelayTarget> from,boost::shared_ptr<RelayTarget> to)
		:from_(from),to_(to){}
	RelayLine(const RelayLine& rl):from_(rl.from_),to_(rl.to_){}
	boost::shared_ptr<RelayTarget> from_;
	boost::shared_ptr<RelayTarget> to_;
};


class RelayTargetManager
{
public:
	RelayTargetManager() {}
	~RelayTargetManager() {}
	void set(const std::string& key,boost::shared_ptr<RelayTarget> value);
	boost::shared_ptr<RelayTarget> get(const std::string& key);
	
private:
	std::map<std::string,boost::shared_ptr<RelayTarget> > map_;
};

typedef base::Singleton<RelayTargetManager> RTM;


}







#endif
