/*
 * net_queue.h
 *
 *  Created on: 2013年12月17日
 *      Author: jimm
 */

#ifndef NET_QUEUE_H_
#define NET_QUEUE_H_

#include "net_namespace.h"
#include "net_packet.h"

#include <list>
using namespace std;

NETEVENT_NAMESPACE_BEGIN

class CNetQueue
{
public:
	void Push(NetPacket *pPacket);

	NetPacket *Pop();

	NetPacket *Front();

	bool Empty();

protected:
	list<NetPacket *>		m_stNetPacketQueue;
};

NETEVENT_NAMESPACE_END

#endif /* NET_QUEUE_H_ */
