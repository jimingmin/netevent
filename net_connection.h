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

	virtual int32_t GetSize();
	//同步关闭
	virtual void Close(int32_t nCloseCode = 0);
	//异步关闭
	virtual void AsyncClose(int32_t nCloseCode = 0);
	//写入数据
	virtual int32_t Write(uint8_t *pBuf, int32_t nBufSize);

	//写入到底层buf
	int32_t WriteCompleted(uint8_t *pBuf, int32_t nBufSize);
	//设置解包器
	void SetPacketParser(IPacketParser *pPacketParser);
	IPacketParser *GetPacketParser();
	//设置io处理器
	void SetIOHandler(IIOHandler *pIOHandler);
	IIOHandler *GetIOHandler();
	//设置是否正在关闭标识
	void SetClosing(bool bClosing);
	bool GetClosing();
	//获取nethandler
	CNetHandler *GetNetHandler();

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
	CycleBuffer<enmRecvBufferSize>	m_stRecvBuffer;	//接收缓冲区
	IPacketParser			*m_pPacketParser;
	IIOHandler				*m_pIOHandler;
	bool					m_bIsClosing;			//是否正在关闭
};

NETEVENT_NAMESPACE_END

#endif /* NET_CONNECTION_H_ */
