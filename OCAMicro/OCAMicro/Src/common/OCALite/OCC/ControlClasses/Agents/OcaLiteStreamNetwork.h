/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteStreamNetwork
 *
 */
#ifndef OCALITESTREAMNETWORK_H
#define OCALITESTREAMNETWORK_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Agents/OcaLiteAgent.h>
#include <OCC/ControlDataTypes/OcaLiteBlobDataType.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkSystemInterfaceID.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----
class OcaLiteNetworkSignalChannel;
class OcaLiteStreamConnector;

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_STREAMNETWORK_CLASSID   OCA_AGENT_CLASSID,static_cast< ::OcaUint16>(10)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for defining stream network.
 */
class OcaLiteStreamNetwork : public ::OcaLiteAgent
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetLinkType() */
        GET_LINK_TYPE                   = 1,
        /** GetIDAdvertised() */
        GET_ID_ADVERTISED               = 2,
        /** SetIDAdvertised() */
        SET_ID_ADVERTISED               = 3,
        /** GetControlProtocol() */
        GET_CONTROL_PROTOCOL            = 4,
        /** GetMediaProtocol() */
        GET_MEDIA_PROTOCOL              = 5,
        /** GetStatus() */
        GET_STATUS                      = 6,
        /** GetStatistics() */
        GET_STATISTICS                  = 7,
        /** ResetStatistics() */
        RESET_STATISTICS                = 8,
        /** GetSystemInterfaces() */
        GET_SYSTEM_INTERFACES           = 9,
        /** SetSystemInterfaces() */
        SET_SYSTEM_INTERFACES           = 10,
        /** GetStreamConnectorsSource() */
        GET_STREAM_CONNECTORS_SOURCE    = 11,
        /** SetStreamConnectorsSource() */
        SET_STREAM_CONNECTORS_SOURCE    = 12,
        /** GetStreamConnectorsSink() */
        GET_STREAM_CONNECTORS_SINK      = 13,
        /** SetStreamConnectorsSink() */
        SET_STREAM_CONNECTORS_SINK      = 14,
        /** GetSignalChannelsSource() */
        GET_SIGNAL_CHANNELS_SOURCE      = 15,
        /** SetSignalChannelsSource() */
        SET_SIGNAL_CHANNELS_SOURCE      = 16,
        /** GetSignalChannelsSink() */
        GET_SIGNAL_CHANNELS_SINK        = 17,
        /** SetSignalChannelsSink() */
        SET_SIGNAL_CHANNELS_SINK        = 18
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Network link type - e.g. wired Ethernet, USB, ... See the OcaNetworkType enum for details. This
            is a read-only property whose value is fixed to the class that is inherited from OcaNetwork to
            implement each specific type of network. */
        OCA_PROP_LINK_TYPE                  = 1,
        /** ID by which this device is known on the network, i.e. the host name or GUID that this device
            publishes in the network's directory/discovery system. */
        OCA_PROP_ID_ADVERTISED              = 2,
        /** Type of control protocol used by the network (OCAnn) or NONE if this network is not used for
            control. */
        OCA_PROP_CONTROL_PROTOCOL           = 3,
        /** Type of media transport protocol used by the network, or NONE if this network is not used for
            media transport. */
        OCA_PROP_MEDIA_PROTOCOL             = 4,
        /** Operational status of the network. */
        OCA_PROP_STATUS                     = 5,
        /** Collection of identifiers of system interface(s) used by the network. A "system interface" is the
            system service through which network traffic passes into and out of the device -- e.g. a socket.
            The identifier format is system and network dependent; for OCA purposes, it is maintained as a
            variable-length blob which the protocol does not inspect. */
        OCA_PROP_SYSTEM_INTERFACES          = 6,
        /** List of object numbers of source OcaStreamConnector objects collected by this network. */
        OCA_PROP_STREAM_CONNECTORS_SOURCE   = 7,
        /** List of object numbers of sink OcaStreamConnector objects collected by this network. */
        OCA_PROP_STREAM_CONNECTORS_SINK     = 8,
        /** List of object numbers of source OcaNetworkSignalChannel objects collected by this network. */
        OCA_PROP_SIGNAL_CHANNELS_SOURCE     = 9,
        /** List of object numbers of sink OcaNetworkSignalChannel objects collected by this network. */
        OCA_PROP_SIGNAL_CHANNELS_SINK       = 10,
        /** Error statistics for this network */
        OCA_PROP_STATISTICS                 = 11
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteStreamNetwork();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----
    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);


    ::OcaBoolean Initialize();

    void Teardown();
protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   linkType                The network link type.
     * @param[in]   idAdvertised            Default name by which this device
     *                                      advertises itself on this network.
     * @param[in]   controlProtocol         The network control protocol
     * @param[in]   mediaProtocol           The network media protocol
     * @param[in]   systemInterfaces        Collection of identifiers of system interface(s) used by
     *                                      the network. A "system interface" is the system service through
     *                                      which network traffic passes into and out of the device
     *                                      -- e.g. a socket. The identifier format is system and network
     *                                      dependent; for OCA purposes, it is maintained as a
     *                                      variable-length blob which the protocol does not inspect.
     */
    OcaLiteStreamNetwork(::OcaONo objectNumber,
                         ::OcaBoolean lockable,
                         const ::OcaLiteString& role,
                         ::OcaLiteNetworkLinkType linkType,
                         const ::OcaLiteBlobDataType& idAdvertised,
                         ::OcaLiteNetworkControlProtocol controlProtocol,
                         ::OcaLiteNetworkMediaProtocol mediaProtocol);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    ::OcaBoolean AddSignalSourceSink(::OcaLiteNetworkSignalChannel& signalChannel);

    ::OcaBoolean AddConnectorSourceSink(::OcaLiteStreamConnector& connector);

    ::OcaBoolean RemoveConnectorSourceSink(::OcaLiteStreamConnector& connector);

private:
    /** The network's link type. */
    ::OcaLiteNetworkLinkType                        m_networkLinkType;

    /** The network's advertised ID. */
    ::OcaLiteBlobDataType                          m_idAdvertised;

    /** The network's control protocol. */
    ::OcaLiteNetworkControlProtocol                 m_networkControlProtocol;

    /** The network's media protocol */
    ::OcaLiteNetworkMediaProtocol                   m_networkMediaProtocol;

    /** List with single sources and sinks */
    ::OcaLiteList< ::OcaONo>                        m_signalSource;
    ::OcaLiteList< ::OcaONo>                        m_signalSink;

    /** List with single sources and sinks stream connectors */
    ::OcaLiteList< ::OcaONo>                        m_connectorsSource;
    ::OcaLiteList< ::OcaONo>                        m_connectorsSink;

    /** private copy constructor, no copying of object allowed */
    OcaLiteStreamNetwork(const ::OcaLiteStreamNetwork&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteStreamNetwork& operator=(const ::OcaLiteStreamNetwork&);
};

#endif // OCALITESTREAMNETWORK_H
