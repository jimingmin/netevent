/*
 * net_reactor.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_REACTOR_H_
#define NET_REACTOR_H_

#include "net_namespace.h"
#include "net_typedef.h"

NETEVENT_NAMESPACE_BEGIN

enum
{
	mask_read	= 0x01,
	mask_write	= 0x02,
	mask_error	= 0x03,
};

class CSocket;

class IReactor
{
public:
	virtual ~IReactor(){};

	virtual int32_t Create(int32_t nMaxConn) = 0;

	virtual int32_t RegistEvent(CSocket *pSocket, uint32_t nEvents) = 0;

	virtual int32_t RemoveEvent(CSocket *pSocket, uint32_t nEvents) = 0;

	virtual int32_t DeleteEvent(CSocket *pSocket) = 0;

	virtual int32_t HandleMessage(int32_t nWaitTimeout) = 0;
};

NETEVENT_NAMESPACE_END

#endif /* NET_REACTOR_H_ */
