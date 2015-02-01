/*
 * net_packetparserfactory.h
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

#include "net_packetparserfactory.h"
#include "net_packetparser.h"
#include "../common/common_memmgt.h"

NETEVENT_NAMESPACE_BEGIN

CNetPacketParserFactory::CNetPacketParserFactory(callback_net_parser func_net_parser)
{
	m_pFuncNetParser = func_net_parser;
}

IPacketParser *CNetPacketParserFactory::Create()
{
	CNetPacketParser *pNetPacketParser = NEW(CNetPacketParser);
	pNetPacketParser->SetNetParser(m_pFuncNetParser);
	return pNetPacketParser;
}

void CNetPacketParserFactory::Destory(IPacketParser *pParser)
{
	DELETE(pParser);
}

NETEVENT_NAMESPACE_END
