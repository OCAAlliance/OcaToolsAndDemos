/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMediaClockRate
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaClockRate.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaClockRate::OcaLiteMediaClockRate()
    : ::IOcaLiteMarshal(),
      m_nominalRate(static_cast< ::OcaFrequency>(0.0)),
      m_pullRange(static_cast< ::OcaFrequency>(0.0)),
      m_accuracy(static_cast< ::OcaFloat32>(0.0)),
      m_jitterMax(static_cast< ::OcaFloat32>(0.0))
{
}

OcaLiteMediaClockRate::OcaLiteMediaClockRate(::OcaFrequency nominalRate,
                                     ::OcaFrequency pullRange,
                                     ::OcaFloat32 accuracy,
                                     ::OcaFloat32 jitterMax)
    : ::IOcaLiteMarshal(),
      m_nominalRate(nominalRate),
      m_pullRange(pullRange),
      m_accuracy(accuracy),
      m_jitterMax(jitterMax)
{
}

OcaLiteMediaClockRate::OcaLiteMediaClockRate(const ::OcaLiteMediaClockRate& source)
    : ::IOcaLiteMarshal(source),
      m_nominalRate(source.m_nominalRate),
      m_pullRange(source.m_pullRange),
      m_accuracy(source.m_accuracy),
      m_jitterMax(source.m_jitterMax)
{
}

OcaLiteMediaClockRate::~OcaLiteMediaClockRate()
{
}

::OcaLiteMediaClockRate& OcaLiteMediaClockRate::operator=(const ::OcaLiteMediaClockRate& source)
{
    if (this != &source)
    {
        m_nominalRate = source.m_nominalRate;
        m_pullRange = source.m_pullRange;
        m_accuracy = source.m_accuracy;
        m_jitterMax = source.m_jitterMax;
    }

    return *this;
}

bool OcaLiteMediaClockRate::operator==(const ::OcaLiteMediaClockRate& rhs) const
{
    bool result(CompareValue< ::OcaFrequency>(m_nominalRate, rhs.m_nominalRate));
    result = result && CompareValue< ::OcaFrequency>(m_pullRange, rhs.m_pullRange);
    result = result && CompareValue< ::OcaFloat32>(m_accuracy, rhs.m_accuracy);
    result = result && CompareValue< ::OcaFloat32>(m_jitterMax, rhs.m_jitterMax);

    return result;
}

bool OcaLiteMediaClockRate::operator!=(const ::OcaLiteMediaClockRate& rhs) const
{
    return !(operator==(rhs));
}

::OcaLiteString OcaLiteMediaClockRate::ToString() const
{
    ::OcaLiteString result("nominalRate: ");
    result += ValueToString< ::OcaFrequency>(m_nominalRate);
    result += ::OcaLiteString(",pullRange: ");
    result += ValueToString< ::OcaFrequency>(m_pullRange);
    result += ::OcaLiteString(",accuracy: ");
    result += ValueToString< ::OcaFloat32>(m_accuracy);
    result += ::OcaLiteString(",jitterMax: ");
    result += ValueToString< ::OcaFloat32>(m_jitterMax);

    return result;
}

void OcaLiteMediaClockRate::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    MarshalValue< ::OcaFrequency>(m_nominalRate, destination, writer);
    MarshalValue< ::OcaFrequency>(m_pullRange, destination, writer);
    MarshalValue< ::OcaFloat32>(m_accuracy, destination, writer);
    MarshalValue< ::OcaFloat32>(m_jitterMax, destination, writer);
}

bool OcaLiteMediaClockRate::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(UnmarshalValue< ::OcaFrequency>(m_nominalRate, bytesLeft, source, reader));
    result = result && UnmarshalValue< ::OcaFrequency>(m_pullRange, bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaFloat32>(m_accuracy, bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaFloat32>(m_jitterMax, bytesLeft, source, reader);

    if (!result)
    {
        m_nominalRate = static_cast< ::OcaFrequency>(0.0);
        m_pullRange = static_cast< ::OcaFrequency>(0.0);
        m_accuracy = static_cast< ::OcaFloat32>(0.0);
        m_jitterMax = static_cast< ::OcaFloat32>(0.0);
    }

    return result;
}

::OcaUint32 OcaLiteMediaClockRate::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(GetSizeValue< ::OcaFrequency>(m_nominalRate, writer));
    length += GetSizeValue< ::OcaFrequency>(m_pullRange, writer);
    length += GetSizeValue< ::OcaFloat32>(m_accuracy, writer);
    length += GetSizeValue< ::OcaFloat32>(m_jitterMax, writer);

    return length;
}
