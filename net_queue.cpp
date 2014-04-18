/*
 * net_queue.cpp
 *
 *  Created on: 2013年12月17日
 *      Author: jimm
 */

#include "net_queue.h"

NETEVENT_NAMESPACE_BEGIN

void CNetQueue::Push(NetPacket *pPacket)
{
	if(pPacket != NULL)
	{
		m_stNetPacketQueue.push_back(pPacket);
	}
}

NetPacket *CNetQueue::Pop()
{
	if(m_stNetPacketQueue.empty())
	{
		return NULL;
	}

	NetPacket *pPacket = m_stNetPacketQueue.front();
	m_stNetPacketQueue.pop_front();
	return pPacket;
}

NetPacket *CNetQueue::Front()
{
	if(m_stNetPacketQueue.empty())
	{
		return NULL;
	}

	return m_stNetPacketQueue.front();
}

bool CNetQueue::Empty()
{
	return m_stNetPacketQueue.empty();
}

NETEVENT_NAMESPACE_END
