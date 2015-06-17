/*
 * net_thread.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_HANDLER_H_
#define NET_HANDLER_H_

#include "net_namespace.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_socket.h"
#include "net_epoll.h"
#include "net_closeevent.h"
#include "net_connmgt.h"
#include "../common/common_runnable.h"
#include "../common/common_export.h"

#include <list>
using namespace std;

struct NetFuncEntry;

NETEVENT_NAMESPACE_BEGIN

#define RECONNECT_TIME		10//(s)

enum
{
	enmReactorType_Select		= 0x00,		//select模式
	enmReactorType_Epoll		= 0x01,		//epoll模式
};

class IReactor;

class CNetHandler : public IRunnable
{
	friend class CConnection;
public:
	EXPORT CNetHandler();
	EXPORT virtual ~CNetHandler();

	EXPORT int32_t CreateReactor(int32_t nReacotType = enmReactorType_Epoll);

	EXPORT int32_t DestoryReactor();

	EXPORT int32_t Run();

	EXPORT IReactor *GetReactor();

	EXPORT int32_t RegistEvent(CSocket *pSocket, uint32_t nEvents);

	EXPORT int32_t RemoveEvent(CSocket *pSocket, uint32_t nEvents);

	EXPORT int32_t DeleteEvent(CSocket *pSocket);

	CConnectTimerMgt &GetConnectTimerMgt();

	CConnMgt &GetConnMgt();

	void PushCloseEvent(CloseEvent *pCloseEvent);

	void SetNetHandlerCallBack(NetFuncEntry *pCallBack);

	NetFuncEntry *GetNetHandlerCallBack();

protected:
	void PushPacket(NetPacket *pPacket);

	int32_t SendPacket(NetPacket *pPacket);

	int32_t MessagePump();

	int32_t HandleTimeoutEvent();

	int32_t HandleAsyncCloseEvent();

protected:
	IReactor	*m_pReactor;

	int64_t		m_nReconnectTime;
	int64_t		m_nLastConnectTime;

	list<NetPacket *>	m_stSendQueue;
	list<CloseEvent *>	m_stCloseQueue;
	CConnectTimerMgt	m_stConnectTimerMgt;

	CConnMgt			m_stConnMgt;
	NetFuncEntry		*m_pCallBackSet;
};

NETEVENT_NAMESPACE_END


#endif /* NET_HANDLER_H_ */
