#ifndef RELAY_IMS_H_
#define RELAY_IMS_H_

namespace c2s
{
class RelayIMS:public IRelayTarget
{
public:
	virtual ~IRelayIMS();
	virtual void send(uint64_t& business_id,service_engine::rpc::MessageBody& body);
	virtual boost::shared_ptr<C2SHandler> get(uint64_t& business_id);

};

}

#endif
