/*
 * net_select.cpp
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#include "net_select.h"
#include "net_namespace.h"
#include "net_socket.h"
#include "../common/common_api.h"

NETEVENT_NAMESPACE_BEGIN

CSelect::CSelect()
{
	m_nMaxFD = 0;
}

CSelect::~CSelect()
{

}

int32_t CSelect::Create(int32_t nMaxConn)
{
	m_nMaxFD = nMaxConn;
	if(m_nMaxFD > FD_SETSIZE)
	{
		m_nMaxFD = FD_SETSIZE;
	}

	return 0;
}

int32_t CSelect::RegistEvent(CSocket *pSocket, uint32_t nEvents)
{
	uint32_t nIOEvents = pSocket->GetIOEvents();
	nIOEvents |= nEvents;
	pSocket->SetIOEvents(nIOEvents);

	SocketFD nSocketFD = pSocket->GetSocketFD();
	m_stSocketMap[nSocketFD] = pSocket;

	return 0;
}

int32_t CSelect::RemoveEvent(CSocket *pSocket, uint32_t nEvents)
{
	uint32_t nIOEvents = pSocket->GetIOEvents();
	nIOEvents &= (~nEvents);
	pSocket->SetIOEvents(nIOEvents);

	SocketFD nSocketFD = pSocket->GetSocketFD();
	m_stSocketMap[nSocketFD] = pSocket;

	return 0;
}

int32_t CSelect::DeleteEvent(CSocket *pSocket)
{
	uint32_t nIOEvents = pSocket->GetIOEvents();
	pSocket->SetIOEvents(0);

	SocketFD nSocketFD = pSocket->GetSocketFD();

	FD_CLR(nSocketFD, &m_stReadSet);
	FD_CLR(nSocketFD, &m_stReadSet);
	FD_CLR(nSocketFD, &m_stReadSet);

	return 0;
}

int32_t CSelect::HandleMessage(int32_t nWaitTimeout)
{
	if(m_stSocketMap.empty())
	{
		Delay(10000);
		return 0;
	}

	FD_ZERO(&m_stReadSet);
	FD_ZERO(&m_stWriteSet);
	FD_ZERO(&m_stExceptSet);

	int32_t nMaxFD = 0;

	SocketMap::iterator it = m_stSocketMap.begin();
	for(; it != m_stSocketMap.end(); )
	{
		CSocket *pSocket = it->second;
		uint32_t nIOEvents = pSocket->GetIOEvents();
		if(nIOEvents == 0)
		{
			m_stSocketMap.erase(it++);
			continue;
		}

		SocketFD nSocketFD = pSocket->GetSocketFD();
		if((nIOEvents & mask_read) == mask_read)
		{
			FD_SET(nSocketFD, &m_stReadSet);
		}

		if((nIOEvents & mask_write) == mask_write)
		{
			FD_SET(nSocketFD, &m_stWriteSet);
		}

		//FD_SET(nSocketFD, &m_stExceptSet);

		if(nSocketFD > (uint32_t)nMaxFD)
		{
			nMaxFD = nSocketFD;
		}

		++it;
	}

	if(nMaxFD++ <= 0)
	{
		return 0;
	}

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = nWaitTimeout;

	int32_t nEventCount = select(nMaxFD, &m_stReadSet, &m_stWriteSet, &m_stExceptSet, &timeout);
	if(nEventCount <= 0)
	{
		return 0;
	}

	it = m_stSocketMap.begin();
	for(; it != m_stSocketMap.end();)
	{
		CSocket *pSocket = it->second;
		SocketFD nSocketFD = pSocket->GetSocketFD();

		if(FD_ISSET(nSocketFD, &m_stReadSet))
		{
			pSocket->ReadEvent();

			SessionStatus nSessionStatus = pSocket->GetSessionStatus();
			if((nSessionStatus == enmSessionStatus_Closed) || (nSessionStatus == enmSessionStatus_Error))
			{
				m_stSocketMap.erase(it++);
				continue;
			}
		}

		if(FD_ISSET(nSocketFD, &m_stWriteSet))
		{
			pSocket->WriteEvent();

			SessionStatus nSessionStatus = pSocket->GetSessionStatus();
			if((nSessionStatus == enmSessionStatus_Closed) || (nSessionStatus == enmSessionStatus_Error))
			{
				m_stSocketMap.erase(it++);
				continue;
			}
		}

		if(FD_ISSET(nSocketFD, &m_stExceptSet))
		{
			pSocket->ErrorEvent();

			SessionStatus nSessionStatus = pSocket->GetSessionStatus();
			if((nSessionStatus == enmSessionStatus_Closed) || (nSessionStatus == enmSessionStatus_Error))
			{
				m_stSocketMap.erase(it++);
				continue;
			}
		}

		++it;
	}

	return 0;
}

NETEVENT_NAMESPACE_END
