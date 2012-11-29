#ifndef CLIENT_CONNECTION_H_
#define CLIENT_CONNECTION_H_

#include <stdint.h>
#include "rpc_handler.h"

namespace c2s
{

class ClientConnection
{
	uint64_t connection_id_;
	uint64_t uin_;

};


}



#endif
