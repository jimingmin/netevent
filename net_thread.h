/*
 * net_thread.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_THREAD_H_
#define NET_THREAD_H_

#include "net_singleton.h"
#include "net_namespace.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_socket.h"
#include "net_epoll.h"


NETEVENT_NAMESPACE_BEGIN

//enum enmSocketType
//{
//	enmSocketType_invalid = 0,
//	enmSocketType_listen =	1,			//监听socket
//	enmSocketType_common =  2,			//普通socket
//};

//最大的服务器类型个数
//#define MAX_PEERTYPE_COUNT 64

#define RECONNECT_TIME		10//(s)

class CFrameNetThread
{
public:
	CFrameNetThread();
	virtual ~CFrameNetThread();

	int32_t Initialize();

	int32_t Resume();

	int32_t Uninitialize();

	bool Execute();

	IReactor *GetReactor();

protected:
	int32_t MessagePump();

	int32_t SendMessage();

	int32_t HandleTimeOutEvent();

protected:
	IReactor	*m_pReactor;

	int64_t		m_nReconnectTime;
	int64_t		m_nLastConnectTime;
};


#define	g_FrameNetThread					CSingleton<CFrameNetThread>::GetInstance()

NETEVENT_NAMESPACE_END


#endif /* NET_THREAD_H_ */
