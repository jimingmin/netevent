/*
 * net_thread.cpp
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#include "net_api.h"
#include "net_datetime.h"
#include "net_socket.h"
#include "net_thread.h"
#include "net_eventid.h"
#include "net_timer.h"
#include "net_select.h"
#include "net_epoll.h"

#ifndef WIN32
#include<cstring>
#include <errno.h>
#endif

NETEVENT_NAMESPACE_BEGIN

CFrameNetThread::CFrameNetThread()
{
	m_pReactor = NULL;
	m_nReconnectTime = 10;
	m_nLastConnectTime = 0;
}

CFrameNetThread::~CFrameNetThread()
{

}

int32_t CFrameNetThread::Initialize()
{
	m_pReactor = new(nothrow) CSelect();
	int32_t nRet = m_pReactor->Create(0xffff);
	if(nRet < 0)
	{
		return -1;
	}

	return S_OK;
}

int32_t CFrameNetThread::Resume()
{
	return S_OK;
}

int32_t CFrameNetThread::Uninitialize()
{
	if(m_pReactor != NULL)
	{
		delete m_pReactor;
	}
	return S_OK;
}

IReactor *CFrameNetThread::GetReactor()
{
	return m_pReactor;
}

bool CFrameNetThread::Execute()
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

int32_t CFrameNetThread::MessagePump()
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

int32_t CFrameNetThread::SendMessage()
{
	return S_OK;
}

int32_t CFrameNetThread::HandleTimeOutEvent()
{
	int32_t nTimerCount = g_FrameSocketTimerMgt.GetTimerCount();

	for(int32_t i = 0; i < nTimerCount; ++i)
	{
		CFrameSocketTimer *pTimer = g_FrameSocketTimerMgt.GetFirstSocketTimer();
		if(pTimer->m_nEndTime > CDateTime::CurrentDateTime().Seconds())
		{
			continue;
		}

		CSocket *pSocket = pTimer->pSocket;
		SocketTimerProc pTimerProc = pTimer->pTimerProc;

		(pSocket->*pTimerProc)(pTimer);
	}

	return S_OK;
}

NETEVENT_NAMESPACE_END




