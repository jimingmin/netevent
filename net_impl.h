/*
 * net_impl.h
 *
 *  Created on: 2013Äê11ÔÂ4ÈÕ
 *      Author: jimm
 */

#ifndef NET_IMPL_H_
#define NET_IMPL_H_

#include "net_namespace.h"
#include "net_packet.h"

NETEVENT_NAMESPACE_BEGIN

class IEventHandler
{
public:
	virtual ~IEventHandler(){};

	virtual int32_t ReadEvent() = 0;

	virtual int32_t WriteEvent() = 0;

	virtual int32_t ErrorEvent() = 0;
};

class IPacketParser
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

//class IMsgHead
//{
//public:
//	virtual ~IMsgHead(){};
//
//	virtual int32_t Encode(uint8_t *pBuf, uint32_t nBufSize) = 0;
//
//	virtual int32_t Decode(uint8_t *pBuf, uint32_t nBufSize) = 0;
//};
//
//class IMsgBody
//{
//public:
//	virtual ~IMsgBody(){};
//
//	virtual int32_t Encode(uint8_t *pBuf, uint32_t nBufSize) = 0;
//
//	virtual int32_t Decode(uint8_t *pBuf, uint32_t nBufSize) = 0;
//};

class IIOHandler
{
public:
	virtual ~IIOHandler(){};

	virtual int32_t OnOpened(IEventHandler *pHandler) = 0;

	virtual int32_t OnRecved(IEventHandler *pHandler, uint8_t *pBuf, uint32_t nBufSize) = 0;

	virtual int32_t OnSent(IEventHandler *pHandler, uint8_t *pBuf, uint32_t nBufSize) = 0;

	virtual int32_t OnClosed(IEventHandler *pHandler) = 0;

	virtual int32_t OnError(IEventHandler *pHandler) = 0;
};

NETEVENT_NAMESPACE_END

#endif /* NET_IMPL_H_ */
