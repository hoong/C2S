#ifndef RELAY_IMS_H_
#define RELAY_IMS_H_

namespace c2s
{
class RelayIMS:public RelayTarget
{
public:
	virtual ~RelayIMS(){};
	virtual void relay(uint64_t& uin,service_engine::rpc::MessageBody& body);
	virtual boost::shared_ptr<C2SHandler> relayHandler(uint64_t& uin);

};

}

#endif
