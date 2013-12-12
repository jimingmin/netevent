/*
 * net_datetime.h
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */

#ifndef NET_DATETIME_H_
#define NET_DATETIME_H_

#include "net_namespace.h"
#include "net_typedef.h"

NETEVENT_NAMESPACE_BEGIN

enum
{
	enmDateTimeStringSize				= 20,
	enmDateTimeStringLength				= 19
};


//日期时间类
class CDateTime
{
public:
	static CDateTime CurrentDateTime();

public:
	CDateTime();
	CDateTime(const int64_t nDateTime);
	CDateTime(const char * szDateTimeString);
	CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay);
	CDateTime(const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond);
	CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond, const uint16_t nMilliseconds);
	CDateTime(const CDateTime& dt);
	virtual ~CDateTime();


public:
	uint16_t Year() const;
	uint16_t Month() const;
	uint16_t Day() const;
	uint16_t DayOfWeek() const;
	uint16_t DayOfYear() const;
	uint16_t Hour() const;
	uint16_t Minute() const;
	uint16_t Second() const;
	uint16_t Millisecond() const;

	int64_t Seconds() const;

	int64_t GetValue() const;

	int32_t Date() const;

public:
	void ToDateTimeString(char *szDest) const;
	void ToDateString(char *szDest, int32_t nFormat = 0) const;
	void ToTimeString(char *szDest, int32_t nFormat = 0) const;

protected:
	int64_t			m_nDateTime;	//格林威治标准时间1970-01-01到所表示时间的间隔微秒数
};


NETEVENT_NAMESPACE_END

#endif /* NET_DATETIME_H_ */
