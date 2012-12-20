#ifndef TARGET_IMS_H_
#define TARGET_IMS_H_

#include "ketama/KetamaHash.h"
#include "c2s_server_handler.h"
#include "json_man.h"

namespace c2s
{
class TargetIMS:public RelayTarget
{
public:
	TargetIMS(boost::shared_ptr<ketama::KetamaHash> hash,
			boost::shared_ptr<C2SServerChannelFactory> handler_factory): 
		sp_hash_(hash),sp_handler_factory(handler_factory) 
	{};
	virtual ~TargetIMS(){}

	virtual void relay(uint64_t& uin,service_engine::rpc::MessageBody& body);
	virtual boost::shared_ptr<C2SHandler> relayHandler(uint64_t& uin);
	/*
	virtual void registerHandler(std::string& ,boost::shared_ptr<C2SHandler>) ;
	virtual void unregisterHandler(std::string& ,boost::shared_ptr<C2SHandler>);
	*/

	/* configuration format:
	 *
	 *  {[{"first":"192.168.1.1:2012","second":"192.168.1.1:2013"},
	 *   {"first":"192.168.1.2:2012","second":"192.168.1.2:2013"},
	 *   ...
	 *  ]}
	 */
	int init(const std::string& conf);
private:
	//server = first;second   192.168.1.1:2012;192.168.1.2:2013
	boost::shared_ptr<ketama::KetamaHash> sp_hash_;
	boost::shared_ptr<C2SServerChannelFactory> sp_handler_factory_;
	JsonMan json_;
};

}

#endif
