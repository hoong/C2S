/*
 * c2s_service.h
 *
 *  Created on: 2012-10-25
 *      Author: root
 */

#ifndef C2S_SERVICE_H_
#define C2S_SERVICE_H_

#include "service_engine/service.h"

namespace c2s
{

class C2SService : public service_engine::ServiceT<C2SService>
{
public:
	C2SService(const service_engine::CommandLineParser& parser);
	virtual ~C2SService();

	virtual void run();

};

} /* namespace c2s */
#endif /* C2S_SERVICE_H_ */
