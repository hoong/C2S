#include "method_ims.h"

namespace c2s
{

MethodIMS::MethodIMS(RelayMethodParam& param)
	  : sphandler_(param.handler),
	    request_body_(param.request_body),
	    done_(0)
{

}


void MethodIMS::onCall()
{
	try
	{
		if (sphandler_->line().to_.get() == NULL)
		{
			static const std::string err = "No Target Found";
			finish(EC_RPC_INTERNAL_ERROR,err);
			return;
		}
		if (request_body_.router_size() ==0)
		{
			static const std::string err = "No route info Found";
			finish(EC_RPC_INTERNAL_ERROR,err);
			return;
		}
		std::vector<uint64_t> router_list;
		for (int i =0;i<request_body_.router_size();i++)
		{
			router_list.push_back(request_body_.router(i));
		}
		request_body_.clear_router();
		int idx = 0
		for (int idx = 0;idx<router_list.size() -1;idx++)
		{
			request_body_.add_router(router_list[idx]);
		}

		sphandler_->line().to_->relay(router_list[idx],request_body_);

	}
	catch(std::exception& e)
	{
		finish(EC_RPC_INTERNAL_ERROR,e.what());
	}	


	

}

}
