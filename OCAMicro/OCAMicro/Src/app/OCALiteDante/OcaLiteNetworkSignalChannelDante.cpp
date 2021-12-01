/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteNetworkSignalChannelDante
 *
 */

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkSignalChannelDante.h"
#include "DanteLiteHostInterface.h"

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_NETWORK_SIGNAL_CHANNEL_DANTE_CLASSID};
const ::OcaLiteClassID          OcaLiteNetworkSignalChannelDante::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ---

OcaLiteNetworkSignalChannelDante::OcaLiteNetworkSignalChannelDante(::OcaONo objectNumber,
                                                                   ::OcaBoolean lockable,
                                                                   const ::OcaLiteString& role,
                                                                   const ::OcaLiteList< ::OcaLitePort>& ports,
                                                                   const ::OcaLiteNetworkSignalChannelDanteID& idAdvertised,
                                                                   ::OcaONo network,
                                                                   ::OcaLiteNetworkMediaSourceOrSink sourceOrSink,
                                                                   const ::OcaLiteList< ::OcaLiteEncoding>& encodings,
                                                                   ::OcaLiteFrequency sampleRate,
                                                                   ::OcaUint16 channelNr)
    : ::OcaLiteNetworkSignalChannel(objectNumber, 
                                    lockable, role, 
                                    ports, idAdvertised, 
                                    network, sourceOrSink),
       m_encodings(encodings),
       m_sampleRate(sampleRate),
       m_channelNr(channelNr),
       m_DanteSubscription(idAdvertised)
{
    RemoteChannelIDChanged(::OcaLiteNetworkSignalChannelDanteID(::OcaLiteString("")));
}

OcaLiteNetworkSignalChannelDante::~OcaLiteNetworkSignalChannelDante()
{
}

::OcaLiteStatus OcaLiteNetworkSignalChannelDante::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                                          ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_ERROR);
    if (!IsLocked(sessionID))
    {
        if (methodID.GetDefLevel() == CLASS_ID.GetFieldCount())
        {
            ::OcaUint8* responseBuffer(NULL);
            const ::OcaUint8* pCmdParameters(parameters);
            ::OcaUint32 bytesLeft(parametersSize);

            switch (methodID.GetMethodIndex())
            {
            case GET_ENCODINGS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_encodings.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            m_encodings.Marshal(&pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_CHANNEL_LABEL:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteString label;
                        DanteLiteHostInterfaceGetDanteChannelLabel(m_channelNr, (GetSourceOrSink() == OCANETWORKMEDIASOURCEORSINK_SINK), label);
                        
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + label.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            label.Marshal(&pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_SAMPLERATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteFrequency>(m_sampleRate, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaLiteFrequency>(m_sampleRate, &pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_CHANNEL_NUMBER:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaUint16>(m_channelNr, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaUint16>(m_channelNr, &pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case SET_CHANNEL_LABEL:
                {
                    rc = OCASTATUS_NOT_IMPLEMENTED;
                }
                break;
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteNetworkSignalChannel::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaClassVersionNumber OcaLiteNetworkSignalChannelDante::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteNetworkSignalChannel::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteNetworkSignalChannelDante::SetRemoteChannelIDValue(::OcaLiteNetworkSignalChannelID signalChannelID)
{
    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    if (GetSourceOrSink() == OCANETWORKMEDIASOURCEORSINK_SINK)
    {
        OcaLiteNetworkSignalChannelDanteID* signalChannelIDDante(OcaLiteNetworkSignalChannelDanteID::CreateFromBase(signalChannelID));
        if (NULL != signalChannelIDDante)
        {
            // Subscribe to the channel
            if(DanteLiteHostInterfaceSetDanteChannelSubscription(m_channelNr, signalChannelIDDante->GetStringValue().GetLength() > 0 ? signalChannelIDDante->GetStringValue().GetString().c_str() : NULL))
            {
                rc = OCASTATUS_OK;
            }

            delete signalChannelIDDante;
        }
    }

    return rc;
}

void OcaLiteNetworkSignalChannelDante::DanteRemoteChannelIDChanged(::OcaLiteNetworkSignalChannelID remoteChannelID)
{
    OcaLiteNetworkSignalChannel::RemoteChannelIDChanged(remoteChannelID);
    m_DanteSubscription.GetStringValueFromBase(remoteChannelID, m_SubscriptionStr);
}

::OcaBoolean OcaLiteNetworkSignalChannelDante::GetSubscriptionName(OcaLiteString &Name)
{
    if(m_SubscriptionStr.GetLength()) {
        Name = m_SubscriptionStr;
        return true;
    }
    return false;
}
