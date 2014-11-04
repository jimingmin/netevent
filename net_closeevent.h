/*
 * net_closeevent.h
 *
 *  Created on: 2013Äê12ÔÂ16ÈÕ
 *      Author: jimm
 */

#ifndef NET_CLOSEEVENT_H_
#define NET_CLOSEEVENT_H_

#include "net_namespace.h"
#include "net_typedef.h"
#include "../common/common_object.h"

NETEVENT_NAMESPACE_BEGIN

class CloseEvent : public CBaseObject
{
public:
	CloseEvent()
	{
		m_nSessionID = 0;
		m_nCloseCode = 0;
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
		return sizeof(*this);
	}

	SessionID		m_nSessionID;
	int32_t			m_nCloseCode;
};

NETEVENT_NAMESPACE_END

#endif /* NET_CLOSEEVENT_H_ */
