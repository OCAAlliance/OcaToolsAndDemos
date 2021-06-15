/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteTimePTP
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <time.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteTimePTP.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

/** The maximum value of the nano seconds. */
#define MAX_NANO_SECONDS_VALUE  static_cast< ::OcaUint32>(1000000000)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteTimePTP::OcaLiteTimePTP()
    : ::IOcaLiteMarshal(),
	  m_negative(static_cast<::OcaBoolean>(false)),
	  m_seconds(),
	  m_nanoSeconds(static_cast<::OcaUint32>(0))
{
}

OcaLiteTimePTP::OcaLiteTimePTP(::OcaBoolean negative,
							   const ::OcaLitePTPSeconds& seconds,
							   ::OcaUint32 nanoSeconds)
	: ::IOcaLiteMarshal(),
	  m_negative(negative),
	  m_seconds(seconds),
	  m_nanoSeconds(nanoSeconds)
{
	while (m_nanoSeconds >= MAX_NANO_SECONDS_VALUE)
	{
		++m_seconds;
		m_nanoSeconds -= MAX_NANO_SECONDS_VALUE;
	}
}

OcaLiteTimePTP::OcaLiteTimePTP(const ::OcaLiteTimePTP& source)
    : ::IOcaLiteMarshal(source),
	  m_negative(source.m_negative),
	  m_seconds(source.m_seconds),
	  m_nanoSeconds(source.m_nanoSeconds)
{
}

OcaLiteTimePTP::~OcaLiteTimePTP()
{
}

::OcaLiteTimePTP& OcaLiteTimePTP::operator+=(const ::OcaLiteTimePTP& timePTP)
{
	return PerformBinaryOperation(m_negative == timePTP.m_negative, timePTP);
}

::OcaLiteTimePTP& OcaLiteTimePTP::operator-=(const ::OcaLiteTimePTP& timePTP)
{
	return PerformBinaryOperation(m_negative != timePTP.m_negative, timePTP);
}

::OcaLiteTimePTP& OcaLiteTimePTP::operator=(const ::OcaLiteTimePTP& source)
{
	if (this != &source)
	{
		m_negative = source.m_negative;
		m_seconds = source.m_seconds;
		m_nanoSeconds = source.m_nanoSeconds;
	}

	return *this;
}

bool OcaLiteTimePTP::operator==(const ::OcaLiteTimePTP& rhs) const
{
	bool result(m_negative == rhs.m_negative);
	result = result && (m_seconds == rhs.m_seconds);
	result = result && (m_nanoSeconds == rhs.m_nanoSeconds);

	return result;
}

bool OcaLiteTimePTP::operator!=(const ::OcaLiteTimePTP& rhs) const
{
	return !(operator==(rhs));
}

::OcaLiteTimePTP OcaLiteTimePTP::CreateTime(::OcaUint16 year, ::OcaUint8 month, ::OcaUint8 day, ::OcaUint8 hour, ::OcaUint8 minute, ::OcaUint8 second, ::OcaUint32 nanoSeconds)
{
	assert(year >= static_cast<::OcaUint16>(1970));

	struct tm dateTime;
	::memset(&dateTime, 0, sizeof(dateTime));
	dateTime.tm_year = static_cast<int>(year - static_cast<::OcaUint16>(1900));
	dateTime.tm_mon = static_cast<int>(month - static_cast<::OcaUint8>(1));
	dateTime.tm_mday = static_cast<int>(day);
	dateTime.tm_hour = static_cast<int>(hour);
	dateTime.tm_min = static_cast<int>(minute);
	dateTime.tm_sec = static_cast<int>(second);
	dateTime.tm_isdst = -1; // -1 computes whether standard time or daylight saving time is in effect
	::time_t timeValue(::mktime(&dateTime));

	return ::OcaLiteTimePTP(static_cast<::OcaBoolean>(false), ::OcaLitePTPSeconds(static_cast<::OcaUint64>(static_cast<INT64>(timeValue))), nanoSeconds);
}

bool OcaLiteTimePTP::ParseTime(const ::OcaLiteTimePTP& timePTP, ::OcaUint16& year, ::OcaUint8& month, ::OcaUint8& day, ::OcaUint8& hour, ::OcaUint8& minute, ::OcaUint8& second, ::OcaUint32& nanoSeconds)
{
	bool bSuccess(false);

	::time_t timeValue(static_cast<::time_t>(static_cast<UINT64>(timePTP.GetSeconds())));

	struct tm* dateTime(::localtime(&timeValue));
	if (NULL != dateTime)
	{
		year = static_cast<::OcaUint16>(dateTime->tm_year + 1900);
		month = static_cast<::OcaUint8>(dateTime->tm_mon + 1);
		day = static_cast<::OcaUint8>(dateTime->tm_mday);
		hour = static_cast<::OcaUint8>(dateTime->tm_hour);
		minute = static_cast<::OcaUint8>(dateTime->tm_min);
		second = static_cast<::OcaUint8>(dateTime->tm_sec);
		nanoSeconds = timePTP.GetNanoSeconds();
		bSuccess = true;
	}

	return bSuccess;
}

void OcaLiteTimePTP::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
	assert(NULL != destination);

	MarshalValue< ::OcaBoolean>(m_negative, destination, writer);
	m_seconds.Marshal(destination, writer);
	MarshalValue< ::OcaUint32>(m_nanoSeconds, destination, writer);
}

bool OcaLiteTimePTP::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
	assert(NULL != source);

	bool result(UnmarshalValue< ::OcaBoolean>(m_negative, bytesLeft, source, reader));
	result = result && m_seconds.Unmarshal(bytesLeft, source, reader);
	result = result && UnmarshalValue< ::OcaUint32>(m_nanoSeconds, bytesLeft, source, reader);

	if (!result)
	{
		m_negative = static_cast<::OcaBoolean>(false);
		m_seconds = ::OcaLitePTPSeconds();
		m_nanoSeconds = static_cast<::OcaUint32>(0);
	}

	return result;
}

::OcaUint32 OcaLiteTimePTP::GetSize(const ::IOcaLiteWriter& writer) const
{
	::OcaUint32 length(GetSizeValue< ::OcaBoolean>(m_negative, writer));
	length += m_seconds.GetSize(writer);
	length += GetSizeValue< ::OcaUint32>(m_nanoSeconds, writer);

	return length;
}

::OcaLiteTimePTP& OcaLiteTimePTP::PerformBinaryOperation(bool simpleAddition, const ::OcaLiteTimePTP& timePTP)
{
	if (simpleAddition)
	{
		m_seconds += timePTP.m_seconds;
		m_nanoSeconds += timePTP.m_nanoSeconds;
		if (m_nanoSeconds >= MAX_NANO_SECONDS_VALUE)
		{
			++m_seconds;
			m_nanoSeconds -= MAX_NANO_SECONDS_VALUE;
		}
	}
	else
	{
		if ((m_seconds < timePTP.m_seconds) ||
			((m_seconds == timePTP.m_seconds) &&
			(m_nanoSeconds < timePTP.m_nanoSeconds)))
		{
			// The sign will change, as the absolute added value is larger than the current value

			m_negative = !m_negative;

			if (m_seconds < timePTP.m_seconds)
			{
				m_seconds = (timePTP.m_seconds - m_seconds);
				if (m_nanoSeconds > timePTP.m_nanoSeconds)
				{
					m_nanoSeconds = MAX_NANO_SECONDS_VALUE - (m_nanoSeconds - timePTP.m_nanoSeconds);
					--m_seconds;
				}
				else
				{
					m_nanoSeconds = (timePTP.m_nanoSeconds - m_nanoSeconds);
				}
			}
			else
			{
				assert(m_seconds == timePTP.m_seconds);
				m_seconds = ::OcaLitePTPSeconds(static_cast<::OcaUint64>(0));
				m_nanoSeconds = (timePTP.m_nanoSeconds - m_nanoSeconds);
			}
		}
		else
		{
			// Simple subtraction applies, as the sign will not change

			m_seconds -= timePTP.m_seconds;
			if (m_nanoSeconds < timePTP.m_nanoSeconds)
			{
				m_nanoSeconds = MAX_NANO_SECONDS_VALUE - (timePTP.m_nanoSeconds - m_nanoSeconds);
				--m_seconds;
			}
			else
			{
				m_nanoSeconds -= timePTP.m_nanoSeconds;
			}
		}
	}

	return *this;
}

// ---- Additional operators ----

::OcaLiteTimePTP operator+(::OcaLiteTimePTP lhs, const ::OcaLiteTimePTP& rhs)
{
	lhs += rhs;
	return lhs;
}

::OcaLiteTimePTP operator-(::OcaLiteTimePTP lhs, const ::OcaLiteTimePTP& rhs)
{
	lhs += rhs;
	return lhs;
}
