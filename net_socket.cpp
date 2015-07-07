/*
 * net_socket.cpp
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#include "../common/common_api.h"
#include "net_socket.h"
#include "net_eventid.h"
#include "net_handler.h"
#include "net_reactor.h"
#include "net_logger.h"

#include <time.h>

NETEVENT_NAMESPACE_BEGIN

CSocket::CSocket()
{
	Init();
}

CSocket::CSocket(CNetHandler *pNetHandler)
{
	Init();
	m_pNetHandler = pNetHandler;
}

CSocket::~CSocket()
{
	if (enmInvalidSocketFD != m_nSocketFD)
	{
		Close(SYS_EVENT_CONN_CONFLICT);
	}
}

int32_t CSocket::Init()
{
	m_nSessionID = 0;
	m_nSocketFD = enmInvalidSocketFD;
	m_nSessionType = enmSessionType_Communicate;
	m_nSessionStatus = enmSessionStatus_Closed;
	memset(m_szClientAddress, 0, sizeof(m_szClientAddress));
	m_nPeerAddress = 0;
	m_nPeerPort = 0;
	m_nLocalAddress = 0;
	m_nLocalPort = 0;
	m_nCreateTime = 0;
	m_nTotalRecvBytes = 0;
	m_nLastRecvTime = 0;
	m_nTotalSendBytes = 0;
	m_nLastSendTime = 0;
	m_bSourceRole = true;
	m_stSendBuffer.Reset();
	m_pConnectTimer = NULL;
	m_pNetHandler = NULL;
	m_nIOEvents = 0;
	m_pParam = NULL;

	return 0;
}

int32_t CSocket::Uninit()
{
	return Init();
}

int32_t CSocket::GetSize()
{
	return sizeof(*this);
}

//设置会话ID,全局唯一
void CSocket::SetSessionID(SessionID nID)
{
	m_nSessionID = nID;
}
//获取会话ID,全局唯一
SessionID CSocket::GetSessionID()
{
	return m_nSessionID;
}

//设置套接字描述符
void CSocket::SetSocketFD(SocketFD nSocketFD)
{
	m_nSocketFD = nSocketFD;
}

//获取套接字描述符
SocketFD CSocket::GetSocketFD() const
{
	return m_nSocketFD;
}

//设置套接字状态
void CSocket::SetSessionStatus(SessionStatus nSocketStatus)
{
	m_nSessionStatus = nSocketStatus;
}

//获取套接字状态
SessionStatus CSocket::GetSessionStatus() const
{
	return m_nSessionStatus;
}

void CSocket::SetSessionType(SessionType nSocketType)
{
	m_nSessionType = nSocketType;
}

SessionType CSocket::GetSessionType()
{
	return m_nSessionType;
}

char *CSocket::GetPeerAddressStr()
{
	return m_szClientAddress;
}

void CSocket::SetPeerAddress(const char *szAddress)
{
	if(szAddress != NULL)
	{
		strncpy(m_szClientAddress, szAddress, strlen(szAddress));
	}
}

void CSocket::SetPeerAddress(uint32_t nAddress)
{
	m_nPeerAddress = nAddress;

	char *pAddress = inet_ntoa_f(m_nPeerAddress);
	strncpy(m_szClientAddress, pAddress, strlen(pAddress));
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

void CSocket::SetLocalAddress(uint32_t nAddress)
{
	m_nLocalAddress = nAddress;
}

uint32_t CSocket::GetLocalAddress()
{
	return m_nLocalAddress;
}

void CSocket::SetLocalPort(uint16_t nPort)
{
	m_nLocalPort = nPort;
}

uint16_t CSocket::GetLocalPort()
{
	return m_nLocalPort;
}

//获取连接创建时间
time_t CSocket::GetCreateTime()
{
	return m_nCreateTime;
}

//获取最后一次接收数据时间
time_t CSocket::GetLastRecvTime()
{
	return m_nLastRecvTime;
}

//获取最后一次发送数据时间
time_t CSocket::GetLastSendTime()
{
	return m_nLastSendTime;
}

//获取接收的总数据字节数
uint64_t CSocket::GetTotalRecvBytes()
{
	return m_nTotalRecvBytes;
}

//获取发送的总数据字节数
uint64_t CSocket::GetTotalSendBytes()
{
	return m_nTotalSendBytes;
}

//设置附加参数
void CSocket::SetParamPtr(void *ptr)
{
	m_pParam = ptr;
}
//获取附加参数
void *CSocket::GetParamPtr()
{
	return m_pParam;
}

//设置是否主动发起连接
void CSocket::SetSourceRole(bool bIsSourceRole)
{
	m_bSourceRole = bIsSourceRole;
}
//是否主动发起连接
bool CSocket::IsSourceRole()
{
	return m_bSourceRole;
}

void CSocket::SetCreateTime(time_t nCreateTime)
{
	m_nCreateTime = nCreateTime;
}

void CSocket::SetNetHandler(CNetHandler *pNetHandler)
{
	m_pNetHandler = pNetHandler;
}


void CSocket::SetConnectTimer(CConnectTimer *pTimer)
{
	m_pConnectTimer = pTimer;
}

CConnectTimer *CSocket::GetSessionTimer()
{
	return m_pConnectTimer;
}

int32_t CSocket::ReadEvent()
{
	if((m_nSessionStatus == enmSessionStatus_Closed) ||
		(m_nSessionStatus == enmSessionStatus_Error))
	{
		return S_OK;
	}

	int32_t nRet = S_OK;

	//是监听套接字的读事件
	if(enmSessionType_Listen == m_nSessionType)
	{
		struct sockaddr_in stPeerAddress;
		socklen_t nPeerAddressLen = (socklen_t)sizeof(stPeerAddress);

		SocketFD nAcceptFD = enmInvalidSocketFD;
		do
		{
			nAcceptFD = accept(m_nSocketFD, (struct sockaddr *)&stPeerAddress, &nPeerAddressLen);
			if(nAcceptFD == INVALID_SOCKET)
			{
				break;
			}

			nRet = OnAccept(nAcceptFD, stPeerAddress, nPeerAddressLen);
			if(nRet < 0)
			{
#ifdef WIN32
				closesocket(nAcceptFD);
#else
				close(nAcceptFD);
#endif
			}
		}while(nAcceptFD > 0);

	}
	//非监听套接字
	else
	{
		int32_t nError = 0;
		socklen_t nLen = sizeof(int32_t);

		if(0 != getsockopt(m_nSocketFD, SOL_SOCKET, SO_ERROR, (char *)&nError, &nLen))
		{
			nError = 0;
		}

		nRet = OnRead(nError);
	}

	return nRet;
}

int32_t CSocket::WriteEvent()
{
	if((m_nSessionStatus == enmSessionStatus_Closed) ||
		(m_nSessionStatus == enmSessionStatus_Error))
	{
		return S_OK;
	}

	int32_t nError = 0;
	socklen_t nLen = sizeof(int32_t);

	if(getsockopt(m_nSocketFD, SOL_SOCKET, SO_ERROR, (char *)&nError, &nLen) < 0)
	{
		return E_UNKNOWN;
	}

	if(m_nSessionStatus == enmSessionStatus_Connecting)
	{
		struct sockaddr_in sa;
		socklen_t nLen = sizeof(sa);
		if(!getpeername(m_nSocketFD, (struct sockaddr *)&sa, &nLen))
		{
			return Connected();
		}
	}

	return OnWrite(nError);
}

int32_t CSocket::ErrorEvent()
{
	if(m_pConnectTimer != NULL)
	{
		m_pNetHandler->GetConnectTimerMgt().DestroyConnectTimer(m_pConnectTimer);
		m_pConnectTimer = NULL;
	}

	if((m_nSessionStatus == enmSessionStatus_Closed) ||
			(m_nSessionStatus == enmSessionStatus_Error))
	{
		return S_OK;
	}

	int32_t nError = 0;
	socklen_t nLen = sizeof(int32_t);

	if(0 != getsockopt(m_nSocketFD, SOL_SOCKET, SO_ERROR, (char *)&nError, &nLen))
	{
		nError = 0;
	}

	return OnError(nError);
}

int32_t CSocket::Write(uint8_t *pBuf, int32_t nBufSize)
{
	return 0;
}

//接收连接回调(重载此函数，可以在这里做些连接信息初始化的工作)
int32_t CSocket::OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
		socklen_t nPeerAddressLen)
{
	return S_OK;
}

//连接成功回调(重载此函数，可以在这里做些连接信息初始化的工作)
int32_t CSocket::OnConnected()
{
	return S_OK;
}

int32_t CSocket::OnConnectTimeout()
{
	return S_OK;
}

//断开连接回调(重载此函数，可以在这里做些资源回收的工作)
int32_t CSocket::OnDisconnect(int32_t nCloseCode)
{
	return S_OK;
}

//发送缓存中剩余的数据
int32_t CSocket::SendRestData()
{
	int32_t nSendBytes = 0;
	//int32_t nWriteBytes = 0;
	//先发送缓存中剩余的数据
	if(m_stSendBuffer.Size() > 0)
	{
		uint8_t arrPacketBuf[enmRecvBufferSize];
		int32_t nWantSize = sizeof(arrPacketBuf);

		int32_t nRealSize = m_stSendBuffer.Read(arrPacketBuf, nWantSize);
		nSendBytes = nWrite(arrPacketBuf, nRealSize);
		if(nSendBytes < nRealSize)
		{
			int32_t nRewriteIndex = 0;
			if(nSendBytes > 0)
			{
				nRewriteIndex = nSendBytes;
			}
			//剩余数据写回缓存
			m_stSendBuffer.WriteToHead(&arrPacketBuf[nRewriteIndex], nRealSize - nRewriteIndex);
#ifdef WIN32
			if((ErrorNo() != EAGAIN) && (ErrorNo() != WSAEWOULDBLOCK))
#else
			if(ErrorNo() != EAGAIN)
#endif
			{
				nSendBytes = -1;
				//break;
				//Close();
			}
		}
	}
	ChangeWriteEvent();

	return nSendBytes;
}

int32_t CSocket::Open()
{
	//若socket连接已存在则先关闭套接字
	if (enmInvalidSocketFD != m_nSocketFD)
	{
		WRITE_DEBUG_LOG(g_pLoggerConfig, "open socket, close exist socket!{sessionid=%u}\n", m_nSessionID);
		Close(SYS_EVENT_CONN_CONFLICT);
	}

	//打开套接字
	m_nSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (enmInvalidSocketFD == m_nSocketFD)
	{
		return E_SOCKETCREATE;
	}

	set_non_block(m_nSocketFD);
	m_nSessionStatus = enmSessionStatus_Opened;

	return S_OK;
}


//关闭套接字
void CSocket::Close(int32_t nCloseCode)
{
	if (enmInvalidSocketFD != m_nSocketFD)
	{
		if(m_nSessionStatus != enmSessionStatus_Closed)
		{
			Disconnected(nCloseCode);
			//更新套接字状态
			m_nSessionStatus = enmSessionStatus_Closed;
		}
#ifdef WIN32
		closesocket(m_nSocketFD);
#else
		close(m_nSocketFD);
#endif
		m_nSocketFD = enmInvalidSocketFD;

		Uninit();
	}
}

int32_t CSocket::Shutdown()
{
	//shutdown(m_nSocketFD, 0);
	return 0;
}

//异步关闭
void CSocket::AsyncClose(int32_t nCloseCode)
{
	ASSERT(1);
}

//与服务端建立连接
int32_t CSocket::Connect(const char* szRemoteIP, uint16_t nPort)
{
	return S_OK;
}

//定时器事件
int32_t CSocket::OnTimerEvent(CConnectTimer *pTimer)
{
	return ConnectTimeout();
}

//连接超时
int32_t CSocket::ConnectTimeout()
{
	if(m_pConnectTimer != NULL)
	{
		m_pNetHandler->GetConnectTimerMgt().DestroyConnectTimer(m_pConnectTimer);
		m_pConnectTimer = NULL;
	}

	//多次发送连接请求的情况下，可能某一次连接成功了，另外一些连接请求超时
	if((m_nSessionStatus == enmSessionStatus_Connected) ||
				(m_nSessionStatus == enmSessionStatus_Available))
	{
		return S_OK;
	}

	m_nSessionStatus = enmSessionStatus_Closed;

	if(m_pNetHandler != NULL)
	{
		m_pNetHandler->DeleteEvent(this);
	}

	return OnConnectTimeout();
}

//连接成功的回调
int32_t CSocket::Connected()
{
	if(m_pConnectTimer != NULL)
	{
		m_pNetHandler->GetConnectTimerMgt().DestroyConnectTimer(m_pConnectTimer);
		m_pConnectTimer = NULL;
	}

	if((m_nSessionStatus != enmSessionStatus_Connected) &&
			(m_nSessionStatus != enmSessionStatus_Available))
	{
		m_nSessionStatus = enmSessionStatus_Connected;
	}

	sockaddr_in stLocalAddr;
	memset(&stLocalAddr, 0, sizeof(stLocalAddr));
	int32_t nAddrLen = sizeof(stLocalAddr);

	int32_t nRet = getsockname(m_nSocketFD, (sockaddr *)&stLocalAddr, (socklen_t *)&nAddrLen);
	if(nRet != 0)
	{
		WRITE_DEBUG_LOG(g_pLoggerConfig, "it's not conneced, so close it!{sessionid=%u}\n", m_nSessionID);
		Close(SYS_EVENT_CONN_CONFLICT);

		return S_OK;
	}

	//如果本地地址和端口等于远程地址和端口，就断开连接
	if(((strcmp(m_szClientAddress, "127.0.0.1") == 0) ||
				(strcmp(m_szClientAddress, inet_ntoa(stLocalAddr.sin_addr)) == 0)) &&
				(stLocalAddr.sin_port == htons(m_nPeerPort)))
	{
		WRITE_DEBUG_LOG(g_pLoggerConfig, "it's self connected, so close it!{sessionid=%u}\n", m_nSessionID);
		Close(SYS_EVENT_CONN_CONFLICT);

		return S_OK;
	}

	m_nLocalAddress = stLocalAddr.sin_addr.s_addr;
	m_nLocalPort = stLocalAddr.sin_port;

	return OnConnected();
}

//连接成功的回调
int32_t CSocket::Disconnected(int32_t nCloseCode)
{
	if(m_pConnectTimer != NULL)
	{
		m_pNetHandler->GetConnectTimerMgt().DestroyConnectTimer(m_pConnectTimer);
		m_pConnectTimer = NULL;
	}

	return OnDisconnect(nCloseCode);
}

//接收数据
int32_t CSocket::Recv(uint8_t *pBuffer, int32_t nSize, int32_t& nRecvBytes)
{
	nRecvBytes = 0;

	//套接字是否打开
	if (enmInvalidSocketFD == m_nSocketFD)
	{
		return E_SOCKETNOTCREATED;
	}

	if((enmSessionStatus_Available != m_nSessionStatus)&&(enmSessionStatus_Connected != m_nSessionStatus))
	{
		struct sockaddr_in sa;
		socklen_t nLen = sizeof(sa);
		if(!getpeername(m_nSocketFD, (struct sockaddr *)&sa, &nLen))
		{
			Connected();
		}
		else
		{
			return E_SOCKETERROR;
		}
	}

	//m_nLastRecvTime = time(NULL);

	//接收数据
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
#ifdef WIN32
			if((ErrorNo() != EAGAIN) && (ErrorNo() != WSAEWOULDBLOCK))
#else
			if(ErrorNo() != EAGAIN)
#endif
			{
				return E_SOCKETERROR;
			}
		}
		return S_OK;
	}
	else if (0 == n)
	{
		//连接已断开
		return E_SOCKETDISCONNECTED;
	}
#ifdef WIN32
	else if((ErrorNo() != EAGAIN) && (ErrorNo() != WSAEWOULDBLOCK))
#else
	else if(ErrorNo() != EAGAIN)
#endif
	{
		//接收出错
		return E_SOCKETERROR;
	}

	return S_OK;
}
//发送数据
int32_t CSocket::Send(const uint8_t *pBuffer, const int32_t nLength, int32_t& nSendBytes)
{
	nSendBytes = 0;

	//套接字是否打开
	if (enmInvalidSocketFD == m_nSocketFD)
	{
		return E_SOCKETNOTCREATED;
	}

	if((enmSessionStatus_Available != m_nSessionStatus)&&(enmSessionStatus_Connected != m_nSessionStatus))
	{
		struct sockaddr_in sa;
		socklen_t nLen = sizeof(sa);
		if(!getpeername(m_nSocketFD, (struct sockaddr *)&sa, &nLen))
		{
			Connected();
		}
		else
		{
			return E_SOCKETERROR;
		}

	}

	//发送缓存还有数据
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
				//剩余数据写回发送缓存
				m_stSendBuffer.WriteToHead(&arrSendBuf[nRewriteIndex], nRealRestSize - nRewriteIndex);

				ChangeWriteEvent();
#ifdef WIN32
				if((ErrorNo() != EAGAIN) && (ErrorNo() != WSAEWOULDBLOCK))
#else
				if(ErrorNo() != EAGAIN)
#endif
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
		m_stSendBuffer.Write(pBuffer, nLength);
		return S_OK;
	}

	nSendBytes = nWrite(pBuffer, nLength);
	if(nSendBytes < nLength)
	{
		int32_t nRewriteIndex = 0;
		if(nSendBytes > 0)
		{
			nRewriteIndex = nSendBytes;
		}
		//剩余数据写回发送缓存
		m_stSendBuffer.WriteToHead(&pBuffer[nRewriteIndex], nLength - nRewriteIndex);

		ChangeWriteEvent();
#ifdef WIN32
		if((ErrorNo() != EAGAIN) && (ErrorNo() != WSAEWOULDBLOCK))
#else
		if(ErrorNo() != EAGAIN)
#endif
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

//尝试尽最大努力读取想读的数据
int32_t CSocket::nRead(uint8_t *pBuffer, const int32_t nLength,bool &isConnClose)
{
	int32_t nReadBytes = 0;
	isConnClose = false;
	if ((NULL == pBuffer) || (0 == nLength))
	{
		return nReadBytes;
	}

	//套接字是否打开
	if ((enmInvalidSocketFD == m_nSocketFD) || ((enmSessionStatus_Available != m_nSessionStatus)&&(enmSessionStatus_Connected != m_nSessionStatus)))
	{
		return nReadBytes;
	}

	do
	{
		int32_t nRecvBytes = recv(m_nSocketFD, (char *)(pBuffer + nReadBytes), nLength - nReadBytes, 0);
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

	if(nReadBytes > 0)
	{
		m_nTotalRecvBytes += nReadBytes;
	}
	m_nLastRecvTime = time(NULL);

	return nReadBytes;
}

//尝试尽最大努力写入想写的数据
int32_t CSocket::nWrite(const uint8_t *pBuffer, const int32_t nLength)
{
	int32_t nWriteBytes = 0;

	if ((NULL == pBuffer) || (0 == nLength))
	{
		return nWriteBytes;
	}

	//套接字是否打开
	if ((enmInvalidSocketFD == m_nSocketFD) || ((enmSessionStatus_Available != m_nSessionStatus)&&(enmSessionStatus_Connected != m_nSessionStatus)))
	{
		return nWriteBytes;
	}

	do
	{
		int32_t nBytes = send(m_nSocketFD, (char *)(pBuffer + nWriteBytes), nLength - nWriteBytes, 0);
		if(nBytes > 0)
		{
			nWriteBytes += nBytes;
		}
		else if(nBytes <= 0)
		{
//			if(EAGAIN != errno)
//			{
//				//出现异常则先写入发送缓存
//				m_stSendBuffer.WriteToHead(pBuffer + nWriteBytes, nLength - nWriteBytes);
//				CloseSocket();
//			}
			break;
		}
	}while(nWriteBytes < nLength);

	if(nWriteBytes > 0)
	{
		m_nTotalSendBytes += nWriteBytes;
	}
	m_nLastSendTime = time(NULL);

	return nWriteBytes;
}

//改变epoll事件
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



