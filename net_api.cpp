/*
 * net_api.cpp
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#ifndef WIN32

#include <fcntl.h>
#include <sys/ioctl.h>

#include <sys/param.h>
#include <sys/cdefs.h>

#include "net_api.h"

#else

#include <errno.h>
#include "net_api.h"

int epoll_create(int __size)
{
	return -1;
}

int epoll_ctl(int __epfd, int __op, int __fd, struct epoll_event *__ev)
{
	return -1;
}


int epoll_wait(int __epfd, struct epoll_event *__events, int __maxevents, int __timeout)
{
	return -1;
}

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	int32_t set_non_block(SocketFD fd)
	{
#ifdef WIN32
		unsigned long ul=1;  
		int32_t ret;  
		ret = ioctlsocket(fd, FIONBIO, (unsigned long *)&ul);//设置成非阻塞模式
		if(ret == SOCKET_ERROR)//设置失败
		{
			return -1;
		}  
#else
		int32_t flag = 0;
		flag = fcntl(fd, F_GETFL, 0);
		if (flag < 0)
		{
			return -1;
		}

		flag |= O_NONBLOCK;
		flag |= O_NDELAY;

		if (fcntl(fd, F_SETFL, flag) < 0)
		{
			return -1;
		}
#endif

		return 0;
	}

	int32_t set_reuse_addr(SocketFD fd)
	{
#ifdef WIN32
		return 0;
#else
		//设置套接字属性
		int32_t op = 1;
		return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&op, sizeof(op));
#endif
	}

#ifdef __cplusplus
}
#endif


