/************************************************************************************
 *
 *  @file  CKetamaHash.cpp
 *  @brief 定义一致性HASH类
 *
 *  <b>文件名</b>      : CKetamaHash.cpp
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发部
 *  @n@n<b>作  者</b>  : chenfy
 *  @n@n<b>创建时间</b>: 2009-9-21W
 *  @n@n<b>文件描述</b>: 定义一致性HASH类
 *  @version		版本	修改者		  时间		  描述@n
 *  @n		        chenfy              2009-09-21
 *
 ************************************************************************************/

#include "KetamaHash.h"
#include "logger.h"
#include <string.h>

namespace c2s 
{
namespace ketama 
{

	KetamaHash::KetamaHash(key_t key)
	{
		m_rollflag = 1;  //需要重新生成
		m_kc = NULL;
		m_slist = NULL;

		if(!ketama_serverlist_init(&m_slist, key))
		{
			LOG(error)<<"ketama_serverlist_init error:"<<ketama_error()<<ENDL;
		}
	}

	KetamaHash::~KetamaHash()
	{
		ketama_smoke(m_kc);
		ketama_serverlist_smoke(m_slist);
	}

	//添加服务器
	int KetamaHash::AddServer(const char* serverName, const unsigned long memory)
	{
		if(!ketama_add_server(m_slist, serverName, memory))
		{
			LOG(error)<<"ketama_add_server error:"<<ketama_error()<<ENDL;
			return -1;
		}
		m_rollflag = 1;  //需要重新生成
		return 0;
	}

	//获取服务器
	const char* KetamaHash::GetServer(const char* szKey)
	{
		LOG(trace)<<"[KetamaHash]::GetServer('"<<szKey<<"')"<<ENDL;
		if(m_rollflag)
		{
			SetRoll();
			m_rollflag = 0;
		}

		mcs* m = ketama_get_server(m_kc ,szKey);
		if(NULL != m)
		{
			return (const char*)m->name;
		}

		LOG(trace)<<"[KetamaHash]::GetServer ErrMsg="<<ketama_error()<<ENDL;

		return NULL;
	}

	//删除服务器
	void KetamaHash::ClearServer()
	{
		ketama_serverlist_clear(m_slist);
		m_rollflag = 1;  //需要重新生成
	}

	//设置服务器分布
	int KetamaHash::SetRoll()
	{
		ketama_smoke(m_kc);
		return ketama_roll(&m_kc, m_slist);
	}

}
}
