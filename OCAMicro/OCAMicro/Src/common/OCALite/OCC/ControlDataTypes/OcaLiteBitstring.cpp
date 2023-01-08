/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteBitstring
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteBitstring.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBitstring::OcaLiteBitstring()
    : ::IOcaLiteMarshal(),
      m_nrBits(static_cast< ::OcaUint16>(0))
{
    ::memset(m_bitstring, 0, MAX_NR_OF_BYTES_IN_BITSTRING);
}

OcaLiteBitstring::OcaLiteBitstring(::OcaUint16 nrBits)
    : ::IOcaLiteMarshal(),
      m_nrBits(nrBits)
{
    if (static_cast< ::OcaUint16>(0) < m_nrBits)
    {
        ::OcaUint16 nrBytes(GetNrBytes());
        assert(nrBytes <= MAX_NR_OF_BYTES_IN_BITSTRING);
        ::memset(m_bitstring, 0, static_cast<size_t>(nrBytes));
    }
}

OcaLiteBitstring::OcaLiteBitstring(::OcaUint16 nrBits, const ::OcaUint8* bitstring)
    : ::IOcaLiteMarshal(),
      m_nrBits(nrBits)
{
    if (static_cast< ::OcaUint16>(0) < m_nrBits)
    {
        assert(NULL != bitstring);
        ::OcaUint16 nrBytes(GetNrBytes());
        assert(nrBytes <= MAX_NR_OF_BYTES_IN_BITSTRING);
        ::memcpy(m_bitstring, bitstring, static_cast<size_t>(nrBytes));
    }
}

OcaLiteBitstring::OcaLiteBitstring(const ::OcaLiteBitstring& source)
    : ::IOcaLiteMarshal(source),
      m_nrBits(source.m_nrBits)
{
    if (static_cast< ::OcaUint16>(0) < m_nrBits)
    {
        assert(NULL != source.m_bitstring);
        ::OcaUint16 nrBytes(GetNrBytes());
        assert(nrBytes <= MAX_NR_OF_BYTES_IN_BITSTRING);
        ::memcpy(m_bitstring, source.m_bitstring, static_cast<size_t>(nrBytes));
    }
}

OcaLiteBitstring::~OcaLiteBitstring()
{
}

::OcaUint16 OcaLiteBitstring::GetNrBytes() const
{
    ::OcaUint16 nrBytes(static_cast< ::OcaUint16>(m_nrBits / static_cast< ::OcaUint16>(8)));
    if (static_cast< ::OcaUint16>(0) != static_cast< ::OcaUint16>(m_nrBits % static_cast< ::OcaUint16>(8)))
    {
        nrBytes++;
    }
    return nrBytes;
}

bool OcaLiteBitstring::GetBit(::OcaUint16 bitNr) const
{
    bool result(false);

    if (bitNr < m_nrBits)
    {
        ::OcaUint16 byte(static_cast< ::OcaUint16>(bitNr / static_cast< ::OcaUint16>(8)));
        ::OcaUint8 mask(static_cast< ::OcaUint8>(0x80 >> (bitNr % static_cast< ::OcaUint16>(8))));
        assert(NULL != m_bitstring);
        result = (static_cast< ::OcaUint8>(0) != (m_bitstring[byte] & mask));
    }

    return result;
}

bool OcaLiteBitstring::SetBit(::OcaUint16 bitNr, bool value)
{
    bool result(false);
    if (bitNr < m_nrBits)
    {
        ::OcaUint16 byte(static_cast< ::OcaUint16>(bitNr / static_cast< ::OcaUint16>(8)));
        ::OcaUint8 mask(static_cast< ::OcaUint8>(0x80 >> (bitNr % static_cast< ::OcaUint16>(8))));
        assert(NULL != m_bitstring);
        if (value)
        {
            // Set to 1
            m_bitstring[byte] |= mask;
        }
        else
        {
            // Set to 0
            m_bitstring[byte] &= ~mask;
        }
        result = true;
    }
    return result;
}

::OcaLiteBitstring& OcaLiteBitstring::operator=(const ::OcaLiteBitstring& source)
{
    if (this != &source)
    {
        m_nrBits = source.m_nrBits;
        if (static_cast< ::OcaUint16>(0) < m_nrBits)
        {
            assert(NULL != source.m_bitstring);
            ::OcaUint16 nrBytes(GetNrBytes());
            assert(nrBytes <= MAX_NR_OF_BYTES_IN_BITSTRING);
            ::memcpy(m_bitstring, source.m_bitstring, static_cast<size_t>(nrBytes));
        }
    }

    return *this;
}

bool OcaLiteBitstring::operator==(const ::OcaLiteBitstring& rhs) const
{
    bool result(m_nrBits == rhs.m_nrBits);
    if (result &&
        (static_cast< ::OcaUint16>(0) < m_nrBits))
    {
        assert(NULL != m_bitstring);
        assert(NULL != rhs.m_bitstring);
        ::OcaUint16 nrBytes(GetNrBytes());
        result = (0 == ::memcmp(m_bitstring, rhs.m_bitstring, static_cast<size_t>(nrBytes)));
    }

    return result;
}

bool OcaLiteBitstring::operator!=(const ::OcaLiteBitstring& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteBitstring::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_nrBits, destination);
    ::OcaUint16 nrBytes(GetNrBytes());
    writer.Write(m_bitstring, static_cast< ::OcaUint32>(nrBytes), destination);
}

bool OcaLiteBitstring::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    // Delete the old bitstring
    m_nrBits = static_cast< ::OcaUint16>(0);

    bool result(reader.Read(bytesLeft, source, m_nrBits));
    ::OcaUint16 nrBytes(GetNrBytes());
    if (result &&
        (static_cast< ::OcaUint16>(0) < m_nrBits))
    {
        result = (nrBytes <= MAX_NR_OF_BYTES_IN_BITSTRING);
    }
    result = result && reader.Read(bytesLeft, source, static_cast< ::OcaUint32>(nrBytes), m_bitstring);

    // Clear all unmarshalled data if unmarshalling failed
    if (!result)
    {
        m_nrBits = static_cast< ::OcaUint16>(0);
    }

    return result;
}

::OcaUint32 OcaLiteBitstring::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_nrBits));
    ::OcaUint16 nrBytes(GetNrBytes());
    length += writer.GetSize(m_bitstring, static_cast< ::OcaUint32>(nrBytes));

    return length;
}
