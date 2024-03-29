#include "relay_stub.h"
// RelayStubsPoolTraits

RelayStubsPoolTraits::~RelayStubsPoolTraits()
{
}


// RelayStubTask

RelayStubsMultiThreadPoolTraits::RelayStubTask::RelayStubTask(boost::shared_ptr<RelayMethodBase> method)
: sp_method_(method)
{
}

RelayStubsMultiThreadPoolTraits::RelayStubTask::~RelayStubTask()
{
}

void RelayStubsMultiThreadPoolTraits::RelayStubTask::run()
{
	if (sp_method_)
	{
		sp_method_->call();
	}
}


// RelayStubsMultiThreadPoolTraits

RelayStubsMultiThreadPoolTraits::RelayStubsMultiThreadPoolTraits(int nthread)
{
	thread_pool_.start(nthread);
}

RelayStubsMultiThreadPoolTraits::~RelayStubsMultiThreadPoolTraits()
{
}

void RelayStubsMultiThreadPoolTraits::push(boost::shared_ptr<RelayMethodBase> method)
{
	thread_pool_.scheduleTask(new RelayStubTask(method));
}

void RelayStubsMultiThreadPoolTraits::exit()
{
	thread_pool_.exit();
}


// RelayStubsPool
RelayStubsPool::RelayStubsPool(boost::shared_ptr<RelayStubsPoolTraits> traits)
: sp_pool_traits_(traits)
{
}

RelayStubsPool::~RelayStubsPool()
{
	sp_pool_traits_->exit();
}


// RelayStubsScheduler

RelayStubsScheduler::RelayStubsScheduler(boost::shared_ptr<RelayStubsPoolTraits> pool_traits)
: sp_pool_traits_(pool_traits)
{
}

RelayStubsScheduler::~RelayStubsScheduler()
{
}

void RelayStubsScheduler::invokeStub(boost::shared_ptr<C2SHandler> handler, auto_ptr<rpc::MessageBody>& request_body)
{
	std::vector<PickNote>::iterator it;
	for (it = pick_list_.begin(); it!= pick_list_.end(); it++ )
	{
		if (it->pick_->pick(request_body))
		{
			it->func_(handler,request_body,sp_pool_traits);
			return ;
		}

	}

	THROW_RPC_CALL_EXCEPTION(rpc::EC_RPC_METHOD_NOT_FOUND, "method not found");

}


