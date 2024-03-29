#ifndef RELAY_STUB_H_
#define RELAY_STUB_H_

#include "pch.h"
#include <map>
#include <boost/function.hpp>
#include "base/packet/packet_handler.h"
#include "base/threadpool.h"
#include "rpc_stub.h"

namespace c2s
{
// RelayMethod
/*
class SE_API RelayMethodBase : public boost::enable_shared_from_this<RelayMethodBase>
{
public:
	virtual ~RelayMethodBase();
	virtual void call() = 0;
};

struct RelayMethodParam
{
	RelayMethodParam(boost::shared_ptr<C2SHandler>& _h, auto_ptr<rpc::MessageBody>& _r)
		: handler(_h), request_body(_r) {}

	boost::shared_ptr<C2SHandler>& handler;
	auto_ptr<rpc::MessageBody>& request_body;
};
*/

class SE_API PickBase
{
public:
	virtual ~PickBase() {}
	virtual bool pick(RpcMessageBody& body) = 0;

};

template<class PICK>
class SE_API RelayMethod : public service_engine::RpcMethodBase
{
public:
	typedef typename PICK Pick;
public:
	RelayMethod(RpcMethodParam& param)
	  : sphandler_(param.handler),
	    request_body_(param.request_body),
	    done_(0)
	{

		expired_time_ = boost::get_system_time() +
				boost::posix_time::milliseconds(
						request_body_->has_expired_msec() ?
								request_body_->expired_msec() : Service::instance()->getProperties().get("rpc.expire_time", 20000));
	}

	virtual ~RelayMethod() {
		finish(rpc::EC_RPC_INTERNAL_ERROR, "uncompleted method!");
	}

	virtual void onCall() = 0 ;

public:
	void finish(int err_code, const char* err_msg)
	{
		if (!boost::interprocess::ipcdetail::atomic_cas32(&done_, 1, 0))
		{
			if (sphandler_ && request_body_->op() == rpc::OP_REQUEST)
				sphandler_->sendFailedResponse(*request_body_, err_code, err_msg);
		}
	}

	void finish()
	{
		if (!boost::interprocess::ipcdetail::atomic_cas32(&done_, 1, 0))
		{
			if (sphandler_ && request_body_->op() == rpc::OP_REQUEST)
				sphandler_->sendSuccessResponse(*request_body_, resp_);
		}
	}

	bool isExpired() const {
		return expired_time_ < boost::get_system_time();
	}

private:
	virtual void call()
	{
		try
		{
			onCall();
		}
		catch (RpcCallException& e)
		{
			LOG(error) << EXCEPTION_DIAG_INFO(e) << ENDL;
			finish(e.errcode(), e.errmsg());
		}
		catch (exception& e)
		{
			LOG(error) << EXCEPTION_DIAG_INFO(e) << ENDL;
			finish(rpc::EC_RPC_INTERNAL_ERROR, e.what());
		}
	}

protected:
	boost::shared_ptr<C2SHandler> sphandler_;
	auto_ptr<rpc::MessageBody> request_body_;

private:
	uint32_t done_;
};





/*
class SE_API RelayStubsPoolTraits
{
public:
	virtual ~RelayStubsPoolTraits();

	virtual void push(boost::shared_ptr<RelayMethodBase> method) = 0;
	virtual void exit() = 0;
};

class SE_API RelayStubsMultiThreadPoolTraits : public RelayStubsPoolTraits
{
public:
	class SE_API RelayStubTask : public base::Task
	{
	public:
		RelayStubTask(boost::shared_ptr<RelayMethodBase> method);
		virtual ~RelayStubTask();

		virtual void run();

	private:
		boost::shared_ptr<RelayMethodBase> sp_method_;
	};

public:
	RelayStubsMultiThreadPoolTraits(int nthread);
	virtual ~RelayStubsMultiThreadPoolTraits();

	virtual void push(boost::shared_ptr<RelayMethodBase> method);
	virtual void exit();

private:
	base::ThreadPool thread_pool_;
};

template <class METHOD>
struct SE_API RelayStub
{
	static void process(boost::shared_ptr<RelayHandler> handler, auto_ptr<service_engine::rpc::MessageBody>& request_body, boost::shared_ptr<RelayStubsPoolTraits> pool_traits)
	{

		boost::shared_ptr<METHOD> spMethod;

		RelayMethodParam param(handler, request_body);
		try
		{
			spMethod.reset(new METHOD(param));
			pool_traits->push(spMethod);
		}
		catch (RelayCallException& e)
		{
			LOG(error) << "rpc stub process failed: " << EXCEPTION_DIAG_INFO(e) << ENDL;

			if (!spMethod)
				handler->sendFailedResponse(*request_body, e.errcode(), e.what());
			else
				spMethod->finish(e.errcode(), e.what());
		}
		catch (exception& e)
		{
			LOG(error) << "rpc stub process failed: " << EXCEPTION_DIAG_INFO(e) << ENDL;

			if (!spMethod)
				handler->sendFailedResponse(*request_body, rpc::EC_RPC_INTERNAL_ERROR, e.what());
		}
	}
};
*/

/*
class RelayStubsScheduler;
class SE_API RelayStubsPool
{
	friend class RelayStubsScheduler;
public:
	RelayStubsPool(boost::shared_ptr<RelayStubsPoolTraits> traits);
	virtual ~RelayStubsPool();

private:
	boost::shared_ptr<RelayStubsPoolTraits> sp_pool_traits_;
};
*/


class RelayStubsScheduler
{
public:
	RelayStubsScheduler(boost::shared_ptr<service_engine::RpcStubsPoolTraits> pool_traits);
	virtual ~RelayStubsScheduler();

	void invokeStub(boost::shared_ptr<C2SHandler> handler, auto_ptr<rpc::MessageBody>& request_body);
	template <class METHOD>
	void registerStub()
	{
		PickNode p;
		p.pick_.reset(new METHOD::Pick());
		p.func_ = RelayStub<METHOD>::process 
		pick_list_.push_back(p);
	}

private:
	typedef boost::function<void (boost::shared_ptr<service_engine::RpcHandler> handler, auto_ptr<rpc::MessageBody>& request_body, boost::shared_ptr<service_engine::RpcStubsPoolTraits> pool_traits)> func_t;

	func_t func_;
	struct PickNode {
		boost::shared_ptr<PickBase> pick_;
		func_t func_;
	};

	std::vector<PickNote> pick_list_;

	boost::shared_ptr<service_engine::RpcStubsPoolTraits> sp_pool_traits_;
};


}


#endif
