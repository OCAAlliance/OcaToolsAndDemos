/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLitePortID
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLitePortID.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLitePortID::OcaLitePortID()
    : ::IOcaLiteMarshal(),
      m_mode(static_cast< ::OcaPortMode>(0)),
      m_index(static_cast< ::OcaUint16>(0))
{
}

OcaLitePortID::OcaLitePortID(::OcaPortMode mode, ::OcaUint16 idx)
    : ::IOcaLiteMarshal(),
      m_mode(mode),
      m_index(idx)
{
    assert(m_mode > 0);
    assert(m_mode < OCAPORTMODE_MAXIMUM);
}

OcaLitePortID::OcaLitePortID(const ::OcaLitePortID& source)
    : ::IOcaLiteMarshal(source),
      m_mode(source.m_mode),
      m_index(source.m_index)
{
}

OcaLitePortID::~OcaLitePortID()
{
}

::OcaLitePortID& OcaLitePortID::operator=(const ::OcaLitePortID& source)
{
    if (this != &source)
    {
        m_mode = source.m_mode;
        m_index = source.m_index;
    }

    return *this;
}

bool OcaLitePortID::operator==(const ::OcaLitePortID& rhs) const
{
    bool result(m_mode == rhs.m_mode);
    result = result && (m_index == rhs.m_index);

    return result;
}

bool OcaLitePortID::operator!=(const ::OcaLitePortID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLitePortID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    MarshalValue< ::OcaPortMode>(m_mode, destination, writer);
    writer.Write(m_index, destination);
}

bool OcaLitePortID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(UnmarshalValue< ::OcaPortMode>(m_mode, bytesLeft, source, reader));
    result = result && reader.Read(bytesLeft, source, m_index);

    if (!result)
    {
        m_mode = static_cast< ::OcaPortMode>(0);
        m_index = static_cast< ::OcaUint16>(0);
    }

    return result;
}

::OcaUint32 OcaLitePortID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(GetSizeValue< ::OcaPortMode>(m_mode, writer));
    length += writer.GetSize(m_index);

    return length;
}

// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaPortMode>(const ::OcaPortMode& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaPortMode>(::OcaPortMode& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 mode(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, mode));
    if (result)
    {
        if ((mode > static_cast< ::OcaUint8>(0)) &&
            (mode < static_cast< ::OcaUint8>(OCAPORTMODE_MAXIMUM)))
        {
            value = static_cast< ::OcaPortMode>(mode);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaPortMode>(const ::OcaPortMode& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
