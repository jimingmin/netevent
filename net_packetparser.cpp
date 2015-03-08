/*
 * net_packetparser.cpp
 *
 *  Created on: 2015Äê01ÔÂ26ÈÕ
 *      Author: jimm
 */

#include "net_packetparser.h"

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

int32_t CNetPacketParser::Parser(const uint8_t arrInputBuf[], const uint32_t nInputBufSize, uint8_t arrOutputBuf[], int32_t nOutputBufSize)
{
	return m_pFuncNetParser(arrInputBuf, nInputBufSize, arrOutputBuf, nOutputBufSize);
}

void CNetPacketParser::SetNetParser(callback_net_parser func_net_parser)
{
	m_pFuncNetParser = func_net_parser;
}

NETEVENT_NAMESPACE_END
