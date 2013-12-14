/*
 * net_timer.h
 *
 *  Created on: 2013Äê11ÔÂ4ÈÕ
 *      Author: jimm
 */

#ifndef NET_TIMER_H_
#define NET_TIMER_H_

#include "net_namespace.h"
#include "../common/common_singleton.h"

#include <list>

using namespace std;

NETEVENT_NAMESPACE_BEGIN

class CSocket;
class CConnectTimer;

typedef int32_t (CSocket::*ConnectTimerProc)(CConnectTimer *pTimer);

class CConnectTimer
{
public:
	CConnectTimer()
	{
		Reset();
	}

	void Reset()
	{
		pSocket = NULL;
		pTimerProc = NULL;
		m_nStartTime = 0;
		m_nEndTime = 0;
	}

	CSocket			*pSocket;
	ConnectTimerProc	pTimerProc;
	int64_t			m_nStartTime;
	int64_t			m_nEndTime;
};

class CConnectTimerMgt
{
public:

	CConnectTimer *CreateConnectTimer(CSocket *pSocket, ConnectTimerProc pTimerProc, int64_t nTimeOut);

	CConnectTimer *GetConnectTimer(CSocket *pSocket);

	CConnectTimer *GetConnectTimer(SocketFD nSocketFD);

	CConnectTimer *GetFirstConnectTimer();

	int32_t DestroyConnectTimer(CConnectTimer *pTimer);

	int32_t GetConnectTimerCount();

protected:
	list<CConnectTimer *>		m_stConnectTimerList;
};

#define	g_ConnectTimerMgt					CSingleton<CConnectTimerMgt>::GetInstance()

NETEVENT_NAMESPACE_END

#endif /* NET_TIMER_H_ */
