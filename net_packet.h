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
#include "../common/common_object.h"

#ifdef WIN32
#pragma warning(disable:4200)
#endif

NETEVENT_NAMESPACE_BEGIN

class NetPacket : public CObject
{
public:
	NetPacket()
	{
		m_nNetPacketLen = 0;
		m_nSessionID = 0;
	}

	virtual int32_t Init()
	{
		return 0;
	}

	virtual int32_t Uninit()
	{
		return 0;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this) + m_nNetPacketLen;
	}

	uint32_t 		m_nNetPacketLen;	//不包含自身4字节的长度
	SessionID		m_nSessionID;		//连接的唯一标示
	uint8_t			m_pNetPacket[0];
};

NETEVENT_NAMESPACE_END

#endif /* NET_PACKET_H_ */
