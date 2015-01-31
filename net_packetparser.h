/*
 * net_packetparser.h
 *
 *  Created on: 2015Äê01ÔÂ26ÈÕ
 *      Author: jimm
 */

#ifndef NET_PACKETPARSER_H_
#define NET_PACKETPARSER_H_

#include "net_impl.h"

NETEVENT_NAMESPACE_BEGIN

class CNetPacketParser : public IPacketParser
{
public:

	virtual ~CNetPacketParser(){};

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	virtual int32_t Parser(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t &nPacketSize);

};

NETEVENT_NAMESPACE_END

#endif /* NET_PACKETPARSER_H_ */
