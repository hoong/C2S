#ifndef C2S_METHOD_CLIENT_H_
#define C2S_METHOD_CLIENT_H_
#include "relay_stub.h"

namespace c2s
{

class MethodClient:public RelayMethod 
{
public:
	MethodClient(RelayMethodParam& param);
	virtual ~MethodClient() {}

	virtual onCall();


};

}
#endif
