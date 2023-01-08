/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMediaTransportNetworkAes67
 *
 */
#ifndef OCALITEMEDIATRANSPORTNETWORKAES67_H
#define OCALITEMEDIATRANSPORTNETWORKAES67_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Networks/OcaLiteMediaTransportNetwork.h>
#include <OCC/ControlDataTypes/OcaLiteWorkerDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteApplicationNetworkDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----


// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_MEDIA_TRANSPORT_NETWORK_AES67_CLASSID   OCA_MEDIA_TRANSPORT_NETWORK_CLASSID,OCA_CLASS_ID_PROPRIETARY_MANUFACTURER_ID(OCA_ALLIANCE_COMPANY_ID),static_cast< ::OcaUint16>(1)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Media transport adaptation for the AES67 protocol.
 */
class OcaLiteMediaTransportNetworkAes67 : public ::OcaLiteMediaTransportNetwork
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetSendPacketTimes() */
        GET_SEND_PACKET_TIMES           = 1,
        /** GetReceivePacketTimes() */
        GET_RECEIVE_PACKET_TIMES        = 2,
        /** GetMinReceiveBufferCapacity() */
        GET_MIN_RECEIVE_BUFFER_CAPACITY = 3,
        /** GetMaxReceiveBufferCapacity() */
        GET_MAX_RECEIVE_BUFFER_CAPACITY = 4,
        /** GetTransmissionTimeVariation() */
        GET_TRANSMISSION_TIME_VARIATION = 5,
        /** GetSupportedDiscoverySystems() */
        GET_SUPPORTED_DISCOVERY_SYSTEMS = 6
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** The possible packet times of the device in the send direction. If the device is a receive-only device this will be an
            empty list. */
        OCA_PROP_SEND_PACKET_TIMES              = 1,
        /** The possible packet times of the device in the receive direction. If the device is a send-only device this will be an
            empty list. */
        OCA_PROP_RECEIVE_PACKET_TIMES           = 2,
        /** The minimum receive buffer capacity of the device. Will be 0 for a send-only device. */
        OCA_PROP_MIN_RECEIVE_BUFFER_CAPACITY    = 3,
        /** The maximum receive buffer capacity of the device. Will be 0 for a send-only device. */
        OCA_PROP_MAX_RECEIVE_BUFFER_CAPACITY    = 4,
        /** The sender transmission time variation from the nominal transmission time of the device. Will be 0 for a receive-only
            device. */
        OCA_PROP_TRANSMISSION_TIME_VARIATION    = 5,
        /** Indicates what AES67 discovery systems are supported by the device. */
        OCA_PROP_SUPPORTED_DISCOVERY_SYSTEMS    = 6
    };

    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                Read-only text property that describes object's role
     *                                  in the device. Particularly useful for workers, e.g. "Input 1 Gain".
     * @param[in] minAlignmentLevel     The minimum alignment level.
     * @param[in] maxAlignmentLevel     The maximum alignment level.
     * @param[in] defaultAlignmentGain  The default alignment gain.
     * @param[in] minAlignmentGain      The minimum alignment gain.
     * @param[in] maxAlignmentGain      The maximum alignmnet gain.
     */
    OcaLiteMediaTransportNetworkAes67(::OcaONo objectNumber,
                                      ::OcaBoolean lockable,
                                      const ::OcaLiteString& role,
                                      ::OcaDBfs defaultAlignmentLevel,
                                      ::OcaDBfs minAlignmentLevel,
                                      ::OcaDBfs maxAlignmentLevel,
                                      ::OcaDB defaultAlignmentGain,
                                      ::OcaDB minAlignmentGain,
                                      ::OcaDB maxAlignmentGain);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaTransportNetworkAes67();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----

    virtual ::OcaLiteStatus Control(::OcaLiteApplicationNetworkCommand command)
    {
        return OCASTATUS_NOT_IMPLEMENTED;
    }

    virtual ::OcaLiteStatus GetMediaProtocol(::OcaLiteNetworkMediaProtocol& protocol) const;

    virtual ::OcaLiteStatus GetMaxSourceConnectors(::OcaUint16& maxSourceConnectors) const;

    virtual ::OcaLiteStatus GetMaxSinkConnectors(::OcaUint16& maxSinkConnectors) const;

    virtual ::OcaLiteStatus GetMaxPinsPerConnector(::OcaUint16& maxPins) const;

    virtual ::OcaLiteStatus GetMaxPortsPerPin(::OcaUint16& maxPorts) const;

    /**
     * Gets the value of the SendPacketTimes property.
     *
     * @param[out]  sendPacketTimes     Output parameter containing the value of the
     *                                  SendPacketTimes property.
     * @return Indicates success of the operation
     */
    ::OcaLiteStatus GetSendPacketTimes(::OcaLiteList< ::OcaTimeInterval>& sendPacketTimes) const;

    /**
     * Gets the value of the ReceivePacketTimes property.
     *
     * @param[out]  receivePacketTimes  Output parameter containing the value of the
     *                                  ReceivePacketTimes property.
     * @return Indicates success of the operation
     */
    ::OcaLiteStatus GetReceivePacketTimes(::OcaLiteList< ::OcaTimeInterval>& receivePacketTimes) const;

    /**
     * Gets the value of the MinReceiveBufferCapacity property.
     *
     * @param[out]  minReceiveBufferCapacity    Output parameter containing the value
     *                                          of the MinReceiveBufferCapacity property.
     * @return Indicates success of the operation
     */
    ::OcaLiteStatus GetMinReceiveBufferCapacity(::OcaTimeInterval& minReceiveBufferCapacity) const;

    /**
     * Gets the value of the MaxReceiveBufferCapacity property.
     *
     * @param[out]  maxReceiveBufferCapacity    Output parameter containing the value
     *                                          of the MaxReceiveBufferCapacity property.
     * @return Indicates success of the operation
     */
    ::OcaLiteStatus GetMaxReceiveBufferCapacity(::OcaTimeInterval& maxReceiveBufferCapacity) const;

    /**
     * Gets the value of the TransmissionTimeVariation property.
     *
     * @param[out]  transmissionTimeVariation   Output parameter containing the value
     *                                          of the TransmissionTimeVariation property.
     * @return Indicates success of the operation
     */
    ::OcaLiteStatus GetTransmissionTimeVariation(::OcaTimeInterval& transmissionTimeVariation) const;

    /**
     * Gets the value of the SupportedDiscoverySystems property.
     *
     * @param[out]  supportedDiscoverySystems   Output parameter containing the value of
     *                                          the SupportedDiscoverySystems property.
     * @return Indicates success of the operation
     */
    ::OcaLiteStatus GetSupportedDiscoverySystems(::OcaLiteList< ::OcaLiteDiscoverySystemAES67>& supportedDiscoverySystems) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    // ---- Miscellaneous methods ----

    virtual ::OcaBoolean Initialize();

    virtual void Teardown();

    // ---- Interface for throwing events ----
    virtual void OnTxStreamCreated(UINT16 streamId,
                                   bool multicast,
                                   UINT8 encoding,
                                   UINT32 sampleRate,
                                   UINT16 nrChannels,
                                   const UINT16* channels,
                                   UINT16 sdpVersion,
                                   const std::string& userName,
                                   UINT64 sessionID,
                                   UINT64 sessionVersion,
                                   const std::string& originAddress,
                                   const std::string& sessionName,
                                   const std::string& destinationAddress,
                                   UINT16 destinationPort,
                                   UINT8 timeToLive,
                                   const std::string& mediaLabel,
                                   UINT64 packetTime,
                                   UINT32 offset,
                                   UINT8 payloadType);

    virtual void OnRxStreamCreated(UINT16 streamId,
                                   bool multicast,
                                   UINT8 encoding,
                                   UINT32 sampleRate,
                                   UINT16 nrChannels,
                                   const UINT16* channels,
                                   UINT16 sdpVersion,
                                   const std::string& userName,
                                   UINT64 sessionID,
                                   UINT64 sessionVersion,
                                   const std::string& originAddress,
                                   const std::string& sessionName,
                                   const std::string& destinationAddress,
                                   UINT16 destinationPort,
                                   const std::string& mediaLabel,
                                   UINT64 packetTime,
                                   UINT32 offset,
                                   UINT64 linkOffset,
                                   UINT8 payloadType);

    virtual void OnTxStreamModified(UINT16 streamId,
                                    UINT16 nrChannels,
                                    const UINT16* channels);

    virtual void OnRxStreamModified(UINT16 streamId,
                                    UINT16 nrChannels,
                                    const UINT16* channels);

    virtual void OnTxStreamDeleted(UINT16 streamId);

    virtual void OnRxStreamDeleted(UINT16 streamId);

protected:
    virtual const ::OcaLiteClassID& GetClassID() const;

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    virtual ::OcaLiteStatus SetPortNameValue(const ::OcaLitePortID& id, const ::OcaLiteString& name);

    virtual ::OcaLiteStatus GetServiceIDValue(::OcaLiteApplicationNetworkServiceID& name) const;

    virtual ::OcaLiteStatus GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces) const;

    virtual ::OcaLiteStatus AddSourceConnectorValue(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSourceConnector& connector);

    virtual ::OcaLiteStatus AddSinkConnectorValue(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSinkConnector& connector);

    virtual ::OcaLiteStatus SetSourceConnectorPinMapValue(::OcaLiteMediaSourceConnector& connector,
                                                      const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                      ::OcaLiteMediaConnectorStatus& status);

    virtual ::OcaLiteStatus SetSinkConnectorPinMapValue(::OcaLiteMediaSinkConnector& connector,
                                                    const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                    ::OcaLiteMediaConnectorStatus& status);

    virtual ::OcaLiteStatus DeleteSourceConnectorValue(const ::OcaLiteMediaSourceConnector& connector);

    virtual ::OcaLiteStatus DeleteSinkConnectorValue(const ::OcaLiteMediaSinkConnector& connector);

private:
    /**
     * Creates the ports for the channels.
     *
     * @param[in]   isSource        Indicates if the ports to create are for the source channels (<b>true</b>)
     *                              or the sink channels (<b>false</b>).
     * @param[in]   nrOfChannels    The number of channels to create the ports for.
     */
    void CreatePorts(bool isSource, UINT16 nrOfChannels);

    /**
     * Adds an existing transmit stream to the administration.
     *
     * @param[in]   streamId                The host interface ID of the stream.
     * @param[in]   multicast               True if the stream is multicast, false if not.
     * @param[in]   encoding                The encoding used.
     * @param[in]   sampleRate              The sample rate (Hz) used.
     * @param[in]   nrChannels              The number of entries in the channels parameter.
     * @param[in]   channels                Pointer to the channels (zero-based) in the stream.
     *                                      HI_AES67_INVALID_CHANNEL is used to indicate an empty slot in the stream.
     * @param[in]   sdpVersion              Contains the SDP version used in the SDP description of the stream.
     * @param[in]   userName                Contain the user's login on the source device.
     * @param[in]   sessionID               Contains the session identifier.
     * @param[in]   sessionVersion          Contains the version number of the session.
     * @param[in]   originAddress           Contains the IPv4 address the source device uses as source address of
     *                                      the stream.
     * @param[in]   sessionName             The session name of the stream.
     * @param[in]   destinationAddress      Contains the IPv4 address of the receiver device.
     * @param[in]   destinationPort         Contains the IP port of the receiver device.
     * @param[in]   timeToLive              Indicates the TTL value of a multicast stream. For unicast streams this is not relevant
     *                                      and the value can be ignored.
     * @param[in]   mediaLabel              May contain the optional medial label of the stream.
     * @param[in]   packetTime              Contains the desired packet time of the stream in microseconds.
     * @param[in]   offset                  Indicates the relationship of the media clock to the RTP clock.
     * @param[in]   payloadType             The RTP payload type.
     */
    void AddTransmitStream(UINT16 streamId,
                           bool multicast,
                           UINT8 encoding,
                           UINT32 sampleRate,
                           UINT16 nrChannels,
                           const UINT16* channels,
                           UINT16 sdpVersion,
                           const std::string& userName,
                           UINT64 sessionID,
                           UINT64 sessionVersion,
                           const std::string& originAddress,
                           const std::string& sessionName,
                           const std::string& destinationAddress,
                           UINT16 destinationPort,
                           UINT8 timeToLive,
                           const std::string& mediaLabel,
                           UINT64 packetTime,
                           UINT32 offset,
                           UINT8 payloadType);

    /**
     * Adds an existing receive stream to the administration.
     *
     * @param[in]   streamId                The host interface ID of the stream.
     * @param[in]   multicast               True if the stream is multicast, false if not.
     * @param[in]   encoding                The encoding used.
     * @param[in]   sampleRate              The sample rate (Hz) used.
     * @param[in]   nrChannels              The value indicates the size of the passed array.
     * @param[in]   channels                Pointer to the channels (zero-based) in the stream.
     *                                      The first entry is the number of channels in the first
     *                                      stream slot (may be 0) followed by the indicated channels.
     *                                      After this comes the number of channels in the second stream
     *                                      slot followed by indicated channels, and so on.
     * @param[in]   sdpVersion              Contains the SDP version used in the SDP description of the stream.
     * @param[in]   userName                Contain the user's login on the source device.
     * @param[in]   sessionID               Contains the session identifier.
     * @param[in]   sessionVersion          Contains the version number of the session.
     * @param[in]   originAddress           Contains the IPv4 address the source device uses as source address of
     *                                      the stream.
     * @param[in]   sessionName             The session name of the stream.
     * @param[in]   destinationAddress      Contains the IPv4 address of the receiver device.
     * @param[in]   destinationPort         Contains the IP port of the receiver device.
     * @param[in]   mediaLabel              May contain the optional medial label of the stream.
     * @param[in]   packetTime              Contains the desired packet time of the stream in microseconds.
     * @param[in]   offset                  Indicates the relationship of the media clock to the RTP clock.
     * @param[in]   linkOffset              Indicates the link offset at the receive in microseconds.
     * @param[in]   payloadType             The RTP payload type.
     */
    void AddReceiveStream(UINT16 streamId,
                          bool multicast,
                          UINT8 encoding,
                          UINT32 sampleRate,
                          UINT16 nrChannels,
                          const UINT16* channels,
                          UINT16 sdpVersion,
                          const std::string& userName,
                          UINT64 sessionID,
                          UINT64 sessionVersion,
                          const std::string& originAddress,
                          const std::string& sessionName,
                          const std::string& destinationAddress,
                          UINT16 destinationPort,
                          const std::string& mediaLabel,
                          UINT64 packetTime,
                          UINT32 offset,
                          UINT64 linkOffset,
                          UINT8 payloadType);

    /**
     * Converts a channel array to a channel pin map.
     *
     * @param[in]   nrChannels          The number of slots in the stream which equals the number of channels in the channels parameter.
     * @param[in]   channels            Pointer to the channels (zero-based) in the stream. HI_AES67_INVALID_CHANNEL is used to indicate
     *                                  an empty slot in the stream.
     * @param[out]  channelPinMap       The converted channel pin map.
     */
    static void ConvertToPinMap(UINT16 nrChannels,
                                const UINT16* channels,
                                ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap);

    /**
     * Converts a channel pin map to a channel array.
     *
     * @param[in]       channelPinMap       The channel pin map.
     * @param[in]       nrChannels          The number of slots in the stream which equals the number of channels in the channels parameter.
     * @param[in,out]   channels            Pointer to the channels (zero-based) in the stream.
     *                                      Use HI_AES67_INVALID_CHANNEL to indicate an empty slot in the stream.
     */
    static void ConvertFromPinMap(const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                  UINT16 nrChannels,
                                  UINT16* channels);

    /**
     * Converts a channel array to a channel pin map.
     *
     * @param[in]   nrChannels          The number of entries in the channels parameter.
     * @param[in]   channels            Pointer to the channels (zero-based) in the stream.
     *                                  The first entry is the number of channels in the first
     *                                  stream slot (may be 0) followed by the indicated channels.
     *                                  After this comes the number of channels in the second stream
     *                                  slot followed by indicated channels, and so on.
     * @param[out]  nrStreamSlots       The number of slots in the stream.
     * @param[out]  channelPinMap       The converted channel pin map.
     */
    static void ConvertToPinMap(UINT16 nrChannels,
                                const UINT16* channels,
                                ::OcaUint16& nrStreamSlots,
                                ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap);

    /**
     * Converts a channel pin map to a channel array.
     *
     * @param[in]       channelPinMap       The channel pin map.
     * @param[in]       nrStreamSlots       The number of slots in the stream.
     * @param[in,out]   nrChannels          The number of entries in the channels parameter. The output
     *                                      value will indicate the number of entries filled in the
     *                                      channels parameter.
     * @param[in,out]   channels            Pointer to the channels (zero-based) in the pin map.
     *                                      The first entry is the number of channels in the first
     *                                      stream slot (may be 0) followed by the indicated channels.
     *                                      After this comes the number of channels in the second stream
     *                                      slot followed by indicated channels, and so on.
     */
    static void ConvertFromPinMap(const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                  ::OcaUint16 nrStreamSlots,
                                  UINT16& nrChannels,
                                  UINT16* channels);

    /**
     * Verifies the coding parameters for a connector and returns the verified values that can
     * be passed to the Host Interface.
     *
     * @param[in]   isSource        Indicates if the ports to create are for the source channels (<b>true</b>)
     *                              or the sink channels (<b>false</b>).
     * @param[in]   coding          The coding to verify.
     * @param[out]  encoding        The selected encoding.
     * @param[out]  sampleRate      The selected sample rate.
     * @return Indicates success of the operation.
     */
    ::OcaLiteStatus DetermineVerifiedEncodingAndSampleRate(bool isSource,
                                                           const ::OcaLiteMediaCoding& coding,
                                                           UINT8& encoding,
                                                           UINT32& sampleRate);

    /**
     * Determines the codec parameters based on the given data.
     *
     * @param[in]   encoding        The encoding.
     * @param[out]  codecParameters The codec parameters.
     * @return Indicates success of the operation.
     */
    static ::OcaLiteStatus DetermineCodecParameters(UINT8 encoding,
                                                   ::OcaLiteString& codecParameters);


    /** private copy constructor, no copying of object allowed */
    OcaLiteMediaTransportNetworkAes67(const ::OcaLiteMediaTransportNetworkAes67&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteMediaTransportNetworkAes67& operator=(const ::OcaLiteMediaTransportNetworkAes67&);
};

#endif // OCALITEMEDIATRANSPORTNETWORKAES67_H
