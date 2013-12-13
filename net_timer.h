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
class CFrameSocketTimer;

typedef int32_t (CSocket::*SocketTimerProc)(CFrameSocketTimer *pTimer);

class CFrameSocketTimer
{
public:
	CFrameSocketTimer()
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
	SocketTimerProc	pTimerProc;
	int64_t			m_nStartTime;
	int64_t			m_nEndTime;
};

class CFrameSocketTimerMgt
{
public:

	CFrameSocketTimer *CreateSocketTimer(CSocket *pSocket, SocketTimerProc pTimerProc, int64_t nTimeOut);

	CFrameSocketTimer *GetSocketTimer(CSocket *pSocket);

	CFrameSocketTimer *GetSocketTimer(SocketFD nSocketFD);

	CFrameSocketTimer *GetFirstSocketTimer();

	int32_t DestroySocketTimer(CFrameSocketTimer *pTimer);

	int32_t GetTimerCount();

protected:
	list<CFrameSocketTimer *>		m_stFrameSocketTimerList;
};

#define	g_FrameSocketTimerMgt					CSingleton<CFrameSocketTimerMgt>::GetInstance()

NETEVENT_NAMESPACE_END

#endif /* NET_TIMER_H_ */
