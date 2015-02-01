/*
 * net_messagehandler.cpp
 *
 *  Created on: 2014年12月30日
 *      Author: jimm
 */

#include "net_impl.h"
#include "net_messagehandler.h"
#include "net_namespace.h"
#include "net_connection.h"
#include "net_handler.h"
#include "net_typedef.h"

NETEVENT_NAMESPACE_BEGIN

int32_t CNetMessageHandler::OnOpened(IIOSession *pIoSession)
{
	CConnection *pConn = dynamic_cast<CConnection *>(pIoSession);
	if(pConn == NULL)
	{
		return 0;
	}

	CNetHandler *pNetHandler = pConn->GetNetHandler();
	//主动发起连接
	if(pIoSession->IsSourceRole())
	{
		pNetHandler->GetNetHandlerCallBack()->func_net_connected(pIoSession->GetSessionID(),
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}
	else
	{
		pNetHandler->GetNetHandlerCallBack()->func_net_accepted(pIoSession->GetSessionID(),
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}

	return 0;
}

int32_t CNetMessageHandler::OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	CConnection *pConn = dynamic_cast<CConnection *>(pIoSession);
	if(pConn == NULL)
	{
		return 0;
	}

	CNetHandler *pNetHandler = pConn->GetNetHandler();
	return pNetHandler->GetNetHandlerCallBack()->func_net_read(pIoSession->GetSessionID(),
		pBuf, nBufSize);
}

int32_t CNetMessageHandler::OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	CConnection *pConn = dynamic_cast<CConnection *>(pIoSession);
	if(pConn == NULL)
	{
		return 0;
	}

	CNetHandler *pNetHandler = pConn->GetNetHandler();
	return pNetHandler->GetNetHandlerCallBack()->func_net_writen(pIoSession->GetSessionID(),
		pBuf, nBufSize);
}

int32_t CNetMessageHandler::OnClosed(IIOSession *pIoSession)
{
	CConnection *pConn = dynamic_cast<CConnection *>(pIoSession);
	if(pConn == NULL)
	{
		return 0;
	}

	CNetHandler *pNetHandler = pConn->GetNetHandler();
	return pNetHandler->GetNetHandlerCallBack()->func_net_closed(pIoSession->GetSessionID(),
		pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
}

int32_t CNetMessageHandler::OnError(IIOSession *pIoSession)
{
	CConnection *pConn = dynamic_cast<CConnection *>(pIoSession);
	if(pConn == NULL)
	{
		return 0;
	}

	CNetHandler *pNetHandler = pConn->GetNetHandler();
	return pNetHandler->GetNetHandlerCallBack()->func_net_error(pIoSession->GetSessionID(), 0);
}

int32_t CNetMessageHandler::OnTimeout(IIOSession *pIoSession)
{
	CConnection *pConn = dynamic_cast<CConnection *>(pIoSession);
	if(pConn == NULL)
	{
		return 0;
	}

	CNetHandler *pNetHandler = pConn->GetNetHandler();
	return pNetHandler->GetNetHandlerCallBack()->func_net_connect_timeout(pIoSession->GetSessionID(), 
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

}


NETEVENT_NAMESPACE_END
