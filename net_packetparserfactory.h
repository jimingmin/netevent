/*
 * net_packetparserfactory.h
 *
 *  Created on: 2014Äê12ÔÂ29ÈÕ
 *      Author: jimm
 */

#ifndef NET_PACKETPARSERFACTORY_H_
#define NET_PACKETPARSERFACTORY_H_

#include "net_impl.h"
#include "net_typedef.h"

NETEVENT_NAMESPACE_BEGIN

class CNetPacketParserFactory : public IPacketParserFactory
{
public:
	CNetPacketParserFactory(callback_net_parser func_net_parser);

	virtual IPacketParser *Create();

	virtual void Destory(IPacketParser *pParser);

protected:
	callback_net_parser			m_pFuncNetParser;
};


NETEVENT_NAMESPACE_END

#endif /* NET_PACKETPARSERFACTORY_H_ */
