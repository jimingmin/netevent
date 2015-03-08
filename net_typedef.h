/*
 * net_typedef.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_TYPEDEF_H_
#define NET_TYPEDEF_H_

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "../common/common_typedef.h"


#ifdef WIN32
typedef SOCKET		SocketFD;
enum
{
	enmInvalidSocketFD		= INVALID_SOCKET,		//无效套接字
};
#else
typedef int32_t			SocketFD;
enum
{
	enmInvalidSocketFD		= -1,					//无效套接字
};
#endif

typedef uint32_t		SessionID;

//套接字类型
typedef enum enmSessionType
{
	enmSessionType_Listen				= 0,		//监听套接字
	enmSessionType_Communicate			= 1,		//通信套接字
} SessionType;

//套接字连接状态
typedef enum enmSessionStatus
{
	enmSessionStatus_Closed				= 0,		//
	enmSessionStatus_Opened				= 1,
	enmSessionStatus_Connecting			= 2,
	enmSessionStatus_Connected			= 3,
	enmSessionStatus_Error				= 4,
	enmSessionStatus_Available			= 5,
} SessionStatus;

enum
{
	enmRecvBufferSize				= 0x00040000,		//接收缓冲区大小
	enmSendBufferSize				= 0x00040000,		//发送缓冲区大小
};


enum
{
	enmMaxConnectCount					= 500,			//最大的连接数
	enmDefaultConnectionTimeout			= 1000,			//默认连接超时时间
};


enum
{
	enmMaxMessageSize					= 0x40000,	//最大消息长度, 1024*256, 256k
	enmMaxIPAddressLength				= 20,	//IP地址最大长度
	enmWaitTimeout						= 50,	//50(ms)
};


//包解析函数
typedef int32_t (*callback_net_parser)(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t nPacketSize);
//接收连接回调函数
typedef int32_t (*callback_net_accepted)(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);
//连接成功回调函数
typedef int32_t (*callback_net_connected)(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);
//连接超时回调函数
typedef int32_t (*callback_net_connect_timeout)(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);
//读完成回调函数
typedef int32_t (*callback_net_read)(SessionID nSessionID, uint8_t *pData, int32_t nBytes);
//写完成回调函数
typedef int32_t (*callback_net_writen)(SessionID nSessionID, uint8_t *pData, int32_t nBytes);
//连接关闭回调函数
typedef int32_t (*callback_net_closed)(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);
//连接出错回调函数
typedef int32_t (*callback_net_error)(SessionID nSessionID, int32_t nErrorID);

#ifdef __cplusplus
extern "C"
{
#endif

struct NetFuncEntry
{
//public:
//	CallBackSet()
//	{
//		func_net_accepted = NULL;
//		func_net_connected = NULL;
//		func_net_read = NULL;
//		func_net_writen = NULL;
//		func_net_closed = NULL;
//	}

	callback_net_parser				func_net_parser;
	callback_net_accepted			func_net_accepted;
	callback_net_connected			func_net_connected;
	callback_net_connect_timeout	func_net_connect_timeout;
	callback_net_read				func_net_read;
	callback_net_writen				func_net_writen;
	callback_net_closed				func_net_closed;
	callback_net_error				func_net_error;
};
#ifdef __cplusplus
}
#endif


#endif /* NET_TYPEDEF_H_ */
