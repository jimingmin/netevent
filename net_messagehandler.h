/*
 * net_messagehandler.h
 *
 *  Created on: 2014Äê12ÔÂ29ÈÕ
 *      Author: jimm
 */

#ifndef NET_MESSAGEHANDLER_H_
#define NET_MESSAGEHANDLER_H_

#include "net_impl.h"
#include "net_ioadapter.h"
#include "net_namespace.h"

NETEVENT_NAMESPACE_BEGIN

class CNetMessageHandler : public CIOHandlerAdapter
{
public:
	virtual int32_t OnOpened(IIOSession *pIoSession);

	virtual int32_t OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnClosed(IIOSession *pIoSession);

	virtual int32_t OnError(IIOSession *pIoSession);

	virtual int32_t OnTimeout(IIOSession *pIoSession);

};

NETEVENT_NAMESPACE_END

#endif	/* NET_MESSAGEHANDLER_H_ */
