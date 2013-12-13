/*
 * net_socket.cpp
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#include "net_api.h"
#include "net_socket.h"
#include "net_eventid.h"

#include <time.h>

NETEVENT_NAMESPACE_BEGIN

CSocket::CSocket()
{
	Reset();
}

CSocket::~CSocket()
{
	if (enmInvalidSocketFD != m_nSocketFD)
	{
		CloseSocket(SYS_EVENT_CONN_CONFLICT);
	}
}

//�����׽���������
void CSocket::SetSocketFD(SocketFD nSocketFD)
{
	m_nSocketFD = nSocketFD;
}

//��ȡ�׽���������
SocketFD CSocket::GetSocketFD() const
{
	return m_nSocketFD;
}

//�����׽���״̬
void CSocket::SetSocketStatus(SocketStatus nSocketStatus)
{
	m_nSocketStatus = nSocketStatus;
}

//��ȡ�׽���״̬
SocketStatus CSocket::GetStatus() const
{
	return m_nSocketStatus;
}

void CSocket::SetSocketType(SocketType nSocketType)
{
	m_nSocketType = nSocketType;
}

SocketType CSocket::GetSocketType()
{
	return m_nSocketType;
}

void CSocket::SetSocketAttr(SocketAttr nSocketAttr)
{
	m_nSocketAttr = nSocketAttr;
}

SocketAttr CSocket::GetSocketAttr()
{
	return m_nSocketAttr;
}

char *CSocket::GetPeerAddressStr()
{
	return m_szClientAddress;
}

void CSocket::SetPeerAddress(uint32_t nAddress)
{
	m_nPeerAddress = nAddress;

	char *pAddress = inet_ntoa_f(m_nPeerAddress);
	strncpy(m_szClientAddress, pAddress, sizeof(m_szClientAddress));
}

uint32_t CSocket::GetPeerAddress()
{
	return m_nPeerAddress;
}

void CSocket::SetPeerPort(uint16_t nPort)
{
	m_nPeerPort = nPort;
}

uint16_t CSocket::GetPeerPort()
{
	return m_nPeerPort;
}

uint32_t CSocket::GetListenAddress()
{
	return m_nListenAddress;
}

uint16_t CSocket::GetListenPort()
{
	return m_nListenPort;
}

void CSocket::SetBelongPort(uint16_t nPort)
{
	m_nBelongPort = nPort;
}

uint16_t CSocket::GetBelongPort()
{
	return m_nBelongPort;
}

void CSocket::SetCreateTime(time_t nCreateTime)
{
	m_nCreateTime = nCreateTime;
}

void CSocket::SetNetHandler(CNetHandler *pNetHandler)
{
	m_pNetHandler = pNetHandler;
}

int32_t CSocket::ReadEvent()
{
	int32_t nRet = S_OK;

	//�Ǽ����׽��ֵĶ��¼�
	if(enmSocketType_Listen == m_nSocketType)
	{
		struct sockaddr_in stPeerAddress;
		socklen_t nPeerAddressLen = (socklen_t)sizeof(stPeerAddress);

		SocketFD nAcceptFD = enmInvalidSocketFD;
		do
		{
			nAcceptFD = accept(m_nSocketFD, (struct sockaddr *)&stPeerAddress, &nPeerAddressLen);
			if(nAcceptFD < 0)
			{
				break;
			}

			nRet = OnAccept(nAcceptFD, stPeerAddress, nPeerAddressLen);
			if(nRet < 0)
			{
				close(nAcceptFD);
			}
		}while(nAcceptFD > 0);

	}
	//�Ǽ����׽���
	else
	{
		int32_t nError = 0;
		socklen_t nLen = sizeof(int32_t);

		if(0 != getsockopt(m_nSocketFD, SOL_SOCKET, SO_ERROR, &nError, &nLen))
		{
			nError = 0;
		}

		nRet = OnRead(nError);
	}

	return nRet;
}

int32_t CSocket::WriteEvent()
{
	int32_t nError = 0;
	socklen_t nLen = sizeof(int32_t);

	if(0 != getsockopt(m_nSocketFD, SOL_SOCKET, SO_ERROR, &nError, &nLen))
	{
		nError = 0;
	}

	if(m_nSocketStatus == enmSocketStatus_Connecting)
	{
		int32_t nError = 0;
//		socklen_t nLen = sizeof(int32_t);
		//���ӳɹ�
		if(0 == nError)
		{
			return Connected();
		}
	}

	return OnWrite(nError);
}

int32_t CSocket::ErrorEvent()
{
	if((m_nSocketStatus == enmSocketStatus_Closed) ||
			(m_nSocketStatus == enmSocketStatus_Opened) ||
			(m_nSocketStatus == enmSocketStatus_Error))
	{
		return S_OK;
	}

	int32_t nError = 0;
	socklen_t nLen = sizeof(int32_t);

	if(0 != getsockopt(m_nSocketFD, SOL_SOCKET, SO_ERROR, &nError, &nLen))
	{
		nError = 0;
	}

	return OnError(nError);
}

//�������ӻص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
int32_t CSocket::OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
		socklen_t nPeerAddressLen)
{
	return S_OK;
}

//���ӳɹ��ص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
int32_t CSocket::OnConnected()
{
	return S_OK;
}

int32_t CSocket::OnConnectTimeout()
{
	return S_OK;
}

//�Ͽ����ӻص�(���ش˺�����������������Щ��Դ���յĹ���)
int32_t CSocket::OnDisconnect(int32_t nCloseCode)
{
	return S_OK;
}

//���ͻ�����ʣ�������
int32_t CSocket::SendRestData()
{
	int32_t nRet = S_OK;
	//int32_t nWriteBytes = 0;
	//�ȷ��ͻ�����ʣ�������
	if(m_stSendBuffer.Size() > 0)
	{
		uint8_t arrPacketBuf[enmRecvBufferSize];
		int32_t nWantSize = sizeof(arrPacketBuf);

		int32_t nRealSize = m_stSendBuffer.Read(arrPacketBuf, nWantSize);
		int32_t nBytes = nWrite(arrPacketBuf, nRealSize);
		if(nBytes < nRealSize)
		{
			int32_t nRewriteIndex = 0;
			if(nBytes > 0)
			{
				nRewriteIndex = nBytes;
			}
			//ʣ������д�ػ���
			m_stSendBuffer.WriteToHead(&arrPacketBuf[nRewriteIndex], nRealSize - nRewriteIndex);
			if(errno != EAGAIN)
			{
				nRet = E_SOCKETERROR;
				//break;
			}
		}
	}
	ChangeWriteEvent();

	return nRet;
}

int32_t CSocket::OpenSocketAsServer(const char* szLocalIP, uint16_t nPort)
{
	//��socket�����Ѵ������ȹر��׽���
	if (enmInvalidSocketFD != m_nSocketFD)
	{
		CloseSocket(SYS_EVENT_CONN_CONFLICT);
	}

	//���׽���
	m_nSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (enmInvalidSocketFD == m_nSocketFD)
	{
		return E_SOCKETCREATE;
	}

	if((m_nSocketType == enmSocketType_Listen) || (nPort > 0))
	{
		//�����׽�������
		int32_t op = 1;
		int32_t ret = setsockopt(m_nSocketFD, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
		if (0 != ret)
		{
			CloseSocket();
			return E_SOCKETOPTION;
		}
		//����������ַ&�˿���Ϣ
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		if((NULL == szLocalIP) || (strlen(szLocalIP) == 0))
		{
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			addr.sin_addr.s_addr = inet_addr(szLocalIP);
		}
		addr.sin_port = htons(nPort);

		ret = bind(m_nSocketFD, (struct sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			CloseSocket();
			return E_SOCKETBIND;
		}

		//��ʼ����
		ret = listen(m_nSocketFD, SOMAXCONN);
		if (0 != ret)
		{
			CloseSocket();
			return E_SOCKETLISTEN;
		}

		m_nListenAddress = addr.sin_addr.s_addr;
		m_nListenPort = nPort;
	}

	//����Ϊ������
	set_non_block(m_nSocketFD);

	//�����׽������ͺ�״̬
	m_nSocketType = enmSocketType_Listen;
	m_nSocketStatus = enmSocketStatus_Opened;

	return S_OK;
}

int32_t CSocket::OpenSocketAsClient(const char* szLocalIP/* = NULL*/)
{
	//��socket�����Ѵ������ȹر��׽���
	if (enmInvalidSocketFD != m_nSocketFD)
	{
		CloseSocket(SYS_EVENT_CONN_CONFLICT);
	}
	//���׽���
	m_nSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (enmInvalidSocketFD == m_nSocketFD)
	{
		return E_SOCKETCREATE;
	}

	if (NULL != szLocalIP)
	{
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(szLocalIP);
		int32_t ret = bind(m_nSocketFD, (struct sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			CloseSocket();
			return E_SOCKETBIND;
		}

	}

	set_non_block(m_nSocketFD);

	//�����׽������ͺ�״̬
	m_nSocketType = enmSocketType_Communicate;
	m_nSocketStatus = enmSocketStatus_Opened;
	return S_OK;
}

//�ر��׽���
void CSocket::CloseSocket(int32_t nCloseCode)
{
	if (enmInvalidSocketFD != m_nSocketFD)
	{
		if(m_nSocketStatus != enmSocketStatus_Closed)
		{
			Disconnected(nCloseCode);
			//�����׽���״̬
			m_nSocketStatus = enmSocketStatus_Closed;
		}
		close(m_nSocketFD);
		m_nSocketFD = enmInvalidSocketFD;

		Clear();
	}
}

//�����˽�������
int32_t CSocket::Connect(const char* szRemoteIP, uint16_t nPort)
{
	if (NULL == szRemoteIP)
	{
		return E_REMOTEIP;
	}

	//�ж��׽�������
	if (enmSocketType_Communicate != m_nSocketType)
	{
		return E_SOCKETTYPE;
	}

	//�׽����Ƿ��
	if ((enmInvalidSocketFD == m_nSocketFD) || (enmSocketStatus_Opened != m_nSocketStatus))
	{
		return E_SOCKETNOTCREATED;
	}

	strcpy(m_szClientAddress, szRemoteIP);
	m_nPeerPort = nPort;
	m_nPeerAddress = (uint32_t)inet_addr(szRemoteIP);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(szRemoteIP);
	addr.sin_port = htons(nPort);

	m_pSocketTimer = g_FrameSocketTimerMgt.CreateSocketTimer(this,
			static_cast<SocketTimerProc>(&CSocket::OnTimerEvent), 3);
	if(m_pSocketTimer == NULL)
	{
		return E_UNKNOWN;
	}

	//��������˽�������
	int32_t ret = connect(m_nSocketFD, (const struct sockaddr*)&addr, sizeof(addr));
	if (0 != ret)
	{
		if (errno != EINPROGRESS)
		{
			CloseSocket();
			return E_SOCKETCONNECT;
		}
		else
		{
			//���peer�����socket�����ӳ��
			//g_FrameSocketMgt.AddSocket(m_nPeerType, m_nPeerID, this);

			m_nSocketStatus = enmSocketStatus_Connecting;
			return E_SOCKET_CONNECTING;
		}
	}

	//���peer�����socket�����ӳ��
	//g_FrameSocketMgt.AddSocket(m_nPeerType, m_nPeerID, this);

	//�����׽���״̬
	m_nSocketStatus = enmSocketStatus_Connected;

	Connected();

	return S_OK;
}

//��ʱ���¼�
int32_t CSocket::OnTimerEvent(CFrameSocketTimer *pTimer)
{
	return ConnectTimeout();
}

//���ӳ�ʱ
int32_t CSocket::ConnectTimeout()
{
	if(m_pSocketTimer != NULL)
	{
		g_FrameSocketTimerMgt.DestroySocketTimer(m_pSocketTimer);
		m_pSocketTimer = NULL;
	}

	//��η����������������£�����ĳһ�����ӳɹ��ˣ�����һЩ��������ʱ
	if((m_nSocketStatus == enmSocketStatus_Connected) ||
			(m_nSocketStatus == enmSocketStatus_Available))
	{
		return S_OK;
	}

	m_nSocketStatus = enmSocketStatus_Closed;

	if(m_pNetHandler != NULL)
	{
		m_pNetHandler->DeleteEvent(this);
	}

	return OnConnectTimeout();
}

//���ӳɹ��Ļص�
int32_t CSocket::Connected()
{
	if(m_pSocketTimer != NULL)
	{
		g_FrameSocketTimerMgt.DestroySocketTimer(m_pSocketTimer);
		m_pSocketTimer = NULL;
	}

	if((m_nSocketStatus != enmSocketStatus_Connected) &&
			(m_nSocketStatus != enmSocketStatus_Available))
	{
		m_nSocketStatus = enmSocketStatus_Connected;
	}

	sockaddr_in stLocalAddr;
	memset(&stLocalAddr, 0, sizeof(stLocalAddr));
	int32_t nAddrLen = sizeof(stLocalAddr);

	int32_t nRet = getsockname(m_nSocketFD, (sockaddr *)&stLocalAddr, (socklen_t *)&nAddrLen);
	if(nRet != 0)
	{
		CloseSocket(SYS_EVENT_CONN_CONFLICT);

		return S_OK;
	}

	//������ص�ַ�Ͷ˿ڵ���Զ�̵�ַ�Ͷ˿ڣ��ͶϿ�����
	if(((strcmp(m_szClientAddress, "127.0.0.1") == 0) ||
			(strcmp(m_szClientAddress, inet_ntoa(stLocalAddr.sin_addr)) == 0)) &&
			(stLocalAddr.sin_port == htons(m_nPeerPort)))
	{
		CloseSocket(SYS_EVENT_CONN_CONFLICT);

		return S_OK;
	}

	return OnConnected();
}

//���ӳɹ��Ļص�
int32_t CSocket::Disconnected(int32_t nCloseCode)
{
	if(m_pSocketTimer != NULL)
	{
		g_FrameSocketTimerMgt.DestroySocketTimer(m_pSocketTimer);
		m_pSocketTimer = NULL;
	}

	return OnDisconnect(nCloseCode);
}

//��������
int32_t CSocket::Recv(uint8_t *pBuffer, int32_t nSize, int32_t& nRecvBytes)
{
	nRecvBytes = 0;

	//�׽����Ƿ��
	if (enmInvalidSocketFD == m_nSocketFD)
	{
		return E_SOCKETNOTCREATED;
	}

	if((enmSocketStatus_Available != m_nSocketStatus)&&(enmSocketStatus_Connected != m_nSocketStatus))
	{
		struct sockaddr_in sa;
		socklen_t nLen = sizeof(sa);
		if(!getpeername(m_nSocketFD, (struct sockaddr *)&sa, &nLen))
		{
			Connected();
			return S_OK;
		}

		return E_SOCKETERROR;
	}

	m_nLastRecvTime = time(NULL);

	//��������
	bool isConnClose = false;
	int32_t n = nRead(pBuffer, nSize,isConnClose);
	if (0 < n)
	{
		nRecvBytes = (uint32_t)n;
		if(isConnClose)
		{
			return E_SOCKETDISCONNECTED;
		}
		if(nRecvBytes < nSize)
		{
			if(errno != EAGAIN)
			{
				return E_SOCKETERROR;
			}
		}
		return S_OK;
	}
	else if (0 == n)
	{
		//�����ѶϿ�
		return E_SOCKETDISCONNECTED;
	}
	else if (EAGAIN != errno)
	{
		//���ճ���
		return E_SOCKETERROR;
	}

	return S_OK;
}
//��������
int32_t CSocket::Send(const uint8_t *pBuffer, const int32_t nLength, int32_t& nSendBytes)
{
	nSendBytes = 0;

	//�׽����Ƿ��
	if (enmInvalidSocketFD == m_nSocketFD)
	{
		return E_SOCKETNOTCREATED;
	}

	if((enmSocketStatus_Available != m_nSocketStatus)&&(enmSocketStatus_Connected != m_nSocketStatus))
	{
		struct sockaddr_in sa;
		socklen_t nLen = sizeof(sa);
		if(!getpeername(m_nSocketFD, (struct sockaddr *)&sa, &nLen))
		{
			Connected();
			return S_OK;
		}

		return E_SOCKETERROR;
	}

	//���ͻ��滹������
	if(m_stSendBuffer.Size() > 0)
	{
		uint8_t arrSendBuf[enmRecvBufferSize];
		int32_t nWantSendSize = sizeof(arrSendBuf);
		int32_t nRealRestSize = m_stSendBuffer.Read(arrSendBuf, nWantSendSize);
		if(nRealRestSize > 0)
		{
			int32_t nSendSizes = nWrite(arrSendBuf, nRealRestSize);
			if(nSendSizes < nRealRestSize)
			{
				int32_t nRewriteIndex = 0;
				if(nSendSizes > 0)
				{
					nRewriteIndex = nSendSizes;
				}
				//ʣ������д�ط��ͻ���
				m_stSendBuffer.WriteToHead(&arrSendBuf[nRewriteIndex], nRealRestSize - nRewriteIndex);

				ChangeWriteEvent();
				if(errno != EAGAIN)
				{
					return E_SOCKETERROR;
				}
			}
		}
	}

	if ((NULL == pBuffer) || (0 == nLength))
	{
		return S_FALSE;
	}

	if(m_stSendBuffer.Size() > 0)
	{
		nSendBytes = nLength;
		ChangeWriteEvent();
		return m_stSendBuffer.Write(pBuffer, nLength);
	}

	nSendBytes = nWrite(pBuffer, nLength);
	if(nSendBytes < nLength)
	{
		int32_t nRewriteIndex = 0;
		if(nSendBytes > 0)
		{
			nRewriteIndex = nSendBytes;
		}
		//ʣ������д�ط��ͻ���
		m_stSendBuffer.WriteToHead(&pBuffer[nRewriteIndex], nLength - nRewriteIndex);

		ChangeWriteEvent();
		if(errno != EAGAIN)
		{
			return E_SOCKETERROR;
		}
	}
	ChangeWriteEvent();

	return S_OK;
}

uint32_t CSocket::GetIOEvents()
{
	return m_nIOEvents;
}

void CSocket::SetIOEvents(uint32_t nEvents)
{
	m_nIOEvents = nEvents;
}

//���Ծ����Ŭ����ȡ���������
int32_t CSocket::nRead(uint8_t *pBuffer, const int32_t nLength,bool &isConnClose)
{
	int32_t nReadBytes = 0;
	isConnClose = false;
	if ((NULL == pBuffer) || (0 == nLength))
	{
		return nReadBytes;
	}

	//�׽����Ƿ��
	if ((enmInvalidSocketFD == m_nSocketFD) || ((enmSocketStatus_Available != m_nSocketStatus)&&(enmSocketStatus_Connected != m_nSocketStatus)))
	{
		return nReadBytes;
	}

	do
	{
		int32_t nRecvBytes = recv(m_nSocketFD, pBuffer + nReadBytes, nLength - nReadBytes, 0);
		if(nRecvBytes > 0)
		{
			nReadBytes += nRecvBytes;
		}
		else if(nRecvBytes <= 0)
		{
			if(nRecvBytes == 0)
			{
				isConnClose = true;
			}
//			if(errno != EAGAIN)
//			{
//				CloseSocket();
//			}
			break;
		}
	}while(nReadBytes < nLength);

	m_nTotalRecvBytes += nReadBytes;
	m_nLastRecvTime = time(NULL);

	return nReadBytes;
}

//���Ծ����Ŭ��д����д������
int32_t CSocket::nWrite(const uint8_t *pBuffer, const int32_t nLength)
{
	int32_t nWriteBytes = 0;

	if ((NULL == pBuffer) || (0 == nLength))
	{
		return nWriteBytes;
	}

	//�׽����Ƿ��
	if ((enmInvalidSocketFD == m_nSocketFD) || ((enmSocketStatus_Available != m_nSocketStatus)&&(enmSocketStatus_Connected != m_nSocketStatus)))
	{
		return nWriteBytes;
	}

	do
	{
		int32_t nBytes = send(m_nSocketFD, pBuffer + nWriteBytes, nLength - nWriteBytes, 0);
		if(nBytes > 0)
		{
			nWriteBytes += nBytes;
		}
		else if(nBytes <= 0)
		{
//			if(EAGAIN != errno)
//			{
//				//�����쳣����д�뷢�ͻ���
//				m_stSendBuffer.WriteToHead(pBuffer + nWriteBytes, nLength - nWriteBytes);
//				CloseSocket();
//			}
			break;
		}
	}while(nWriteBytes < nLength);

	m_nTotalSendBytes += nWriteBytes;
	m_nLastSendTime = time(NULL);

	return nWriteBytes;
}

//�ı�epoll�¼�
void CSocket::ChangeWriteEvent()
{
	if((m_stSendBuffer.Size() > 0) && ((m_nIOEvents & mask_write) == 0))
	{
		m_pNetHandler->RegistEvent(this, mask_write);
	}
	else if((m_stSendBuffer.Size() <= 0) && ((m_nIOEvents & mask_write) != 0))
	{
		m_pNetHandler->RemoveEvent(this, mask_write);
	}
}

NETEVENT_NAMESPACE_END



