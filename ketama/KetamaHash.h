/************************************************************************************
	*
	*  @file  CKetamaHash.h
	*  @brief 定义一致性HASH类
	*
	*  <b>文件名</b>      : CKetamaHash.h
	*  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发部
	*  @n@n<b>作  者</b>  : chenfy
	*  @n@n<b>创建时间</b>: 2009-9-21
	*  @n@n<b>文件描述</b>: 定义一致性HASH类
	*  @version		版本	修改者		  时间		  描述@n
	*  @n		        chenfy              2009-09-21
	*  @n           tantan              2009-11-11
	*
 ************************************************************************************/
#if !defined(__ObjectOrientedModel_1_CKetamaHash_h)
#define __ObjectOrientedModel_1_CKetamaHash_h

#include "ketama.h"
#include <stdio.h>
#include <stdlib.h>

namespace c2s 
{
namespace ketama
{
	/************************************************************************************
	* @class KetamaHash
	* @brief  定义一致性HASH类
	*
	*  负责一致性HASH函数，用于选取服务器进行存储
	************************************************************************************/
	class KetamaHash
	{
	public:
		KetamaHash(key_t key);
		virtual ~KetamaHash();

		int AddServer(const char* serverName, const unsigned long memory = 1);
		const char* GetServer(const char* szKey);
		void ClearServer();

	private:
		int SetRoll();

		ketama_continuum m_kc;				//服务器分布的结构
		serverlist* m_slist;					//服务器列表
		int m_rollflag;
	};
}
}
#endif
