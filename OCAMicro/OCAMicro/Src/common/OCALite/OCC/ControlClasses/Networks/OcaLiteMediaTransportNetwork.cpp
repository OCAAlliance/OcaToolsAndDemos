/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaTransportNetwork
 *
 */
 
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteMediaConnectorStatusChangedEventData.h>
#include <OCC/ControlDataTypes/OcaLiteMediaSinkConnector.h>
#include <OCC/ControlDataTypes/OcaLiteMediaSinkConnectorChangedEventData.h>
#include <OCC/ControlDataTypes/OcaLiteMediaSourceConnector.h>
#include <OCC/ControlDataTypes/OcaLiteMediaSourceConnectorChangedEventData.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaTransportNetwork.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_MEDIA_TRANSPORT_NETWORK_CLASSID};
const ::OcaLiteClassID          OcaLiteMediaTransportNetwork::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

/** The internal ID of an empty internal connector. */
#define EMPTY_ID_INTERNAL           static_cast< ::OcaLiteMediaConnectorID>(0)

// ---- Helper functions ----

/**
 * Comparator structure to find an empty connector
 */
template <class ConnectorType>
struct connector_empty : public unary_function<ConnectorType, bool>
{
    bool operator()(const ConnectorType& obj) const
    {
        return (obj.IsEmpty());
    }
};

/**
 * Comparator structure to compare the internal ID of a connector
 */
template <class ConnectorType>
struct connector_internal_id : public unary_function<ConnectorType, bool>
{
    explicit connector_internal_id(::OcaLiteMediaConnectorID idInternal)
        : m_idInternal(idInternal)
    {
    }

    bool operator()(const ConnectorType& obj) const
    {
        return (!obj.IsEmpty() && (obj.GetIDInternal() == m_idInternal));
    }

    ::OcaLiteMediaConnectorID   m_idInternal;
};

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaTransportNetwork::OcaLiteMediaTransportNetwork(::OcaONo objectNumber,
                                                           ::OcaBoolean lockable,
                                                           const ::OcaLiteString& role)
    : ::OcaLiteApplicationNetwork(objectNumber, lockable, role),
      m_ocaPorts(),
      m_sourceConnectors(),
      m_sinkConnectors(),
      m_connectorsStatuses(),
      m_pSourceConnectorChangedEventDelegate(NULL),
      m_pSinkConnectorChangedEventDelegate(NULL),
      m_pConnectorStatusChangedEventDelegate(NULL)
{
    ::OcaLiteNetworkManager& networkManager(::OcaLiteNetworkManager::GetInstance());
    static_cast<void>(networkManager.AddMediaTransportNetwork(*this));
}

OcaLiteMediaTransportNetwork::~OcaLiteMediaTransportNetwork()
{
    m_pSourceConnectorChangedEventDelegate = NULL;
    m_pSinkConnectorChangedEventDelegate = NULL;
    m_pConnectorStatusChangedEventDelegate = NULL;

    ::OcaLiteNetworkManager& networkManager(::OcaLiteNetworkManager::GetInstance());
    networkManager.RemoveMediaTransportNetwork(*this);
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetPorts(::OcaLiteList< ::OcaLitePort>& ports) const
{
    ::OcaLiteStatus rc(InternalGetPorts(ports));

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetPortName(const ::OcaLitePortID& id, ::OcaLiteString& name) const
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    for (PortList::const_iterator iter(m_ocaPorts.begin());
         iter != m_ocaPorts.end();
         ++iter)
    {
        if (iter->GetID() == id)
        {
            name = iter->GetName();
            rc = OCASTATUS_OK;
            break;
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetSourceConnectors(::OcaLiteList< ::OcaLiteMediaSourceConnector>& connectors) const
{
    connectors.Clear();
    for (SourceConnectorList::const_iterator it(m_sourceConnectors.begin());
         it != m_sourceConnectors.end();
         ++it)
    {
        if (!it->IsEmpty())
        {
            ::OcaLiteMediaSourceConnector sourceConnector;
            it->AssignToOca(sourceConnector);
            connectors.Add(sourceConnector);
        }
    }
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetSourceConnector(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSourceConnector& connector) const
{
    ::OcaLiteStatus rc(GetSourceConnectorValue(id, connector));

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetSinkConnectors(::OcaLiteList< ::OcaLiteMediaSinkConnector>& connectors) const
{
    connectors.Clear();
    for (SinkConnectorList::const_iterator it(m_sinkConnectors.begin());
         it != m_sinkConnectors.end();
         ++it)
    {
        if (!it->IsEmpty())
        {
            ::OcaLiteMediaSinkConnector sinkConnector;
            it->AssignToOca(sinkConnector);
            connectors.Add(sinkConnector);
        }
    }

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetSinkConnector(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSinkConnector& connector) const
{
    ::OcaLiteStatus rc(GetSinkConnectorValue(id, connector));

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetConnectorsStatuses(::OcaLiteList< ::OcaLiteMediaConnectorStatus>& statuses) const
{
    statuses = m_connectorsStatuses;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetConnectorStatus(::OcaLiteMediaConnectorID id, ::OcaLiteMediaConnectorStatus& status) const
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsStatuses.GetCount(); i++)
    {
        const ::OcaLiteMediaConnectorStatus& connectorStatus(m_connectorsStatuses.GetItem(i));
        if (connectorStatus.GetConnectorID() == id)
        {
            status = connectorStatus;
            rc = OCASTATUS_OK;
            break;
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::AddSourceConnector(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSourceConnector& connector)
{
    // Retrieve some boundary values
    ::OcaUint16 maxPins(static_cast< ::OcaUint16>(0));
    ::OcaLiteStatus rc(GetMaxPinsPerConnector(maxPins));

    // Check the data structure
    bool bConnectorOK((connector.GetPinCount() <= maxPins) &&
                      ArePortsInChannelPinMapValid(connector.GetChannelPinMap()) &&
                      (EMPTY_ID_INTERNAL == connector.GetIDInternal()));

    // Check the initial state
    if (OCASTATUS_OK == rc)
    {
        if ((OCAMEDIACONNECTORSTATE_STOPPED != initialState) &&
            (OCAMEDIACONNECTORSTATE_RUNNING != initialState) &&
            (OCAMEDIACONNECTORSTATE_PAUSED != initialState))
        {
            OCA_LOG_ERROR_PARAMS("Incorrect initial state %u", initialState);
            rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
        }
    }

    if (bConnectorOK &&
        (OCASTATUS_OK == rc))
    {
        // Find the first empty slot
        SourceConnectorList::iterator emptySlot(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_empty<InternalMediaSourceConnector>()));
        if (m_sourceConnectors.end() != emptySlot)
        {
            // Reserve the empty slot and pass the connector to the derived class.
            emptySlot->Reserve();
            rc = AddSourceConnectorValue(initialState, connector);

            if ((EMPTY_ID_INTERNAL != connector.GetIDInternal()) &&
                !ConnectorsContainID(connector.GetIDInternal()))
            {
                // Find the reserved connector again, since the mutex may have been released and/or the list may have changed
                emptySlot = std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(EMPTY_ID_INTERNAL));
                if (m_sourceConnectors.end() != emptySlot)
                {
                    if ((OCASTATUS_OK == rc) ||
                        (OCASTATUS_PARTIALLY_SUCCEEDED == rc))
                    {
                        *emptySlot = connector;
                        SourceConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_ADDED, OCAMEDIACONNECTORELEMENT_ADDED);
                        ::OcaLiteMediaConnectorStatus connectorStatus(connector.GetIDInternal(), initialState, static_cast< ::OcaUint16>(0));
                        m_connectorsStatuses.Add(connectorStatus);
                        ConnectorStatusChanged(connectorStatus);
                    }
                    else
                    {
                        emptySlot->Clear();
                    }
                }
            }
            else
            {
                emptySlot->Clear();
                rc = OCASTATUS_DEVICE_ERROR;
                OCA_LOG_ERROR_PARAMS("The ID internal of the returned connector (%u) is not unique or valid", connector.GetIDInternal());
            }
        }
        else
        {
            rc = OCASTATUS_INVALID_REQUEST;
            OCA_LOG_WARNING("No source connectors available");
        }
    }
    else
    {
        rc = OCASTATUS_PARAMETER_ERROR;
        OCA_LOG_ERROR_PARAMS("Source connector or channel pin map is incorrect: max nr of pins = %u", maxPins);
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::AddSinkConnector(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSinkConnector& connector)
{
    // Retrieve some boundary values
    ::OcaUint16 maxPins(static_cast< ::OcaUint16>(0));
    ::OcaUint16 maxPorts(static_cast< ::OcaUint16>(0));
    ::OcaLiteStatus rc(GetMaxPinsPerConnector(maxPins));
    if (OCASTATUS_OK == rc)
    {
        rc = GetMaxPortsPerPin(maxPorts);
    }

    // Check the data structure
    bool bConnectorOK((connector.GetPinCount() <= maxPins) &&
                      ArePortsInChannelPinMapValid(connector.GetChannelPinMap()) &&
                      (EMPTY_ID_INTERNAL == connector.GetIDInternal()));

    // Check the initial state
    if (OCASTATUS_OK == rc)
    {
        if ((OCAMEDIACONNECTORSTATE_STOPPED != initialState) &&
            (OCAMEDIACONNECTORSTATE_RUNNING != initialState) &&
            (OCAMEDIACONNECTORSTATE_PAUSED != initialState))
        {
            OCA_LOG_ERROR_PARAMS("Incorrect initial state %u", initialState);
            rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
        }
    }

    if (bConnectorOK &&
        (OCASTATUS_OK == rc))
    {
        // Find the first empty slot
        SinkConnectorList::iterator emptySlot(std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_empty<InternalMediaSinkConnector>()));
        if (m_sinkConnectors.end() != emptySlot)
        {
            // Reserve the empty slot and pass the connector to the derived class.
            emptySlot->Reserve();
            rc = AddSinkConnectorValue(initialState, connector);

            if ((EMPTY_ID_INTERNAL != connector.GetIDInternal()) &&
                !ConnectorsContainID(connector.GetIDInternal()))
            {
                // Find the reserved connector again, since the mutex may have been released and/or the list may have changed
                emptySlot = std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(EMPTY_ID_INTERNAL));
                if (m_sinkConnectors.end() != emptySlot)
                {
                    if ((OCASTATUS_OK == rc) ||
                        (OCASTATUS_PARTIALLY_SUCCEEDED == rc))
                    {
                        *emptySlot = connector;
                        SinkConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_ADDED, OCAMEDIACONNECTORELEMENT_ADDED);
                        ::OcaLiteMediaConnectorStatus connectorStatus(connector.GetIDInternal(), initialState, static_cast< ::OcaUint16>(0));
                        m_connectorsStatuses.Add(connectorStatus);
                        ConnectorStatusChanged(connectorStatus);
                    }
                    else
                    {
                        emptySlot->Clear();
                    }
                }
            }
            else
            {
                emptySlot->Clear();
                rc = OCASTATUS_DEVICE_ERROR;
                OCA_LOG_ERROR_PARAMS("The ID internal of the returned connector (%u) is not unique or valid", connector.GetIDInternal());
            }
        }
        else
        {
            rc = OCASTATUS_INVALID_REQUEST;
            OCA_LOG_WARNING("No sink connectors available");
        }
    }
    else
    {
        rc = OCASTATUS_PARAMETER_ERROR;
        OCA_LOG_ERROR_PARAMS("Sink connector or channel pin map is incorrect: max nr of pins = %u, max nr of ports per pin = %u", maxPins, maxPorts);
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::SetSourceConnectorPinMap(::OcaLiteMediaConnectorID connectorID,
                                                                       const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    SourceConnectorList::iterator connectorIt(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(connectorID)));
    if (m_sourceConnectors.end() != connectorIt)
    {
        // Check the data structure
        if (ArePortsInChannelPinMapValid(channelPinMap))
        {
            // Find the status for the connector
            ::OcaLiteMediaConnectorStatus status;
            for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsStatuses.GetCount(); i++)
            {
                const ::OcaLiteMediaConnectorStatus& listStatus(m_connectorsStatuses.GetItem(i));
                if (listStatus.GetConnectorID() == connectorID)
                {
                    status = listStatus;
                    break;
                }
            }
            assert(status.GetConnectorID() == connectorID);

            ::OcaLiteMediaSourceConnector connector;
            connectorIt->AssignToOca(connector);
            ::OcaLiteMediaSourceConnector oldConnector(connector);  // Make a copy for later checks
            rc = SetSourceConnectorPinMapValue(connector, channelPinMap, status);

            if ((OCASTATUS_OK == rc) ||
                (OCASTATUS_PARTIALLY_SUCCEEDED == rc))
            {
                if (oldConnector != connector)
                {
                    // Find the connector again, since the mutex may have been released and/or the list may have changed
                    connectorIt = std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(connectorID));
                    if (m_sourceConnectors.end() != connectorIt)
                    {
                        *connectorIt = connector;
                        SourceConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_CHANGED, OCAMEDIACONNECTORELEMENT_PINMAP);
                    }
                }

                UpdateConnectorStatus(status);
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
            OCA_LOG_ERROR_PARAMS("Channel pin map is incorrect: max nr of pins = %u", connectorIt->GetPinCount());
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::SetSinkConnectorPinMap(::OcaLiteMediaConnectorID connectorID,
                                                                     const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap)
{
    // Retrieve some boundary values
    ::OcaUint16 maxPorts(static_cast< ::OcaUint16>(0));
    ::OcaLiteStatus rc(GetMaxPortsPerPin(maxPorts));

    if (OCASTATUS_OK == rc)
    {
        SinkConnectorList::iterator connectorIt(std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(connectorID)));
        if (m_sinkConnectors.end() != connectorIt)
        {
            // Check the data structure
            if (ArePortsInChannelPinMapValid(channelPinMap))
            {
                // Find the status for the connector
                ::OcaLiteMediaConnectorStatus status;
                for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsStatuses.GetCount(); i++)
                {
                    const ::OcaLiteMediaConnectorStatus& listStatus(m_connectorsStatuses.GetItem(i));
                    if (listStatus.GetConnectorID() == connectorID)
                    {
                        status = listStatus;
                        break;
                    }
                }
                assert(status.GetConnectorID() == connectorID);

                ::OcaLiteMediaSinkConnector connector;
                connectorIt->AssignToOca(connector);
                ::OcaLiteMediaSinkConnector oldConnector(connector);    // Make a copy for later checks
                rc = SetSinkConnectorPinMapValue(connector, channelPinMap, status);

                if ((OCASTATUS_OK == rc) ||
                    (OCASTATUS_PARTIALLY_SUCCEEDED == rc))
                {
                    if (oldConnector != connector)
                    {
                        // Find the connector again, since the mutex may have been released and/or the list may have changed
                        connectorIt = std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(connectorID));
                        if (m_sinkConnectors.end() != connectorIt)
                        {
                            *connectorIt = connector;
                            SinkConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_CHANGED, OCAMEDIACONNECTORELEMENT_PINMAP);
                        }
                    }

                    UpdateConnectorStatus(status);
                }
            }
            else
            {
                rc = OCASTATUS_PARAMETER_ERROR;
                OCA_LOG_ERROR_PARAMS("Channel pin map is incorrect: max nr of pins = %u, max nr of ports per pin = %u", connectorIt->GetPinCount(), maxPorts);
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
        }
    }
    else
    {
        rc = OCASTATUS_DEVICE_ERROR;
        OCA_LOG_ERROR_PARAMS("Unable to retrieve the maximum number of ports per pin (rc = %u)", rc);
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::DeleteConnector(::OcaLiteMediaConnectorID connectorID)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    // Find the connector in the list of source connectors
    SourceConnectorList::iterator sourceIt(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(connectorID)));
    SinkConnectorList::iterator sinkIt(m_sinkConnectors.end());
    if (m_sourceConnectors.end() != sourceIt)
    {
        ::OcaLiteMediaSourceConnector connector;
        sourceIt->AssignToOca(connector);
        rc = DeleteSourceConnectorValue(connector);
        if (OCASTATUS_OK == rc)
        {
            // Find the connector again, since the mutex may have been released and/or the list may have changed
            sourceIt = std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(connectorID));
            if (m_sourceConnectors.end() != sourceIt)
            {
                sourceIt->Clear();
                SourceConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_DELETED, OCAMEDIACONNECTORELEMENT_DELETED);
            }
        }
    }
    else
    {
        sinkIt = std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(connectorID));
        if (m_sinkConnectors.end() != sinkIt)
        {
            ::OcaLiteMediaSinkConnector connector;
            sinkIt->AssignToOca(connector);
            rc = DeleteSinkConnectorValue(connector);
            if (OCASTATUS_OK == rc)
            {
                // Find the connector again, since the mutex may have been released and/or the list may have changed
                sinkIt = std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(connectorID));
                if (m_sinkConnectors.end() != sinkIt)
                {
                    sinkIt->Clear();
                    SinkConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_DELETED, OCAMEDIACONNECTORELEMENT_DELETED);
                }
            }
        }
    }

    if (OCASTATUS_OK == rc)
    {
        assert((m_sourceConnectors.end() != sourceIt) || (m_sinkConnectors.end() != sinkIt));

        // Delete the status from the list of statuses
        for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsStatuses.GetCount(); i++)
        {
            const ::OcaLiteMediaConnectorStatus& status(m_connectorsStatuses.GetItem(i));
            if (status.GetConnectorID() == connectorID)
            {
                ::OcaLiteMediaConnectorStatus connectorStatus(status.GetConnectorID(), OCAMEDIACONNECTORSTATE_STOPPED, status.GetErrorCode());
                m_connectorsStatuses.RemovePosition(i);
                ConnectorStatusChanged(connectorStatus);
                break;
            }
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_MEDIA_PROTOCOL:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteNetworkMediaProtocol protocol;
                        rc = GetMediaProtocol(protocol);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteNetworkMediaProtocol>(protocol, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteNetworkMediaProtocol>(protocol, &pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_PORTS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLitePort> ocaPorts;
                        rc = GetPorts(ocaPorts);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ocaPorts.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ocaPorts.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_PORT_NAME:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLitePortID portId;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        (portId.Unmarshal(bytesLeft, &pCmdParameters, reader)))
                    {
                        ::OcaLiteString name;
                        rc = GetPortName(portId, name);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     name.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                name.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_MAX_SOURCE_CONNECTORS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 maxSourceConnectors;
                        rc = GetMaxSourceConnectors(maxSourceConnectors);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     writer.GetSize(maxSourceConnectors));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                writer.Write(maxSourceConnectors, &pResponse);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_MAX_SINK_CONNECTORS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 maxSinkConnectors;
                        rc = GetMaxSinkConnectors(maxSinkConnectors);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     writer.GetSize(maxSinkConnectors));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                writer.Write(maxSinkConnectors, &pResponse);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_MAX_PINS_PER_CONNECTOR:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 maxPins;
                        rc = GetMaxPinsPerConnector(maxPins);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     writer.GetSize(maxPins));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                writer.Write(maxPins, &pResponse);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_MAX_PORTS_PER_PIN:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 maxPorts;
                        rc = GetMaxPortsPerPin(maxPorts);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     writer.GetSize(maxPorts));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                writer.Write(maxPorts, &pResponse);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_SOURCE_CONNECTORS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteMediaSourceConnector> connectors;
                        rc = GetSourceConnectors(connectors);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     connectors.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                connectors.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_SOURCE_CONNECTOR:
                {
                    ::OcaLiteMediaConnectorID connectorId;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, connectorId))
                    {
                        ::OcaLiteMediaSourceConnector connector;
                        rc = GetSourceConnector(connectorId, connector);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     connector.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                connector.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_SINK_CONNECTORS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteMediaSinkConnector> connectors;
                        rc = GetSinkConnectors(connectors);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     connectors.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                connectors.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_SINK_CONNECTOR:
                {
                    ::OcaLiteMediaConnectorID connectorId;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, connectorId))
                    {
                        ::OcaLiteMediaSinkConnector connector;
                        rc = GetSinkConnector(connectorId, connector);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     connector.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                connector.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_CONNECTORS_STATUSES:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteMediaConnectorStatus> statuses;
                        rc = GetConnectorsStatuses(statuses);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     statuses.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                statuses.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_CONNECTOR_STATUS:
                {
                    ::OcaLiteMediaConnectorID connectorId;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, connectorId))
                    {
                        ::OcaLiteMediaConnectorStatus status;
                        rc = GetConnectorStatus(connectorId, status);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     status.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                status.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case ADD_SOURCE_CONNECTOR:
                {
                    ::OcaLiteMediaConnectorState connectorState;
                    ::OcaLiteMediaSourceConnector sourceConnector;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
                        UnmarshalValue< ::OcaLiteMediaConnectorState>(connectorState, bytesLeft, &pCmdParameters, reader) &&
                        sourceConnector.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = AddSourceConnector(connectorState, sourceConnector);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                sourceConnector.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                sourceConnector.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case ADD_SINK_CONNECTOR:
                {
                    ::OcaLiteMediaConnectorState connectorState;
                    ::OcaLiteMediaSinkConnector sinkConnector;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
                        UnmarshalValue< ::OcaLiteMediaConnectorState>(connectorState, bytesLeft, &pCmdParameters, reader) &&
                        sinkConnector.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = AddSinkConnector(connectorState, sinkConnector);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                sinkConnector.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                sinkConnector.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case SET_SOURCE_CONNECTOR_PINMAP:
                {
                    ::OcaLiteMediaConnectorID connectorId;
                    ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> pinMap;
                    ::OcaUint8 numberOfParameters(0);

                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, connectorId) &&
                        pinMap.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetSourceConnectorPinMap(connectorId, pinMap);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(0/*NrParameters*/), &pResponse);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case SET_SINK_CONNECTOR_PINMAP:
                {
                    ::OcaLiteMediaConnectorID connectorId;
                    ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> pinMap;
                    ::OcaUint8 numberOfParameters(0);

                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, connectorId) &&
                        pinMap.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetSinkConnectorPinMap(connectorId, pinMap);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(0/*NrParameters*/), &pResponse);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case DELETE_CONNECTOR:
                {
                    ::OcaLiteMediaConnectorID connectorId;
                    ::OcaUint8 numberOfParameters(0);

                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, connectorId))
                    {
                        rc = DeleteConnector(connectorId);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(0/*NrParameters*/), &pResponse);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case SET_PORT_NAME:
            case SET_CONNECTOR_CONNECTION:
            case SET_CONNECTOR_CODING:
            case CONTROL_CONNECTOR:
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
            rc = OcaLiteApplicationNetwork::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaBoolean OcaLiteMediaTransportNetwork::AddEventSubscription(const ::OcaLiteEventID& eventID,
                                                                IEventDelegate& eventDelegate)
{
    bool bSuccess(false);

    if (eventID.GetDefLevel() == CLASS_ID.GetFieldCount())
    {
        switch (eventID.GetEventIndex())
        {
        case OCA_EVENT_SOURCE_CONNECTOR_CHANGED:
            if (NULL == m_pSourceConnectorChangedEventDelegate)
            {
                m_pSourceConnectorChangedEventDelegate = &eventDelegate;
                bSuccess = true;
            }
            break;
        case OCA_EVENT_SINK_CONNECTOR_CHANGED:
            if (NULL == m_pSinkConnectorChangedEventDelegate)
            {
                m_pSinkConnectorChangedEventDelegate = &eventDelegate;
                bSuccess = true;
            }
            break;
        case OCA_EVENT_CONNECTOR_STATUS_CHANGED:
            if (NULL == m_pConnectorStatusChangedEventDelegate)
            {
                m_pConnectorStatusChangedEventDelegate = &eventDelegate;
                bSuccess = true;
            }
            break;
        default:
            break;
        }   
    }
    else
    {
        bSuccess = static_cast<bool>(OcaLiteApplicationNetwork::AddEventSubscription(eventID, eventDelegate));
    }

    return static_cast< ::OcaBoolean>(bSuccess);
}

void OcaLiteMediaTransportNetwork::RemoveEventSubscription(const ::OcaLiteEventID& eventID)
{
    if (eventID.GetDefLevel() == CLASS_ID.GetFieldCount())
    {
        switch (eventID.GetEventIndex())
        {
        case OCA_EVENT_SOURCE_CONNECTOR_CHANGED:
            m_pSourceConnectorChangedEventDelegate = NULL;
            break;
        case OCA_EVENT_SINK_CONNECTOR_CHANGED:
            m_pSinkConnectorChangedEventDelegate = NULL;
            break;
        case OCA_EVENT_CONNECTOR_STATUS_CHANGED:
            m_pConnectorStatusChangedEventDelegate = NULL;
            break;
        default:
            OCA_LOG_WARNING("Invalid event id");
            break;
        }
    }
    else
    {
        OcaLiteApplicationNetwork::RemoveEventSubscription(eventID);
    }
}

::OcaClassVersionNumber OcaLiteMediaTransportNetwork::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteApplicationNetwork::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

void OcaLiteMediaTransportNetwork::ReserveConnectors(::OcaUint16 nrSourceConnectors, ::OcaUint16 nrSinkConnectors, ::OcaUint16 maxPins, ::OcaUint16 maxPorts)
{
    // Add an empty source connector for every possible slot and reserve space for the pins and slots
    m_sourceConnectors.reserve(static_cast<size_t>(nrSourceConnectors));
    for (::OcaUint16 connector(static_cast< ::OcaUint16>(0)); connector < nrSourceConnectors; connector++)
    {
        m_sourceConnectors.push_back(InternalMediaSourceConnector());
        SourceConnectorList::reverse_iterator it(m_sourceConnectors.rbegin());
        it->ReserveChannelPinMap(maxPins);
    }

    // Add an empty sink connector for every possible slot and reserve space for the pins and slots
    m_sinkConnectors.reserve(static_cast<size_t>(nrSinkConnectors));
    for (::OcaUint16 connector(static_cast< ::OcaUint16>(0)); connector < nrSinkConnectors; connector++)
    {
        m_sinkConnectors.push_back(InternalMediaSinkConnector());
        SinkConnectorList::reverse_iterator it(m_sinkConnectors.rbegin());
        it->ReserveChannelPinMap(maxPins, maxPorts);
    }

    // Reserve the space for the statuses of the connectors
    m_connectorsStatuses.Reserve(nrSourceConnectors + nrSinkConnectors);
}


::OcaLiteStatus OcaLiteMediaTransportNetwork::GetSourceConnectorValue(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSourceConnector& connector) const
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    SourceConnectorList::const_iterator connectorIt(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(id)));
    if (m_sourceConnectors.end() != connectorIt)
    {
        connectorIt->AssignToOca(connector);
        rc = OCASTATUS_OK;
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::GetSinkConnectorValue(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSinkConnector& connector) const
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    SinkConnectorList::const_iterator connectorIt(std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(id)));
    if (m_sinkConnectors.end() != connectorIt)
    {
        connectorIt->AssignToOca(connector);
        rc = OCASTATUS_OK;
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalGetPorts(::OcaLiteList< ::OcaLitePort>& ports) const
{
    ports.Clear();
    for (PortList::const_iterator iter(m_ocaPorts.begin()); iter != m_ocaPorts.end(); ++iter)
    {
        ports.Add(*iter);
    }

    return OCASTATUS_OK;
}

const ::OcaLitePort* OcaLiteMediaTransportNetwork::InternalGetPort(const ::OcaLitePortID& id) const
{
    const ::OcaLitePort* pPort(NULL);

    for (PortList::const_iterator iter(m_ocaPorts.begin()); iter != m_ocaPorts.end(); ++iter)
    {
        if (iter->GetID() == id)
        {
            pPort = &(*iter);
            break;
        }
    }

    return pPort;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalAddPort(const ::OcaLiteString& label, ::OcaPortMode portMode, ::OcaLitePortID& id)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    // Determine the new index
    ::OcaUint16 newIndex(static_cast< ::OcaUint16>(1));
    while ((newIndex > static_cast< ::OcaUint16>(0)) && PortsContainIndex(portMode, newIndex))
    {
        newIndex++;
    }

    if (newIndex > static_cast< ::OcaUint16>(0))
    {
        ::OcaLitePortID newPortID(portMode, newIndex);
        id = newPortID;

        ::OcaLitePort port(GetObjectNumber(), id, label);
        m_ocaPorts.push_back(port);

        PortsChanged(OCAPROPERTYCHANGETYPE_ITEM_ADDED);
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalDeletePort(const ::OcaLitePortID& id)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    for (PortList::iterator iter(m_ocaPorts.begin()); iter != m_ocaPorts.end(); ++iter)
    {
        if (iter->GetID() == id)
        {
            static_cast<void>(m_ocaPorts.erase(iter));

            PortsChanged(OCAPROPERTYCHANGETYPE_ITEM_DELETED);

            rc = OCASTATUS_OK;

            break;
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalAddSourceConnector(::OcaLiteMediaConnectorState initialState,
                                                                         const ::OcaLiteMediaSourceConnector& connector)
{
    assert((EMPTY_ID_INTERNAL != connector.GetIDInternal()) && !ConnectorsContainID(connector.GetIDInternal()));

    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    // Find the first empty slot
    SourceConnectorList::iterator emptySlot(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_empty<InternalMediaSourceConnector>()));
    if (m_sourceConnectors.end() != emptySlot)
    {
        *emptySlot = connector;
        SourceConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_ADDED, OCAMEDIACONNECTORELEMENT_ADDED);
        ::OcaLiteMediaConnectorStatus connectorStatus(connector.GetIDInternal(), initialState, static_cast< ::OcaUint16>(0));
        m_connectorsStatuses.Add(connectorStatus);
        ConnectorStatusChanged(connectorStatus);

        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_WARNING("No source connectors available");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalAddSinkConnector(::OcaLiteMediaConnectorState initialState,
                                                                       const ::OcaLiteMediaSinkConnector& connector)
{
    assert((EMPTY_ID_INTERNAL != connector.GetIDInternal()) && !ConnectorsContainID(connector.GetIDInternal()));

    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    // Find the first empty slot
    SinkConnectorList::iterator emptySlot(std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_empty<InternalMediaSinkConnector>()));
    if (m_sinkConnectors.end() != emptySlot)
    {
        *emptySlot = connector;
        SinkConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_ADDED, OCAMEDIACONNECTORELEMENT_ADDED);
        ::OcaLiteMediaConnectorStatus connectorStatus(connector.GetIDInternal(), initialState, static_cast< ::OcaUint16>(0));
        m_connectorsStatuses.Add(connectorStatus);
        ConnectorStatusChanged(connectorStatus);

        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_WARNING("No sink connectors available");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalUpdateSourceConnector(const ::OcaLiteMediaSourceConnector& connector,
                                                                    ::OcaLiteMediaConnectorElement changedElement)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    SourceConnectorList::iterator connectorIt(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(connector.GetIDInternal())));
    if (m_sourceConnectors.end() != connectorIt)
    {
        *connectorIt = connector;
        SourceConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_CHANGED, changedElement);

        rc = OCASTATUS_OK;
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalUpdateSinkConnector(const ::OcaLiteMediaSinkConnector& connector,
                                                                  ::OcaLiteMediaConnectorElement changedElement)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    SinkConnectorList::iterator connectorIt(std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(connector.GetIDInternal())));
    if (m_sinkConnectors.end() != connectorIt)
    {
        *connectorIt = connector;
        SinkConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_CHANGED, changedElement);

        rc = OCASTATUS_OK;
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::InternalDeleteConnector(::OcaLiteMediaConnectorID id)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    // Find the connector in the list of source connectors
    SourceConnectorList::iterator sourceIt(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(id)));
    SinkConnectorList::iterator sinkIt(m_sinkConnectors.end());
    if (m_sourceConnectors.end() != sourceIt)
    {
        ::OcaLiteMediaSourceConnector connector;
        sourceIt->AssignToOca(connector);
        sourceIt->Clear();

        SourceConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_DELETED, OCAMEDIACONNECTORELEMENT_DELETED);

        rc = OCASTATUS_OK;
    }
    else
    {
        sinkIt = std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(id));
        if (m_sinkConnectors.end() != sinkIt)
        {
            ::OcaLiteMediaSinkConnector connector;
            sinkIt->AssignToOca(connector);
            sinkIt->Clear();

            SinkConnectorChanged(connector, OCAPROPERTYCHANGETYPE_ITEM_DELETED, OCAMEDIACONNECTORELEMENT_DELETED);

            rc = OCASTATUS_OK;
        }
    }

    if (OCASTATUS_OK == rc)
    {
        assert((m_sourceConnectors.end() != sourceIt) || (m_sinkConnectors.end() != sinkIt));

        // Delete the status from the list of statuses
        for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsStatuses.GetCount(); i++)
        {
            const ::OcaLiteMediaConnectorStatus& status(m_connectorsStatuses.GetItem(i));
            if (status.GetConnectorID() == id)
            {
                ::OcaLiteMediaConnectorStatus connectorStatus(status.GetConnectorID(), OCAMEDIACONNECTORSTATE_STOPPED, status.GetErrorCode());
                m_connectorsStatuses.RemovePosition(i);
                ConnectorStatusChanged(connectorStatus);
                break;
            }
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetwork::SetPortNameValue(const ::OcaLitePortID&, const ::OcaLiteString&)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

void OcaLiteMediaTransportNetwork::UpdateConnectorStatus(const ::OcaLiteMediaConnectorStatus& status)
{
    // Find the status in the current list of status
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsStatuses.GetCount(); i++)
    {
        ::OcaLiteMediaConnectorStatus& listStatus(m_connectorsStatuses.GetItem(i));
        if (listStatus.GetConnectorID() == status.GetConnectorID())
        {
            if (listStatus != status)
            {
                // The status has actually changed, so update it in the list and send out the event
                listStatus = status;
                ConnectorStatusChanged(listStatus);
            }
            break;
        }
    }
}

bool OcaLiteMediaTransportNetwork::PortsContainIndex(::OcaPortMode portMode, ::OcaUint16 portIndex) const
{
    bool result(false);
    for (PortList::const_iterator iter(m_ocaPorts.begin()); iter != m_ocaPorts.end(); ++iter)
    {
        if ((iter->GetID().GetMode() == portMode) &&
            (iter->GetID().GetIndex() == portIndex))
        {
            result = true;
            break;
        }
    }

    return result;
}

bool OcaLiteMediaTransportNetwork::ConnectorsContainID(::OcaLiteMediaConnectorID id) const
{
    bool result(false);

    SourceConnectorList::const_iterator sourceIt(std::find_if(m_sourceConnectors.begin(), m_sourceConnectors.end(), connector_internal_id<InternalMediaSourceConnector>(id)));
    SinkConnectorList::const_iterator sinkIt(m_sinkConnectors.end());
    if (m_sourceConnectors.end() != sourceIt)
    {
        result = true;
    }
    else
    {
        sinkIt = std::find_if(m_sinkConnectors.begin(), m_sinkConnectors.end(), connector_internal_id<InternalMediaSinkConnector>(id));
        result = (m_sinkConnectors.end() != sinkIt);
    }

    return result;
}

void OcaLiteMediaTransportNetwork::PortsChanged(::OcaLitePropertyChangeType changeType)
{
    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_PORTS));
    ::OcaLiteList< ::OcaLitePort> ports;
    if (OCASTATUS_OK == InternalGetPorts(ports))
    {
        ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaLitePort> > eventData(GetObjectNumber(),
                                                                                     propertyID,
                                                                                     ports,
                                                                                     changeType);
        PropertyChanged(eventData);
    }
}

void OcaLiteMediaTransportNetwork::SourceConnectorChanged(const ::OcaLiteMediaSourceConnector& connector,
                                                      ::OcaLitePropertyChangeType changeType,
                                                      ::OcaLiteMediaConnectorElement changedElement)
{
    if (NULL != m_pSourceConnectorChangedEventDelegate)
    {
        // Send the event
        ::OcaLiteEventID eventID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_EVENT_SOURCE_CONNECTOR_CHANGED));
        ::OcaLiteEvent event(GetObjectNumber(), eventID);
        ::OcaLiteMediaSourceConnectorChangedEventData eventData(event, connector, changeType, changedElement);
        IEventDelegate* pEventDelegate(m_pSourceConnectorChangedEventDelegate);

        pEventDelegate->OnEvent(eventData);
    }
}

void OcaLiteMediaTransportNetwork::SinkConnectorChanged(const ::OcaLiteMediaSinkConnector& connector,
                                                    ::OcaLitePropertyChangeType changeType,
                                                    ::OcaLiteMediaConnectorElement changedElement)
{
    if (NULL != m_pSinkConnectorChangedEventDelegate)
    {
        // Send the event
        ::OcaLiteEventID eventID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_EVENT_SINK_CONNECTOR_CHANGED));
        ::OcaLiteEvent event(GetObjectNumber(), eventID);
        ::OcaLiteMediaSinkConnectorChangedEventData eventData(event, connector, changeType, changedElement);
        IEventDelegate* pEventDelegate(m_pSinkConnectorChangedEventDelegate);

        pEventDelegate->OnEvent(eventData);
    }
}

void OcaLiteMediaTransportNetwork::ConnectorStatusChanged(const ::OcaLiteMediaConnectorStatus& status)
{
    if (NULL != m_pConnectorStatusChangedEventDelegate)
    {
        // Send the event
        ::OcaLiteEventID eventID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_EVENT_CONNECTOR_STATUS_CHANGED));
        ::OcaLiteEvent event(GetObjectNumber(), eventID);
        ::OcaLiteMediaConnectorStatusChangedEventData eventData(event, status);
        IEventDelegate* pEventDelegate(m_pConnectorStatusChangedEventDelegate);

        pEventDelegate->OnEvent(eventData);
    }
}

// ---- OcaLiteMediaTransportNetwork::InternalMediaSourceConnector Class Implementation ----

OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::InternalMediaSourceConnector()
    : m_isEmpty(true),
      m_idInternal(EMPTY_ID_INTERNAL),
      m_idExternal(),
      m_connection(),
      m_coding(),
      m_pinCount(static_cast< ::OcaUint16>(0)),
      m_channelPinMap()
{
}

OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::InternalMediaSourceConnector(const InternalMediaSourceConnector& source)
    : m_isEmpty(source.m_isEmpty),
      m_idInternal(source.m_idInternal),
      m_idExternal(source.m_idExternal),
      m_connection(source.m_connection),
      m_coding(source.m_coding),
      m_pinCount(source.m_pinCount),
      m_channelPinMap(source.m_channelPinMap)
{
}

OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::~InternalMediaSourceConnector()
{
}

void OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::ReserveChannelPinMap(::OcaUint16 maxPins)
{
    m_channelPinMap.reserve(static_cast<size_t>(maxPins));
}

bool OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::IsEmpty() const
{
    return m_isEmpty;
}

void OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::Reserve()
{
    m_isEmpty = false;
}

void OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::Clear()
{
    m_isEmpty = true;
    m_idInternal = EMPTY_ID_INTERNAL;
    m_idExternal = ::OcaLiteString();
    m_connection = ::OcaLiteMediaConnection();
    m_coding = ::OcaLiteMediaCoding();
    m_pinCount = static_cast< ::OcaUint16>(0);
    m_channelPinMap.clear();
}

bool OcaLiteMediaTransportNetwork::ArePortsInChannelPinMapValid(const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap) const
{
    bool allValid(true);

    for (::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>::Iterator it(channelPinMap.Begin());
         allValid && (it != channelPinMap.End());
         ++it)
    {
        allValid = PortsContainIndex(it.GetValue().GetMode(), it.GetValue().GetIndex());
    }

    return allValid;
}

bool OcaLiteMediaTransportNetwork::ArePortsInChannelPinMapValid(const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap) const
{
    bool allValid(true);

    for (::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>::Iterator it(channelPinMap.Begin());
         allValid && (it != channelPinMap.End());
         ++it)
    {
        allValid = PortsContainIndex(it.GetValue().GetMode(), it.GetValue().GetIndex());
    }

    return allValid;
}

::OcaLiteMediaConnectorID OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::GetIDInternal() const
{
    return m_idInternal;
}

::OcaUint16 OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::GetPinCount() const
{
    return m_pinCount;
}

void OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::AssignToOca(::OcaLiteMediaSourceConnector& target) const
{
    assert(!m_isEmpty);
    target.SetIDInternal(m_idInternal);
    target.SetIDExternal(m_idExternal);
    target.SetConnection(m_connection);
    target.SetCoding(m_coding);
    target.SetPinCount(m_pinCount);

    // Copy the channel pin map
    ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> channelPinMap;
    for (std::vector< ::OcaLiteMapItem< ::OcaUint16, ::OcaLitePortID> >::const_iterator it(m_channelPinMap.begin());
         it != m_channelPinMap.end();
         ++it)
    {
        channelPinMap.Add(*it);
    }
    target.SetChannelPinMap(channelPinMap);
}

OcaLiteMediaTransportNetwork::InternalMediaSourceConnector& OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::operator=(const InternalMediaSourceConnector& source)
{
    if (this != &source)
    {
        m_isEmpty = source.m_isEmpty;
        m_idInternal = source.m_idInternal;
        m_idExternal = source.m_idExternal;
        m_connection = source.m_connection;
        m_coding = source.m_coding;
        m_pinCount = source.m_pinCount;
        m_channelPinMap.reserve(source.m_channelPinMap.capacity());
        m_channelPinMap = source.m_channelPinMap;
    }

    return *this;
}

OcaLiteMediaTransportNetwork::InternalMediaSourceConnector& OcaLiteMediaTransportNetwork::InternalMediaSourceConnector::operator=(const ::OcaLiteMediaSourceConnector& source)
{
    m_isEmpty = false;
    m_idInternal = source.GetIDInternal();
    m_idExternal = source.GetIDExternal();
    m_connection = source.GetConnection();
    m_coding = source.GetCoding();
    m_pinCount = source.GetPinCount();

    // Copy the channel pin map
    const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> channelPinMap(source.GetChannelPinMap());
    m_channelPinMap.clear();
    for (::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>::Iterator it(channelPinMap.Begin());
         it != channelPinMap.End();
         ++it)
    {
        m_channelPinMap.push_back(*it);
    }

    return *this;
}

// ---- OcaLiteMediaTransportNetwork::InternalMediaSinkConnector Class Implementation ----

OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::InternalMediaSinkConnector()
    : m_isEmpty(true),
      m_idInternal(EMPTY_ID_INTERNAL),
      m_idExternal(),
      m_connection(),
      m_coding(),
      m_pinCount(static_cast< ::OcaUint16>(0)),
      m_channelPinMap()
{
}

OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::InternalMediaSinkConnector(const InternalMediaSinkConnector& source)
    : m_isEmpty(source.m_isEmpty),
      m_idInternal(source.m_idInternal),
      m_idExternal(source.m_idExternal),
      m_connection(source.m_connection),
      m_coding(source.m_coding),
      m_pinCount(source.m_pinCount),
      m_channelPinMap(source.m_channelPinMap)
{
}

OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::~InternalMediaSinkConnector()
{
}

void OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::ReserveChannelPinMap(::OcaUint16 maxPins, ::OcaUint16 maxPorts)
{
    m_channelPinMap.reserve(static_cast<size_t>(maxPins * maxPorts));
}

bool OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::IsEmpty() const
{
    return m_isEmpty;
}

void OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::Reserve()
{
    m_isEmpty = false;
}

void OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::Clear()
{
    m_isEmpty = true;
    m_idInternal = EMPTY_ID_INTERNAL;
    m_idExternal = ::OcaLiteString();
    m_connection = ::OcaLiteMediaConnection();
    m_coding = ::OcaLiteMediaCoding();
    m_pinCount = static_cast< ::OcaUint16>(0);
    m_channelPinMap.clear();
}

::OcaLiteMediaConnectorID OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::GetIDInternal() const
{
    return m_idInternal;
}

::OcaUint16 OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::GetPinCount() const
{
    return m_pinCount;
}

void OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::AssignToOca(::OcaLiteMediaSinkConnector& target) const
{
    assert(!m_isEmpty);
    target.SetIDInternal(m_idInternal);
    target.SetIDExternal(m_idExternal);
    target.SetConnection(m_connection);
    target.SetCoding(m_coding);
    target.SetPinCount(m_pinCount);

    // Copy the channel pin map
    ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> channelPinMap;
    for (std::vector< ::OcaLiteMapItem< ::OcaUint16, ::OcaLitePortID> >::const_iterator it(m_channelPinMap.begin());
         it != m_channelPinMap.end();
         ++it)
    {
        channelPinMap.Add(*it);
    }
    target.SetChannelPinMap(channelPinMap);
}

OcaLiteMediaTransportNetwork::InternalMediaSinkConnector& OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::operator=(const InternalMediaSinkConnector& source)
{
    if (this != &source)
    {
        m_isEmpty = source.m_isEmpty;
        m_idInternal = source.m_idInternal;
        m_idExternal = source.m_idExternal;
        m_connection = source.m_connection;
        m_coding = source.m_coding;
        m_pinCount = source.m_pinCount;
        m_channelPinMap.reserve(source.m_channelPinMap.capacity());
        m_channelPinMap = source.m_channelPinMap;
    }

    return *this;
}

OcaLiteMediaTransportNetwork::InternalMediaSinkConnector& OcaLiteMediaTransportNetwork::InternalMediaSinkConnector::operator=(const ::OcaLiteMediaSinkConnector& source)
{
    m_isEmpty = false;
    m_idInternal = source.GetIDInternal();
    m_idExternal = source.GetIDExternal();
    m_connection = source.GetConnection();
    m_coding = source.GetCoding();
    m_pinCount = source.GetPinCount();

    // Copy the channel pin map
    const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> channelPinMap(source.GetChannelPinMap());
    m_channelPinMap.clear();
    for (::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>::Iterator it(channelPinMap.Begin());
         it != channelPinMap.End();
         ++it)
    {
        m_channelPinMap.push_back(*it);
    }

    return *this;
}
