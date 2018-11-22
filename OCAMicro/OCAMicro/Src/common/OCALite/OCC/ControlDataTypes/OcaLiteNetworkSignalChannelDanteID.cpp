/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkSignalChannelDanteID
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>
#include <OCP.1/Ocp1LiteReader.h>
#include <OCP.1/Ocp1LiteWriter.h>
// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkSignalChannelDanteID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkSignalChannelDanteID::OcaLiteNetworkSignalChannelDanteID(const ::OcaLiteString& danteID)
    : ::OcaLiteNetworkSignalChannelID(),
      m_danteId(danteID)
{
    UpdateBlob();
}

OcaLiteNetworkSignalChannelDanteID::OcaLiteNetworkSignalChannelDanteID(const ::OcaLiteNetworkSignalChannelDanteID& source)
    : ::OcaLiteNetworkSignalChannelID(source),
      m_danteId(source.m_danteId)
{
    UpdateBlob();
}

OcaLiteNetworkSignalChannelDanteID::~OcaLiteNetworkSignalChannelDanteID()
{
}

const ::OcaLiteString& OcaLiteNetworkSignalChannelDanteID::GetdanteId() const
{
    return m_danteId;
}

::OcaLiteNetworkSignalChannelDanteID& OcaLiteNetworkSignalChannelDanteID::operator=(const ::OcaLiteNetworkSignalChannelDanteID& source)
{
    if (this != &source)
    {
        OcaLiteNetworkSignalChannelID::operator=(source);
        m_danteId = source.m_danteId;
    }

    return *this;
}

::OcaBoolean OcaLiteNetworkSignalChannelDanteID::getDanteIDFromBase(const ::OcaLiteNetworkSignalChannelID& baseClass, OcaLiteString &danteID)
{
    return GetValuesFromBase(baseClass, danteID);
}

::OcaLiteNetworkSignalChannelDanteID* OcaLiteNetworkSignalChannelDanteID::CreateFromBase(const ::OcaLiteNetworkSignalChannelID& baseClass)
{
    ::OcaLiteNetworkSignalChannelDanteID* result(NULL);
    ::OcaLiteString danteID;

    if (GetValuesFromBase(baseClass, danteID))
    {
        result = new ::OcaLiteNetworkSignalChannelDanteID(danteID);
    }
    else
    {
        OCA_LOG_WARNING("Channel ID not an OcaLiteNetworkSignalChannelDanteID");
    }

    return result;
}

bool OcaLiteNetworkSignalChannelDanteID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteNetworkSignalChannelID::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_danteId);
    return success;
}

bool OcaLiteNetworkSignalChannelDanteID::GetValuesFromBase(const ::OcaLiteNetworkSignalChannelID& baseClass, ::OcaLiteString& danteId)
{
    ::OcaUint32 bytesLeft(static_cast< ::OcaUint32>(baseClass.GetValue().GetDataSize()));

    ::Ocp1LiteReader reader;
    const ::OcaUint8* buffer(baseClass.GetValue().GetData());
    bool success(danteId.Unmarshal(bytesLeft, &buffer, reader));
    success = success && (static_cast< ::OcaUint32>(0) == bytesLeft);

    return success;
}

void OcaLiteNetworkSignalChannelDanteID::UpdateBlob()
{
    ::Ocp1LiteWriter writer;
    ::OcaUint32 size(m_danteId.GetSize(writer));
    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(size)]);
    ::OcaUint8* pBuffer(buffer);

    m_danteId.Marshal(&pBuffer, writer);

    ::OcaLiteBlob ocaBlob(static_cast< ::OcaUint16>(size), buffer);
    OcaLiteNetworkSignalChannelID::SetValue(ocaBlob);

    delete[] buffer;
}
