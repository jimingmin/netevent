/*
 * net_ioadapter.h
 *
 *  Created on: 2013Äê12ÔÂ17ÈÕ
 *      Author: jimm
 */

#ifndef NET_IOADAPTER_H_
#define NET_IOADAPTER_H_

#include "net_namespace.h"
#include "net_impl.h"

NETEVENT_NAMESPACE_BEGIN

class CIOHandlerAdapter : public IIOHandler
{
public:
	virtual int32_t OnOpened(IEventHandler *pHandler);

	virtual int32_t OnRecved(IEventHandler *pHandler, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnSent(IEventHandler *pHandler, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnClosed(IEventHandler *pHandler);

	virtual int32_t OnError(IEventHandler *pHandler);
};

NETEVENT_NAMESPACE_END

#endif /* NET_IOADAPTER_H_ */
