/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaStreamParametersAes67
 *
 */

// ---- Include system wide include files ----
#include <OCP.1/Ocp1LiteWriter.h>
#include <OCP.1/Ocp1LiteReader.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaStreamParametersAES67.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaStreamParametersAes67::OcaLiteMediaStreamParametersAes67()
    : ::OcaLiteMediaStreamParameters(),
    m_sdpVersion(),
    m_userName(),
    m_sessionID(),
    m_sessionVersion(),
    m_originAddress(),
    m_sessionName(),
    m_destinationAddress(),
    m_timeToLive(),
    m_mediaLabel(),
    m_packetTime(),
    m_offset(),
    m_linkOffset(),
    m_payloadType()
{
    UpdateBlob();
}

OcaLiteMediaStreamParametersAes67::OcaLiteMediaStreamParametersAes67(::OcaUint16 sdpVersion,
                                                                     const ::OcaLiteString& userName,
                                                                     ::OcaUint64 sessionID,
                                                                     ::OcaUint64 sessionVersion,
                                                                     const ::Ocp1LiteNetworkAddress& originAddress,
                                                                     const ::OcaLiteString& sessionName,
                                                                     const ::Ocp1LiteNetworkAddress& destinationAddress,
                                                                     ::OcaUint8 timeToLive,
                                                                     const ::OcaLiteString& mediaLabel,
                                                                     ::OcaTimeInterval packetTime,
                                                                     ::OcaUint32 offset,
                                                                     ::OcaTimeInterval linkOffset,
                                                                     ::OcaUint8 payloadType)
    : ::OcaLiteMediaStreamParameters(),
    m_sdpVersion(sdpVersion),
    m_userName(userName),
    m_sessionID(sessionID),
    m_sessionVersion(sessionVersion),
    m_originAddress(originAddress),
    m_sessionName(sessionName),
    m_destinationAddress(destinationAddress),
    m_timeToLive(timeToLive),
    m_mediaLabel(mediaLabel),
    m_packetTime(packetTime),
    m_offset(offset),
    m_linkOffset(linkOffset),
    m_payloadType(payloadType)
{
    UpdateBlob();
}

OcaLiteMediaStreamParametersAes67::OcaLiteMediaStreamParametersAes67(const ::OcaLiteMediaStreamParametersAes67& source)
    : ::OcaLiteMediaStreamParameters(source),
    m_sdpVersion(source.m_sdpVersion),
    m_userName(source.m_userName),
    m_sessionID(source.m_sessionID),
    m_sessionVersion(source.m_sessionVersion),
    m_originAddress(source.m_originAddress),
    m_sessionName(source.m_sessionName),
    m_destinationAddress(source.m_destinationAddress),
    m_timeToLive(source.m_timeToLive),
    m_mediaLabel(source.m_mediaLabel),
    m_packetTime(source.m_packetTime),
    m_offset(source.m_offset),
    m_linkOffset(source.m_linkOffset),
    m_payloadType(source.m_payloadType)
{
    UpdateBlob();
}

OcaLiteMediaStreamParametersAes67::~OcaLiteMediaStreamParametersAes67()
{
}

::OcaLiteMediaStreamParametersAes67& OcaLiteMediaStreamParametersAes67::operator=(const ::OcaLiteMediaStreamParametersAes67& source)
{
    if (this != &source)
    {
        OcaLiteMediaStreamParameters::operator=(source);
        m_sdpVersion = source.m_sdpVersion;
        m_userName = source.m_userName;
        m_sessionID = source.m_sessionID;
        m_sessionVersion = source.m_sessionVersion;
        m_originAddress = source.m_originAddress;
        m_sessionName = source.m_sessionName;
        m_destinationAddress = source.m_destinationAddress;
        m_timeToLive = source.m_timeToLive;
        m_mediaLabel = source.m_mediaLabel;
        m_packetTime = source.m_packetTime;
        m_offset = source.m_offset;
        m_linkOffset = source.m_linkOffset;
        m_payloadType = source.m_payloadType;
    }

    return *this;
}

::OcaLiteMediaStreamParametersAes67* OcaLiteMediaStreamParametersAes67::CreateFromBase(const ::OcaLiteMediaStreamParameters& baseClass)
{
    ::OcaLiteMediaStreamParametersAes67* result(NULL);
    ::OcaUint16 sdpVersion;
    ::OcaLiteString userName;
    ::OcaUint64 sessionID;
    ::OcaUint64 sessionVersion;
    ::Ocp1LiteNetworkAddress originAddress;
    ::OcaLiteString sessionName;
    ::Ocp1LiteNetworkAddress destinationAddress;
    ::OcaUint8 timeToLive;
    ::OcaLiteString mediaLabel;
    ::OcaTimeInterval packetTime;
    ::OcaUint32 offset;
    ::OcaTimeInterval linkOffset;
    ::OcaUint8 payloadType;

    if (GetValuesFromBase(baseClass, sdpVersion, userName, sessionID, sessionVersion, originAddress, sessionName, destinationAddress, timeToLive, mediaLabel, packetTime, offset, linkOffset, payloadType))
    {
        result = new ::OcaLiteMediaStreamParametersAes67(sdpVersion, userName, sessionID, sessionVersion, originAddress, sessionName, destinationAddress, timeToLive, mediaLabel, packetTime, offset, linkOffset, payloadType);
    }
    else
    {
        OCA_LOG_WARNING("Stream parameters not an OcaLiteMediaStreamParametersAes67");
    }

    return result;
}

bool OcaLiteMediaStreamParametersAes67::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteMediaStreamParameters::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_sdpVersion, m_userName, m_sessionID, m_sessionVersion, m_originAddress, m_sessionName, m_destinationAddress, m_timeToLive, m_mediaLabel, m_packetTime, m_offset, m_linkOffset, m_payloadType);
    return success;
}

bool OcaLiteMediaStreamParametersAes67::GetValuesFromBase(const ::OcaLiteMediaStreamParameters& baseClass, ::OcaUint16& sdpVersion, ::OcaLiteString& userName, ::OcaUint64& sessionID, ::OcaUint64& sessionVersion, ::Ocp1LiteNetworkAddress& originAddress, ::OcaLiteString& sessionName, ::Ocp1LiteNetworkAddress& destinationAddress, ::OcaUint8& timeToLive, ::OcaLiteString& mediaLabel, ::OcaTimeInterval& packetTime, ::OcaUint32& offset, ::OcaTimeInterval& linkOffset, ::OcaUint8& payloadType)
{
    bool success(true);

    const ::OcaUint8* buffer(baseClass.GetValue().GetData());
    ::OcaUint32 bytesLeft(baseClass.GetValue().GetDataSize());

    Ocp1LiteReader reader;

    success = reader.Read(bytesLeft, &buffer, sdpVersion);
    success = success && userName.Unmarshal(bytesLeft, &buffer, reader);
    success = success && reader.Read(bytesLeft, &buffer, sessionID);
    success = success && reader.Read(bytesLeft, &buffer, sessionVersion);
    success = success && originAddress.Unmarshal(bytesLeft, &buffer, reader);
    success = success && sessionName.Unmarshal(bytesLeft, &buffer, reader);
    success = success && destinationAddress.Unmarshal(bytesLeft, &buffer, reader);
    success = success && reader.Read(bytesLeft, &buffer, timeToLive);
    success = success && mediaLabel.Unmarshal(bytesLeft, &buffer, reader);
    success = success && reader.Read(bytesLeft, &buffer, packetTime);
    success = success && reader.Read(bytesLeft, &buffer, offset);
    success = success && reader.Read(bytesLeft, &buffer, linkOffset);
    success = success && reader.Read(bytesLeft, &buffer, payloadType);

    return success;
}

void OcaLiteMediaStreamParametersAes67::UpdateBlob()
{
    Ocp1LiteWriter writer;
    ::OcaUint32 totSize(writer.GetSize(m_sdpVersion));
    totSize += m_userName.GetSize(writer);
    totSize += writer.GetSize(m_sessionID);
    totSize += writer.GetSize(m_sessionVersion);
    totSize += m_originAddress.GetSize(writer);
    totSize += m_sessionName.GetSize(writer);
    totSize += m_destinationAddress.GetSize(writer);
    totSize += writer.GetSize(m_timeToLive);
    totSize += m_mediaLabel.GetSize(writer);
    totSize += writer.GetSize(m_packetTime);
    totSize += writer.GetSize(m_offset);
    totSize += writer.GetSize(m_linkOffset);
    totSize += writer.GetSize(m_payloadType);

    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(totSize)]);
    ::OcaUint8* pBuffer(buffer);

    // Fill the buffer
    writer.Write(m_sdpVersion, &pBuffer);
    m_userName.Marshal(&pBuffer, writer);
    writer.Write(m_sessionID, &pBuffer); 
    writer.Write(m_sessionVersion, &pBuffer);
    m_originAddress.Marshal(&pBuffer, writer);
    m_sessionName.Marshal(&pBuffer, writer);
    m_destinationAddress.Marshal(&pBuffer, writer);
    writer.Write(m_timeToLive, &pBuffer);
    m_mediaLabel.Marshal(&pBuffer, writer);
    writer.Write(m_packetTime, &pBuffer);
    writer.Write(m_offset, &pBuffer);
    writer.Write(m_linkOffset, &pBuffer);
    writer.Write(m_payloadType, &pBuffer);

    ::OcaLiteBlob ocaBlob(static_cast< ::OcaUint16>(totSize), buffer);
    OcaLiteMediaStreamParameters::SetValue(ocaBlob);

    delete[] buffer;
}
