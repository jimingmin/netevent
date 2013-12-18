/*
 * net_connection.h
 *
 *  Created on: 2013年12月16日
 *      Author: jimm
 */

#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_

#include "net_namespace.h"
#include "net_socket.h"
#include "net_impl.h"

NETEVENT_NAMESPACE_BEGIN

class CConnection : public CSocket
{
public:
	CConnection(CNetHandler *pNetHandler, IPacketParser *pPacketParser, IIOHandler *pIOHandler);

	virtual int32_t Write(uint8_t *pBuf, int32_t nBufSize);
protected:
	//读事件回调
	virtual int32_t OnRead(int32_t nErrorCode);
	//写事件回调
	virtual int32_t OnWrite(int32_t nErrorCode);
	//异常事件回调
	virtual int32_t OnError(int32_t nErrorCode);
	//连接成功回调(重载此函数，可以在这里做些连接信息初始化的工作)
	virtual int32_t OnConnected();
	//连接超时回调
	virtual int32_t OnConnectTimeout();
	//断开连接回调(重载此函数，可以在这里做些资源回收的工作)
	virtual int32_t OnDisconnect(int32_t nCloseCode);

protected:
	IPacketParser			*m_pPacketParser;
	IIOHandler				*m_pIOHandler;
};

NETEVENT_NAMESPACE_END

#endif /* NET_CONNECTION_H_ */
