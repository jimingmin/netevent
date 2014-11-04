/*
 * net_connmgt.cpp
 *
 *  Created on: 2013年12月16日
 *      Author: jimm
 */

#include "net_connmgt.h"
#include "../common/common_memmgt.h"

#include <list>
using namespace std;

NETEVENT_NAMESPACE_BEGIN

SessionID CConnMgt::g_nConnectionID = 0;

CConnection *CConnMgt::CreateConnection(CNetHandler *pNetHandler, IPacketParserFactory *pPacketParserFactory, IIOHandler *pIOHandler)
{
	CConnection *pConn = NULL;
	if(m_stUnusedConnList.empty())
	{
		uint8_t *pMem = MALLOC(sizeof(CConnection));
		if(pMem != NULL)
		{
			IPacketParser *pPacketParser = pPacketParserFactory->Create();
			pConn = new(pMem) CConnection(pNetHandler, pPacketParser, pIOHandler);
		}
	}
	else
	{
		pConn = m_stUnusedConnList.front();
		m_stUnusedConnList.pop_front();

		pConn->SetClosing(false);
		pConn->SetNetHandler(pNetHandler);
		pConn->SetPacketParser(pPacketParserFactory->Create());
		pConn->SetIOHandler(pIOHandler);
	}

	m_stParserConnMap[pConn] = pPacketParserFactory;

	return pConn;
}

void CConnMgt::DestroyConnection(CConnection *pConn, IPacketParser *pPacketParser)
{
	ParserConnMap::iterator it = m_stParserConnMap.find(pConn);
	if(it != m_stParserConnMap.end())
	{
		m_stParserConnMap[pConn]->Destory(pPacketParser);
		m_stParserConnMap.erase(it);
	}
	m_stUnusedConnList.push_back(pConn);
}

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

bool CConnMgt::UnregistConnection(SessionID nID)
{
	return m_stUsedConnMap.erase(nID) > 0;
}

bool CConnMgt::UnregistConnection(CConnection *pConn)
{
	if(pConn == NULL)
	{
		return false;
	}

	SessionID nID = pConn->GetSessionID();
	return m_stUsedConnMap.erase(nID) > 0;
}

list<CConnection *> CConnMgt::GetConnList()
{
	list<CConnection *> stConnList;
	ConnMap::iterator it = m_stUsedConnMap.begin();
	for(; it != m_stUsedConnMap.end(); ++it)
	{
		stConnList.push_back(it->second);
	}
	return stConnList;
}

NETEVENT_NAMESPACE_END

