#include "target_ims.h"
#include "relay_exception.h"

void parseServerPair(const std::string& servers,std::pair<base::net::SockAddr,base::net::SockAddr>& server_pair)
{
	size_t pos = servers.find_first_of(';');
	std::string first_server = pos.substr(0,pos-1);
	std::string second_server = pos.substr(pos+1);
	SockAddr first_sock(first_server.c_str(),first_server.length());
	SockAddr second_sock(second_server.c_str(),second_server.length());
	server_pair.first = first_sock;
	server_pair.seconf = second_sock;
}


namespace c2s
{

void TargetIMS::init(const std::string& conf)
{
	static const std::string err = "unavailable config";
	json_ << conf;
	if (!json_.isAvail())
	{
		THROW_RPC_CALL_EXCEPTION(EC_RPC_INTERNAL_ERROR,err);
	}

	Json::Value ims_array; 
	static const std::string path = ".";
	json_.get(".",ims_array);
	if (ims_array.empty())
	{
		THROW_RPC_CALL_EXCEPTION(EC_RPC_INTERNAL_ERROR,err);
	}

}

void TargetIMS::relay(uint64_t& uin,service_engine::rpc::MessageBody& body)
{
	boost::shared_ptr<C2SHandler> handler = relayHandler(conn_id);
	if (NULL == handler.get())
	{
		static const std::string err = "IMS not found";
		THROW_RPC_CALL_EXCEPTION(EC_RPC_TRANSPORT_ERROR,err);
	};

	handler->relay(body);
}

boost::shared_ptr<C2SHandler> TargetIMS::relayHandler(uint64_t& uin)
{
	char key[21];
	memset(key,0,sizeof(key));
	sprintf(key,"%llu",uin);
	std::string servers = sp_hash_->GetServer(key);
	std::pair<base::net::SockAddr,base::net::SockAddr> server_pair;
	parseServerPair(servers,server_pair);
	boost::shared_ptr<C2SHandler> ret(NULL);

	if (  (ret = sp_handler_factory_->createHandler(server_pair.first) && ret.get()) ||
			(ret = sp_handler_factory_->createHandler(server_pair.second) && ret.get()) 
		)
	{
	}
	return ret;

}

}
