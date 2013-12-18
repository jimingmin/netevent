/*
 * net_thread.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_THREAD_H_
#define NET_THREAD_H_

#include "net_namespace.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_socket.h"
#include "net_epoll.h"
#include "net_queue.h"

NETEVENT_NAMESPACE_BEGIN

#define RECONNECT_TIME		10//(s)

enum
{
	enmReactorType_Select		= 0x00,		//select模式
	enmReactorType_Epoll		= 0x01,		//epoll模式
};


class CNetHandler
{
public:
	CNetHandler();
	virtual ~CNetHandler();

	int32_t CreateReactor(int32_t nReacotType = enmReactorType_Epoll);

	int32_t DestoryReactor();

	bool Process();

	IReactor *GetReactor();

	int32_t RegistEvent(CSocket *pSocket, uint32_t nEvents);

	int32_t RemoveEvent(CSocket *pSocket, uint32_t nEvents);

	int32_t DeleteEvent(CSocket *pSocket);

	void PushPacket(NetPacket *pPacket);

protected:
	int32_t MessagePump();

	int32_t SendPacket();

	int32_t HandleTimeOutEvent();

protected:
	IReactor	*m_pReactor;

	int64_t		m_nReconnectTime;
	int64_t		m_nLastConnectTime;

	CNetQueue	m_stSendQueue;
};

NETEVENT_NAMESPACE_END


#endif /* NET_THREAD_H_ */
