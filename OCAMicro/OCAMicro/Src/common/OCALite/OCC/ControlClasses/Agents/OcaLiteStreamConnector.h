/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaStreamConnector
 *
 */

#ifndef OCALITESTREAMCONNECTOR_H
#define OCALITESTREAMCONNECTOR_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Agents/OcaLiteAgent.h>
#include <OCC/ControlClasses/Workers/Networking/OcaLiteNetworkSignalChannel.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteMap.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteStream.h>
#include <OCC/ControlDataTypes/OcaLiteStreamConnectorID.h>
#include <OCC/ControlDataTypes/OcaLitePortID.h>

// ---- Include local include files ----
#include "OcaLiteStreamNetwork.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_STREAM_CONNECTOR_CLASSID  OCA_AGENT_CLASSID,static_cast< ::OcaUint16>(11)

/**
 * Status options for a stream connector.
 */
enum OcaLiteStreamConnectorStatus  /* maps onto OcaUint8 */
{
    /** Connector is not ready to transfer data. */
    OCASTREAMCONNECTORSTATUS_NOTAVAILABLE   = 0,
    /** Connector is ready for data transfer but is not connected to any streams. */
    OCASTREAMCONNECTORSTATUS_IDLE = 1,
    /** Connector is connected to at least one stream. */
    OCASTREAMCONNECTORSTATUS_CONNECTED = 2,
    /** Connector is connected to at least one stream but data transfer has been halted by controller request. */
    OCASTREAMCONNECTORSTATUS_PAUSED = 3,
    /** Maximum value used for range checking */
    OCASTREAMCONNECTORSTATUS_MAXIMUM
};

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Agent class for objects ("connectors") that allow connection of streams to the device. Streams may be
 * single channels or multichannel groups. A connector is either a source or a sink. Sources are 
 * sometimes called "talkers". Sinks are sometimes called "listeners".<br />
 * Each connector links to zero or more OcaStream data objects. Each OcaStream object represents a
 * signal flow to or from a local connector to a remote connector. The remote connector is usually, but not
 * necessarily, in a different node. Each connector collects zero or more signal channels. A signal channel
 * is an instance of OcaNetworkSignalChannel.Each signal channel exposes one media channel of the stream to the 
 * interior of the device. A signal channel therefore is a Worker that contains exactly one OcaPort data
 * object. Each OcaNetworkStreamConnector object belongs to a particular instance of OcaStreamNetwork or a
 * subclass of OcaStreamNetwork .<br />
 * Each OcaStreamConnector is linked to its network through the Owner property. When a controller creates 
 * an OcaStreamConnector object dynamically, <br />
 * - The controller must store the Object Number of the corresponding OcaStreamNetwork object in the Owner property.<br />
 * - Upon receiving the Owner property change, the OcaNetworkStreamConnector object must register.<br />
 * itself with the given stream network object via some internal means.<br />
 * 
 * This class may be subclassed to support various network types.
 */
class OcaLiteStreamConnector : public ::OcaLiteAgent
{
public:
    /**
    * Constructor
    *
    * @param[in]   objectNumber        Object number of this instance.
    * @param[in]   lockable            Indicates whether or not the object
    *                                  is lockable.
    * @param[in]   role                The role of this instance.
    * @param[in]   owner               Default object number of OcaStreamNetwork object to which this
    *                                  connector belongs.
    * @param[in]   idAdvertised        Default identifier of the connector.
    * @param[in]   sourceOrSink        Default I/O direction for this port. Sink = from this
    *                                  network into device; source = from device into network,
    * @param[in]   pinCount            The number of pins this connector has in its Pins property.
    */
    OcaLiteStreamConnector(::OcaONo objectNumber,
                           ::OcaBoolean lockable,
                           const ::OcaLiteString& role,
                           ::OcaONo owner,
                           const ::OcaLiteStreamConnectorID& idAdvertised,
                           const ::OcaLiteNetworkMediaSourceOrSink& sourceOrSink,
                           ::OcaUint16 pinCount);

    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetOwnerNetwork() */
        GET_OWNER_NETWORK           = 1,
        /** SetOwnerNetwork() */
        SET_OWNER_NETWORK           = 2,
        /** GetIDAdvertised() */
        GET_ID_ADVERTISED           = 3,
        /** SetIDAdvertised() */
        SET_ID_ADVERTISED           = 4,
        /** GetSourceOrSink() */
        GET_SOURCE_OR_SINK          = 5,
        /** SetSourceOrSink() */
        SET_SOURCE_OR_SINK          = 6,
        /** ConnectStream() */
        CONNECT_STREAM              = 7,
        /** DisconnectStream() */
        DISCONNECT_STREAM           = 8,
        /** GetStreams() */
        GET_STREAMS                 = 9,
        /** GetPins() */
        GET_PINS                    = 10,
        /** GetStatus() */
        GET_STATUS                  = 11
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Object number of stream network object (OcaStreamNetwork or one of its subclasses) to which
            this connector belongs. In reconfigurable devices, a controller that creates an
            OcaStreamConnector object must store the appropriate stream network object number
            into this property. It is assumed that, upon receiving a value into its Owner property, the 
            terminus object will by internal means register itself with the identified stream network.*/
        OCA_PROP_OWNER_NETWORK      = 1,
        /** Character name or binary identifier of this connector. This ID is advertised on the network to be
            found by other devices' discovery processes. */
        OCA_PROP_ID_ADVERTISED      = 2,
        /** Specifies whether this connector is for output (source) or input (sink) */
        OCA_PROP_SOURCE_OR_SINK     = 3,
        /** The list of OcaStream data objects contained in (i.e. connected to) this connector. */
        OCA_PROP_STREAMS            = 4,
        /** The map of connector pin indexes to OcaNetworkSignalChannel[Source|Sink] objects
            collected by this connector. The pin indexes are fixed indexes 1 to n, where n is the number of
            channels the connector accommodates (determined when the connector is created). If a certain
            pin in the connector is currently not attached the OcaONo of that index is 0. */
        OCA_PROP_PINS               = 5,
        /** Status of this terminus. */
        OCA_PROP_STATUS             = 6
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteStreamConnector();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----

    /**
     * Gets the object number of the OcaStreamNetwork object to which this connector belongs.
     *
     * @param[out]   network    Object number of stream network object that owns this terminus.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetOwnerNetwork(::OcaONo& network) const;

    /** 
     * Gets the value of the IDAdvertised property.
     *
     * @param[out]   idAdvertised   The value of the IDAdvertised property
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetIDAdvertised(::OcaLiteStreamConnectorID& idAdvertised) const;

    /**
     * Gets the value of the SourceOrSink property.
     *
     * @param[out]   sourceOrSink   The value of the SourceOrSink property.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetSourceOrSink(::OcaLiteNetworkMediaSourceOrSink& sourceOrSink) const;

    /**
     * Gets the list of OcaNetworkStream items connected to this connector.
     *
     * @param[out]  streams     The value of the Streams property.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetStreams(::OcaLiteList< ::OcaLiteStream>& streams) const;

    /**
     * Gets the map of connector pins to signal channels (identification of 
     * which signal channel is attached to which pin).
     *
     * @param[out]  pins    The map of connector pins to signalchannels.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetPins(::OcaLiteMap< ::OcaLiteStreamConnectorPinIndex, ::OcaONo>& pins) const;

    /**
     * Gets the value of the Status property.
     *
     * @param[out]   status         The value of the Status property
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetStatus(::OcaLiteStreamConnectorStatus& status) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    /**
     * Add a signal channel to this connector. This method should only be called by the
     * OcaNetworkSignalChannel class.
     *
     * @param[in]   signalChannel   The signal channel oNo to add.
     * @param[in]   index           The pin index at which the signal channel is to be attached.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus AddSignalChannel(::OcaONo signalChannel, ::OcaLiteStreamConnectorPinIndex index);

    /**
     * Delete a signal channel from this connector. This method should only be called by the
     * OcaStreamConnector class.
     *
     * @param[in]   signalChannel   The signal channel oNo to delete.
     * @param[in]   index           The pin index from which the signal channel is to be deleted.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus DeleteSignalChannel(::OcaONo signalChannel, ::OcaLiteStreamConnectorPinIndex index);

    void SetStream(const ::OcaLiteStream& stream)
    {
        m_stream = stream;
    }

protected:

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:

    /** Object number of stream network object (OcaStreamNetwork or one of its subclasses) to which
        this connector belongs. In reconfigurable devices, a controller that creates an
        OcaNetworkStreamConnector object must store the appropriate stream network object number
        into this property. */
    ::OcaONo                                                        m_ownerNetwork;
    /** Character name or binary identifier of the port that is advertised on the network to be found by other devices' discovery processes. */
    ::OcaLiteStreamConnectorID                                      m_idAdvertised;
    /** Describes whether this media port is source (emits signals into the network) or sink (receives signals from the network). */
    ::OcaLiteNetworkMediaSourceOrSink                               m_sourceOrSink;    /** The map of connector pin indexes to OcaNetworkSignalChannel[Source|Sink] objects
        collected by this connector. The pin indexes are fixed indexes 1 to n, where n is the number of
        channels the connector accommodates (determined when the connector is created). If a certain
        pin in the connector is currently not attached the OcaONo of that index is 0. */
    ::OcaLiteMap< ::OcaLiteStreamConnectorPinIndex, ::OcaONo>       m_pins;
    /** Status of this terminus. */
    ::OcaLiteStreamConnectorStatus                                  m_status;
    /** The last used index of a connected stream. */
    ::OcaLiteStreamIndex                                            m_lastStreamIndex;

    ::OcaLiteStream                                                    m_stream;
    /** private copy constructor, no copying of object allowed */
    OcaLiteStreamConnector(const ::OcaLiteStreamConnector&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteStreamConnector& operator=(const ::OcaLiteStreamConnector&);
};

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::OcaLiteStreamConnectorStatus>(const ::OcaLiteStreamConnectorStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteStreamConnectorStatus>(::OcaLiteStreamConnectorStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteStreamConnectorStatus>(const ::OcaLiteStreamConnectorStatus& value, const ::IOcaLiteWriter& writer);

#endif // OCALITESTREAMCONNECTOR_H
