/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The AES67 configuration interface of the Host Interface.
 *
 */

#ifndef AES67LITEHOSTINTERFACE_H
#define AES67LITEHOSTINTERFACE_H 

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/** Definition of an invalid channel */
#define AES67_INVALID_CHANNEL 0xFFFF
/** Definition of PCM-16 encoding */
#define AES67_ENCODING_LPCM16 16
/** Definition of PCM-24 encoding */
#define AES67_ENCODING_LPCM24 24

/** Definition of bonjour discovery protocol */
#define AES67_DISCOVERYPROTOCOL_BONJOUR 0x1
/** Definition of sap discovery protocol */
#define AES67_DISCOVERYPROTOCOL_SAP 0x2
/** Definition of axia discovery protocol */
#define AES67_DISCOVERYPROTOCOL_AXIA_DISCOVERY_PROTOCOL 0x4
/** Definition of wheatstone wheatnet IP discovery protocol */
#define AES67_DISCOVERYPROTOCOL_WHEATSTONE_WHEATNET_IP_DISCOVERY_PROTOCOL 0x8

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Initialize the AES67 hostinterface object by creating
 * a connection to the AES67 daemons.
 *
 * @return Indicates whether successfully a connection has been setup 
 *         to the AES67 daemons
 */
bool AES67LiteHostInterfaceInitialize(void);

/**
 * Get the number of receive and transmit channels
 * available in this device 
 *
 * @param[out] nrRxChannels The number of receive channels
 * @param[out] nrTxChannels The number of transmit channels
 *
 * @return The result of the operation
 */
bool AES67LiteHostInterfaceGetNumberOfChannels(UINT16& nrRxChannels, UINT16& nrTxChannels);

/**
 * Get the max number of stream / flows / decoders/encoders available in this device.
 *
 * @param[out] nrRxStreams  The number of receive streams available.
 * @param[out] nrTxStreams  The number of transmit streama available.
 *
 * @return The result of the operation
 */
bool AES67LiteHostInterfaceGetNumberOfStreams(UINT16& nrRxStreams, UINT16& nrTxStreams);

/**
 * Get the maximum number of slots available in a stream. And get the maximum number of channels available in a receive slot.
 *
 * @param[out] maxStreamSlots       The maximum number of slots in a flow.
 * @param[out] maxChannelsRxSlot    The maximum number of receive channels which can be pointed to from a single slot.
 *
 * @return The result of the operation
 */
bool AES67LiteHostInterfaceGetMaximumStreamSlots(UINT16& maxStreamSlots, UINT16& maxChannelsRxSlot);

/**
 * Get a list with the available send packet times.
 * 
 * @param[out] packetTime  The available send packet times (in ms).
 *
 * @return The result of the operation
 */
bool AES67LiteHostInterfaceGetSendPacketTimes(std::vector<UINT64>& packetTimes);

/**
 * Get a list with the available receive packet times.
 *
 * @param[out] packetTime  The available receive packet times (in ms).
 *
 * @return The result of the operation
 */
bool AES67LiteHostInterfaceGetReceivePacketTimes(std::vector<UINT64>& packetTimes);

/**
 * Get the minimum and maximum receive capacity
 * 
 * @param[out] minCapacity  The minimum receive capacity.
 * @param[out] maxCapacity  The maximum receive capacity.
 *
 * @return The result of the operation
 */
bool AES67LiteHostInterfaceGetReceiveBufferCapacity(UINT64& minCapacity, UINT64& maxCapacity);

/**
 * Get the transmission time variation.
 *
 * @param[out] variation    The variation in transmission time.
 * 
 * @return The result of the operation.
 */
bool AES67LiteHostInterfaceGetTransmissionTimeVariation(UINT32& variation);

/**
 * Get the AES67 channel name
 *
 * @param[in] channel       The channel number
 * @param[in] bRxChannel    True for a receive channel, false for a transmit channel.
 *
 * @return The channel name
 */
const char* AES67LiteHostInterfaceGetAES67ChannelName(UINT16 channel, bool bRxChannel);

/**
 * Get the current AES67 device name
 *
 * @return The device name
 */
const char* AES67LiteHostInterfaceGetDeviceName(void);

/**
 * Get the sampe rate
 *
 * @param[in] channel       The channel number
 * @param[in] bRxChannel    True for a receive channel, false for a transmit channel.
 *
 * @return The sample rate
 */
UINT32 AES67LiteHostInterfaceGetSampleRate(UINT16 channel, bool bRxChannel);

/**
 * Get the encodings
 *
 * @param[in] channel       The channel number
 * @param[in] bRxChannel    True for a receive channel, false for a transmit channel.
 *
 * @return 0x1 -> PCM16, 0x2 -> PCM24, 0x4 -> PCM32.
 */
UINT32 AES67LiteHostInterfaceGetEncodings(UINT16 channel, bool bRxChannel);

/**
 * Get the supported discovery protocols
 *
 * @return Bitflag containt the supported protocols. See AES67_DISCOVERYPROTOCOL_*
 */
UINT32 AES67LiteHostInterfaceGetSupportedDiscoveryProtocols();

/**
 * Get a list of active rx streams.
 * 
 * @param[out] rxStreamIds  The list with active rx streams.
 *
 * @return The result of the operation.
 */
bool AES67LiteHostInterfaceGetRxStreamIds(std::vector<UINT16>& rxStreamIds);

/**
 * Get the parameters of an active rx stream
 *
 * @param[out] streamId              The none-zero hardware identifier.
 * @param[out] multicast             True iff multicast.
 * @param[out] encoding              The number of bits in the encoding
 * @param[out] sampleRate            The sample rate
 * @param[out] nrChannels            The number of channels in the channels array.
 * @param[out] channels              The channels, with <number of channels> channels[<number of channels>],<number of channels> channels[<number of channels>],etc.
 * @param[out] sdpVersion            The sdp version
 * @param[out] userName              The user name
 * @param[out] sessionID             The session ID.
 * @param[out] sessionVersion        The session version
 * @param[out] originAddress         The origin address of the sream
 * @param[out] sessionName           The session name
 * @param[out] destinationAddress    The destination address of the stream
 * @param[out] destinationPort       The destination port
 * @param[out] mediaLabel            The media label
 * @param[out] packetTime            The packet time.
 * @param[out] offset                The offset.
 * @param[out] linkOffset            The receiver latency.
 * @param[out] payloadType           The payload type.
 *
 * @retun The result.
 */
bool AES67LiteHostInterfaceGetRxStream(UINT16 streamId, bool& multicast, UINT8& encoding, UINT32& sampleRate, UINT16& nrChannels, UINT16* channels, UINT16& sdpVersion, std::string& userName, 
                                       UINT64& sessionID, UINT64& sessionVersion, std::string& originAddress, std::string& sessionName, std::string& destinationAddress, UINT16& destinationPort, 
                                       std::string& mediaLabel, UINT64& packetTime, UINT32& offset, UINT64& linkOffset, UINT8& payloadType);

/**
 * Stop the sink stream 
 *
 * @param[in] streamId      The none-zero stream ID.
 * @param[in] channelCount  The number of channels.
 * @param[in] channels      Array containing the zero-based channels in the stream. 0xFFFF for an emty slot.
 *
 * @return The result of the operation.
 */
bool AES67LiteHostInterfaceStopRxStream(UINT16 streamId, UINT16 channelCount, UINT16* channels);

/**
 * Setup a sink (network receive) stream. On input the parameters hold the requested values, values can be modified if something is not available from hardware.
 *
 * @param[in]     multicast             True iff multicast.
 * @param[in]     encoding              The number of bits in the encoding
 * @param[in]     sampleRate            The sample rate
 * @param[in]     nrStreamSlots         The number of slots in the stream
 * @param[in/out] nrChannels            The number of channels in the channels array.
 * @param[in/out] channels              The channels, with <number of channels> channels[<number of channels>],<number of channels> channels[<number of channels>],etc.
 * @param[out]    streamId              The none-zero hardware identifier.
 * @param[in/out] sdpVersion            The sdp version
 * @param[in/out] userName              The user name
 * @param[in/out] sessionID             The session ID.
 * @param[in/out] sessionVersion        The session version
 * @param[in/out] originAddress         The origin address of the sream
 * @param[in/out] sessionName           The session name
 * @param[in/out] destinationAddress    The destination address of the stream
 * @param[in/out] destinationPort       The destination port
 * @param[in/out] timeToLive            The time to live
 * @param[in/out] mediaLabel            The media label
 * @param[in/out] packetTime            The packet time.
 * @param[in/out] offset                The offset.
 * @param[in/out] payloadType           The payload type.
 *
 * @retun The result.
 */
bool AES67LiteHostInterfaceSetupRxStream(bool multicast, UINT8 encoding, UINT32 sampleRate, UINT16 nrStreamSlots, UINT16& nrChannels, UINT16* channels, UINT16& streamId, 
                                         UINT16& sdpVersion, std::string& userName, UINT64& sessionID, UINT64& sessionVersion, std::string& originAddress, 
                                         std::string& sessionName, std::string& destinationAddress, UINT16& destinationPort, UINT8& timeToLive, const std::string& mediaLabel, 
                                         UINT64& packetTime, UINT32& offset, UINT8& payloadType);

/**
 * Setup a source (network transmit) stream. On input the parameters hold the requested values, values can be modified if something is not available from hardware.
 *
 * @param[in]     multicast             True iff multicast.
 * @param[in]     encoding              The number of bits in the encoding
 * @param[in]     sampleRate            The sample rate
 * @param[in]     nrStreamSlots         The number of slots in the stream
 * @param[in/out] nrChannels            The number of channels in the channels array.
 * @param[in/out] channels              The channels, with <number of channels> channels[<number of channels>],<number of channels> channels[<number of channels>],etc.
 * @param[out]    streamId              The none-zero hardware identifier.
 * @param[in/out] sdpVersion            The sdp version
 * @param[in/out] userName              The user name
 * @param[in/out] sessionID             The session ID.
 * @param[in/out] sessionVersion        The session version
 * @param[in/out] originAddress         The origin address of the sream
 * @param[in/out] sessionName           The session name
 * @param[in/out] destinationAddress    The destination address of the stream
 * @param[in/out] destinationPort       The destination port
 * @param[in/out] timeToLive            The time to live
 * @param[in/out] mediaLabel            The media label
 * @param[in/out] packetTime            The packet time.
 * @param[in/out] offset                The offset.
 * @param[in/out] payloadType           The payload type.
 *
 * @retun The result.
 */
bool AES67LiteHostInterfaceSetupTxStream(bool multicast, UINT8 encoding, UINT32 sampleRate, UINT16& nrChannels, UINT16* channels, UINT16& streamId, UINT16& sdpVersion, 
                                         std::string& userName, UINT64& sessionID, UINT64& sessionVersion, std::string& originAddress, std::string& sessionName, 
                                         std::string& destinationAddress, UINT16& destinationPort, UINT8& timeToLive, std::string& mediaLabel, UINT64& packetTime, 
                                         UINT32& offset, UINT8& payloadType);

/**
 * Modify the rx stream.
 *
 * @param[in]     multicast             True iff multicast.
 * @param[in]     encoding              The number of bits in the encoding
 * @param[in]     sampleRate            The sample rate
 * @param[in]     nrStreamSlots         The number of slots in the stream
 * @param[out]    streamId              The none-zero hardware identifier.
 * @param[in/out] sdpVersion            The sdp version
 * @param[in/out] userName              The user name
 * @param[in/out] sessionID             The session ID.
 * @param[in/out] sessionVersion        The session version
 * @param[in/out] originAddress         The origin address of the sream
 * @param[in/out] sessionName           The session name
 * @param[in/out] destinationAddress    The destination address of the stream
 * @param[in/out] destinationPort       The destination port
 * @param[in/out] timeToLive            The time to live
 * @param[in/out] mediaLabel            The media label
 * @param[in/out] packetTime            The packet time.
 * @param[in/out] offset                The offset.
 * @param[in/out] payloadType           The payload type.
 * @param[in/out] nrChannelsOld         The number of channels in the old channels array.
 * @param[in/out] channelsOld           The old channels, with <number of channels> channels[<number of channels>],<number of channels> channels[<number of channels>],etc.
 * @param[in/out] nrChannels            The number of channels in the new channels array.
 * @param[in/out] channels              The new channels, with <number of channels> channels[<number of channels>],<number of channels> channels[<number of channels>],etc.
 *
 * @retun The result.
 */
bool AES67LiteHostInterfaceModifyRxStreamChannels(UINT16 streamId, bool multicast, UINT8 encoding, UINT32 sampleRate, UINT16 nrStreamSlots, UINT16 sdpVersion, 
                                                  const std::string& userName, UINT64 sessionID, UINT64 sessionVersion, const std::string& originAddress, 
                                                  const std::string& sessionName, const std::string& destinationAddress, UINT16 destinationPort, UINT8 timeToLive,
                                                  const std::string& mediaLabel, UINT64 packetTime, UINT32 offset, UINT8 payloadType, UINT16 nrChannelsOld,
                                                  UINT16* channelsOld, UINT16& nrChannels, UINT16* channels);

/**
 * Get a list of active tx streams.
 * 
 * @param[out] txStreamIds  The list with active tx streams.
 *
 * @return The result of the operation.
 */
bool AES67LiteHostInterfaceGetTxStreamIds(std::vector<UINT16>& txStreamIds);

/**
 * Stop the transmit stream
 *
 * @param[in] streamId      The none-zero stream ID.
 * @param[in] channelCount  The number of channels.
 * @param[in] channels      Array containing the zero-based channels in the stream. 0xFFFF for an emty slot.
 *
 * @return The result of the operation.
 */
bool AES67LiteHostInterfaceStopTxStream(UINT16 streamId, UINT16 channelCount, UINT16* channels);

/**
 * Get the active source stream.
 *
 * @param[in]  streamId             The index of the stream in the device
 * @param[out] multicast            True iff multicast
 * @param[out] encoding             The encoding
 * @param[out] sampleRate           The sample rate
 * @param[out] nrChannels           The number of channels
 * @param[out] channels             List with channels, 0xFFFF for an empty channel
 * @param[out] sdpVersion           The sdp version.
 * @param[out] userName             The user name.
 * @param[out] sessionID            The session ID.
 * @param[out] sessionVersion       The session version
 * @param[out] originAddress        The origin address.
 * @param[out] sessionName          The session name.
 * @param[out] destinationAddress   The destination IP address.
 * @param[out] destinationPort      The destination port.
 * @param[out] timeToLive           The time to live.
 * @param[out] mediaLabel           The media label.
 * @param[out] packetTime           The packet time.
 * @param[out] offset               The media clock offset.
 * @param[out] payloadType          The payload type.
 *

 * @return The result of the operation.
 */
bool AES67LiteHostInterfaceGetTxStream(UINT16 streamId, bool& multicast, UINT8& encoding, UINT32& sampleRate, UINT16& nrChannels, UINT16* channels, 
                                       UINT16& sdpVersion, std::string& userName, UINT64& sessionID, UINT64& sessionVersion, std::string& originAddress,
                                       std::string& sessionName, std::string& destinationAddress, UINT16& destinationPort, UINT8& timeToLive, 
                                       std::string& mediaLabel, UINT64& packetTime, UINT32& offset, UINT8& payloadType);
/*
 * Create a tx stream
 *
 * @param[in]       multicast               True if the stream must be multicast, false if not.
 * @param[in]       encoding                The encoding to use.
 * @param[in]       sampleRate              The sample rate(Hz) to use.
 * @param[in, out]  nrChannels              The number of slots in the stream which equals the number of channels in the channels parameter.
 * @param[in, out]  channels                Pointer to the channels(zero - based) in the stream.
 *                                          Use 0xFFFF to indicate an empty slot in the stream.<br / >
 *                                          This list may be updated by the underlying implementation to indicate
 *                                          the actual assignment in the stream.If the list is changed, the
 *                                          output value of nrChannels will reflect the number of channels encoded
 *                                          in this output array.
 * @param[out]      streamId                The host interface ID of the created stream.
 * @param[in, out]  sdpVersion              Contains the SDP version used in the SDP description of the stream.
 * @param[in, out]  userName                May contain the user's login on the source device. If the source
 *                                          device does not support the concept of user IDs either and empty string
 *                                          or the one - character string '-' can be passed.
 * @param[in, out]  sessionID               Contains the session identifier.Passing 0 means that the source device
 *                                          must generate a unique session ID by itself.
 * @param[in, out]  sessionVersion          Contains the version number of the session.Passing 0 means that the source
 *                                          device must generate a version number by itself.
 * @param[in, out]  originAddress           Must contain the IPv4 address the source device uses as source address of
 *                                          the stream.Passing IPv4 address 0.0.0.0 means that the device should decide
 *                                          for itself what IPv4 address it uses as source address of the stream.
 * @param[in, out]  sessionName             The session name of the stream.
 * @param[in, out]  destinationAddress      Contains the IPv4 address of the receiver device.Passing IPv4 address 0.0.0.0
 *                                          means that the device should decide for itself what IPv4 address it uses as
 *                                          destination address of the stream.
 * @param[in, out]  destinationPort         Contains the suggested IP port of the receiver device.Note that the port is nothing
 *                                          more than a suggestion, the receiver device is leading and may decide to use a different
 *                                          port.Passing a port 0 means that there is no suggestion, i.e.it is completely left up
 *                                          to the receiver.
 * @param[in, out]  timeToLive              Indicates the TTL value of a multicast stream.For unicast streams this is not relevant
 *                                          and the value is ignored.
 * @param[in]       mediaLabel              May contain the optional medial label of the stream.An empty string may be passed.
 * @param[in, out]  packetTime              Contains the desired packet time of the stream in microseconds.
 * @param[out]      offset                  Indicates the relationship of the media clock to the RTP clock.This is an output
 *                                          parameter for the transmit stream.
 * @param[in, out] payloadType              The payload type.
 * @return True if the transmit stream is created, false if not.
 */
bool AES67LiteHostInterfaceCreateTxStream(bool multicast, UINT8 encoding, UINT32 sampleRate, UINT16& nrChannels, UINT16* channels, UINT16& streamId, UINT16& sdpVersion, 
                                          std::string& userName, UINT64& sessionID, UINT64& sessionVersion, std::string& originAddress, std::string& sessionName, 
                                          std::string& destinationAddress, UINT16& destinationPort, UINT8& timeToLive, const std::string& mediaLabel, UINT64& packetTime, 
                                          UINT32& offset, UINT8& payloadType);



/** 
 * @param[in] streamId                The host interface ID of the created stream.
 * @param[in] multicast               True if the stream must be multicast, false if not.
 * @param[in] encoding                The encoding to use.
 * @param[in] sampleRate              The sample rate(Hz) to use.
 * @param[in] sdpVersion              Contains the SDP version used in the SDP description of the stream.
 * @param[in] userName                May contain the user's login on the source device. If the source
 *                                    device does not support the concept of user IDs either and empty string
 *                                    or the one - character string '-' can be passed.
 * @param[in] sessionID               Contains the session identifier.Passing 0 means that the source device
 *                                    must generate a unique session ID by itself.
 * @param[in] sessionVersion          Contains the version number of the session.Passing 0 means that the source
 *                                    device must generate a version number by itself.
 * @param[in] originAddress           Must contain the IPv4 address the source device uses as source address of
 *                                    the stream.Passing IPv4 address 0.0.0.0 means that the device should decide
 *                                    for itself what IPv4 address it uses as source address of the stream.
 * @param[in] sessionName             The session name of the stream.
 * @param[in] destinationAddress      Contains the IPv4 address of the receiver device.Passing IPv4 address 0.0.0.0
 *                                    means that the device should decide for itself what IPv4 address it uses as
 *                                    destination address of the stream.
 * @param[in] destinationPort         Contains the suggested IP port of the receiver device.Note that the port is nothing
 *                                    more than a suggestion, the receiver device is leading and may decide to use a different
 *                                    port.Passing a port 0 means that there is no suggestion, i.e.it is completely left up
 *                                    to the receiver.
 * @param[in] timeToLive              Indicates the TTL value of a multicast stream.For unicast streams this is not relevant
 *                                    and the value is ignored.
 * @param[in] mediaLabel              May contain the optional medial label of the stream.An empty string may be passed.
 * @param[in] packetTime              Contains the desired packet time of the stream in microseconds.
 * @param[in] offset                  Indicates the relationship of the media clock to the RTP clock.This is an output
 *                                    parameter for the transmit stream.
 * @param[in] nrChannelsOld           The number of slots in the stream which equals the number of channels in the channels parameter.
 * @param[in] channelsOld             Pointer to the channels(zero - based) in the stream.
 *                                    Use 0xFFFF to indicate an empty slot in the stream.
 * @param[in, out]  nrChannels        The number of slots in the stream which equals the number of channels in the channels parameter.
 * @param[in, out]  channels          Pointer to the channels(zero - based) in the stream.
 *                                    Use 0xFFFF to indicate an empty slot in the stream.<br / >
 *                                    This list may be updated by the underlying implementation to indicate
 *                                    the actual assignment in the stream.If the list is changed, the
 *                                    output value of nrChannels will reflect the number of channels encoded
 *                                    in this output array.
 */
bool AES67LiteHostInterfaceModifyTxStreamChannels(UINT16 streamId, bool multicast, UINT8 encoding, UINT32 sampleRate, UINT16& sdpVersion, std::string& userName,
                                                  UINT64& sessionID, UINT64& sessionVersion, std::string& originAddress, std::string& sessionName, std::string& destinationAddress,
                                                  UINT16& destinationPort, UINT8& timeToLive, const std::string& mediaLabel, UINT64& packetTime, UINT32& offset,
                                                  UINT16 nrChannelsOld, UINT16* channelsOld, UINT16 nrChannels, UINT16* channels);

/**
 * Perofrm a run.
 */
void AES67LiteHostInterfaceRun();

#endif // AES67LITEHOSTINTERFACE_H
