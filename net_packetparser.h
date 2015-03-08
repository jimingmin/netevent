/*
 * net_packetparser.h
 *
 *  Created on: 2015Äê01ÔÂ26ÈÕ
 *      Author: jimm
 */

#ifndef NET_PACKETPARSER_H_
#define NET_PACKETPARSER_H_

#include "net_impl.h"
#include "net_typedef.h"

NETEVENT_NAMESPACE_BEGIN

class CNetPacketParser : public IPacketParser
{
public:

	virtual ~CNetPacketParser(){};

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	virtual int32_t Parser(const uint8_t arrInputBuf[], const uint32_t nInputBufSize, uint8_t arrOutputBuf[], int32_t nOutputBufSize);

	void SetNetParser(callback_net_parser func_net_parser);
protected:
	callback_net_parser m_pFuncNetParser;
};

NETEVENT_NAMESPACE_END

#endif /* NET_PACKETPARSER_H_ */
