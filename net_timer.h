/*
 * net_timer.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_TIMER_H_
#define NET_TIMER_H_

#include "net_namespace.h"
#include "../common/common_object.h"

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
		Init();
	}

	virtual int32_t Init()
	{
		pSocket = NULL;
		pTimerProc = NULL;
		m_nStartTime = 0;
		m_nEndTime = 0;
		return 0;
	}

	virtual int32_t Uninit()
	{
		return 0;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}

	CSocket				*pSocket;
	ConnectTimerProc	pTimerProc;
	int64_t				m_nStartTime;
	int64_t				m_nEndTime;
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
