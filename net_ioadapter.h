/*
 * net_ioadapter.h
 *
 *  Created on: 2013年12月17日
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
	EXPORT virtual int32_t OnOpened(IIOSession *pIoSession);

	EXPORT virtual int32_t OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	EXPORT virtual int32_t OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	EXPORT virtual int32_t OnClosed(IIOSession *pIoSession);

	EXPORT virtual int32_t OnError(IIOSession *pIoSession);

	EXPORT virtual int32_t OnTimeout(IIOSession *pIoSession);
};

NETEVENT_NAMESPACE_END

#endif /* NET_IOADAPTER_H_ */
