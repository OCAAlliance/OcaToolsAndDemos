/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteStreamConnector
 *
 */

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStreamConnector.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_STREAM_CONNECTOR_CLASSID};
const ::OcaLiteClassID              OcaLiteStreamConnector::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStreamConnector::OcaLiteStreamConnector(::OcaONo objectNumber,
                                               ::OcaBoolean lockable,
                                               const ::OcaLiteString& role,
                                               ::OcaONo owner,
                                               const ::OcaLiteStreamConnectorID& idAdvertised,
                                               const ::OcaLiteNetworkMediaSourceOrSink& sourceOrSink,
                                               ::OcaUint16 pinCount)
                                               : ::OcaLiteAgent(objectNumber, lockable, role),
      m_ownerNetwork(owner),
      m_idAdvertised(idAdvertised),
      m_sourceOrSink(sourceOrSink),
      m_status(OCASTREAMCONNECTORSTATUS_IDLE),
      m_lastStreamIndex(static_cast< ::OcaLiteStreamIndex>(0))
{
    for(::OcaUint16 pin(static_cast< ::OcaUint16>(0)); pin < pinCount; pin++)
    {
        ::OcaLiteMapItem< ::OcaLiteStreamConnectorPinIndex, ::OcaONo>item(pin, OCA_INVALID_ONO);
        m_pins.Add(item);
    }
}

OcaLiteStreamConnector::~OcaLiteStreamConnector()
{
    m_pins.Clear();
}

::OcaLiteStatus OcaLiteStreamConnector::GetOwnerNetwork(::OcaONo& network) const
{
    network = m_ownerNetwork;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteStreamConnector::GetIDAdvertised(::OcaLiteStreamConnectorID& idAdvertised) const
{
    idAdvertised = m_idAdvertised;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteStreamConnector::GetSourceOrSink(::OcaLiteNetworkMediaSourceOrSink& sourceOrSink) const
{
    sourceOrSink = m_sourceOrSink;

    return OCASTATUS_OK;
}


::OcaLiteStatus OcaLiteStreamConnector::GetStreams(::OcaLiteList< ::OcaLiteStream>& streams) const
{
    streams.Clear();
    streams.Add(m_stream);

    return OCASTATUS_OK;
}


::OcaLiteStatus OcaLiteStreamConnector::GetPins(::OcaLiteMap< ::OcaLiteStreamConnectorPinIndex, ::OcaONo>& pins) const
{
    pins.Clear();
    pins = m_pins;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteStreamConnector::GetStatus(::OcaLiteStreamConnectorStatus& status) const
{
    status = m_status;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteStreamConnector::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_OWNER_NETWORK:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaONo owner;
                        rc = GetOwnerNetwork(owner);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                    ::GetSizeValue< ::OcaONo>(owner, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                MarshalValue< ::OcaONo>(owner, &pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }    
                    }
                }
                break;           
            case GET_ID_ADVERTISED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteStreamConnectorID streamConnectorId;
                        rc = GetIDAdvertised(streamConnectorId);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                ::GetSizeValue< ::OcaLiteStreamConnectorID>(streamConnectorId, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                streamConnectorId.Marshal(&pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_SOURCE_OR_SINK:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteNetworkMediaSourceOrSink sourceOrSink;
                        rc = GetSourceOrSink(sourceOrSink);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteNetworkMediaSourceOrSink>(sourceOrSink, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                MarshalValue< ::OcaLiteNetworkMediaSourceOrSink>(sourceOrSink, &pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_STREAMS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteStream> streams;
                        rc = GetStreams(streams);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     streams.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                streams.Marshal(&pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_PINS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteMap< ::OcaLiteStreamConnectorPinIndex, ::OcaONo> pins;
                        rc = GetPins(pins);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     pins.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                pins.Marshal(&pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_STATUS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteStreamConnectorStatus status;
                        rc = GetStatus(status);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                ::GetSizeValue< ::OcaLiteStreamConnectorStatus>(status, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                MarshalValue< ::OcaLiteStreamConnectorStatus>(status, &pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case SET_OWNER_NETWORK:
            case SET_ID_ADVERTISED:
            case SET_SOURCE_OR_SINK:
            case CONNECT_STREAM:
            case DISCONNECT_STREAM:
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteAgent::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaLiteStatus OcaLiteStreamConnector::AddSignalChannel(::OcaONo signalChannel, ::OcaLiteStreamConnectorPinIndex index)
{
    ::OcaLiteStatus rc(OCASTATUS_PROCESSING_FAILED);
    ::OcaLiteNetworkSignalChannel* pNetworkSignalChannel(dynamic_cast< ::OcaLiteNetworkSignalChannel*>(::OcaLiteBlock::GetRootBlock().GetOCAObject(signalChannel)));
    if (NULL != pNetworkSignalChannel)
    {
        ::OcaLiteNetworkMediaSourceOrSink sourceOrSink(pNetworkSignalChannel->GetSourceOrSink());
        if (m_sourceOrSink == sourceOrSink)
        {
            rc = OCASTATUS_OK;
        }
        else
        {
            rc = OCASTATUS_PROCESSING_FAILED;
        }
    }

    if (OCASTATUS_OK == rc)
    {
        m_pins.Remove(index, OCA_INVALID_ONO);

        ::OcaLiteMapItem< ::OcaLiteStreamConnectorPinIndex, ::OcaONo>item(index, signalChannel);
        m_pins.Add(item);
    }
    else
    {
        OCA_LOG_WARNING("Unable to add signal channel to this connector");
    }

    return rc;
}

::OcaLiteStatus OcaLiteStreamConnector::DeleteSignalChannel(::OcaONo signalChannel, ::OcaLiteStreamConnectorPinIndex index)
{   
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);
    if (m_pins.ContainsKey(index) && (OCA_INVALID_ONO != m_pins.GetValue(index)))
    {
        rc = OCASTATUS_OK;
    }
    
    if (OCASTATUS_OK == rc)
    {
        m_pins.Remove(index, signalChannel);

        if (!m_pins.ContainsKey(index))
        {
            ::OcaLiteMapItem< ::OcaLiteStreamConnectorPinIndex, ::OcaONo> item(index, OCA_INVALID_ONO);
            m_pins.Add(item);
        }
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Unable to delete signal channel %u to connector %u", index, GetObjectNumber());
    }
    return rc;
}

::OcaClassVersionNumber OcaLiteStreamConnector::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteAgent::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaLiteStreamConnectorStatus>(const ::OcaLiteStreamConnectorStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteStreamConnectorStatus>(::OcaLiteStreamConnectorStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 status(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, status));
    if (result)
    {
        if (status < static_cast< ::OcaUint8>(OCASTREAMCONNECTORSTATUS_MAXIMUM))
        {
            value = static_cast< ::OcaLiteStreamConnectorStatus>(status);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteStreamConnectorStatus>(const ::OcaLiteStreamConnectorStatus& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
