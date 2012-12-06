#ifndef TARGET_SERVER_H_
#define TARGET_SERVER_H_

namespace c2s
{
class TargetServer:public RelayTarget
{
public:
	virtual ~TargetServer(){}
	virtual void relay(uint64_t& uin,service_engine::rpc::MessageBody& body);
	virtual boost::shared_ptr<C2SHandler> relayHandler(uint64_t& uin);
	/*
	virtual void registerHandler(std::string& ,boost::shared_ptr<C2SHandler>) ;
	virtual void unregisterHandler(std::string& ,boost::shared_ptr<C2SHandler>);
	*/
};

}

#endif
