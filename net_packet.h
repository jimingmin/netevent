/*
 * net_packet.h
 *
 *  Created on: 2013年12月16日
 *      Author: Administrator
 */

#ifndef NET_PACKET_H_
#define NET_PACKET_H_

#include "net_namespace.h"
#include "net_typedef.h"
#include <list>
using namespace std;

NETEVENT_NAMESPACE_BEGIN

class NetPacket
{
public:
	NetPacket()
	{
		m_nNetPacketLen = 0;
		m_nConnectionID = 0;
	}

	virtual ~NetPacket()
	{
	}

	uint32_t 		m_nNetPacketLen;	//不包含自身4字节的长度
	ConnectionID	m_nConnectionID;	//连接的唯一标示
	uint8_t			m_pNetPacket[0];
};

typedef list<NetPacket *>	NetPacketList;

NETEVENT_NAMESPACE_END

#endif /* NET_PACKET_H_ */
