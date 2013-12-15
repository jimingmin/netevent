/*
 * net_thread.cpp
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#include "net_api.h"
#include "net_socket.h"
#include "net_handler.h"
#include "net_eventid.h"
#include "net_timer.h"
#include "net_select.h"
#include "net_epoll.h"
#include "../common/common_datetime.h"

#ifndef WIN32
#include<cstring>
#include <errno.h>
#endif

NETEVENT_NAMESPACE_BEGIN

CNetHandler::CNetHandler()
{
	m_pReactor = NULL;
	m_nReconnectTime = 10;
	m_nLastConnectTime = 0;
}

CNetHandler::~CNetHandler()
{

}

int32_t CNetHandler::CreateReactor(int32_t nReactorType/* = enmReactorType_Epoll*/)
{
	if(nReactorType == enmReactorType_Select)
	{
		m_pReactor = new(nothrow) CSelect();
	}
	else
	{
		m_pReactor = new(nothrow) CEpoll();
	}

	int32_t nRet = m_pReactor->Create(0xffff);
	if(nRet < 0)
	{
		return -1;
	}

	return S_OK;
}

int32_t CNetHandler::DestoryReactor()
{
	if(m_pReactor != NULL)
	{
		delete m_pReactor;
	}
	return S_OK;
}

IReactor *CNetHandler::GetReactor()
{
	return m_pReactor;
}

int32_t CNetHandler::RegistEvent(CSocket *pSocket, uint32_t nEvents)
{
	if(m_pReactor == NULL)
	{
		return 1;
	}

	return m_pReactor->RegistEvent(pSocket, nEvents);
}

int32_t CNetHandler::RemoveEvent(CSocket *pSocket, uint32_t nEvents)
{
	if(m_pReactor == NULL)
	{
		return 1;
	}

	return m_pReactor->RemoveEvent(pSocket, nEvents);
}

int32_t CNetHandler::DeleteEvent(CSocket *pSocket)
{
	if(m_pReactor == NULL)
	{
		return 1;
	}

	return m_pReactor->DeleteEvent(pSocket);
}

bool CNetHandler::Process()
{
	bool bHasData = false;
	//while(!GetTerminated())
	//{
		//消息泵
		if(MessagePump() > 0)
		{
			bHasData = true;
		}
		//处理socket超时事件
		HandleTimeOutEvent();
	//}
	return bHasData;
}

int32_t CNetHandler::MessagePump()
{
	int32_t nMessageResult = SendMessage();

	int32_t nWaitTimeout = 0;
	if(nMessageResult == S_FALSE)
	{
		nWaitTimeout = 50;
	}
	int32_t nEventCount = 0;
	if(m_pReactor != NULL)
	{
		nEventCount = m_pReactor->HandleMessage(nWaitTimeout);
	}

	return nEventCount;
}

int32_t CNetHandler::SendMessage()
{
	return S_OK;
}

int32_t CNetHandler::HandleTimeOutEvent()
{
	int32_t nTimerCount = g_ConnectTimerMgt.GetConnectTimerCount();

	for(int32_t i = 0; i < nTimerCount; ++i)
	{
		CConnectTimer *pTimer = g_ConnectTimerMgt.GetFirstConnectTimer();
		if(pTimer->m_nEndTime > CDateTime::CurrentDateTime().Seconds())
		{
			continue;
		}

		CSocket *pSocket = pTimer->pSocket;
		ConnectTimerProc pTimerProc = pTimer->pTimerProc;

		(pSocket->*pTimerProc)(pTimer);
	}

	return S_OK;
}

NETEVENT_NAMESPACE_END




