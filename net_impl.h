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
#include "../common/common_object.h"

NETEVENT_NAMESPACE_BEGIN

class IIOSession : public CObject
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

	virtual int32_t ReadEvent() = 0;

	virtual int32_t WriteEvent() = 0;

	virtual int32_t ErrorEvent() = 0;

	virtual int32_t Write(uint8_t *pBuf, int32_t nBufSize) = 0;
};

class IPacketParser : public CObject
{
public:
	virtual ~IPacketParser(){};

	virtual void InputData(uint8_t *pBuf, int32_t nSize) = 0;

	virtual NetPacket *Parser() = 0;
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
