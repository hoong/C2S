#include "method_client.h"
#include "c2s_client_handler.h"
#include "service_engine/rpc_proto/rpc.pb.h"


namespace c2s
{
MethodClient::MethodClient(RelayMethodParam& param)
	  : sphandler_(param.handler),
	    request_body_(param.request_body),
	    done_(0)
{
	boost::shared_ptr<C2SClientHandler> handler = boost::dynamic_pointer_cast<C2SClientHandler>(sphandler_);
	if (handler.get() == NULL)
	{
		throw RelayCallException(EC_RPC_INTERNAL_ERROR,"Wrong Client Handler");
	}

}

void MethodClient::onCall()
{
	try
	{
		boost::shared_ptr<C2SClientHandler> handler =
		       	boost::dynamic_pointer_cast<C2SClientHandler>(sphandler_);
		if (request_body_.cmd() == CmdID_IMS_LOGIN /* to-do ..*/)
		{

		}
		
		if (uin_ == 0)
		{
			uin_ = request_body_.router(0);
		}

		if (uin_ != 0 )
		{
			request_body_.add_router(handler->conn_id() );
			boost::shared_ptr<RelayTarget> target = handler->line().to_;
			if (target.get() != NULL)
			{
				target->relay(handler->conn_id(),request_body_);
			}
			else
			{
				static const std::string err = "No Target Found";
				finish(EC_RPC_INTERNAL_ERROR,err );
			}
		}
		else 
		{
			static const std::string err = "No User Found";
			finish(EC_RPC_INTERNAL_ERROR,err);
		}

	}
	catch(RelayCallException& e )
	{
		finish(EC_RPC_INTERNAL_ERROR,e.what() );
	}
	catch(std::exception& e )
	{
		finish(EC_RPC_INTERNAL_ERROR,e.what() );
	}

}

}
