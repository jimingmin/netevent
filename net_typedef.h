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

typedef uint32_t		ConnectionID;

//套接字类型
typedef enum enmSocketType
{
	enmSocketType_Listen				= 0,		//监听套接字
	enmSocketType_Communicate			= 1,		//通信套接字
} SocketType;

//套接字属性
typedef enum enmSocketAttr
{
	enmSocketAttr_Unknown				= 0,		//未知套接字
	enmSocketAttr_Outside				= 1,		//外部套接字
	enmSocketAttr_Inside				= 2,		//内部套接字
} SocketAttr;

//套接字连接状态
typedef enum enmSocketStatus
{
	enmSocketStatus_Closed				= 0,		//
	enmSocketStatus_Opened				= 1,
	enmSocketStatus_Connecting			= 2,
	enmSocketStatus_Connected			= 3,
	enmSocketStatus_Error				= 4,
	enmSocketStatus_Available			= 5,
} SocketStatus;

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
