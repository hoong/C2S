#include "relay_target_manager.h"

namespace c2s
{
void RelayTargetManager::set(const std::string& key,boost::shared_ptr<RelayTarget> value)
{
	map_.insert(std::pair<std::string,boost::shared_ptr<RelayTarget> >(key,value));
}

boost::shared_ptr<RelayTarget> RelayTargetManager::get(const std::string& key)
{
	std::map<std::string,boost::shared_ptr<RelayTarget> >::iterator it;
	it = map_.find(key);
	if (it == map_.end())
	{
		return boost::shared_ptr<RelayTarget>(NULL);
	}
	return it->second;
}

}
