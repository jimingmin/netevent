/*
 * net_ioadapter.cpp
 *
 *  Created on: 2013Äê12ÔÂ17ÈÕ
 *      Author: jimm
 */

#include "net_ioadapter.h"

NETEVENT_NAMESPACE_BEGIN

int32_t CIOHandlerAdapter::OnOpened(IIOSession *pIoSession)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnClosed(IIOSession *pIoSession)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnError(IIOSession *pIoSession)
{
	return 0;
}

NETEVENT_NAMESPACE_END
