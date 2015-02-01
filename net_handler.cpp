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
#include "net_connmgt.h"
#include "net_api.h"
#include "../common/common_datetime.h"
#include "../common/common_memmgt.h"

#ifndef WIN32
#include<cstring>
#endif

NETEVENT_NAMESPACE_BEGIN

CNetHandler::CNetHandler()
{
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int32_t err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
#endif
	m_pReactor = NULL;
	m_nReconnectTime = 10;
	m_nLastConnectTime = 0;
	m_pCallBackSet = NULL;
}

CNetHandler::~CNetHandler()
{
#ifdef WIN32
	WSACleanup();
#endif
}

int32_t CNetHandler::CreateReactor(int32_t nReactorType/* = enmReactorType_Epoll*/)
{
	if(nReactorType == enmReactorType_Select)
	{
		m_pReactor = new(nothrow) CSelect();
	}
	else
	{
#ifdef WIN32
		m_pReactor = new(nothrow) CSelect();
#else
		m_pReactor = new(nothrow) CEpoll();
#endif
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

CConnectTimerMgt &CNetHandler::GetConnectTimerMgt()
{
	return m_stConnectTimerMgt;
}

CConnMgt &CNetHandler::GetConnMgt()
{
	return m_stConnMgt;
}

void CNetHandler::PushPacket(NetPacket *pPacket)
{
	m_stSendQueue.push_back(pPacket);
}

void CNetHandler::PushCloseEvent(CloseEvent *pCloseEvent)
{
	m_stCloseQueue.push_back(pCloseEvent);
}

void CNetHandler::SetNetHandlerCallBack(NetFuncEntry *pCallBack)
{
	m_pCallBackSet = pCallBack;
}

NetFuncEntry *CNetHandler::GetNetHandlerCallBack()
{
	return m_pCallBackSet;
}

//0表示无数据处理 1表示有数据处理
int32_t CNetHandler::Run()
{
	int32_t bHasData = 0;

	//消息泵
	if(MessagePump() > 0)
	{
		bHasData = 1;
	}
	//处理socket超时事件
	HandleTimeoutEvent();
	//处理close事件
	HandleAsyncCloseEvent();

	return bHasData;
}

int32_t CNetHandler::MessagePump()
{
	int32_t nWaitTimeout = enmWaitTimeout;

	while(!m_stSendQueue.empty())
	{
		NetPacket *pPacket = m_stSendQueue.front();
		m_stSendQueue.pop_front();
		int32_t nMessageResult = SendPacket(pPacket);
		if(nMessageResult == S_FALSE)
		{
		}
		nWaitTimeout = 0;
		DELETE(pPacket);
	}

	int32_t nEventCount = 0;
	if(m_pReactor != NULL)
	{
		nEventCount = m_pReactor->HandleMessage(nWaitTimeout);
	}

	return nEventCount;
}

int32_t CNetHandler::SendPacket(NetPacket *pPacket)
{
	CConnection *pConnection = m_stConnMgt.GetConnection(pPacket->m_nSessionID);
	if(pConnection == NULL)
	{
		return S_FALSE;
	}

	int32_t nSendBytes = 0;
	if(pConnection->Send(pPacket->m_pNetPacket, pPacket->m_nNetPacketLen, nSendBytes) == S_OK)
	{
		//写到循环buf了
		pConnection->WriteCompleted(pPacket->m_pNetPacket, nSendBytes);
	}
	else
	{
		pConnection->Close();
	}

	return S_OK;
}

int32_t CNetHandler::HandleTimeoutEvent()
{
	int32_t nTimerCount = m_stConnectTimerMgt.GetConnectTimerCount();

	for(int32_t i = 0; i < nTimerCount; ++i)
	{
		CConnectTimer *pTimer = m_stConnectTimerMgt.GetFirstConnectTimer();
		if(pTimer->m_nEndTime > CTimeValue::CurrentTime().Milliseconds())
		{
			continue;
		}

		CSocket *pSocket = pTimer->pSocket;
		ConnectTimerProc pTimerProc = pTimer->pTimerProc;

		(pSocket->*pTimerProc)(pTimer);
	}

	return S_OK;
}

int32_t CNetHandler::HandleAsyncCloseEvent()
{
	while(!m_stCloseQueue.empty())
	{
		CloseEvent *pCloseEvent = m_stCloseQueue.front();
		m_stCloseQueue.pop_front();

		CConnection *pConn = m_stConnMgt.GetConnection(pCloseEvent->m_nSessionID);
		if(pConn != NULL)
		{
			pConn->Close(pCloseEvent->m_nCloseCode);
		}

		DELETE(pCloseEvent);
	}
	return S_OK;
}

NETEVENT_NAMESPACE_END

