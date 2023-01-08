/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLitePTPSeconds.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLitePTPSeconds::OcaLitePTPSeconds()
    : ::IOcaLiteMarshal(),
      m_seconds(static_cast< ::OcaUint64>(0))
{
}

OcaLitePTPSeconds::OcaLitePTPSeconds(::OcaUint64 seconds)
    : ::IOcaLiteMarshal(),
      m_seconds(seconds)
{
    assert(seconds <= MAX_PTP_SECONDS_VALUE);
}

OcaLitePTPSeconds::OcaLitePTPSeconds(const ::OcaLitePTPSeconds& source)
    : ::IOcaLiteMarshal(source),
      m_seconds(source.m_seconds)
{
}

OcaLitePTPSeconds::~OcaLitePTPSeconds()
{
}

OcaLitePTPSeconds::operator ::OcaUint64() const
{
    return m_seconds;
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator+=(const ::OcaLitePTPSeconds& seconds)
{
    return operator+=(seconds.m_seconds);
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator+=(::OcaUint64 seconds)
{
    assert((m_seconds + seconds) <= MAX_PTP_SECONDS_VALUE);
    m_seconds += seconds;

    return *this;
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator++()
{
    assert(m_seconds < MAX_PTP_SECONDS_VALUE);

    m_seconds++;

    return *this;
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator-=(const ::OcaLitePTPSeconds& seconds)
{
    return operator-=(seconds.m_seconds);
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator-=(::OcaUint64 seconds)
{
    assert((static_cast<INT64>(m_seconds) - static_cast<INT64>(seconds)) >= 0);
    m_seconds -= seconds;

    return *this;
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator--()
{
    assert(m_seconds > static_cast< ::OcaUint64>(1));

    m_seconds--;

    return *this;
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator=(const ::OcaLitePTPSeconds& source)
{
    if (this != &source)
    {
        m_seconds = source.m_seconds;
    }

    return *this;
}

::OcaLitePTPSeconds& OcaLitePTPSeconds::operator=(::OcaUint64 seconds)
{
    assert(seconds <= MAX_PTP_SECONDS_VALUE);

    m_seconds = seconds;

    return *this;
}

bool OcaLitePTPSeconds::operator==(const ::OcaLitePTPSeconds& rhs) const
{
    bool result(m_seconds == rhs.m_seconds);

    return result;
}

bool OcaLitePTPSeconds::operator==(::OcaUint64 rhs) const
{
    bool result(m_seconds == rhs);

    return result;
}

bool OcaLitePTPSeconds::operator!=(const ::OcaLitePTPSeconds& rhs) const
{
    return !(operator==(rhs));
}

bool OcaLitePTPSeconds::operator!=(::OcaUint64 rhs) const
{
    return !(operator==(rhs));
}

bool OcaLitePTPSeconds::operator<(const ::OcaLitePTPSeconds& rhs) const
{
    return (m_seconds < rhs.m_seconds);
}

bool OcaLitePTPSeconds::operator<(::OcaUint64 rhs) const
{
    return (m_seconds < rhs);
}

bool OcaLitePTPSeconds::operator<=(const ::OcaLitePTPSeconds& rhs) const
{
    return !(operator>(rhs));
}

bool OcaLitePTPSeconds::operator<=(::OcaUint64 rhs) const
{
    return !(operator>(rhs));
}

bool OcaLitePTPSeconds::operator>(const ::OcaLitePTPSeconds& rhs) const
{
    return (m_seconds > rhs.m_seconds);
}

bool OcaLitePTPSeconds::operator>(::OcaUint64 rhs) const
{
    return (m_seconds > rhs);
}

bool OcaLitePTPSeconds::operator>=(const ::OcaLitePTPSeconds& rhs) const
{
    return !(operator<(rhs));
}

bool OcaLitePTPSeconds::operator>=(::OcaUint64 rhs) const
{
    return !(operator<(rhs));
}

void OcaLitePTPSeconds::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    assert(NULL != destination);

    MarshalValue< ::OcaUint64>(m_seconds, destination, writer);
}

bool OcaLitePTPSeconds::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    assert(NULL != source);

    bool result(UnmarshalValue< ::OcaUint64>(m_seconds, bytesLeft, source, reader));

    if (!result)
    {
        m_seconds = static_cast< ::OcaUint64>(0);
    }

    return result;
}

::OcaUint32 OcaLitePTPSeconds::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(GetSizeValue< ::OcaUint64>(m_seconds, writer));

    return length;
}

// ---- Additional operators ----

::OcaLitePTPSeconds operator+(::OcaLitePTPSeconds lhs, const ::OcaLitePTPSeconds& rhs)
{
    lhs += rhs;
    return lhs;
}

::OcaLitePTPSeconds operator+(::OcaLitePTPSeconds lhs, ::OcaUint64 rhs)
{
    lhs += rhs;
    return lhs;
}

::OcaLitePTPSeconds operator-(::OcaLitePTPSeconds lhs, const ::OcaLitePTPSeconds& rhs)
{
    lhs += rhs;
    return lhs;
}

::OcaLitePTPSeconds operator-(::OcaLitePTPSeconds lhs, ::OcaUint64 rhs)
{
    lhs += rhs;
    return lhs;
}
