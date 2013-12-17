/*
 * net_connmgt.h
 *
 *  Created on: 2013Äê12ÔÂ16ÈÕ
 *      Author: jimm
 */

#ifndef NET_CONNMGT_H_
#define NET_CONNMGT_H_

#include "net_namespace.h"
#include "net_typedef.h"
#include "net_connection.h"
#include "../common/common_singleton.h"

#include <map>
using namespace std;

NETEVENT_NAMESPACE_BEGIN

typedef map<ConnectionID, CConnection *>	ConnMap;

class CConnMgt
{
public:
	void RegistConnection(CConnection *pConn);

	CConnection *GetConnection(ConnectionID nID);

	void UnregistConnection(ConnectionID nID);

	void UnregistConnection(CConnection *pConn);

protected:
	static ConnectionID			g_nConnectionID;
	ConnMap						m_stUsedConnMap;
};

#define g_ConnMgt			CSingleton<CConnMgt>::GetInstance()

NETEVENT_NAMESPACE_END

#endif /* NET_CONNMGT_H_ */
