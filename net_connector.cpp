/*
 * net_connector.cpp
 *
 *  Created on: 2013年12月16日
 *      Author: jimm
 */

#include "net_connector.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_timer.h"
#include "net_handler.h"
#include "net_connection.h"

#include <errno.h>

NETEVENT_NAMESPACE_BEGIN

CConnector::CConnector(CNetHandler *pNetHandler, IPacketParserFactory *pFactory, IIOHandler *pIOHandler)
{
	m_pNetHandler = pNetHandler;
	m_pPacketParserFactory = pFactory;
	m_pIOHandler = pIOHandler;
}

int32_t CConnector::Connect(const char *szRemoteIP, uint16_t nPort)
{
	if (NULL == szRemoteIP)
	{
		return E_REMOTEIP;
	}

	IPacketParser *pPacketParser = m_pPacketParserFactory->Create();
	CSocket *pSocket = new CConnection(m_pNetHandler, pPacketParser, m_pIOHandler);
	if(pSocket == NULL)
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_NULLPOINTER;
	}

	pSocket->OpenSocket();

	//更新套接字类型和状态
	pSocket->SetSessionType(enmSessionType_Communicate);

	//判断套接字类型
	if (enmSessionType_Communicate != pSocket->GetSessionType())
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_SOCKETTYPE;
	}

	//套接字是否打开
	if ((enmInvalidSocketFD == pSocket->GetSocketFD()) || (enmSessionStatus_Opened != pSocket->GetSessionStatus()))
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_SOCKETNOTCREATED;
	}

	pSocket->SetPeerAddress(szRemoteIP);
	pSocket->SetPeerPort(nPort);
	pSocket->SetPeerAddress((uint32_t)inet_addr(szRemoteIP));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(szRemoteIP);
	addr.sin_port = htons(nPort);

	CConnectTimer *pConnTimer = g_ConnectTimerMgt.CreateConnectTimer(pSocket,
			static_cast<ConnectTimerProc>(&CConnection::OnTimerEvent), 3);
	if(pConnTimer == NULL)
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_UNKNOWN;
	}

	pSocket->SetConnectTimer(pConnTimer);

	//与服务器端建立连接
	int32_t ret = connect(pSocket->GetSocketFD(), (const struct sockaddr*)&addr, sizeof(addr));
	if (0 != ret)
	{
		if (errno != EINPROGRESS)
		{
			m_pPacketParserFactory->Destory(pPacketParser);
			pSocket->CloseSocket();
			return E_SOCKETCONNECT;
		}
		else
		{
			m_pNetHandler->RegistEvent(pSocket, mask_read | mask_write);
			pSocket->SetSessionStatus(enmSessionStatus_Connecting);
			return E_SOCKET_CONNECTING;
		}
	}

	m_pNetHandler->RegistEvent(pSocket, mask_read | mask_write);
	//更新套接字状态
	pSocket->SetSessionStatus(enmSessionStatus_Connected);
	return S_OK;
}

//读事件回调
int32_t CConnector::OnRead(int32_t nErrorCode)
{
	return S_OK;
}

//写事件回调
int32_t CConnector::OnWrite(int32_t nErrorCode)
{
	return S_OK;
}

//异常事件回调
int32_t CConnector::OnError(int32_t nErrorCode)
{
	return S_OK;
}


NETEVENT_NAMESPACE_END
