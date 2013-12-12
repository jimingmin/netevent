/*
 * net_impl.h
 *
 *  Created on: 2013Äê11ÔÂ4ÈÕ
 *      Author: jimm
 */

#ifndef NET_IMPL_H_
#define NET_IMPL_H_

#include "net_namespace.h"

NETEVENT_NAMESPACE_BEGIN

class IIOHandler
{
public:
	virtual ~IIOHandler(){};

	virtual int32_t ReadEvent() = 0;

	virtual int32_t WriteEvent() = 0;

	virtual int32_t ErrorEvent() = 0;
};

NETEVENT_NAMESPACE_END

#endif /* NET_IMPL_H_ */
