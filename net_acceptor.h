/*
 * net_acceptor.h
 *
 *  Created on: 2013年12月16日
 *      Author: jimm
 */

#ifndef NET_ACCEPTOR_H_
#define NET_ACCEPTOR_H_

#include "net_namespace.h"
#include "net_socket.h"
#include "net_packetparserfactory.h"
#include "net_messagehandler.h"

NETEVENT_NAMESPACE_BEGIN

class CAcceptor : public CSocket
{
public:
	EXPORT CAcceptor(CNetHandler *pNetHandler, IPacketParserFactory *pFactory, IIOHandler *pIOHandler);

	EXPORT int32_t Bind(const char *szLocalIP, uint16_t nPort);

	EXPORT int32_t Bind(uint16_t nPort);

	EXPORT IPacketParserFactory *GetPacketParserFactory();

	EXPORT IIOHandler *GetIOHandler();

protected:
	//读事件回调
	virtual int32_t OnRead(int32_t nErrorCode);
	//写事件回调
	virtual int32_t OnWrite(int32_t nErrorCode);
	//异常事件回调
	virtual int32_t OnError(int32_t nErrorCode);
	//接收连接回调(重载此函数，可以在这里做些连接信息初始化的工作)
	virtual int32_t OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
			socklen_t nPeerAddressLen);

protected:
	CNetHandler				*m_pNetHandler;
	IPacketParserFactory	*m_pPacketParserFactory;
	IIOHandler				*m_pIOHandler;
};

NETEVENT_NAMESPACE_END

#endif /* NET_ACCEPTOR_H_ */
