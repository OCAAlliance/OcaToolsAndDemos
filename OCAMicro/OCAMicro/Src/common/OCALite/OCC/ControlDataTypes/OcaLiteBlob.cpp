/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteBlob
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteBlob.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBlob::OcaLiteBlob()
    : ::IOcaLiteMarshal(),
      m_dataSize(static_cast< ::OcaUint16>(0))
{
    ::memset(m_data, 0, MAX_NR_OF_BYTES_IN_BLOB);
}

OcaLiteBlob::OcaLiteBlob(::OcaUint16 dataSize, const ::OcaUint8* data)
    : ::IOcaLiteMarshal(),
      m_dataSize(dataSize)
{
    if (static_cast< ::OcaUint16>(0) < m_dataSize)
    {
        assert(m_dataSize <= MAX_NR_OF_BYTES_IN_BLOB);
        ::memcpy(m_data, data, static_cast<size_t>(m_dataSize));
    }
}

OcaLiteBlob::OcaLiteBlob(const ::OcaLiteBlob& source)
    : ::IOcaLiteMarshal(source),
      m_dataSize(source.m_dataSize)      
{
    if (static_cast< ::OcaUint16>(0) < m_dataSize)
    {
        assert(NULL != source.m_data);
        assert(m_dataSize <= MAX_NR_OF_BYTES_IN_BLOB);
        ::memcpy(m_data, source.m_data, static_cast<size_t>(m_dataSize));
    }
}

OcaLiteBlob::~OcaLiteBlob()
{
}

::OcaLiteBlob& OcaLiteBlob::operator=(const ::OcaLiteBlob& source)
{
    if (this != &source)
    {
        m_dataSize = source.m_dataSize;
        if (static_cast< ::OcaUint16>(0) < m_dataSize)
        {
            assert(NULL != source.m_data);
            assert(m_dataSize <= MAX_NR_OF_BYTES_IN_BLOB);
            ::memcpy(m_data, source.m_data, static_cast<size_t>(m_dataSize));
        }
    }

    return *this;
}

bool OcaLiteBlob::operator==(const ::OcaLiteBlob& rhs) const
{
    bool result(m_dataSize == rhs.m_dataSize);
    if (result &&
        (static_cast< ::OcaUint16>(0) < m_dataSize))
    {
        assert(NULL != rhs.m_data);
        result = (0 == ::memcmp(m_data, rhs.m_data, static_cast<size_t>(m_dataSize)));
    }

    return result;
}

bool OcaLiteBlob::operator!=(const ::OcaLiteBlob& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteBlob::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_dataSize, destination);
    writer.Write(m_data, static_cast< ::OcaUint32>(m_dataSize), destination);
}

bool OcaLiteBlob::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    // Delete the old data
    m_dataSize = static_cast< ::OcaUint16>(0);

    bool result(reader.Read(bytesLeft, source, m_dataSize));
    if (result && (m_dataSize > static_cast< ::OcaUint16>(0)))
    {
        result = (m_dataSize <= MAX_NR_OF_BYTES_IN_BLOB);
    }
    result = result && reader.Read(bytesLeft, source, static_cast< ::OcaUint32>(m_dataSize), m_data);

    // Clear all unmarshalled data if unmarshalling failed
    if (!result)
    {
        m_dataSize = static_cast< ::OcaUint16>(0);
    }

    return result;
}

::OcaUint32 OcaLiteBlob::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_dataSize));
    length += writer.GetSize(m_data, static_cast< ::OcaUint32>(m_dataSize));

    return length;
}
