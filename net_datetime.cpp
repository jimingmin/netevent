/*
 * net_datetime.cpp
 *
 *  Created on: 2013年11月4日
 *      Author: jimm
 */


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "net_datetime.h"
#include "net_typedef.h"

NETEVENT_NAMESPACE_BEGIN

#ifdef WIN32
//伪函数
int gettimeofday(struct timeval *a_pstTv, struct timezone *a_pstTz)
{
	return 0;
}
struct tm* localtime_r(const time_t *timep, struct tm *result)
{
	return NULL;
}
char* asctime_r(const struct tm *tm, char *buf)
{
	return NULL;
}
char* ctime_r(const time_t *timep, char *buf)
{
	return NULL;
}
struct tm* gmtime_r(const time_t *timep, struct tm *result)
{
	return NULL;
}
#endif




CDateTime CDateTime::CurrentDateTime()
{
	time_t now;
	time(&now);
	return CDateTime((int64_t)now * US_PER_SECOND);
}

CDateTime::CDateTime()
{
	m_nDateTime = 0;
}

CDateTime::CDateTime(const int64_t nDateTime)
{
	m_nDateTime = nDateTime;
}

CDateTime::CDateTime(const char * szDateTimeString)
{
	struct tm st = { 0 };
	char szDateTime[enmDateTimeStringSize] = { 0 };

	strncpy(szDateTime, szDateTimeString, sizeof(szDateTime) - 1);
	if (strlen(szDateTime) < enmDateTimeStringLength)
	{
		m_nDateTime = 0;
		return;
	}

	char* p = szDateTime;

	p[4] = '\0';
	st.tm_year = atoi(p) - 1900;
	p += 5;

	p[2] = '\0';
	st.tm_mon = atoi(p) - 1;
	p += 3;

	p[2] = '\0';
	st.tm_mday = atoi(p);
	p += 3;

	p[2] = '\0';
	st.tm_hour = atoi(p);
	p += 3;

	p[2] = '\0';
	st.tm_min = atoi(p);
	p += 3;

	p[2] = '\0';
	st.tm_sec = atoi(p);
	p += 3;

	time_t t = mktime(&st);

	struct tm st2 = { 0 };
	localtime_r(&t, &st2);
	time_t t2 = mktime(&st2);

	m_nDateTime = (int64_t)t2 * US_PER_SECOND;
}

CDateTime::CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay)
{
	struct tm st = { 0 };

	st.tm_year = (int)nYear - 1900;
	st.tm_mon = nMonth - 1;
	st.tm_mday = nDay;

	time_t t = mktime(&st);

	struct tm st2 = { 0 };
	localtime_r(&t, &st2);
	time_t t2 = mktime(&st2);

	m_nDateTime = (int64_t)t2 * US_PER_SECOND;
}

CDateTime::CDateTime(const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond)
{
	m_nDateTime = ((((int64_t)nDay * HOUR_PER_DAY + nHour) * MINUTE_PER_HOUR + nMinute) * SECOND_PER_MINUTE + nSecond) * US_PER_SECOND;
}

CDateTime::CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond, const uint16_t nMilliseconds)
{
	struct tm st = { 0 };

	st.tm_year = (int)nYear - 1900;
	st.tm_mon = nMonth - 1;
	st.tm_mday = nDay;
	st.tm_hour = nHour;
	st.tm_min = nMinute;
	st.tm_sec = nSecond;

	time_t t = mktime(&st);

	struct tm st2 = { 0 };
	localtime_r(&t, &st2);
	time_t t2 = mktime(&st2);

	m_nDateTime = (int64_t)t2 * US_PER_SECOND + nMilliseconds * MS_PER_SECOND;
}

CDateTime::CDateTime(const CDateTime& dt)
{
	m_nDateTime = dt.m_nDateTime;
}

CDateTime::~CDateTime()
{

}

uint16_t CDateTime::Year() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_year + 1900;
}

uint16_t CDateTime::Month() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_mon + 1;
}

uint16_t CDateTime::Day() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_mday;
}

uint16_t CDateTime::DayOfWeek() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_wday;
}

uint16_t CDateTime::DayOfYear() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_yday;
}

uint16_t CDateTime::Hour() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_hour;
}

uint16_t CDateTime::Minute() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_min;
}

uint16_t CDateTime::Second() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_sec;
}

uint16_t CDateTime::Millisecond() const
{
	return (uint16_t)((m_nDateTime % US_PER_SECOND) / US_PER_MS);
}

int64_t CDateTime::Seconds() const
{
	return m_nDateTime / US_PER_SECOND;
}

int64_t CDateTime::GetValue() const
{
	return m_nDateTime;
}

int32_t CDateTime::Date() const
{
	return (Year()*10000 + Month()*100 + Day());
}

void CDateTime::ToDateTimeString(char *szDest) const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);
	sprintf(szDest, "%04d-%02d-%02d %02d:%02d:%02d", st.tm_year + 1900, st.tm_mon + 1, st.tm_mday, st.tm_hour, st.tm_min, st.tm_sec);
}

void CDateTime::ToDateString(char *szDest, int32_t nFormat/* = 0*/) const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);
	switch (nFormat)
	{
	case 1:
		{
			char strMonth[] = {0xE6, 0x9C, 0x88, 0};
			char strDay[] = {0xE6, 0x97, 0xA5, 0};
			sprintf(szDest, "%d%s%d%s", st.tm_mon + 1, strMonth, st.tm_mday, strDay);
		}
		break;
	default:
		sprintf(szDest, "%04d-%02d-%02d", st.tm_year + 1900, st.tm_mon + 1, st.tm_mday);
		break;
	}
}

void CDateTime::ToTimeString(char *szDest, int32_t nFormat/* = 0*/) const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);
	switch (nFormat)
	{
	case 1:
		{
			sprintf(szDest, "%02d:%02d", st.tm_hour, st.tm_min);
		}
		break;
	default:
		sprintf(szDest, "%02d:%02d:%02d", st.tm_hour, st.tm_min, st.tm_sec);
		break;
	}
}

NETEVENT_NAMESPACE_END

