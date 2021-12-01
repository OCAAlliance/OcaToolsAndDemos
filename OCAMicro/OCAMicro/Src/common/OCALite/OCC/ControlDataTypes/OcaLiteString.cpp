/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteString
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>
#include <SharedLibraries/utf8/source/utf8.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteString.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----
static const ::OcaUint8 invalidReplacement(static_cast< ::OcaUint8>(0x7));

// ---- Class Implementation ----

OcaLiteString::OcaLiteString()
    : ::IOcaLiteMarshal(),
      m_string(),
      m_bValid(static_cast< ::OcaBoolean>(true))
{
}

OcaLiteString::OcaLiteString(const ::OcaUint8* s)
    : ::IOcaLiteMarshal(),
      m_string(),
      m_bValid(static_cast< ::OcaBoolean>(true))
{
    // Copy the array to the member string
    static_cast<void>(m_string.assign(reinterpret_cast<const char*>(s)));
    VerifyString();
}

OcaLiteString::OcaLiteString(const char* s)
    : ::IOcaLiteMarshal(),
      m_string(),
      m_bValid(static_cast< ::OcaBoolean>(true))
{
    // Copy the array to the member string
    static_cast<void>(m_string.assign(s));
    VerifyString();
}

OcaLiteString::OcaLiteString(const std::string& s)
    : ::IOcaLiteMarshal(),
      m_string(s),
      m_bValid(static_cast< ::OcaBoolean>(true))
{
    VerifyString();
}

OcaLiteString::OcaLiteString(const ::OcaLiteString& source)
    : ::IOcaLiteMarshal(source),
      m_string(source.m_string),
      m_bValid(source.m_bValid)
{
}

OcaLiteString::~OcaLiteString()
{
}

::OcaUint16 OcaLiteString::GetLength() const
{
    return static_cast< ::OcaUint16>(utf8::unchecked::distance(m_string.begin(), m_string.end()));
}

const ::OcaUint8* OcaLiteString::GetValue() const
{
    return reinterpret_cast<const ::OcaUint8*>(m_string.c_str());
}

::OcaLiteString OcaLiteString::operator+(const ::OcaLiteString& s)
{
    return operator+=(s);
}

::OcaLiteString& OcaLiteString::operator+=(const ::OcaLiteString& s)
{
    // First copy the current member string, to enable concatenation to itself
    std::string copied(m_string);
    static_cast<void>(copied.append(reinterpret_cast<const char*>(s.GetValue())));
    m_string = copied;

    return *this;
}

::OcaLiteString& OcaLiteString::operator=(const ::OcaLiteString& source)
{
    if (this != &source)
    {
        m_string = source.m_string;
        m_bValid = source.m_bValid;
    }

    return *this;
}

bool OcaLiteString::operator==(const ::OcaLiteString& rhs) const
{
    bool result(m_string == rhs.m_string);
    result = result && (m_bValid == rhs.m_bValid);

    return result;
}

bool OcaLiteString::operator!=(const ::OcaLiteString& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteString::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 byteLength(static_cast< ::OcaUint32>(m_string.length()));
    writer.Write(GetLength(), destination);
    writer.Write(reinterpret_cast<const ::OcaUint8*>(m_string.c_str()), byteLength, destination);
}

bool OcaLiteString::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint16 stringLength;
    bool result(reader.Read(bytesLeft, source, stringLength));

    // Make the member string empty and reserve enough space to hold a string with all 4-byte characters
    m_string.clear();
    m_string.reserve(static_cast<size_t>((4 * static_cast<UINT32>(stringLength)) + 1));

    if (result &&
        (stringLength > (static_cast< ::OcaUint16>(0))))
    {
        // Read bytes from the buffer until all characters are read (or an error occurred)
        ::OcaUint16 readChars(static_cast< ::OcaUint16>(0));
        ::OcaUint8 b(static_cast< ::OcaUint8>(0));
        do
        {
            result = result && (reader.Read(bytesLeft, source, b));
            m_string.push_back(static_cast<char>(b));

            // Get the sequence length for the given byte
            utf8::uint8_t seqLength(static_cast<utf8::uint8_t>(utf8::internal::sequence_length(&b)));

            // Read more bytes if needed (or an error occurred)
            while (result && (seqLength > 1))   // We already read the first byte of the octet
            {
                result = result && (reader.Read(bytesLeft, source, b));
                m_string.push_back(static_cast<char>(b));
                seqLength--;
            }

            readChars++;
        } while (result && (readChars < stringLength));
    }

    if (result)
    {
        VerifyString();
        result = static_cast<bool>(m_bValid);
    }

    if (!result)
    {
        // Clear the string
        m_string.clear();
    }

    return result;
}

::OcaUint32 OcaLiteString::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 byteLength(static_cast< ::OcaUint32>(m_string.length()));
    ::OcaUint32 length(writer.GetSize(GetLength()));
    length += writer.GetSize(reinterpret_cast<const ::OcaUint8*>(m_string.c_str()), byteLength);

    return length;
}

void OcaLiteString::VerifyString()
{
    // Check if the data is valid
    m_bValid = static_cast< ::OcaBoolean>(utf8::is_valid(m_string.begin(), m_string.end()));

    // If the data is invalid, replace it with the replacement character
    if (!m_bValid)
    {
        assert(utf8::internal::is_code_point_valid(static_cast<utf8::uint32_t>(invalidReplacement)));

        std::string corrected;
        static_cast<void>(utf8::unchecked::replace_invalid(m_string.begin(), m_string.end(), back_inserter(corrected), static_cast<utf8::uint32_t>(invalidReplacement)));
        m_string = corrected;
    }
}
