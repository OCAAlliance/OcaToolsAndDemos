/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteModelGUID
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteModelGUID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteModelGUID::OcaLiteModelGUID()
    : ::IOcaLiteMarshal(),
      m_reserved(),
      m_mfrCode(),
      m_modelCode()
{
}

OcaLiteModelGUID::OcaLiteModelGUID(const ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MFR_CODE_LENGTH>& mfrCode,
                           const ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MODEL_CODE_LENGTH>& modelCode)
    : ::IOcaLiteMarshal(),
      m_reserved(),
      m_mfrCode(mfrCode),
      m_modelCode(modelCode)
{
}

OcaLiteModelGUID::OcaLiteModelGUID(::OcaUint64 guid)
    : ::IOcaLiteMarshal(),
      m_reserved(),
      m_mfrCode(),
      m_modelCode()
{
    // Copy the manufacturer code
    ::OcaUint8 mfrCode[OCA_MODEL_GUID_MFR_CODE_LENGTH];
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < OCA_MODEL_GUID_MFR_CODE_LENGTH; i++)
    {
        ::OcaUint16 shiftBytes(OCA_MODEL_GUID_MODEL_CODE_LENGTH + ((OCA_MODEL_GUID_MFR_CODE_LENGTH - i) - static_cast< ::OcaUint16>(1)));
        mfrCode[i] = static_cast< ::OcaUint8>((guid >> (shiftBytes * static_cast< ::OcaUint16>(8))) & static_cast< ::OcaUint64>(0x0FFLL));
    }
    m_mfrCode = ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MFR_CODE_LENGTH>(mfrCode);

    // Copy the model code
    ::OcaUint8 modelCode[OCA_MODEL_GUID_MODEL_CODE_LENGTH];
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < OCA_MODEL_GUID_MODEL_CODE_LENGTH; i++)
    {
        ::OcaUint16 shiftBytes((OCA_MODEL_GUID_MODEL_CODE_LENGTH - i) - static_cast< ::OcaUint16>(1));
        modelCode[i] = static_cast< ::OcaUint8>((guid >> (shiftBytes * static_cast< ::OcaUint16>(8))) & static_cast< ::OcaUint64>(0x0FFLL));
    }
    m_modelCode = ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MODEL_CODE_LENGTH>(modelCode);
}

OcaLiteModelGUID::OcaLiteModelGUID(const ::OcaLiteModelGUID& source)
    : ::IOcaLiteMarshal(source),
      m_reserved(source.m_reserved),
      m_mfrCode(source.m_mfrCode),
      m_modelCode(source.m_modelCode)
{
}

OcaLiteModelGUID::~OcaLiteModelGUID()
{
}

::OcaLiteModelGUID& OcaLiteModelGUID::operator=(const ::OcaLiteModelGUID& source)
{
    if (this != &source)
    {
        m_reserved = source.m_reserved;
        m_mfrCode = source.m_mfrCode;
        m_modelCode = source.m_modelCode;
    }

    return *this;
}

bool OcaLiteModelGUID::operator==(const ::OcaLiteModelGUID& rhs) const
{
    bool result(m_reserved == rhs.m_reserved);
    result = result && (m_mfrCode == rhs.m_mfrCode);
    result = result && (m_modelCode == rhs.m_modelCode);

    return result;
}

bool OcaLiteModelGUID::operator!=(const ::OcaLiteModelGUID& rhs) const
{
    return !(operator==(rhs));
}

::OcaUint64 OcaLiteModelGUID::ToUint64() const
{
    ::OcaUint64 result(static_cast< ::OcaUint64>(0));

    // Copy the manufacturer code
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < OCA_MODEL_GUID_MFR_CODE_LENGTH; i++)
    {
        result = result << 8;
        result += static_cast< ::OcaUint64>(m_mfrCode.GetValue()[i]);
    }

    // Copy the model code
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < OCA_MODEL_GUID_MODEL_CODE_LENGTH; i++)
    {
        result = result << 8;
        result += static_cast< ::OcaUint64>(m_modelCode.GetValue()[i]);
    }

    return result;
}

void OcaLiteModelGUID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_reserved.Marshal(destination, writer);
    m_mfrCode.Marshal(destination, writer);
    m_modelCode.Marshal(destination, writer);
}

bool OcaLiteModelGUID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_reserved.Unmarshal(bytesLeft, source, reader));
    result = result && m_mfrCode.Unmarshal(bytesLeft, source, reader);
    result = result && m_modelCode.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        m_reserved = ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_RESERVED_LENGTH>();
        m_mfrCode = ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MFR_CODE_LENGTH>();
        m_modelCode = ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MODEL_CODE_LENGTH>();
    }

    return result;
}

::OcaUint32 OcaLiteModelGUID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_reserved.GetSize(writer));
    length += m_mfrCode.GetSize(writer);
    length += m_modelCode.GetSize(writer);

    return length;
}
