#include "target_client.h"


namespace c2s
{

void TargetClient::relay(uint64_t& conn_id,service_engine::rpc::MessageBody& body)
{
	boost::shared_ptr<C2SHandler> handler = relayHandler(conn_id);
	if (NULL == handler.get())
	{
		static const std::string err = "client not found";
		THROW_RPC_CALL_EXCEPTION(EC_RPC_TRANSPORT_ERROR,err);
	};

	handler->relay(body);

}


boost::shared_ptr<C2SHandler> TargetClient::relayHandler(uint64_t& conn_id)
{
	boost::shared_ptr<C2SHandler> ret(NULL);

	{
		const_accessor cacc;
		if (conn_map_::find(cacc,conn_id))
		{
			ret = cacc->second;
			return ret;
		}
	}

	return ret;
}



void TargetClient::registerHandler(const uint64_t& conn_id,boost::shared_ptr<C2SHandler>& handler)
{

	accessor result;
	if (!conn_map_.insert(result,value_type(conn_id,handler)))
	{
		result->second = handler;
	}

}

void TargetClient::unregisterHandler(const uint64_t& conn_id)
{
	accessor result;
	if (conn_map_.find(result,conn_id))
	{
		conn_map_.erase(result);
	}
}

}
