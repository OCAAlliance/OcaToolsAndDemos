/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteNetworkSignalChannel
 *
 */

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Agents/OcaLiteStreamNetwork.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkStatistics.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkSignalChannel.h"

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_NETWORK_SIGNAL_CHANNEL_CLASSID};
const ::OcaLiteClassID          OcaLiteNetworkSignalChannel::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkSignalChannel::OcaLiteNetworkSignalChannel(::OcaONo objectNumber,
                                                         ::OcaBoolean lockable,
                                                         const ::OcaLiteString& role,
                                                         const ::OcaLiteList< ::OcaLitePort>& ports,
                                                         const ::OcaLiteNetworkSignalChannelID& idAdvertised,
                                                         ::OcaONo network,
                                                         ::OcaLiteNetworkMediaSourceOrSink sourceOrSink)
    : ::OcaLiteWorker(objectNumber,
                      lockable,
                      role,
                      ports),
      m_sourceOrSink(sourceOrSink),
      m_status(OCANETWORKSIGNALCHANNELSTATUS_NOT_CONNECTED),
      m_connectorPins(),
      m_remoteChannelID(),
      m_idAdvertised(idAdvertised),
      m_network(network)
{
}

OcaLiteNetworkSignalChannel::~OcaLiteNetworkSignalChannel()
{
}


::OcaLiteStatus OcaLiteNetworkSignalChannel::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_ID_ADVERTISED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_idAdvertised.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            m_idAdvertised.Marshal(&pResponse, writer);

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
            case GET_NETWORK:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaONo>(m_network, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaONo>(m_network, &pResponse, writer);

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
            case GET_REMOTE_CHANNEL_ID:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_remoteChannelID.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            m_remoteChannelID.Marshal(&pResponse, writer);

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
            case SET_REMOTE_CHANNEL_ID:
                {
                    ::OcaLiteNetworkSignalChannelID signalChannelID;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        (signalChannelID.Unmarshal(bytesLeft, &pCmdParameters, reader)))
                    {
                        rc = SetRemoteChannelID(signalChannelID);

                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(0/*NrParameters*/), &pResponse);

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
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteNetworkMediaSourceOrSink>(m_sourceOrSink, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaLiteNetworkMediaSourceOrSink>(m_sourceOrSink, &pResponse, writer);

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
            case GET_STATUS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteNetworkSignalChannelStatus>(m_status, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaLiteNetworkSignalChannelStatus>(m_status, &pResponse, writer);

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
            case GET_CONNECTOR_PINS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_connectorPins.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            m_connectorPins.Marshal(&pResponse, writer);

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
            case SET_NETWORK:
            case SET_ID_ADVERTISED:
            case ADD_TO_CONNECTOR:
            case REMOVE_FROM_CONNECTOR:
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
            rc = OcaLiteWorker::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}



::OcaLiteStatus OcaLiteNetworkSignalChannel::AddToConnector(::OcaONo connector, ::OcaLiteStreamConnectorPinIndex index)
{

    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);
    if ((OCA_INVALID_ONO != connector) && (!m_connectorPins.ContainsKey(connector)))
    {
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Failed to add signal channel to connector %d, already present in %d", connector, m_connectorPins.GetCount());
    }

    if (OCASTATUS_OK == rc)
    {
        //Add the connector
        ::OcaLiteMapItem< ::OcaONo, ::OcaLiteStreamConnectorPinIndex> item(connector, index);
        m_connectorPins.Add(item);

        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CONNECTOR_PINS));
        ::OcaLitePropertyChangedEventData< ::OcaLiteMap< ::OcaONo, ::OcaLiteStreamConnectorPinIndex> > eventData(GetObjectNumber(),
                                                                                                                 propertyID,
                                                                                                                 m_connectorPins,
                                                                                                                 OCAPROPERTYCHANGETYPE_ITEM_ADDED);
        PropertyChanged(eventData, propertyID);
    }
    else
    {
        OCA_LOG_WARNING_PARAMS("Unable to add this signal channel to connector %u", connector);
    }

    return rc;
}

::OcaLiteStatus OcaLiteNetworkSignalChannel::RemoveFromConnector(::OcaONo connector)
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);
    if (m_connectorPins.ContainsKey(connector))
    {
        rc = OCASTATUS_OK;
    }

    if (OCASTATUS_OK == rc)
    {
        m_connectorPins.Remove(connector);

        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CONNECTOR_PINS));
        ::OcaLitePropertyChangedEventData< ::OcaLiteMap< ::OcaONo, ::OcaLiteStreamConnectorPinIndex> > eventData(GetObjectNumber(),
                                                                                                                 propertyID,
                                                                                                                 m_connectorPins,
                                                                                                                 OCAPROPERTYCHANGETYPE_ITEM_DELETED);
        PropertyChanged(eventData, propertyID);
    }
    else
    {
        OCA_LOG_WARNING_PARAMS("Unable to remove this signal channel from connector %u", connector);
    }

    return rc;
}


::OcaLiteStatus OcaLiteNetworkSignalChannel::SetRemoteChannelID(const ::OcaLiteNetworkSignalChannelID& signalChannelID)
{
    ::OcaLiteStatus rc(SetRemoteChannelIDValue(signalChannelID));
    if (OCASTATUS_OK == rc)
    {
        RemoteChannelIDChanged(signalChannelID);
    }
    return rc;
}

::OcaLiteStatus  OcaLiteNetworkSignalChannel::SetRemoteChannelIDValue(const ::OcaLiteNetworkSignalChannelID& signalChannelID)
{
    return OCASTATUS_PROCESSING_FAILED;
}

::OcaClassVersionNumber OcaLiteNetworkSignalChannel::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteWorker::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

void OcaLiteNetworkSignalChannel::StatusChanged(::OcaLiteNetworkSignalChannelStatus status)
{
    m_status = status;

    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STATUS));
    ::OcaLitePropertyChangedEventData< ::OcaLiteNetworkSignalChannelStatus> eventData(GetObjectNumber(),
                                                                                      propertyID,
                                                                                      status,
                                                                                      OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}

void OcaLiteNetworkSignalChannel::RemoteChannelIDChanged(const ::OcaLiteNetworkSignalChannelID& remoteChannelID)
{
    m_remoteChannelID = remoteChannelID;

    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_REMOTE_CHANNEL_ID));
    ::OcaLitePropertyChangedEventData< ::OcaLiteNetworkSignalChannelID> eventData(GetObjectNumber(),
                                                                                  propertyID,
                                                                                  remoteChannelID,
                                                                                  OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}


// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaLiteNetworkSignalChannelStatus>(const ::OcaLiteNetworkSignalChannelStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteNetworkSignalChannelStatus>(::OcaLiteNetworkSignalChannelStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 status(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, status));
    if (result)
    {
        if (status < static_cast< ::OcaUint8>(OCANETWORKSIGNALCHANNELSTATUS_MAXIMUM))
        {
            value = static_cast< ::OcaLiteNetworkSignalChannelStatus>(status);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkSignalChannelStatus>(const ::OcaLiteNetworkSignalChannelStatus& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
