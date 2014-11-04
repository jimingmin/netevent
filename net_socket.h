/*
 * net_socket.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_

#include "net_api.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_namespace.h"
#include "net_impl.h"
#include "net_timer.h"
#include "../common/common_cyclebuffer.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>

#ifndef WIN32
#include <sys/socket.h>
#endif

NETEVENT_NAMESPACE_BEGIN

class CNetHandler;

class CSocket: public IIOSession
{
public:
	CSocket();
	CSocket(CNetHandler *pNetHandler);
	virtual ~CSocket();

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	//设置会话ID,全局唯一
	virtual void SetSessionID(SessionID nID);
	//获取会话ID,全局唯一
	virtual SessionID GetSessionID();
	//设置套接字描述符
	virtual void SetSocketFD(SocketFD nSocketFD);
	//获取套接字描述符
	virtual SocketFD GetSocketFD() const;
	//设置套接字状态
	virtual void SetSessionStatus(SessionStatus nSocketStatus);
	//获取套接字状态
	virtual SessionStatus GetSessionStatus() const;
	//设置会话类型
	virtual void SetSessionType(SessionType nSocketType);
	//获取会话类型
	virtual SessionType GetSessionType();
	//设置远端地址
	virtual void SetPeerAddress(const char *szAddress);
	//设置远端地址
	virtual void SetPeerAddress(uint32_t nAddress);
	//获取远端地址
	virtual char *GetPeerAddressStr();
	//获取远端地址
	virtual uint32_t GetPeerAddress();
	//设置远端端口
	virtual void SetPeerPort(uint16_t nPort);
	//获取远端端口
	virtual uint16_t GetPeerPort();
	virtual void SetLocalAddress(uint32_t nAddress);
	//获取本地地址
	virtual uint32_t GetLocalAddress();
	virtual void SetLocalPort(uint16_t nPort);
	//获取本地端口
	virtual uint16_t GetLocalPort();
	//获取连接创建时间
	virtual time_t GetCreateTime();
	//获取最后一次接收数据时间
	virtual time_t GetLastRecvTime();
	//获取最后一次发送数据时间
	virtual time_t GetLastSendTime();
	//获取接收的总数据字节数
	virtual uint64_t GetTotalRecvBytes();
	//获取发送的总数据字节数
	virtual uint64_t GetTotalSendBytes();
	//设置附加参数
	virtual void SetParamPtr(void *ptr);
	//获取附加参数
	virtual void *GetParamPtr();

	virtual int32_t ReadEvent();

	virtual int32_t WriteEvent();

	virtual int32_t ErrorEvent();

	virtual int32_t Write(uint8_t *pBuf, int32_t nBufSize);

	virtual int32_t Open();
	//关闭套接字
	virtual void Close(int32_t nCloseCode = 0);
	//异步关闭
	virtual void AsyncClose(int32_t nCloseCode = 0);
	//与服务端建立连接
	virtual int32_t Connect(const char* szRemoteIP, uint16_t nPort);
	//接收数据
	virtual int32_t Recv(uint8_t *pBuffer, int32_t nSize, int32_t& nRecvBytes);
	//发送数据
	virtual int32_t Send(const uint8_t *pBuffer, const int32_t nLength, int32_t& nSendBytes);

	void SetCreateTime(time_t nCreateTime);

	void SetNetHandler(CNetHandler *pNetHandler);

	void SetConnectTimer(CConnectTimer *pTimer);

	CConnectTimer *GetSessionTimer();

	uint32_t GetIOEvents();

	void SetIOEvents(uint32_t nEvents);

	//定时器事件
	int32_t OnTimerEvent(CConnectTimer *pTimer);
	//连接成功
	int32_t Connected();
protected:
	//读事件回调
	virtual int32_t OnRead(int32_t nErrorCode) = 0;
	//写事件回调
	virtual int32_t OnWrite(int32_t nErrorCode) = 0;
	//异常事件回调
	virtual int32_t OnError(int32_t nErrorCode) = 0;
	//接收连接回调(重载此函数，可以在这里做些连接信息初始化的工作)
	virtual int32_t OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
			socklen_t nPeerAddressLen);
	//连接成功回调(重载此函数，可以在这里做些连接信息初始化的工作)
	virtual int32_t OnConnected();
	//连接超时回调
	virtual int32_t OnConnectTimeout();
	//断开连接回调(重载此函数，可以在这里做些资源回收的工作)
	virtual int32_t OnDisconnect(int32_t nCloseCode);
	//发送缓存中剩余的数据
	virtual int32_t SendRestData();
	//尝试尽最大努力读取想读的数据
	int32_t nRead(uint8_t *pBuffer, const int32_t nLength,bool &isConnClose);
	//尝试尽最大努力写入想写的数据
	int32_t nWrite(const uint8_t *pBuffer, const int32_t nLength);
	//改变epoll事件
	void ChangeWriteEvent();
private:
	//连接超时
	int32_t ConnectTimeout();
	//连接关闭
	int32_t Disconnected(int32_t nCloseCode);

protected:
	SessionID		m_nSessionID;				//全局唯一ID
	SocketFD		m_nSocketFD;				//套接字描述符
	SessionType		m_nSessionType;				//表示会话是监听类型还是通信类型
	SessionStatus	m_nSessionStatus;			//会话状态
	char			m_szClientAddress[enmMaxIPAddressLength];
	uint32_t		m_nPeerAddress;				//对端ip
	uint16_t		m_nPeerPort;				//对端port
	uint32_t		m_nLocalAddress;			//监听地址
	uint16_t		m_nLocalPort;				//监听端口
	time_t			m_nCreateTime;				//连接创建时间
	time_t			m_nLastRecvTime;			//最近一次从对端接收数据的时间
	time_t			m_nLastSendTime;			//最近一次发给对端数据的时间
	uint64_t		m_nTotalRecvBytes;			//收到的数据大小
	uint64_t		m_nTotalSendBytes;			//发送的数据大小
//	CycleBuffer<enmRecvBufferSize>	m_stRecvBuffer;	//接收缓冲区
	CycleBuffer<enmSendBufferSize>	m_stSendBuffer;	//发送缓存，在套接字异常或者系统发送缓冲满的情况下，数据将会被放进此缓存

	CConnectTimer	*m_pConnectTimer;
	CNetHandler		*m_pNetHandler;
	uint32_t		m_nIOEvents;
	void 			*m_pParam;
};

NETEVENT_NAMESPACE_END


#endif /* NET_SOCKET_H_ */
