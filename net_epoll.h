/*
 * net_epoll.h
 *
 *  Created on: 2013Äê11ÔÂ4ÈÕ
 *      Author: jimm
 */

#ifndef NET_EPOLL_H_
#define NET_EPOLL_H_

#include "net_namespace.h"
#include "net_typedef.h"
#include "net_reactor.h"

#include <sys/epoll.h>
#include <string.h>

NETEVENT_NAMESPACE_BEGIN

class CSocket;

class CEpoll : public IReactor
{
public:
	CEpoll();

	virtual ~CEpoll();

	virtual int32_t Create(int32_t nMaxConn);

	virtual int32_t RegistEvent(CSocket *pSocket, uint32_t nEvents);

	virtual int32_t RemoveEvent(CSocket *pSocket, uint32_t nEvents);

	virtual int32_t DeleteEvent(CSocket *pSocket);

	virtual int32_t HandleMessage(int32_t nWaitTimeout);

	int32_t GetEpollFD();

protected:
	int32_t				m_nEpollEventTimeout;
	int32_t				m_nMaxFD;
	int32_t				m_nEpollFD;
	struct epoll_event	*m_pEpollEvent;
};

NETEVENT_NAMESPACE_END


#endif /* NET_EPOLL_H_ */
