/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteNetworkSignalChannel
 *
 */

#ifndef OCALITENETWORKSIGNALCHANNEL_H
#define OCALITENETWORKSIGNALCHANNEL_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteMap.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>

// ---- Include local include files ----
#include <OCC/ControlClasses/Workers/OcaLiteWorker.h>

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_NETWORK_SIGNAL_CHANNEL_CLASSID  OCA_WORKER_CLASSID,static_cast< ::OcaUint16>(6)

/**
 * Status options for OcaNetworkSignalChannel.
 */
enum OcaLiteNetworkSignalChannelStatus  /* maps onto OcaUint8 */
{
    /** Channel is not ready to transfer data. */
    OCANETWORKSIGNALCHANNELSTATUS_NOT_CONNECTED        = 0,
    /** Channel is ready for data transfer. */
    OCANETWORKSIGNALCHANNELSTATUS_CONNECTED            = 1,
    /** Channel is set up, but data transfer has been halted. */
    OCANETWORKSIGNALCHANNELSTATUS_MUTED                = 2,    
    /** Maximum value used for range checking */
    OCANETWORKSIGNALCHANNELSTATUS_MAXIMUM
};

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Worker that allows connection of one or more internal signal paths to a network signal channel.
 * - For stream-oriented media connection management such as used by AVB, this · worker will be linked
 *   to an OcaNetworkStreamConnector object and to the appropriate OcaStreamNetwork object.
 * - For channel-oriented media connection management, such as the Dante name-based routing
 *   mechanism, this worker will be linked only to the OcaStreamNetwork object.
 */
class OcaLiteNetworkSignalChannel : public ::OcaLiteWorker
{
    public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetIDAdvertised() */
        GET_ID_ADVERTISED               = 1,
        /** SetIDAdvertised() */
        SET_ID_ADVERTISED               = 2,
        /** GetNetwork() */
        GET_NETWORK                     = 3,
        /** SetNetwork() */
        SET_NETWORK                     = 4,
        /** GetConnectorPins() */
        GET_CONNECTOR_PINS              = 5,
        /** AddToConnector() */
        ADD_TO_CONNECTOR                = 6,
        /** RemoveFromConnector() */
        REMOVE_FROM_CONNECTOR           = 7,
        /** GetRemoteChannelID() */
        GET_REMOTE_CHANNEL_ID           = 8,
        /** SetRemoteChannelID() */
        SET_REMOTE_CHANNEL_ID           = 9,
        /** GetSourceOrSink() */
        GET_SOURCE_OR_SINK              = 10,
        /** GetStatus() */
        GET_STATUS                      = 11
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Character name or binary identifier of the port that is advertised on the network to be found by
            other devices' discovery processes. Depending on the media transport architecture being used,
            this ID may be globally unique throughout the network, or only unique within the scope of the
            specific Network instance to which the port is attached. */
        OCA_PROP_ID_ADVERTISED              = 1,
        /** Object number of stream network object (OcaStreamNetwork or one of its subclasses) to which
            this signal channel belongs. */
        OCA_PROP_NETWORK                    = 2,
        /** Map of object numbers of OcaStreamConnector objects to OcaStreamConnectorPinIndex of these
            connectors. This map identifies which OcaStreamConnector objects contain this network signal
            channel, and indicates at what pin of the connector this channel is found. */
        OCA_PROP_CONNECTOR_PINS             = 3,
        /** External ID of ultimate source or destination of signal. */
        OCA_PROP_REMOTE_CHANNEL_ID          = 4,
        /** Describes whether this signal channel is source (emits signals into the network) or sink (receives
            signals from the network). Sources are sometimes called "talkers", and sinks are sometimes
            called "listeners". */
        OCA_PROP_SOURCE_OR_SINK             = 5,
        /** Status of the port */
        OCA_PROP_STATUS                     = 6
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteNetworkSignalChannel();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    void StatusChanged(::OcaLiteNetworkSignalChannelStatus status);

    void RemoteChannelIDChanged(const ::OcaLiteNetworkSignalChannelID& remoteChannelID);

    ::OcaLiteStatus SetRemoteChannelID(const ::OcaLiteNetworkSignalChannelID& signalChannelID);

    ::OcaLiteNetworkMediaSourceOrSink GetSourceOrSink() const
    {
        return m_sourceOrSink;
    }

    ::OcaLiteStatus AddToConnector(::OcaONo connector, ::OcaLiteStreamConnectorPinIndex index);

    ::OcaLiteStatus RemoveFromConnector(::OcaONo connector);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   ports                   The OCA input and output ports.
     * @param[in]   idAdvertised            Default signal channel ID.
     * @param[in]   network                 Default Object number of stream network object (OcaStreamNetwork
     *                                      or one of its subclasses) to which this signal channel belongs.
     * @param[in]   sourceOrSink            Default I/O direction for this port. Sink = from this
     *                                      network into device; source = from device into network.
     */
    OcaLiteNetworkSignalChannel(::OcaONo objectNumber,
                                ::OcaBoolean lockable,
                                const ::OcaLiteString& role,
                                const ::OcaLiteList< ::OcaLitePort>& ports,
                                const ::OcaLiteNetworkSignalChannelID& idAdvertised,
                                ::OcaONo network,
                                ::OcaLiteNetworkMediaSourceOrSink sourceOrSink);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    virtual ::OcaLiteStatus SetRemoteChannelIDValue(const ::OcaLiteNetworkSignalChannelID& signalChannelID);

private:
    /* Indicates whether this is a source or sink channel */
    ::OcaLiteNetworkMediaSourceOrSink                           m_sourceOrSink;

    /* Indicates the current channel status */
    ::OcaLiteNetworkSignalChannelStatus                         m_status;

    /* Connector pins. */
    ::OcaLiteMap< ::OcaONo, ::OcaLiteStreamConnectorPinIndex>   m_connectorPins;

    /* Indicates the current subscription */
    ::OcaLiteNetworkSignalChannelID                             m_remoteChannelID;

    /* Indicates the current channel name */
    ::OcaLiteNetworkSignalChannelID                             m_idAdvertised;

    /* Indicate the owner of this network */
    ::OcaONo                                                    m_network;

    /** private copy constructor, no copying of object allowed */
    OcaLiteNetworkSignalChannel(const ::OcaLiteNetworkSignalChannel&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteNetworkSignalChannel& operator=(const ::OcaLiteNetworkSignalChannel&);
};

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::OcaLiteNetworkSignalChannelStatus>(const ::OcaLiteNetworkSignalChannelStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteNetworkSignalChannelStatus>(::OcaLiteNetworkSignalChannelStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <> 
::OcaUint32 GetSizeValue< ::OcaLiteNetworkSignalChannelStatus>(const ::OcaLiteNetworkSignalChannelStatus& value, const ::IOcaLiteWriter& writer);

#endif // OCALITENETWORKSIGNALCHANNEL_H
