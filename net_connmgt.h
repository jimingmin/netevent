/*
 * net_connmgt.h
 *
 *  Created on: 2013年12月16日
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

typedef map<SessionID, CConnection *>	ConnMap;
typedef list<CConnection *>		UnusedConnList;
typedef map<CConnection *, IPacketParserFactory *>	ParserConnMap;

class CConnMgt
{
public:
	CConnection *CreateConnection(CNetHandler *pNetHandler, IPacketParserFactory *pPacketParserFactory, IIOHandler *pIOHandler);

	void DestroyConnection(CConnection *pConn, IPacketParser *pPacketParser);

	void RegistConnection(CConnection *pConn);

	CConnection *GetConnection(SessionID nID);

	bool UnregistConnection(SessionID nID);

	bool UnregistConnection(CConnection *pConn);

	list<CConnection *> GetConnList();

protected:
	static SessionID			g_nConnectionID;
	ConnMap						m_stUsedConnMap;
	IPacketParserFactory		*m_pPacketParserFactory;
	UnusedConnList				m_stUnusedConnList;
	ParserConnMap				m_stParserConnMap;
};

#define g_ConnMgt			CSingleton<CConnMgt>::GetInstance()

NETEVENT_NAMESPACE_END

#endif /* NET_CONNMGT_H_ */
