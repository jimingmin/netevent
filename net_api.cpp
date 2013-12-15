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

int32_t set_non_block( int32_t fd )
{

	int flag = 0;
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

	return 0;
}

char *inet_ntoa_f(uint32_t ip)
{
	static char buf[16];
	uint8_t *str = (uint8_t *)&ip;
	sprintf(buf, "%d.%d.%d.%d", str[0] & 0xff, str[1] & 0xff, str[2] & 0xff, str[3] & 0xff);

	return buf;
}

#else

#include "net_api.h"

int32_t set_non_block(int32_t fd)
{
	return 0;
}

char *inet_ntoa_f(uint32_t ip)
{
	static char buf[16];
	uint8_t *str = (uint8_t *)&ip;
	sprintf(buf, "%d.%d.%d.%d", str[0] & 0xff, str[1] & 0xff, str[2] & 0xff, str[3] & 0xff);

	return buf;
}

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



