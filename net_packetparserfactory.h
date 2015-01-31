/*
 * net_packetparserfactory.h
 *
 *  Created on: 2014Äê12ÔÂ29ÈÕ
 *      Author: jimm
 */

#ifndef NET_PACKETPARSERFACTORY_H_
#define NET_PACKETPARSERFACTORY_H_

#include "net_impl.h"

NETEVENT_NAMESPACE_BEGIN

class CNetPacketParserFactory : public IPacketParserFactory
{
public:
	CNetPacketParserFactory();

	virtual IPacketParser *Create();

	virtual void Destory(IPacketParser *pParser);
};


NETEVENT_NAMESPACE_END

#endif /* NET_PACKETPARSERFACTORY_H_ */
