#ifndef C2S_RELAY_EXCEPTION_H_
#define C2S_RELAY_EXCEPTION_H_
#include "rpc_exception.h"

namespace c2s
{

typedef service_engine::RpcCallException RelayCallException;

#define THROW_RELAY_CALL_EXCEPTION(code, msg) \
	throw RelayCallException(code, msg, __FILE__, __LINE__)

}

#endif
