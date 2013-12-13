/*
 * net_timer.cpp
 *
 *  Created on: 2013Äê11ÔÂ4ÈÕ
 *      Author: jimm
 */


#include "net_socket.h"
#include "net_timer.h"
#include "../common/common_datetime.h"

NETEVENT_NAMESPACE_BEGIN

CFrameSocketTimer *CFrameSocketTimerMgt::CreateSocketTimer(CSocket *pSocket, SocketTimerProc pTimerProc,
		int64_t nTimeOut)
{
	if((pSocket == NULL) || (nTimeOut <= 0))
	{
		return NULL;
	}

	CFrameSocketTimer *pTimer = new(nothrow) CFrameSocketTimer();
	if(pTimer == NULL)
	{
		return NULL;
	}
	pTimer->pSocket = pSocket;
	pTimer->pTimerProc = pTimerProc;
	pTimer->m_nStartTime = CDateTime::CurrentDateTime().Seconds();
	pTimer->m_nEndTime = CDateTime::CurrentDateTime().Seconds() + nTimeOut;

	m_stFrameSocketTimerList.push_back(pTimer);

	return pTimer;
}

CFrameSocketTimer *CFrameSocketTimerMgt::GetSocketTimer(CSocket *pSocket)
{
	CFrameSocketTimer *pTimer = NULL;

	list<CFrameSocketTimer *>::iterator it = m_stFrameSocketTimerList.begin();
	for(;it != m_stFrameSocketTimerList.end(); it++)
	{
		if(pSocket == (*it)->pSocket)
		{
			pTimer = *it;
			break;
		}
	}

	return pTimer;
}

CFrameSocketTimer *CFrameSocketTimerMgt::GetSocketTimer(SocketFD nSocketFD)
{
	CFrameSocketTimer *pTimer = NULL;

	list<CFrameSocketTimer *>::iterator it = m_stFrameSocketTimerList.begin();
	for(;it != m_stFrameSocketTimerList.end(); it++)
	{
		if(nSocketFD == (*it)->pSocket->GetSocketFD())
		{
			pTimer = *it;
			break;
		}
	}

	return pTimer;
}

CFrameSocketTimer *CFrameSocketTimerMgt::GetFirstSocketTimer()
{
	CFrameSocketTimer *pTimer = NULL;

	if(m_stFrameSocketTimerList.size() > 0)
	{
		pTimer = m_stFrameSocketTimerList.front();
	}

	return pTimer;
}

int32_t CFrameSocketTimerMgt::DestroySocketTimer(CFrameSocketTimer *pTimer)
{
	if(pTimer == NULL)
	{
		return S_OK;
	}

	list<CFrameSocketTimer *>::iterator it = m_stFrameSocketTimerList.begin();
	for(;it != m_stFrameSocketTimerList.end(); it++)
	{
		if(pTimer == *it)
		{
			m_stFrameSocketTimerList.erase(it);
			delete pTimer;
			break;
		}
	}

	return S_OK;
}

int32_t CFrameSocketTimerMgt::GetTimerCount()
{
	return m_stFrameSocketTimerList.size();
}

NETEVENT_NAMESPACE_END

