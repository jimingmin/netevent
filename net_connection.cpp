/*
 * net_connection.cpp
 *
 *  Created on: 2013年12月16日
 *      Author: jimm
 */

#include "net_connection.h"
#include "net_handler.h"
#include "net_eventid.h"
#include "net_connmgt.h"
#include "../common/common_memmgt.h"

NETEVENT_NAMESPACE_BEGIN

CConnection::CConnection(CNetHandler *pNetHandler, IPacketParser *pPacketParser, IIOHandler *pIOHandler)
{
	m_stRecvBuffer.Reset();

	m_pNetHandler = pNetHandler;
	m_pPacketParser = pPacketParser;
	m_pIOHandler = pIOHandler;

	m_bIsClosing = false;
}

int32_t CConnection::GetSize()
{
	return sizeof(*this);
}

void CConnection::SetPacketParser(IPacketParser *pPacketParser)
{
	m_pPacketParser = pPacketParser;
}

IPacketParser *CConnection::GetPacketParser()
{
	return m_pPacketParser;
}

void CConnection::SetIOHandler(IIOHandler *pIOHandler)
{
	m_pIOHandler = pIOHandler;
}

IIOHandler *CConnection::GetIOHandler()
{
	return m_pIOHandler;
}

void CConnection::SetClosing(bool bClosing)
{
	m_bIsClosing = bClosing;
}

bool CConnection::GetClosing()
{
	return m_bIsClosing;
}

CNetHandler *CConnection::GetNetHandler()
{
	return m_pNetHandler;
}

void CConnection::Close(int32_t nCloseCode)
{
	CConnMgt &stConnMgt = m_pNetHandler->GetConnMgt();
	if(stConnMgt.UnregistConnection(this))
	{
		CSocket::Close(nCloseCode);

		stConnMgt.DestroyConnection(this, m_pPacketParser);

		m_stRecvBuffer.Reset();

		m_pNetHandler = NULL;
		m_pPacketParser = NULL;
		m_pIOHandler = NULL;
	}
}

//异步关闭
void CConnection::AsyncClose(int32_t nCloseCode)
{
	if(m_bIsClosing)
	{
		return;
	}

	m_bIsClosing = true;

	CloseEvent *pCloseEvent = NEW(CloseEvent);
	pCloseEvent->m_nSessionID = m_nSessionID;
	pCloseEvent->m_nCloseCode = nCloseCode;

	m_pNetHandler->PushCloseEvent(pCloseEvent);
}

int32_t CConnection::Write(uint8_t *pBuf, int32_t nBufSize)
{
	uint8_t *pMem = MALLOC(sizeof(NetPacket) + nBufSize);

	NetPacket *pPacket = new(pMem) NetPacket();
	pPacket->m_nNetPacketLen = nBufSize;
	pPacket->m_nSessionID = m_nSessionID;
	memcpy(pPacket->m_pNetPacket, pBuf, nBufSize);
	//pPacket->m_pNetPacket[nBufSize] = '\0';//主要是针对消息是字符串的包，增加安全性

	m_pNetHandler->PushPacket(pPacket);

	return nBufSize;
}

int32_t CConnection::WriteCompleted(uint8_t *pBuf, int32_t nBufSize)
{
	m_pIOHandler->OnSent(this, pBuf, nBufSize);

	return nBufSize;
}

//读事件回调
int32_t CConnection::OnRead(int32_t nErrorCode)
{
	int32_t nBufSize = 0;
	uint8_t arrBuf[64 * 1024];

	if(m_stRecvBuffer.Size() > 0)
	{
		nBufSize = m_stRecvBuffer.Read(arrBuf, sizeof(arrBuf));
	}
	int32_t nCloseCode = 0;
	int32_t nRecvBytes = 0;
	int32_t nRet = Recv(&arrBuf[nBufSize], sizeof(arrBuf) - nBufSize, nRecvBytes);
	if(nRet != S_OK)
	{
		nCloseCode = nRet;
	}

	nRecvBytes += nBufSize;

	if(nRecvBytes > 0)
	{
		int32_t nOffset = 0;
		//提取消息包
		do
		{
			int32_t nPacketSize = 0;
			uint8_t arrPacket[enmMaxMessageSize];
			m_pPacketParser->Parser(&arrBuf[nOffset], nRecvBytes - nOffset, arrPacket, nPacketSize);
			if(nPacketSize <= 0)
			{
				m_stRecvBuffer.Write(&arrBuf[nOffset], nRecvBytes - nOffset);
				break;
			}

			nOffset += nPacketSize;

			m_pIOHandler->OnRecved(this, arrPacket, nPacketSize);
			if(m_nSessionStatus != enmSessionStatus_Connected)
			{
				return S_OK;
			}
		}while(true);
	}

	if(nCloseCode != 0)
	{
		Close(nCloseCode);
		return E_SOCKETERROR;
	}

	return S_OK;
}

//写事件回调
int32_t CConnection::OnWrite(int32_t nErrorCode)
{
	if(nErrorCode != 0)
	{
		Close(SYS_EVENT_CONN_ERROR);
		return E_SOCKETERROR;
	}

	int32_t nRet = SendRestData();
	if(nRet < 0)
	{
		Close(SYS_EVENT_CONN_ERROR);
		return E_SOCKETERROR;
	}

	return S_OK;
}

//异常事件回调
int32_t CConnection::OnError(int32_t nErrorCode)
{
	m_pIOHandler->OnError(this);

	return S_OK;
}

//连接成功回调(重载此函数，可以在这里做些连接信息初始化的工作)
int32_t CConnection::OnConnected()
{
	SetSourceRole(true);
	m_pNetHandler->GetConnMgt().RegistConnection(this);

	m_pIOHandler->OnOpened(this);

	return S_OK;
}
//连接超时回调
int32_t CConnection::OnConnectTimeout()
{
	m_pIOHandler->OnTimeout(this);

	return S_OK;
}

//断开连接回调(重载此函数，可以在这里做些资源回收的工作)
int32_t CConnection::OnDisconnect(int32_t nCloseCode)
{
	m_pNetHandler->DeleteEvent(this);
	m_pIOHandler->OnClosed(this);

	return S_OK;
}

NETEVENT_NAMESPACE_END
