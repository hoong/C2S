#ifndef C2S_METHOD_IMS_H_
#define C2S_METHOD_IMS_H_
#include "relay_stub.h"

namespace c2s
{

class MethodIMS:public RelayMethod
{
public:
	MethodIMS(RelayMethodParam& param);
	virtual ~MethodIMS() {}
	virtual void onCall();

};

}


#endif
