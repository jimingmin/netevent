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
};


#endif /* NET_TYPEDEF_H_ */
