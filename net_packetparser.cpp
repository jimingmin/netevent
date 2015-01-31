/*
 * net_packetparser.cpp
 *
 *  Created on: 2015Äê01ÔÂ26ÈÕ
 *      Author: jimm
 */

#include "net_packetparser.h"
extern CallBackSet g_CallBackSet;

NETEVENT_NAMESPACE_BEGIN

int32_t CNetPacketParser::Init()
{
	return 0;
}

int32_t CNetPacketParser::Uninit()
{
	return 0;
}

int32_t CNetPacketParser::GetSize()
{
	return 0;
}

int32_t CNetPacketParser::Parser(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t &nPacketSize)
{
	return g_CallBackSet.func_net_parser(arrBuf, nBufSize, arrPacket, &nPacketSize);
}

NETEVENT_NAMESPACE_END
