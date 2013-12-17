/*
 * net_connmgt.cpp
 *
 *  Created on: 2013Äê12ÔÂ16ÈÕ
 *      Author: jimm
 */

#include "net_connmgt.h"

NETEVENT_NAMESPACE_BEGIN

ConnectionID CConnMgt::g_nConnectionID = 0;

void CConnMgt::RegistConnection(CConnection *pConn)
{
	if(pConn == NULL)
	{
		return;
	}

	ConnectionID nID = ++g_nConnectionID;
	m_stUsedConnMap[nID] = pConn;
	pConn->SetConnectionID(nID);
}

CConnection *CConnMgt::GetConnection(ConnectionID nID)
{
	CConnection *pConn = NULL;
	ConnMap::iterator it = m_stUsedConnMap.find(nID);
	if(it != m_stUsedConnMap.end())
	{
		pConn = it->second;
	}

	return pConn;
}

void CConnMgt::UnregistConnection(ConnectionID nID)
{
	m_stUsedConnMap.erase(nID);
}

void CConnMgt::UnregistConnection(CConnection *pConn)
{
	if(pConn == NULL)
	{
		return;
	}

	ConnectionID nID = pConn->GetConnectionID();
	m_stUsedConnMap.erase(nID);
}

NETEVENT_NAMESPACE_END

