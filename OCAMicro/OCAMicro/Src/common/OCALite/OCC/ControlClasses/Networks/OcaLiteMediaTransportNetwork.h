/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaTransportNetwork
 *
 */
#ifndef OCALITEMEDIATRANSPORTNETWORK_H
#define OCALITEMEDIATRANSPORTNETWORK_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteMap.h>
#include <OCC/ControlDataTypes/OcaLiteApplicationNetworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteMediaConnection.h>
#include <OCC/ControlDataTypes/OcaLiteMediaCoding.h>
#include <OCC/ControlDataTypes/OcaLiteMediaConnectorStatus.h>
#include <OCC/ControlDataTypes/OcaLiteMediaSinkConnector.h>
#include <OCC/ControlDataTypes/OcaLiteMediaSourceConnector.h>
#include <OCC/ControlDataTypes/OcaLiteMultiMap.h>
#include <OCC/ControlDataTypes/OcaLitePort.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>

// ---- Include local include files ----
#include "OcaLiteApplicationNetwork.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_MEDIA_TRANSPORT_NETWORK_CLASSID     OCA_APPLICATION_NETWORK_CLASSID,static_cast< ::OcaUint16>(2)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class from which the media transport network classes inherit.
 */
class OcaLiteMediaTransportNetwork : public ::OcaLiteApplicationNetwork
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetMediaProtocol() */
        GET_MEDIA_PROTOCOL              = 1,
        /** GetPorts() */
        GET_PORTS                       = 2,
        /** GetPortName() */
        GET_PORT_NAME                   = 3,
        /** SetPortName() */
        SET_PORT_NAME                   = 4,
        /** GetMaxSourceConnectors() */
        GET_MAX_SOURCE_CONNECTORS       = 5,
        /** GetMaxSinkConnectors() */
        GET_MAX_SINK_CONNECTORS         = 6,
        /** GetMaxPinsPerConnector() */
        GET_MAX_PINS_PER_CONNECTOR      = 7,
        /** GetMaxPortsPerPin() */
        GET_MAX_PORTS_PER_PIN           = 8,
        /** GetSourceConnectors() */
        GET_SOURCE_CONNECTORS           = 9,
        /** GetSourceConnector() */
        GET_SOURCE_CONNECTOR            = 10,
        /** GetSinkConnectors() */
        GET_SINK_CONNECTORS             = 11,
        /** GetSinkConnector() */
        GET_SINK_CONNECTOR              = 12,
        /** GetConnectorsStatuses() */
        GET_CONNECTORS_STATUSES         = 13,
        /** GetConnectorStatus() */
        GET_CONNECTOR_STATUS            = 14,
        /** AddSourceConnector() */
        ADD_SOURCE_CONNECTOR            = 15,
        /** AddSinkConnector() */
        ADD_SINK_CONNECTOR              = 16,
        /** ControlConnector() */
        CONTROL_CONNECTOR               = 17,
        /** SetSourceConnectorPinMap() */
        SET_SOURCE_CONNECTOR_PINMAP    = 18,
        /** SetSinkConnectorPinMap() */
        SET_SINK_CONNECTOR_PINMAP      = 19,
        /** SetConnectorConnection() */
        SET_CONNECTOR_CONNECTION        = 20,
        /** SetConnectorCoding() */
        SET_CONNECTOR_CODING            = 21,
        /** DeleteConnector() */
        DELETE_CONNECTOR                = 22
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Type of media transport protocol used by the network. */
        OCA_PROP_PROTOCOL               = 1,
        /** The list of ports this network has. Note that these represent network channels of the media transport network.
            Each input port represents a source (transmit) network channel, each output port represents a sink (receive)
            network channel. Such network channels are directly linked to the ports, so the first input port represents
            the first source network channel, etc. */
        OCA_PROP_PORTS                  = 2,
        /** The maximum number of source connectors this media transport network can have (read-only property). */
        OCA_PROP_MAX_SOURCE_CONNECTORS  = 3,
        /** The maximum number of sink connectors this media transport network can have (read-only property). */
        OCA_PROP_MAX_SINK_CONNECTORS    = 4,
        /** The maximum number of pins (channels) in a connector that this network will support. */
        OCA_PROP_MAX_PINS_PER_CONNECTOR = 5,
        /** The maximum number of ports per pin that this network will support. Value of zero
            indicates there is no specific limit. */
        OCA_PROP_MAX_PORTS_PER_PIN      = 6
    };

    /** Event indexes for supported events. */
    enum EventIndex
    {
        /** Event indicating that a media source connector has changed. The change type indicates
            if the connector was added, deleted or changed. */
        OCA_EVENT_SOURCE_CONNECTOR_CHANGED  = 1,
        /** Event indicating that a media sink connector has changed. The change type indicates
            if the connector was added, deleted or changed. */
        OCA_EVENT_SINK_CONNECTOR_CHANGED    = 2,
        /** Event indicating that the status of a source or sink connector has changed. */
        OCA_EVENT_CONNECTOR_STATUS_CHANGED  = 3
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaTransportNetwork();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----

    /**
     * Gets the network's Protocol property.
     *
     * @param[out]   protocol   The value of the protocol property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetMediaProtocol(::OcaLiteNetworkMediaProtocol& protocol) const = 0;


    /**
     * Gets the list of ports owned by the MediaTransportNetwork object (representing the source and sink network channels).
     *
     * @param[out]  ocaPorts    The list with prots.
     * @return Indicates whether the list was successfully retrieved.
     */
    ::OcaLiteStatus GetPorts(::OcaLiteList< ::OcaLitePort>& ocaPorts) const;

    /**
     * Gets the name of the designated port
     *
     * @param[in]   id          Id of the port
     * @param[out]  name        Output parameter that holds the value of
     *                          port name
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetPortName(const ::OcaLitePortID& id, ::OcaLiteString& name) const;

    /**
     * Gets the maximum number of source connectors this media transport network supports.
     *
     * @param[out]  maxSourceConnectors The value of the maximum number of source connectors property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetMaxSourceConnectors(::OcaUint16& maxSourceConnectors) const = 0;

    /**
     * Gets the maximum number of sink connectors this media transport network supports.
     *
     * @param[out]  maxSinkConnectors   The value of the maximum number of sink connectors property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetMaxSinkConnectors(::OcaUint16& maxSinkConnectors) const = 0;

    /**
     * Gets the maximum number of pins (channels) per connector this media transport network supports.
     *
     * @param[out]  maxPins     The value of the maximum number of pins per connector property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetMaxPinsPerConnector(::OcaUint16& maxPins) const = 0;

    /**
     * Gets the maximum number of ports per pin this media transport network supports.
     *
     * @param[out]  maxPorts    The value of the maximum number of ports per pin property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetMaxPortsPerPin(::OcaUint16& maxPorts) const = 0;

    /**
     * Gets the descriptors of all the source (output) connectors collected by this network object.
     *
     * @param[out]  connectors  The value of the source connectors property.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetSourceConnectors(::OcaLiteList< ::OcaLiteMediaSourceConnector>& connectors) const;

    /**
     * Retrieves the descriptor of a given source connector.
     *
     * @param[in]   id          The identification of the connector to retrieve.
     * @param[out]  connector   The source connector with the given identification.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetSourceConnector(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSourceConnector& connector) const;

    /**
     * Gets the descriptors of all the sink (input) connectors collected by this network object.
     *
     * @param[out]  connectors  The value of the sink connectors property.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetSinkConnectors(::OcaLiteList< ::OcaLiteMediaSinkConnector>& connectors) const;

    /**
     * Retrieves the descriptor of a given sink connector.
     *
     * @param[in]   id          The identification of the connector to retrieve.
     * @param[out]  connector   The sink connector with the given identification.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetSinkConnector(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSinkConnector& connector) const;

    /**
     * Gets the status of all the source and sink connectors collected by this network object.
     *
     * @param[out]  statuses    The value of the connector statuses property.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetConnectorsStatuses(::OcaLiteList< ::OcaLiteMediaConnectorStatus>& statuses) const;

    /**
     * Gets the status of a single connector.
     *
     * @param[in]   id          The identification of the connector to retrieve the status of.
     * @param[out]  status      The status of the connector with the given identification.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetConnectorStatus(::OcaLiteMediaConnectorID id, ::OcaLiteMediaConnectorStatus& status) const;

    /**
     * Adds a source connector to this network. Parameters of the new connector are given in the Connector parameter;
     * device returns the same parameter with the new connector ID filled in.
     *
     * @param[in]       initialState    The initial state of the connector.
     * @param[in,out]   connector       The connector to add. This connector will be updated by this method.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus AddSourceConnector(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSourceConnector& connector);

    /**
     * Adds a sink connector to this network. Parameters of the new connector are given in the Connector parameter;
     * device returns the same parameter with the new connector ID filled in.
     *
     * @param[in]       initialStatus   The initial status of the connector.
     * @param[in,out]   connector       The connector to add. This connector will be updated by this method.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus AddSinkConnector(::OcaLiteMediaConnectorState initialStatus, ::OcaLiteMediaSinkConnector& connector);

    /**
     * Change the state of a given connector.
     *
     * @param[in]   connectorID         The identification of the connector to update.
     * @param[in]   command             The command to execute on the connector.
     * @return Indicates the success of the operation.
     */
    ::OcaLiteStatus ControlConnector(::OcaLiteMediaConnectorID connectorID, ::OcaLiteMediaConnectorCommand command);

    /**
     * Set a source connector's channel pin map.
     *
     * @param[in]   connectorID         The identification of the connector to update.
     * @param[in]   channelPinMap       The new channel pin map.
     * @return Indicates the success of the operation.
     */
    ::OcaLiteStatus SetSourceConnectorPinMap(::OcaLiteMediaConnectorID connectorID,
                                         const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap);

    /**
     * Set a sink connector's channel pin map.
     *
     * @param[in]   connectorID         The identification of the connector to update.
     * @param[in]   channelPinMap       The new channel pin map.
     * @return Indicates the success of the operation.
     */
    ::OcaLiteStatus SetSinkConnectorPinMap(::OcaLiteMediaConnectorID connectorID,
                                       const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap);

    /**
     * Deletes a connector from this network. Closes all of its connections.
     *
     * @param[in]   connectorID         The identification of the connector to delete.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus DeleteConnector(::OcaLiteMediaConnectorID connectorID);

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    virtual ::OcaBoolean AddEventSubscription(const ::OcaLiteEventID& eventID, IEventDelegate& eventDelegate);

    virtual void RemoveEventSubscription(const ::OcaLiteEventID& eventID);

    // ---- Miscellaneous methods ----

    /**
     * Initialize this network.
     *
     * @return True if the initialization succeeded, false otherwise.
     */
    virtual ::OcaBoolean Initialize() = 0;

    /**
     * Teardown this network.
     */
    virtual void Teardown() = 0;

protected:

    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                Read-only text property that describes object's role
     *                                  in the device. Particularly useful for workers, e.g. "Input 1 Gain".
     */
    OcaLiteMediaTransportNetwork(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Reserves memory for storing the given number of source connectors and sink connectors.
     *
     * @param[in]   nrSourceConnectors  The number of source connectors to reserve memory for.
     * @param[in]   nrSinkConnectors    The number of sink connectors to reserve memory for.
     * @param[in]   maxPins             The maximum number of pins per connector.
     * @param[in]   maxPorts            The maximum number of ports per pin of a sink connector.
     */
    void ReserveConnectors(::OcaUint16 nrSourceConnectors,
                           ::OcaUint16 nrSinkConnectors,
                           ::OcaUint16 maxPins,
                           ::OcaUint16 maxPorts);


    /**
     * Check if the ports in the channel pin map are part of this network.
     *
     * @param[in]   channelPinMap   The pin map to check.
     * @return True if the ports in the channel pin map are valid.
     */
    bool ArePortsInChannelPinMapValid(const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap) const;

    /**
     * Check if the ports in the channel pin map are part of this network.
     *
     * @param[in]   channelPinMap   The pin map to check.
     * @return True if the ports in the channel pin map are valid.
     */
    bool ArePortsInChannelPinMapValid(const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap) const;

    /**
     * Retrieves the OcaLiteMediaSourceConnector with the given internal ID.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   id                  The internal ID to search for.
     * @param[out]  connector           The found source connector.
     * @return True if the source connector is found, false otherwise.
     */
    ::OcaLiteStatus GetSourceConnectorValue(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSourceConnector& connector) const;

    /**
     * Retrieves the OcaLiteMediaSinkConnector with the given internal ID.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   id                  The internal ID to search for.
     * @param[out]  connector           The found sink connector.
     * @return True if the sink connector is found, false otherwise.
     */
    ::OcaLiteStatus GetSinkConnectorValue(::OcaLiteMediaConnectorID id, ::OcaLiteMediaSinkConnector& connector) const;

    /**
     * Gets the OcaLitePorts
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[out]  ports       Output parameter that holds the value of
     *                          the OcaLitePorts
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalGetPorts(::OcaLiteList< ::OcaLitePort>& ports) const;

    /**
     * Gets the port with the given ID.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   id      ID of the port to retrieve.
     * @returns The port with the given ID, NULL if the port is not found.
     */
    const ::OcaLitePort* InternalGetPort(const ::OcaLitePortID& id) const;

    /**
     * Adds an input or output port
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]  label        Label of the port
     * @param[in]  portMode     The mode of the port
     * @param[out] id           Id of the port
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalAddPort(const ::OcaLiteString& label, ::OcaPortMode portMode, ::OcaLitePortID& id);

    /**
     * Deletes a port
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]  id           Id of the port
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalDeletePort(const ::OcaLitePortID& id);

    /**
     * Adds a source connector and a new status to the administration and raises the associated events.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   initialState    The initial state of the connector.
     * @param[in]   connector       The connector to add. The ID internal will be set to the assigned
     *                              internal ID.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalAddSourceConnector(::OcaLiteMediaConnectorState initialState,
                                           const ::OcaLiteMediaSourceConnector& connector);

    /**
     * Adds a sink connector and a new status to the administration and raises the associated events.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   initialState    The initial status of the connector.
     * @param[in]   connector       The connector to add. The ID internal will be set to the assigned
     *                              internal ID.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalAddSinkConnector(::OcaLiteMediaConnectorState initialState,
                                         const ::OcaLiteMediaSinkConnector& connector);

    /**
     * Saves an updated source connector to the administration and raises the associated events.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   connector       The connector to update.
     * @param[in]   changedElement  The changed element for the property change event.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalUpdateSourceConnector(const ::OcaLiteMediaSourceConnector& connector,
                                              ::OcaLiteMediaConnectorElement changedElement);

    /**
     * Saves an updated sink connector to the administration and raises the associated events.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   connector       The connector to update.
     * @param[in]   changedElement  The changed element for the property change event.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalUpdateSinkConnector(const ::OcaLiteMediaSinkConnector& connector,
                                            ::OcaLiteMediaConnectorElement changedElement);

    /**
     * Deletes a connector from the administration and raises the associated events.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   id      The identification of the connector to delete.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalDeleteConnector(::OcaLiteMediaConnectorID id);

    /**
     * Sets the name of a port of the media transport network object. Can be overridden
     * by derived classes that actually implement the media transport network. As ports
     * are mapped onto hardware by default this method will return "OK" in case port names
     * are stored in the OCA data type and "Not Implemented" when the port names are not
     * stored in the OCA data type (i.e. OCA_OBJECT_READONLY_PORTS is defined).
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]   id          Id of the port
     * @param[in]   name        The value of port name
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetPortNameValue(const ::OcaLitePortID& id, const ::OcaLiteString& name);

    /**
     * Sets the new connector status value in the network's administration and emits the appropriate
     * event if necessary.
     * @note This method does not take the object's mutex itself.
     *       The mutex should already be taken before this method is called.
     *
     * @param[in]       status          The new connector status.
     */
    void UpdateConnectorStatus(const ::OcaLiteMediaConnectorStatus& status);

    /**
     * Adds a source connector with the given ID to this network. The new connector
     * is an output parameter. Must be implemented by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]       initialState    The initial state of the connector.
     * @param[in,out]   connector       The connector to add. This connector will be updated by this method.
     *                                  The ID internal of the passed connector needs to be set to a correct value in
     *                                  this method. The value should be unique in the complete set of all connectors,
     *                                  including both source and sink connectors.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus AddSourceConnectorValue(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSourceConnector& connector) = 0;

    /**
     * Adds a sink connector with the given ID to this network. The new connector
     * is an output parameter. Must be implemented by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     * @note To update the connector status in the administration, use the method UpdateConnectorStatus.
     *
     * @param[in]       initialState    The initial state of the connector.
     * @param[in,out]   connector       The connector to add. This connector will be updated by this method.
     *                                  The ID internal of the passed connector needs to be set to a correct value in
     *                                  this method. The value should be unique in the complete set of all connectors,
     *                                  including both source and sink connectors.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus AddSinkConnectorValue(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSinkConnector& connector) = 0;

    /**
     * Set a source connector's channel pin map.
     * Must be implemented by derived classes that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]   connector           The connector to update. May be updated by this method.
     *                                  A changed value will be notified to any subscribers.
     * @param[in]   channelPinMap       The new channel pin map.
     * @param[in]   status              The current status associated with the connector. May be updated
     *                                  by this method. A changed value will be notified to any subscribers.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetSourceConnectorPinMapValue(::OcaLiteMediaSourceConnector& connector,
                                                      const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                      ::OcaLiteMediaConnectorStatus& status) = 0;

    /**
     * Set a sink connector's channel pin map.
     * Must be implemented by derived classes that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]   connector           The connector to update. May be updated by this method.
     *                                  A changed value will be notified to any subscribers.
     * @param[in]   channelPinMap       The new channel pin map.
     * @param[in]   status              The current status associated with the connector. May be updated
     *                                  by this method. A changed value will be notified to any subscribers.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetSinkConnectorPinMapValue(::OcaLiteMediaSinkConnector& connector,
                                                    const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                    ::OcaLiteMediaConnectorStatus& status) = 0;

    /**
     * Deletes a source connector from this network. Closes all of its connections. Must be implemented by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]   connector       The connector to delete.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus DeleteSourceConnectorValue(const ::OcaLiteMediaSourceConnector& connector) = 0;

    /**
     * Deletes a sink connector from this network. Closes all of its connections. Must be implemented by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]   connector       The connector to delete.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus DeleteSinkConnectorValue(const ::OcaLiteMediaSinkConnector& connector) = 0;

private:
    /**
     * Internal representation of a media source (i.e. output) connector. Connects to an outbound stream. Collected by OcaLiteMediaTransportNetwork.
     */
    class InternalMediaSourceConnector
    {
    public:
        /**
         * Default constructor.
         */
        InternalMediaSourceConnector();

        /**
         * Copy constructor.
         *
         * @param[in]   source      The source to copy the object from.
         */
        InternalMediaSourceConnector(const InternalMediaSourceConnector& source);

        /**
         * Destructor.
         */
        virtual ~InternalMediaSourceConnector();

        /**
         * Reserves the space for the channel pin map.
         *
         * @param[in]   maxPins         The maximum number of pins per connector.
         */
        void ReserveChannelPinMap(::OcaUint16 maxPins);

        /**
         * Indicates if the connector is currently empty, i.e. not used.
         *
         * @return True if the connector is empty, false otherwise.
         */
        bool IsEmpty() const;

        /**
         * Reserves the connector for assignment, so it cannot be used for a new call.
         */
        void Reserve();

        /**
         * Clears the connector to make the new slot empty for future usage.
         */
        void Clear();

        /**
         * Getter for IDInternal.
         *
         * @return The internal ID
         */
        ::OcaLiteMediaConnectorID GetIDInternal() const;

        /**
         * Getter for PinCount.
         *
         * @return The number of pins in this connector.
         */
        ::OcaUint16 GetPinCount() const;

        /**
         * Assigns the internal class to the OCA class.
         *
         * @param[out]  target      The target to assign the object to.
         */
        void AssignToOca(::OcaLiteMediaSourceConnector& target) const;

        /**
         * Assignment operator.
         *
         * @param[in]   source      The source to assign the object from.
         * @return A reference to this object.
         */
        InternalMediaSourceConnector& operator=(const InternalMediaSourceConnector& source);

        /**
         * Assignment operator.
         *
         * @param[in]   source      The source to assign the object from.
         * @return A reference to this object.
         */
        InternalMediaSourceConnector& operator=(const ::OcaLiteMediaSourceConnector& source);

    private:
        /** Indicates if the slot is empty */
        bool                                                    m_isEmpty;

        /** The internal ID. */
        ::OcaLiteMediaConnectorID                                   m_idInternal;

        /** The public name of connector. May be published to the media transport network,
            depending on the type of network. */
        ::OcaLiteString                                             m_idExternal;

        /** The descriptor of the stream connection to this connector. If there is no
            stream connected to this controller, (i.e. property Connected = FALSE),
            the value of this property is undefined. */
        ::OcaLiteMediaConnection                                    m_connection;

        /** The specification of coding used by this connector. */
        ::OcaLiteMediaCoding                                        m_coding;

        /** The number of pins in this connector. */
        ::OcaUint16                                             m_pinCount;

        /** The map of stream pins (source channels) to OCA ports (input ports) of the
            owning OcaLiteMediaTransportNetwork object. */
        std::vector< ::OcaLiteMapItem< ::OcaUint16, ::OcaLitePortID> >  m_channelPinMap;
    };

    /**
     * Internal representation of a media sink (i.e. input) connector.
     */
    class InternalMediaSinkConnector
    {
    public:
        /**
         * Default constructor.
         */
        InternalMediaSinkConnector();

        /**
         * Copy constructor.
         *
         * @param[in]   source      The source to copy the object from.
         */
        InternalMediaSinkConnector(const InternalMediaSinkConnector& source);

        /**
         * Destructor.
         */
        virtual ~InternalMediaSinkConnector();

        /**
         * Reserves the space for the channel pin map.
         *
         * @param[in]   maxPins         The maximum number of pins per connector.
         * @param[in]   maxPorts        The maximum number of ports per pin.
         */
        void ReserveChannelPinMap(::OcaUint16 maxPins, ::OcaUint16 maxPorts);

        /**
         * Indicates if the connector is currently empty, i.e. not used.
         *
         * @return True if the connector is empty, false otherwise.
         */
        bool IsEmpty() const;

        /**
         * Reserves the connector for assignment, so it cannot be used for a new call.
         */
        void Reserve();

        /**
         * Clears the connector to make the new slot empty for future usage.
         */
        void Clear();

        /**
         * Getter for IDInternal.
         *
         * @return The internal ID
         */
        ::OcaLiteMediaConnectorID GetIDInternal() const;

        /**
         * Getter for PinCount.
         *
         * @return The number of pins in this connector.
         */
        ::OcaUint16 GetPinCount() const;

        /**
         * Assigns the internal class to the OCA class.
         *
         * @param[out]  target      The target to assign the object to.
         */
        void AssignToOca(::OcaLiteMediaSinkConnector& target) const;

        /**
         * Assignment operator.
         *
         * @param[in]   source      The source to assign the object from.
         * @return A reference to this object.
         */
        InternalMediaSinkConnector& operator=(const InternalMediaSinkConnector& source);

        /**
         * Assignment operator.
         *
         * @param[in]   source      The source to assign the object from.
         * @return A reference to this object.
         */
        InternalMediaSinkConnector& operator=(const ::OcaLiteMediaSinkConnector& source);

    private:
        /** Indicates if the slot is empty */
        bool                                                    m_isEmpty;

        /** The internal ID. */
        ::OcaLiteMediaConnectorID                                   m_idInternal;

        /** The public name of connector. May be published to the media transport network,
            depending on the type of network. */
        ::OcaLiteString                                             m_idExternal;

        /** The descriptor of the stream connection to this connector. If there is no
            stream connected to this controller, (i.e. property Connected = FALSE),
            the value of this property is undefined. */
        ::OcaLiteMediaConnection                                    m_connection;

        /** The specification of coding used by this connector. */
        ::OcaLiteMediaCoding                                        m_coding;

        /** The number of pins in this connector. */
        ::OcaUint16                                             m_pinCount;

        /** The map of stream pins (sink channels) to OCA ports (output ports) of the
            owning OcaLiteMediaTransportNetwork object. */
        std::vector< ::OcaLiteMapItem< ::OcaUint16, ::OcaLitePortID> >  m_channelPinMap;
    };

    /**
     * Checks if the given port index is already present in the list of ports of the given mode.
     *
     * @param[in]   portMode        The port mode to check for.
     * @param[in]   portIndex       The port index to check for.
     * @return True if the port index is already part of the list of ports of the given mode, false otherwise.
     */
    bool PortsContainIndex(::OcaPortMode portMode, ::OcaUint16 portIndex) const;

    /**
     * Checks if the given connector ID is already present in the lists of connectors.
     *
     * @param[in]   id              The connector ID to check for.
     * @return True if the connector ID is already part of the lists of connectors, false otherwise.
     */
    bool ConnectorsContainID(::OcaLiteMediaConnectorID id) const;

    /**
     * Event that is emitted when the ports have changed.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   changeType      The change type for the property change event.
     */
    void PortsChanged(::OcaLitePropertyChangeType changeType);

    /**
     * Event that is emitted when a source connector is added or deleted.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   connector       The added or deleted source connector.
     * @param[in]   changeType      The change type for the property change event.
     * @param[in]   changedElement  The changed element for the property change event.
     */
    void SourceConnectorChanged(const ::OcaLiteMediaSourceConnector& connector,
                                ::OcaLitePropertyChangeType changeType,
                                ::OcaLiteMediaConnectorElement changedElement);

    /**
     * Event that is emitted when a sink connector is added or deleted.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   connector       The added or deleted sink connector.
     * @param[in]   changeType      The change type for the property change event.
     * @param[in]   changedElement  The changed element for the property change event.
     */
    void SinkConnectorChanged(const ::OcaLiteMediaSinkConnector& connector,
                              ::OcaLitePropertyChangeType changeType,
                              ::OcaLiteMediaConnectorElement changedElement);

    /**
     * Event that is emitted when a connector status has changed.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in]   status          The changed status.
     */
    void ConnectorStatusChanged(const ::OcaLiteMediaConnectorStatus& status);

    /**
     * The list of (input and output) OCA ports the network object has.
     * @note A network object can have no ports (in which case the list is empty).
     */
    typedef std::list< ::OcaLitePort> PortList; 
    PortList                                    m_ocaPorts;


    /** The list of source connectors the network has. */
    typedef std::vector<InternalMediaSourceConnector> SourceConnectorList;
    SourceConnectorList                         m_sourceConnectors;

    /** The list of sink connectors the network has. */
    typedef std::vector<InternalMediaSinkConnector> SinkConnectorList;
    SinkConnectorList                           m_sinkConnectors;

    /** The list of connectors statuses. */
    ::OcaLiteList< ::OcaLiteMediaConnectorStatus>       m_connectorsStatuses;

    /** Pointer to class with eventhandler to call when the
        SourceConnectorChangedEvent is generated. */
    IEventDelegate*                             m_pSourceConnectorChangedEventDelegate;

    /** Pointer to class with eventhandler to call when the
        SinkConnectorChangedEvent is generated. */
    IEventDelegate*                             m_pSinkConnectorChangedEventDelegate;

    /** Pointer to class with eventhandler to call when the
        ConnectorStatusChangedEvent is generated. */
    IEventDelegate*                             m_pConnectorStatusChangedEventDelegate;

    /** private copy constructor, no copying of object allowed */
    OcaLiteMediaTransportNetwork(const ::OcaLiteMediaTransportNetwork&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteMediaTransportNetwork& operator=(const ::OcaLiteMediaTransportNetwork&);
};


#endif // OCALITEMEDIATRANSPORTNETWORK_H
