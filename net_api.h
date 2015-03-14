/*
 * net_api.h
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#ifndef NET_API_H_
#define NET_API_H_

#include <stdio.h>
#include "net_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

int32_t set_non_block(SocketFD fd);

#ifdef __cplusplus
}
#endif


#ifndef WIN32

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __linux__
#include <sys/epoll.h>
#endif

typedef int SOCKET;
#define INVALID_SOCKET -1
#define closesocket close

//block flag
enum enmSocketBlockFlag
{
	enmSocketBlockFlag_lock = 0,
	enmSocketBlockFlag_nonblock = 1,
};

#else

/**
define epoll data on windows platform
*/
#include "../common/common_typedef.h"

typedef int socklen_t;

enum EPOLL_EVENTS
{
	EPOLLIN = 0x001,
 #define EPOLLIN EPOLLIN
	EPOLLPRI = 0x002,
 #define EPOLLPRI EPOLLPRI
	EPOLLOUT = 0x004,
 #define EPOLLOUT EPOLLOUT
	EPOLLRDNORM = 0x040,
 #define EPOLLRDNORM EPOLLRDNORM
	EPOLLRDBAND = 0x080,
 #define EPOLLRDBAND EPOLLRDBAND
	EPOLLWRNORM = 0x100,
 #define EPOLLWRNORM EPOLLWRNORM
	EPOLLWRBAND = 0x200,
 #define EPOLLWRBAND EPOLLWRBAND
	EPOLLMSG = 0x400,
 #define EPOLLMSG EPOLLMSG
	EPOLLERR = 0x008,
 #define EPOLLERR EPOLLERR
	EPOLLHUP = 0x010,
 #define EPOLLHUP EPOLLHUP
	EPOLLONESHOT = (1 << 30),
 #define EPOLLONESHOT EPOLLONESHOT
	EPOLLET = (1 << 31)
 #define EPOLLET EPOLLET
};

typedef union epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event
{
	uint32_t events;      /* Epoll events */
	epoll_data_t data;    /* User data variable */
};

#ifndef EPOLL_CTL_ADD
#define EPOLL_CTL_ADD 1 /* Add a file decriptor to the interface.  */
#endif

#ifndef EPOLL_CTL_DEL
#define EPOLL_CTL_DEL 2 /* Remove a file decriptor from the interface.  */
#endif

#ifndef EPOLL_CTL_MOD
#define EPOLL_CTL_MOD 3 /* Change file decriptor epoll_event structure.  */
#endif



int epoll_create (int __size);

int epoll_ctl (int __epfd, int __op, int __fd, struct epoll_event * __ev);

int epoll_wait (int __epfd, struct epoll_event * __events, int __maxevents, int __timeout);

#endif

#endif /* NET_API_H_ */
