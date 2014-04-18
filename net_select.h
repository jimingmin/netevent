/*
 * net_select.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_SELECT_H_
#define NET_SELECT_H_

#include "net_namespace.h"
#include "net_typedef.h"
#include "net_reactor.h"
//#include <sys/time.h>
#include <sys/types.h>
//#include <unistd.h>
#ifdef unix
#include <sys/select.h>
#endif
#include <map>

using namespace std;

NETEVENT_NAMESPACE_BEGIN

class CSocket;

typedef map<SocketFD, CSocket*>		SocketMap;

class CSelect : public IReactor
{
public:
	CSelect();

	virtual ~CSelect();

	virtual int32_t Create(int32_t nMaxConn);

	virtual int32_t RegistEvent(CSocket *pSocket, uint32_t nEvents);

	virtual int32_t RemoveEvent(CSocket *pSocket, uint32_t nEvents);

	virtual int32_t DeleteEvent(CSocket *pSocket);

	virtual int32_t HandleMessage(int32_t nWaitTimeout);

protected:
	SocketMap		m_stSocketMap;
	fd_set			m_stReadSet;
	fd_set			m_stWriteSet;
	fd_set			m_stExceptSet;
	int32_t			m_nMaxFD;
};

NETEVENT_NAMESPACE_END

#endif /* NET_SELECT_H_ */
