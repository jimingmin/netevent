/*
 * net_messagehandler.cpp
 *
 *  Created on: 2014年12月30日
 *      Author: jimm
 */

#include "net_impl.h"
#include "net_messagehandler.h"
#include "net_namespace.h"

extern CallBackSet g_CallBackSet;

NETEVENT_NAMESPACE_BEGIN

int32_t CNetMessageHandler::OnOpened(IIOSession *pIoSession)
{
	//主动发起连接
	if(pIoSession->IsSourceRole())
	{
		g_CallBackSet.func_net_connected(pIoSession->GetSessionID(), pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}
	else
	{
		g_CallBackSet.func_net_accepted(pIoSession->GetSessionID(), pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}

	return 0;
}

int32_t CNetMessageHandler::OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	if(g_CallBackSet.func_net_read != NULL)
	{
		return g_CallBackSet.func_net_read(pIoSession->GetSessionID(), pBuf, nBufSize);
	}
	return 0;
}

int32_t CNetMessageHandler::OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	if(g_CallBackSet.func_net_writen != NULL)
	{
		return g_CallBackSet.func_net_writen(pIoSession->GetSessionID(), pBuf, nBufSize);
	}
	return 0;
}

int32_t CNetMessageHandler::OnClosed(IIOSession *pIoSession)
{
	if(g_CallBackSet.func_net_closed != NULL)
	{
		return g_CallBackSet.func_net_closed(pIoSession->GetSessionID(), pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}
	return 0;
}

int32_t CNetMessageHandler::OnError(IIOSession *pIoSession)
{
	if(g_CallBackSet.func_net_error != NULL)
	{
		return g_CallBackSet.func_net_error(pIoSession->GetSessionID(), 0);
	}
	return 0;
}

int32_t CNetMessageHandler::OnTimeout(IIOSession *pIoSession)
{
	if(g_CallBackSet.func_net_connect_timeout != NULL)
	{
		return g_CallBackSet.func_net_connect_timeout(pIoSession->GetSessionID(), 
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}
	return 0;
}


NETEVENT_NAMESPACE_END
