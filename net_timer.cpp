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

CConnectTimer *CConnectTimerMgt::CreateConnectTimer(CSocket *pSocket, ConnectTimerProc pTimerProc,
		int64_t nTimeOut)
{
	if((pSocket == NULL) || (nTimeOut <= 0))
	{
		return NULL;
	}

	CConnectTimer *pTimer = new(nothrow) CConnectTimer();
	if(pTimer == NULL)
	{
		return NULL;
	}
	pTimer->pSocket = pSocket;
	pTimer->pTimerProc = pTimerProc;
	pTimer->m_nStartTime = CDateTime::CurrentDateTime().Millisecond();
	pTimer->m_nEndTime = CDateTime::CurrentDateTime().Millisecond() + nTimeOut;

	m_stConnectTimerList.push_back(pTimer);

	return pTimer;
}

CConnectTimer *CConnectTimerMgt::GetConnectTimer(CSocket *pSocket)
{
	CConnectTimer *pTimer = NULL;

	list<CConnectTimer *>::iterator it = m_stConnectTimerList.begin();
	for(;it != m_stConnectTimerList.end(); it++)
	{
		if(pSocket == (*it)->pSocket)
		{
			pTimer = *it;
			break;
		}
	}

	return pTimer;
}

CConnectTimer *CConnectTimerMgt::GetConnectTimer(SocketFD nSocketFD)
{
	CConnectTimer *pTimer = NULL;

	list<CConnectTimer *>::iterator it = m_stConnectTimerList.begin();
	for(;it != m_stConnectTimerList.end(); it++)
	{
		if(nSocketFD == (*it)->pSocket->GetSocketFD())
		{
			pTimer = *it;
			break;
		}
	}

	return pTimer;
}

CConnectTimer *CConnectTimerMgt::GetFirstConnectTimer()
{
	CConnectTimer *pTimer = NULL;

	if(m_stConnectTimerList.size() > 0)
	{
		pTimer = m_stConnectTimerList.front();
	}

	return pTimer;
}

int32_t CConnectTimerMgt::DestroyConnectTimer(CConnectTimer *pTimer)
{
	if(pTimer == NULL)
	{
		return S_OK;
	}

	list<CConnectTimer *>::iterator it = m_stConnectTimerList.begin();
	for(;it != m_stConnectTimerList.end(); it++)
	{
		if(pTimer == *it)
		{
			m_stConnectTimerList.erase(it);
			delete pTimer;
			break;
		}
	}

	return S_OK;
}

int32_t CConnectTimerMgt::GetConnectTimerCount()
{
	return m_stConnectTimerList.size();
}

NETEVENT_NAMESPACE_END

