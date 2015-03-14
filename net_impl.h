/*
 * net_impl.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_IMPL_H_
#define NET_IMPL_H_

#include "net_namespace.h"
#include "net_packet.h"
#include "net_typedef.h"
#include "../common/common_object.h"

NETEVENT_NAMESPACE_BEGIN

class IIOSession : public CBaseObject
{
public:
	virtual ~IIOSession(){};

	//设置会话ID,全局唯一
	virtual void SetSessionID(SessionID nID) = 0;
	//获取会话ID,全局唯一
	virtual SessionID GetSessionID() = 0;
	//设置套接字描述符
	virtual void SetSocketFD(SocketFD nSocketFD) = 0;
	//获取套接字描述符
	virtual SocketFD GetSocketFD() const = 0;
	//设置套接字状态
	virtual void SetSessionStatus(SessionStatus nSocketStatus) = 0;
	//获取套接字状态
	virtual SessionStatus GetSessionStatus() const = 0;
	//设置会话类型
	virtual void SetSessionType(SessionType nSocketType) = 0;
	//获取会话类型
	virtual SessionType GetSessionType() = 0;
	//设置远端地址
	virtual void SetPeerAddress(const char *szAddress) = 0;
	//设置远端地址
	virtual void SetPeerAddress(uint32_t nAddress) = 0;
	//获取远端地址
	virtual char *GetPeerAddressStr() = 0;
	//获取远端地址
	virtual uint32_t GetPeerAddress() = 0;
	//设置远端端口
	virtual void SetPeerPort(uint16_t nPort) = 0;
	//获取远端端口
	virtual uint16_t GetPeerPort() = 0;
	//获取本地地址
	virtual uint32_t GetLocalAddress() = 0;
	//获取本地端口
	virtual uint16_t GetLocalPort() = 0;
	//获取连接创建时间
	virtual time_t GetCreateTime() = 0;
	//获取最后一次接收数据时间
	virtual time_t GetLastRecvTime() = 0;
	//获取最后一次发送数据时间
	virtual time_t GetLastSendTime() = 0;
	//获取接收的总数据字节数
	virtual uint64_t GetTotalRecvBytes() = 0;
	//获取发送的总数据字节数
	virtual uint64_t GetTotalSendBytes() = 0;
	//设置附加参数
	virtual void SetParamPtr(void *ptr) = 0;
	//获取附加参数
	virtual void *GetParamPtr() = 0;
	//设置是否主动发起连接
	virtual void SetSourceRole(bool bIsSourceRole) = 0;
	//是否主动发起连接
	virtual bool IsSourceRole() = 0;

	virtual int32_t ReadEvent() = 0;

	virtual int32_t WriteEvent() = 0;

	virtual int32_t ErrorEvent() = 0;

	virtual int32_t Write(uint8_t *pBuf, int32_t nBufSize) = 0;

	virtual int32_t Open() = 0;
	//关闭套接字
	virtual void Close(int32_t nCloseCode = 0) = 0;
	//异步关闭
	virtual void AsyncClose(int32_t nCloseCode = 0) = 0;
	//与服务端建立连接
	virtual int32_t Connect(const char* szRemoteIP, uint16_t nPort) = 0;
	//接收数据
	virtual int32_t Recv(uint8_t *pBuffer, int32_t nSize, int32_t& nRecvBytes) = 0;
	//发送数据
	virtual int32_t Send(const uint8_t *pBuffer, const int32_t nLength, int32_t& nSendBytes) = 0;
};

class IPacketParser : public CBaseObject
{
public:
	virtual ~IPacketParser(){};

	//virtual void InputData(uint8_t *pBuf, int32_t nSize) = 0;

	/*
	 * 返回值：成功解析出来的包的大小，若buf数据不够解析出一个包则返回0
	 * arrInBuf：待解析的buf
	 * nInBufSize：arrInBuf的大小
	 * arrOutputBuf：成功解析出来的包所存放的buf
	 * nOutputBufSize：传入arrOutpubBuf的大小，传出成功解析出来的包的大小
	 */
	virtual int32_t Parser(const uint8_t arrInputBuf[], const uint32_t nInputBufSize, uint8_t arrOutputBuf[], int32_t &nOutputBufSize) = 0;
};

class IPacketParserFactory
{
public:
	virtual ~IPacketParserFactory(){};

	virtual IPacketParser *Create() = 0;

	virtual void Destory(IPacketParser *pPacketParser) = 0;
};

class IIOHandler
{
public:
	virtual ~IIOHandler(){};

	virtual int32_t OnOpened(IIOSession *pIoSession) = 0;

	virtual int32_t OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize) = 0;

	virtual int32_t OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize) = 0;

	virtual int32_t OnClosed(IIOSession *pIoSession) = 0;

	virtual int32_t OnError(IIOSession *pIoSession) = 0;

	virtual int32_t OnTimeout(IIOSession *pIoSession) = 0;
};

NETEVENT_NAMESPACE_END

#endif /* NET_IMPL_H_ */
