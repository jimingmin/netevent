/*
 * net_connmgt.cpp
 *
 *  Created on: 2013Äê12ÔÂ16ÈÕ
 *      Author: jimm
 */

#include "net_connmgt.h"

NETEVENT_NAMESPACE_BEGIN

SessionID CConnMgt::g_nConnectionID = 0;

void CConnMgt::RegistConnection(CConnection *pConn)
{
	if(pConn == NULL)
	{
		return;
	}

	SessionID nID = ++g_nConnectionID;
	m_stUsedConnMap[nID] = pConn;
	pConn->SetSessionID(nID);
}

CConnection *CConnMgt::GetConnection(SessionID nID)
{
	CConnection *pConn = NULL;
	ConnMap::iterator it = m_stUsedConnMap.find(nID);
	if(it != m_stUsedConnMap.end())
	{
		pConn = it->second;
	}

	return pConn;
}

void CConnMgt::UnregistConnection(SessionID nID)
{
	m_stUsedConnMap.erase(nID);
}

void CConnMgt::UnregistConnection(CConnection *pConn)
{
	if(pConn == NULL)
	{
		return;
	}

	SessionID nID = pConn->GetSessionID();
	m_stUsedConnMap.erase(nID);
}

NETEVENT_NAMESPACE_END

