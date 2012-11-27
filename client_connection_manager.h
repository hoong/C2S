#ifndef CLIENT_CONNECTION_MANAGER_
#define CLIENT_CONNECTION_MANAGER_

namespace c2s
{


class ClientConnectionManager
{
public:
	void relay(service_engine::rpc::MessageBody& body);

};

}
#endif
