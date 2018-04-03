/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaTransportNetworkAes67
 *
 */

// ---- Include system wide include files ----
#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <HostInterfaceLite/OCA/AES67/AES67LiteHostInterface.h>
#include <OCC/ControlClasses/Agents/OcaLiteMediaClock3.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCP.1/Ocp1LiteNetworkAddress.h>
#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>
#include <OCC/ControlDataTypes/OcaLiteMediaStreamParametersAes67.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>
#include <OCF/OcaLiteCommandHandler.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaTransportNetworkAes67.h"

// ---- Helper types and constants ----

static const ::OcaUint16         classID[]   = {OCA_MEDIA_TRANSPORT_NETWORK_AES67_CLASSID};
const ::OcaLiteClassID           OcaLiteMediaTransportNetworkAes67::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT                 0

/** The conversion factor from OcaTimeInterval to microseconds. */
#define OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR   static_cast<FLOAT>(1000000.0f)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaTransportNetworkAes67::OcaLiteMediaTransportNetworkAes67(::OcaONo objectNumber,
                                                                     ::OcaBoolean lockable, 
                                                                     const ::OcaLiteString& role,
                                                                     ::OcaDBfs defaultAlignmentLevel,
                                                                     ::OcaDBfs minAlignmentLevel,
                                                                     ::OcaDBfs maxAlignmentLevel,
                                                                     ::OcaDB defaultAlignmentGain,
                                                                     ::OcaDB minAlignmentGain,
                                                                     ::OcaDB maxAlignmentGain)
    : ::OcaLiteMediaTransportNetwork(objectNumber,
                                     lockable,
                                     role,
                                     defaultAlignmentLevel,
                                     minAlignmentLevel,
                                     maxAlignmentLevel,
                                     defaultAlignmentGain,
                                     minAlignmentGain,
                                     maxAlignmentGain)
{
}

OcaLiteMediaTransportNetworkAes67::~OcaLiteMediaTransportNetworkAes67()
{
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetMediaProtocol(::OcaLiteNetworkMediaProtocol& protocol) const
{
    protocol = OCANETWORKMEDIAPROTOCOL_AES67;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetMaxSourceConnectors(::OcaUint16& maxSourceConnectors) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT16 nrOfTxStreams(0);
    UINT16 nrOfRxStreams(0);
    if (AES67LiteHostInterfaceGetNumberOfStreams(nrOfRxStreams, nrOfTxStreams))
    {
        maxSourceConnectors = static_cast< ::OcaUint16>(nrOfTxStreams);
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the number of streams");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetMaxSinkConnectors(::OcaUint16& maxSinkConnectors) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT16 nrOfTxStreams(0);
    UINT16 nrOfRxStreams(0);
    if (AES67LiteHostInterfaceGetNumberOfStreams(nrOfRxStreams, nrOfTxStreams))
    {
        maxSinkConnectors = static_cast< ::OcaUint16>(nrOfRxStreams);
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the number of streams");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetMaxPinsPerConnector(::OcaUint16& maxPins) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT16 maxStreamSlots(0);
    UINT16 maxChannelsRxSlot(0);
    if (AES67LiteHostInterfaceGetMaximumStreamSlots(maxStreamSlots, maxChannelsRxSlot))
    {
        maxPins = static_cast< ::OcaUint16>(maxStreamSlots);
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the maximum number of pins per connector");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetMaxPortsPerPin(::OcaUint16& maxPorts) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT16 maxStreamSlots(0);
    UINT16 maxChannelsRxSlot(0);
    if (AES67LiteHostInterfaceGetMaximumStreamSlots(maxStreamSlots, maxChannelsRxSlot))
    {
        maxPorts = static_cast< ::OcaUint16>(maxChannelsRxSlot);
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the maximum number of ports per pin");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetSendPacketTimes(::OcaLiteList< ::OcaTimeInterval>& sendPacketTimes) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    sendPacketTimes.Clear();

    std::vector<UINT64> packetTimes;
    if (AES67LiteHostInterfaceGetSendPacketTimes(packetTimes))
    {
        for (std::vector<UINT64>::iterator it(packetTimes.begin()); it != packetTimes.end(); ++it)
        {
            sendPacketTimes.Add(static_cast< ::OcaTimeInterval>(*it / 1000000.0f));
        }
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the packet times");
    }
    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetReceivePacketTimes(::OcaLiteList< ::OcaTimeInterval>& receivePacketTimes) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    receivePacketTimes.Clear();

    std::vector<UINT64> packetTimes;
    if (AES67LiteHostInterfaceGetReceivePacketTimes(packetTimes))
    {
        for (std::vector<UINT64>::iterator it(packetTimes.begin()); it != packetTimes.end(); ++it)
        {
            receivePacketTimes.Add(static_cast< ::OcaTimeInterval>(*it / 1000000.0f));
        }
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the packet times");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetMinReceiveBufferCapacity(::OcaTimeInterval& minReceiveBufferCapacity) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT64 minCapacity(0);
    UINT64 maxCapacity(0);
    if (AES67LiteHostInterfaceGetReceiveBufferCapacity(minCapacity, maxCapacity))
    {
        minReceiveBufferCapacity = static_cast< ::OcaTimeInterval>(minCapacity / 1000000.0f);
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the receive buffer capacity");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetMaxReceiveBufferCapacity(::OcaTimeInterval& maxReceiveBufferCapacity) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT64 minCapacity(0);
    UINT64 maxCapacity(0);
    if (AES67LiteHostInterfaceGetReceiveBufferCapacity(minCapacity, maxCapacity))
    {
        maxReceiveBufferCapacity = static_cast< ::OcaTimeInterval>(maxCapacity / 1000000.0f);
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the receive buffer capacity");
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetTransmissionTimeVariation(::OcaTimeInterval& transmissionTimeVariation) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT32 variation(0);
    if (AES67LiteHostInterfaceGetTransmissionTimeVariation(variation))
    {
        transmissionTimeVariation = static_cast< ::OcaTimeInterval>(variation / 1.0e9f);
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR("Unable to retrieve the transmission time variation");
    }
    
    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetSupportedDiscoverySystems(::OcaLiteList< ::OcaLiteDiscoverySystemAES67>& supportedDiscoverySystems) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    supportedDiscoverySystems.Clear();

    UINT32 discoveryProtocol(AES67LiteHostInterfaceGetSupportedDiscoveryProtocols());
    
    if ((AES67_DISCOVERYPROTOCOL_BONJOUR & discoveryProtocol) == AES67_DISCOVERYPROTOCOL_BONJOUR)
    {
        supportedDiscoverySystems.Add(OCADISCOVERYSYSTEMAES67_BONJOUR);
    }
    if ((AES67_DISCOVERYPROTOCOL_SAP & discoveryProtocol) == AES67_DISCOVERYPROTOCOL_SAP)
    {
        supportedDiscoverySystems.Add(OCADISCOVERYSYSTEMAES67_SAP);
    }
    if ((AES67_DISCOVERYPROTOCOL_AXIA_DISCOVERY_PROTOCOL & discoveryProtocol) == AES67_DISCOVERYPROTOCOL_AXIA_DISCOVERY_PROTOCOL)
    {
        supportedDiscoverySystems.Add(OCADISCOVERYSYSTEMAES67_AXIADISCOVERYPROTOCOL);
    }
    if ((AES67_DISCOVERYPROTOCOL_WHEATSTONE_WHEATNET_IP_DISCOVERY_PROTOCOL & discoveryProtocol) == AES67_DISCOVERYPROTOCOL_WHEATSTONE_WHEATNET_IP_DISCOVERY_PROTOCOL)
    {
        supportedDiscoverySystems.Add(OCADISCOVERYSYSTEMAES67_WHEATSTONEWHEATNETIPDISCOVERYPROTOCOL);
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_SEND_PACKET_TIMES:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaTimeInterval> sendPacketTimes;
                        rc = GetSendPacketTimes(sendPacketTimes);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     sendPacketTimes.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                sendPacketTimes.Marshal(&pResponse, writer);

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
            case GET_RECEIVE_PACKET_TIMES:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaTimeInterval> receivePacketTimes;
                        rc = GetReceivePacketTimes(receivePacketTimes);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     receivePacketTimes.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                receivePacketTimes.Marshal(&pResponse, writer);

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
            case GET_MIN_RECEIVE_BUFFER_CAPACITY:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaTimeInterval minReceiveBufferCapacity;
                        rc = GetMinReceiveBufferCapacity(minReceiveBufferCapacity);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaTimeInterval>(minReceiveBufferCapacity, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                writer.Write(minReceiveBufferCapacity, &pResponse);

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
            case GET_MAX_RECEIVE_BUFFER_CAPACITY:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaTimeInterval maxReceiveBufferCapacity;
                        rc = GetMaxReceiveBufferCapacity(maxReceiveBufferCapacity);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaTimeInterval>(maxReceiveBufferCapacity, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                writer.Write(maxReceiveBufferCapacity, &pResponse);

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
            case GET_TRANSMISSION_TIME_VARIATION:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaTimeInterval transmissionTimeVariation;
                        rc = GetTransmissionTimeVariation(transmissionTimeVariation);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaTimeInterval>(transmissionTimeVariation, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                writer.Write(transmissionTimeVariation, &pResponse);

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
            case GET_SUPPORTED_DISCOVERY_SYSTEMS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteDiscoverySystemAES67> supportedDiscoverySystems;
                        rc = GetSupportedDiscoverySystems(supportedDiscoverySystems);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     supportedDiscoverySystems.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                supportedDiscoverySystems.Marshal(&pResponse, writer);

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
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteMediaTransportNetwork::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaBoolean OcaLiteMediaTransportNetworkAes67::Initialize()
{
    bool success(true);

    UINT16 nrOfTxStreams(0);
    UINT16 nrOfRxStreams(0);
    UINT16 maxStreamSlots(0);
    UINT16 maxChannelsRxSlot(0);

    // Create all the ports and reserve all the connectors
    UINT16 nrOfTxChannels(0);
    UINT16 nrOfRxChannels(0);
    if (AES67LiteHostInterfaceGetNumberOfChannels(nrOfRxChannels, nrOfTxChannels))
    {
        CreatePorts(true/*isSource*/, nrOfTxChannels);
        CreatePorts(false/*isSource*/, nrOfRxChannels);
    }
    else
    {
        OCA_LOG_WARNING("Unable to retrieve the number of channels");
        success = false;
    }

    if (AES67LiteHostInterfaceGetNumberOfStreams(nrOfRxStreams, nrOfTxStreams))
    {
        if (AES67LiteHostInterfaceGetMaximumStreamSlots(maxStreamSlots, maxChannelsRxSlot))
        {
            ReserveConnectors(static_cast< ::OcaUint16>(nrOfTxStreams),
                              static_cast< ::OcaUint16>(nrOfRxStreams),
                              static_cast< ::OcaUint16>(maxStreamSlots),
                              static_cast< ::OcaUint16>(maxChannelsRxSlot));
        }
        else
        {
            OCA_LOG_WARNING("Unable to retrieve the maximum number of slots and channels per slot");
            success = false;
        }
    }
    else
    {
        OCA_LOG_WARNING("Unable to retrieve the number of streams");
        success = false;
    }

    std::vector<UINT16> txStreamIds;
    if (AES67LiteHostInterfaceGetTxStreamIds(txStreamIds))
    {
        for (std::vector<UINT16>::iterator it(txStreamIds.begin());
                it != txStreamIds.end();
                ++it)
        {
            bool multicast;
            UINT8 encoding;
            UINT32 sampleRate;
            UINT16 nrChannels(maxStreamSlots);
            UINT16* channels(new UINT16[nrChannels]);
            UINT16 sdpVersion;
            std::string userName;
            UINT64 sessionID;
            UINT64 sessionVersion;
            std::string originAddress;
            std::string sessionName;
            std::string destinationAddress;
            UINT16 destinationPort;
            UINT8 timeToLive;
            std::string mediaLabel;
            UINT64 packetTime;
            UINT32 offset;
            UINT8 payloadType;
            if (AES67LiteHostInterfaceGetTxStream(*it, multicast, encoding, sampleRate, nrChannels,
                                                  channels, sdpVersion, userName, sessionID,
                                                  sessionVersion, originAddress, sessionName, destinationAddress,
                                                  destinationPort, timeToLive, mediaLabel, packetTime, offset, payloadType))
            {
                AddTransmitStream(*it, multicast, encoding, sampleRate, nrChannels,
                                  channels, sdpVersion, userName, sessionID,
                                  sessionVersion, originAddress, sessionName, destinationAddress,
                                  destinationPort, timeToLive, mediaLabel, packetTime, offset, payloadType);
            }
            else
            {
                OCA_LOG_ERROR_PARAMS("Failed to get tx stream with id %d", *it);
            }

            delete[] channels;
        }
    }
    else
    {
        OCA_LOG_ERROR("Failed to get active tx streams");
    }

    // Get all currently existing receive streams and add them to the administration
    std::vector<UINT16> rxStreamIds;
    if (AES67LiteHostInterfaceGetRxStreamIds(rxStreamIds))
    {
        for (std::vector<UINT16>::iterator it(rxStreamIds.begin());
                it != rxStreamIds.end();
                ++it)
        {
            bool multicast;
            UINT8 encoding;
            UINT32 sampleRate;
            UINT16 nrChannels(static_cast<UINT16>(maxStreamSlots * (1 + maxChannelsRxSlot)));    // Add 1 for the channel count per slot
            UINT16* channels(new UINT16[nrChannels]);
            UINT16 sdpVersion;
            std::string userName;
            UINT64 sessionID;
            UINT64 sessionVersion;
            std::string originAddress;
            std::string sessionName;
            std::string destinationAddress;
            UINT16 destinationPort;
            std::string mediaLabel;
            UINT64 packetTime;
            UINT32 offset;
            UINT64 linkOffset;
            UINT8 payloadType;
            if (AES67LiteHostInterfaceGetRxStream(*it, multicast, encoding, sampleRate, nrChannels,
                                                  channels, sdpVersion, userName, sessionID, sessionVersion,
                                                  originAddress, sessionName, destinationAddress, destinationPort,
                                                  mediaLabel, packetTime, offset, linkOffset, payloadType))
            {
                AddReceiveStream(*it, multicast, encoding, sampleRate, nrChannels, channels,
                                 sdpVersion, userName, sessionID, sessionVersion, originAddress,
                                 sessionName, destinationAddress, destinationPort, mediaLabel, packetTime,
                                 offset, linkOffset, payloadType);
            }
            else
            {
                OCA_LOG_ERROR_PARAMS("Failed to get rx stream with id %d", *it);
            }

            delete[] channels;
        }
    }
    else
    {
        OCA_LOG_ERROR("Failed to get active rx streams");
    }

    if (!success)
    {
        // Try to free the memory
        Teardown();
    }

    return static_cast< ::OcaBoolean>(success);
}

void OcaLiteMediaTransportNetworkAes67::Teardown()
{
    // Remove all the ports
    ::OcaLiteList< ::OcaLitePort> ports;
    static_cast<void>(InternalGetPorts(ports));
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < ports.GetCount(); i++)
    {
        const ::OcaLitePort& port(ports.GetItem(i));
        static_cast<void>(InternalDeletePort(port.GetID()));
    }
}

void OcaLiteMediaTransportNetworkAes67::OnTxStreamCreated(UINT16 streamId,
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
                                                          UINT8 payloadType)
{
    AddTransmitStream(streamId,
                      multicast,
                      encoding,
                      sampleRate,
                      nrChannels,
                      channels,
                      sdpVersion,
                      userName,
                      sessionID,
                      sessionVersion,
                      originAddress,
                      sessionName,
                      destinationAddress,
                      destinationPort,
                      timeToLive,
                      mediaLabel,
                      packetTime,
                      offset,
                      payloadType);
}

void OcaLiteMediaTransportNetworkAes67::OnRxStreamCreated(UINT16 streamId,
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
                                                          UINT8 payloadType)
{
    AddReceiveStream(streamId,
                     multicast,
                     encoding,
                     sampleRate,
                     nrChannels,
                     channels,
                     sdpVersion,
                     userName,
                     sessionID,
                     sessionVersion,
                     originAddress,
                     sessionName,
                     destinationAddress,
                     destinationPort,
                     mediaLabel,
                     packetTime,
                     offset,
                     linkOffset,
                     payloadType);
}

void OcaLiteMediaTransportNetworkAes67::OnTxStreamModified(UINT16 streamId,
                                                       UINT16 nrChannels,
                                                       const UINT16* channels)
{
    // Get the connector
    ::OcaLiteMediaSourceConnector connector;
    ::OcaLiteStatus rc(GetSourceConnectorValue(static_cast< ::OcaLiteMediaConnectorID>(streamId), connector));
    if (OCASTATUS_OK == rc)
    {
        // Create the channel pin map
        ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> channelPinMap;
        ConvertToPinMap(nrChannels, channels, channelPinMap);
        assert(ArePortsInChannelPinMapValid(channelPinMap));
        connector.SetPinCount(static_cast< ::OcaUint16>(nrChannels));

        // Update the channel pin map and store the connector
        connector.SetChannelPinMap(channelPinMap);
        rc = InternalUpdateSourceConnector(connector, OCAMEDIACONNECTORELEMENT_PINMAP);
        if (OCASTATUS_OK != rc)
        {
            OCA_LOG_ERROR_PARAMS("Unable to store updated source connector for stream %u (rc = %u)", streamId, rc);
        }
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Unable to retrieve source connector for stream %u (rc = %u)", streamId, rc);
    }
}

void OcaLiteMediaTransportNetworkAes67::OnRxStreamModified(UINT16 streamId,
                                                       UINT16 nrChannels,
                                                       const UINT16* channels)
{
    // Get the connector
    ::OcaLiteMediaSinkConnector connector;
    ::OcaLiteStatus rc(GetSinkConnectorValue(static_cast< ::OcaLiteMediaConnectorID>(streamId), connector));
    if (OCASTATUS_OK == rc)
    {
        // Create the channel pin map
        ::OcaUint16 nrStreamSlots;
        ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> channelPinMap;
        ConvertToPinMap(nrChannels, channels, nrStreamSlots, channelPinMap);
        assert(ArePortsInChannelPinMapValid(channelPinMap));
        connector.SetPinCount(nrStreamSlots);

        // Update the channel pin map and store the connector
        connector.SetChannelPinMap(channelPinMap);
        rc = InternalUpdateSinkConnector(connector, OCAMEDIACONNECTORELEMENT_PINMAP);
        if (OCASTATUS_OK != rc)
        {
            OCA_LOG_ERROR_PARAMS("Unable to store updated sink connector for stream %u (rc = %u)", streamId, rc);
        }
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Unable to retrieve sink connector for stream %u (rc = %u)", streamId, rc);
    }

}

void OcaLiteMediaTransportNetworkAes67::OnTxStreamDeleted(UINT16 streamId)
{
    ::OcaLiteStatus rc(InternalDeleteConnector(static_cast< ::OcaLiteMediaConnectorID>(streamId)));
    if (OCASTATUS_OK != rc)
    {
        OCA_LOG_ERROR_PARAMS("Unable to remove source connector for stream %u (rc = %u)", streamId, rc);
    }
}

void OcaLiteMediaTransportNetworkAes67::OnRxStreamDeleted(UINT16 streamId)
{
    ::OcaLiteStatus rc(InternalDeleteConnector(static_cast< ::OcaLiteMediaConnectorID>(streamId)));
    if (OCASTATUS_OK != rc)
    {
        OCA_LOG_ERROR_PARAMS("Unable to remove sink connector for stream %u (rc = %u)", streamId, rc);
    }
}

const ::OcaLiteClassID& OcaLiteMediaTransportNetworkAes67::GetClassID() const
{
    return CLASS_ID;
}

::OcaClassVersionNumber OcaLiteMediaTransportNetworkAes67::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteMediaTransportNetwork::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::SetPortNameValue(const ::OcaLitePortID& id, const ::OcaLiteString& name)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetServiceIDValue(::OcaLiteApplicationNetworkServiceID& name) const
{
    name = ::OcaLiteMediaTransportNetworkAes67ServiceID(::OcaLiteString(AES67LiteHostInterfaceGetDeviceName()));
 
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces) const
{
    // Always return an empty list as this property is not relevant for this network
    interfaces.Clear();
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::AddSourceConnectorValue(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSourceConnector& connector)
{
    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    // Check the stream cast mode
    if ((connector.GetConnection().GetStreamCastMode() == OCAMEDIASTREAMCASTMODE_UNICAST) ||
        (connector.GetConnection().GetStreamCastMode() == OCAMEDIASTREAMCASTMODE_MULTICAST))
    {
        // Check the initial state
        if (((connector.GetConnection().GetStreamCastMode() == OCAMEDIASTREAMCASTMODE_MULTICAST) &&
                (OCAMEDIACONNECTORSTATE_RUNNING == initialState)) ||
            ((connector.GetConnection().GetStreamCastMode() == OCAMEDIASTREAMCASTMODE_UNICAST) &&
                (OCAMEDIACONNECTORSTATE_PAUSED == initialState)))
        {
            // Create the correct stream parameters
            ::OcaLiteMediaStreamParametersAes67* parameters(::OcaLiteMediaStreamParametersAes67::CreateFromBase(connector.GetConnection().GetStreamParameters()));
            if (NULL != parameters)
            {
                // Determine the encoding and sample rate
                UINT8 encoding(static_cast< UINT8>(0));
                UINT32 sampleRate(0);
                rc = DetermineVerifiedEncodingAndSampleRate(true/*isSource*/, connector.GetCoding(), encoding, sampleRate);
                if (OCASTATUS_OK != rc)
                {
                    OCA_LOG_ERROR("Failed to verify encoding and/or sample rate");
                }

                if (OCASTATUS_OK == rc)
                {
                    // Check the fields in the stream parameters
                    if ((connector.GetConnection().GetStreamCastMode() != OCAMEDIASTREAMCASTMODE_MULTICAST) &&
                        parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().empty())
                    {
                        OCA_LOG_ERROR("Destination address must be set in case of a unicast connection");
                        rc = OCASTATUS_PARAMETER_ERROR;
                    }
                }

                if (OCASTATUS_OK == rc)
                {
                    // Convert the channel pin map
                    UINT16 nrChannels(static_cast<UINT16>(connector.GetPinCount()));
                    UINT16* channels(new UINT16[nrChannels]);
                    ConvertFromPinMap(connector.GetChannelPinMap(), nrChannels, channels);

                    UINT16 streamId;
                    UINT16 sdpVersion(static_cast<UINT16>(parameters->GetSdpVersion()));
                    std::string userName(parameters->GetUserName().GetString().data(), parameters->GetUserName().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    UINT64 sessionID(static_cast<UINT64>(parameters->GetSessionID()));
                    UINT64 sessionVersion(static_cast<UINT64>(parameters->GetSessionVersion()));
                    std::string originAddress(parameters->GetOriginAddress().GetDestHostOrIPAddress().GetString().data(), parameters->GetOriginAddress().GetDestHostOrIPAddress().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    std::string sessionName(parameters->GetSessionName().GetString().data(), parameters->GetSessionName().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    std::string destinationAddress(parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().data(), parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    UINT16 destinationPort((connector.GetConnection().GetStreamCastMode() != OCAMEDIASTREAMCASTMODE_UNICAST) ? static_cast<UINT16>(parameters->GetDestinationAddress().GetPort()) : 0);
                    UINT8 timeToLive(static_cast<UINT8>(parameters->GetTimeToLive()));
                    std::string mediaLabel(parameters->GetMediaLabel().GetString().data(), parameters->GetMediaLabel().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    UINT64 packetTime(static_cast<UINT64>(static_cast<FLOAT>(parameters->GetPacketTime()) * OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR));
                    UINT32 offset(static_cast<UINT32>(parameters->GetOffset()));
                    UINT8 payloadType(static_cast<UINT8>(parameters->GetPayloadType()));

                    if (AES67LiteHostInterfaceSetupTxStream(connector.GetConnection().GetStreamCastMode() == OCAMEDIASTREAMCASTMODE_MULTICAST,
                                                    encoding,
                                                    sampleRate,
                                                    nrChannels,
                                                    channels,
                                                    streamId,
                                                    sdpVersion,
                                                    userName,
                                                    sessionID,
                                                    sessionVersion,
                                                    originAddress,
                                                    sessionName,
                                                    destinationAddress,
                                                    destinationPort,
                                                    timeToLive,
                                                    mediaLabel,
                                                    packetTime,
                                                    offset, 
                                                    payloadType))
                    {
                        // Update the connection with the returned values
                        ::OcaLiteMediaStreamParametersAes67 newParameters(static_cast< ::OcaUint16>(sdpVersion),
                                                                          ::OcaLiteString(userName),
                                                                          static_cast< ::OcaUint64>(sessionID),
                                                                          static_cast< ::OcaUint64>(sessionVersion),
                                                                          ::Ocp1LiteNetworkAddress(::OcaLiteString(originAddress),
                                                                                              static_cast< ::OcaUint16>(0)),
                                                                          ::OcaLiteString(sessionName),
                                                                          ::Ocp1LiteNetworkAddress(::OcaLiteString(destinationAddress),
                                                                                              static_cast< ::OcaUint16>(destinationPort)),
                                                                          static_cast< ::OcaUint8>(timeToLive),
                                                                          ::OcaLiteString(mediaLabel),
                                                                          static_cast< ::OcaTimeInterval>(packetTime / OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR),
                                                                          static_cast< ::OcaUint32>(offset),
                                                                          static_cast< ::OcaTimeInterval>(0),
                                                                          static_cast< ::OcaUint8>(payloadType));
                        connector.SetIDInternal(static_cast< ::OcaLiteMediaConnectorID>(streamId));
                        ::OcaLiteMediaConnection newConnection(connector.GetConnection().GetSecure(),
                                                            newParameters,
                                                            connector.GetConnection().GetStreamCastMode());
                        connector.SetConnection(newConnection);

                        // Create a new channel pin map from the return values
                        ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> newChannelPinMap;
                        ConvertToPinMap(nrChannels, channels, newChannelPinMap);
                        connector.SetPinCount(static_cast< ::OcaUint16>(nrChannels));

                        // Check if the channel pin map has changed
                        if (connector.GetChannelPinMap() != newChannelPinMap)
                        {
                            connector.SetChannelPinMap(newChannelPinMap);
                            rc = OCASTATUS_PARTIALLY_SUCCEEDED;
                        }
                        else
                        {
                            rc = OCASTATUS_OK;
                        }
                    }
                    else
                    {
                        OCA_LOG_ERROR_PARAMS("Unable to create TX stream for connector %u", connector.GetIDInternal());
                        rc = OCASTATUS_DEVICE_ERROR;
                    }

                    delete[] channels;
                }
            }
            else
            {
                OCA_LOG_ERROR("The connection parameters are not a valid OcaLiteMediaStreamParametersAes67 object");
                rc = OCASTATUS_PARAMETER_ERROR;
            }

            delete parameters;
        }
        else
        {
            // Paused streams are not supported, so the initial state should be running
            OCA_LOG_ERROR_PARAMS("Incorrect initial state %u", initialState);
            rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
        }
    }
    else
    {
        OCA_LOG_ERROR("Stream cast mode must be unicast or multicast");
        rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::AddSinkConnectorValue(::OcaLiteMediaConnectorState initialState, ::OcaLiteMediaSinkConnector& connector)
{
    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    // Check the initial state
    if (OCAMEDIACONNECTORSTATE_RUNNING == initialState)
    {
        // Create the correct stream parameters
        ::OcaLiteMediaStreamParametersAes67* parameters(::OcaLiteMediaStreamParametersAes67::CreateFromBase(connector.GetConnection().GetStreamParameters()));
        if (NULL != parameters)
        {
            // Determine the encoding and sample rate
            UINT8 encoding(static_cast< UINT8>(0));
            UINT32 sampleRate(0);
            rc = DetermineVerifiedEncodingAndSampleRate(false/*isSource*/, connector.GetCoding(), encoding, sampleRate);

            // Check the stream cast mode
            if ((OCASTATUS_OK == rc) &&
                (connector.GetConnection().GetStreamCastMode() != OCAMEDIASTREAMCASTMODE_UNICAST) &&
                (connector.GetConnection().GetStreamCastMode() != OCAMEDIASTREAMCASTMODE_MULTICAST))
            {
                OCA_LOG_ERROR("Stream cast mode must be unicast or multicast");
                rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
            }

            if (OCASTATUS_OK == rc)
            {
                // Check the fields in the stream parameters
                if (parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().empty() ||
                    (static_cast< ::OcaUint16>(0) == parameters->GetDestinationAddress().GetPort()))
                {
                    OCA_LOG_ERROR("Destination address must be set");
                    rc = OCASTATUS_PARAMETER_ERROR;
                }
            }

            ::OcaUint16 maxPorts(static_cast< ::OcaUint16>(0));
            if (OCASTATUS_OK == rc)
            {
                rc = GetMaxPortsPerPin(maxPorts);
                if (OCASTATUS_OK != rc)
                {
                    OCA_LOG_ERROR_PARAMS("Unable to get maximum number of ports per pin (rc = %u)", rc);
                }
            }

            if (OCASTATUS_OK == rc)
            {
                // Convert the channel pin map
                UINT16 nrChannels(static_cast<UINT16>(connector.GetPinCount() * maxPorts));
                UINT16* channels(new UINT16[nrChannels]);
                ConvertFromPinMap(connector.GetChannelPinMap(), connector.GetPinCount(), nrChannels, channels);
                ::OcaLiteMediaStreamParametersAes67* parameters(::OcaLiteMediaStreamParametersAes67::CreateFromBase(connector.GetConnection().GetStreamParameters()));
                if (NULL != parameters)
                {
                    UINT16 streamId;
                    UINT16 sdpVersion(static_cast<UINT16>(parameters->GetSdpVersion()));
                    std::string userName(parameters->GetUserName().GetString().data(), parameters->GetUserName().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    UINT64 sessionID(static_cast<UINT64>(parameters->GetSessionID()));
                    UINT64 sessionVersion(static_cast<UINT64>(parameters->GetSessionVersion()));
                    std::string originAddress(parameters->GetOriginAddress().GetDestHostOrIPAddress().GetString().data(), parameters->GetOriginAddress().GetDestHostOrIPAddress().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    std::string sessionName(parameters->GetSessionName().GetString().data(), parameters->GetSessionName().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    std::string destinationAddress(parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().data(), parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    UINT16 destinationPort(static_cast<UINT16>(parameters->GetDestinationAddress().GetPort()));
                    UINT8 timeToLive(static_cast<UINT8>(parameters->GetTimeToLive()));
                    std::string mediaLabel(parameters->GetMediaLabel().GetString().data(), parameters->GetMediaLabel().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
                    UINT64 packetTime(static_cast<UINT64>(static_cast<FLOAT>(parameters->GetPacketTime()) * OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR));
                    UINT32 offset(static_cast<UINT32>(parameters->GetOffset()));
                    UINT64 linkOffset(static_cast<UINT64>(static_cast<FLOAT>(parameters->GetLinkOffset()) * OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR));
                    UINT8 payloadType(static_cast<UINT8>(parameters->GetPayloadType()));
                    if (AES67LiteHostInterfaceSetupRxStream(connector.GetConnection().GetStreamCastMode() == OCAMEDIASTREAMCASTMODE_MULTICAST,
                                                            encoding,
                                                            sampleRate,
                                                            static_cast<UINT16>(connector.GetPinCount()),
                                                            nrChannels,
                                                            channels,
                                                            streamId,
                                                            sdpVersion,
                                                            userName,
                                                            sessionID,
                                                            sessionVersion,
                                                            originAddress,
                                                            sessionName,
                                                            destinationAddress,
                                                            destinationPort,
                                                            timeToLive,
                                                            mediaLabel,
                                                            packetTime,
                                                            offset,
                                                            payloadType))
                    {
                        // Update the connection with the returned values
                        ::OcaLiteMediaStreamParametersAes67 newParameters(static_cast< ::OcaUint16>(sdpVersion),
                                                                          ::OcaLiteString(userName),
                                                                          static_cast< ::OcaUint64>(sessionID),
                                                                          static_cast< ::OcaUint64>(sessionVersion),
                                                                          ::Ocp1LiteNetworkAddress(::OcaLiteString(originAddress),
                                                                                                   static_cast< ::OcaUint16>(0)),
                                                                          ::OcaLiteString(sessionName),
                                                                          ::Ocp1LiteNetworkAddress(::OcaLiteString(destinationAddress),
                                                                                                   static_cast< ::OcaUint16>(destinationPort)),
                                                                          static_cast< ::OcaUint8>(timeToLive),
                                                                          ::OcaLiteString(mediaLabel),
                                                                          static_cast< ::OcaTimeInterval>(packetTime / OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR),
                                                                          static_cast< ::OcaUint32>(offset),
                                                                          static_cast< ::OcaTimeInterval>(linkOffset / OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR),
                                                                          static_cast< ::OcaUint8>(payloadType));
                        connector.SetIDInternal(static_cast< ::OcaLiteMediaConnectorID>(streamId));
                        ::OcaLiteMediaConnection newConnection(connector.GetConnection().GetSecure(),
                                                               newParameters,
                                                               connector.GetConnection().GetStreamCastMode());
                        connector.SetConnection(newConnection);

                        // Create a new channel pin map from the return values
                        ::OcaUint16 nrStreamSlots;
                        ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> newChannelPinMap;
                        ConvertToPinMap(nrChannels, channels, nrStreamSlots, newChannelPinMap);
                        connector.SetPinCount(nrStreamSlots);

                        // Check if the channel pin map has changed
                        if (connector.GetChannelPinMap() != newChannelPinMap)
                        {
                            connector.SetChannelPinMap(newChannelPinMap);
                            rc = OCASTATUS_PARTIALLY_SUCCEEDED;
                        }
                        else
                        {
                            rc = OCASTATUS_OK;
                        }
                    }
                    else
                    {
                        OCA_LOG_ERROR_PARAMS("Unable to create RX stream for connector %u", connector.GetIDInternal());
                        rc = OCASTATUS_DEVICE_ERROR;
                    }
                }

                delete[] channels;
            }
        }
        else
        {
            OCA_LOG_ERROR("The connection parameters are not a valid OcaLiteMediaStreamParametersAes67 object");
            rc = OCASTATUS_PARAMETER_ERROR;
        }

        delete parameters;
    }
    else
    {
        // Paused streams are not supported, so the initial state should be running
        OCA_LOG_ERROR_PARAMS("Incorrect initial state %u", initialState);
        rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
    }
    
    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::SetSourceConnectorPinMapValue(::OcaLiteMediaSourceConnector& connector,
                                                                                 const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                                                 ::OcaLiteMediaConnectorStatus&)
{
    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    // Convert the channel pin map
    UINT16 nrChannels(static_cast<UINT16>(connector.GetPinCount()));
    UINT16* channels(new UINT16[nrChannels]);
    ConvertFromPinMap(channelPinMap, nrChannels, channels);

    if (false/*AES67LiteHostInterfaceModifyTxStreamChannels(static_cast<UINT16>(connector.GetIDInternal()),
                                            nrChannels,
                                            channels)*/)
    {
        // Create a new channel pin map from the return values
        ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> newChannelPinMap;
        ConvertToPinMap(nrChannels, channels, newChannelPinMap);
        connector.SetPinCount(static_cast< ::OcaUint16>(nrChannels));

        // Check if the channel pin map has changed
        if (channelPinMap != newChannelPinMap)
        {
            rc = OCASTATUS_PARTIALLY_SUCCEEDED;
        }
        else
        {
            rc = OCASTATUS_OK;
        }

        connector.SetChannelPinMap(newChannelPinMap);
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Unable to modify TX stream %u", connector.GetIDInternal());
        rc = OCASTATUS_DEVICE_ERROR;
    }

    delete[] channels;

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::SetSinkConnectorPinMapValue(::OcaLiteMediaSinkConnector& connector,
                                                                               const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                                               ::OcaLiteMediaConnectorStatus&)
{
    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    ::OcaUint16 maxPorts(static_cast< ::OcaUint16>(0));
    rc = GetMaxPortsPerPin(maxPorts);
    if (OCASTATUS_OK != rc)
    {
        OCA_LOG_ERROR_PARAMS("Unable to get maximum number of ports per pin (rc = %u)", rc);
    }
    else
    {
        // Convert the channel pin map
        UINT16 nrChannels(static_cast<UINT16>(connector.GetPinCount() * maxPorts));
        UINT16* channels(new UINT16[nrChannels]);
        UINT16 nrChannelsOld(static_cast<UINT16>(connector.GetPinCount() * maxPorts));
        UINT16* channelsOld(new UINT16[nrChannelsOld]);
        ConvertFromPinMap(channelPinMap, connector.GetPinCount(), nrChannels, channels);
        ConvertFromPinMap(connector.GetChannelPinMap(), connector.GetPinCount(), nrChannelsOld, channelsOld);

        ::OcaLiteMediaStreamParametersAes67* parameters(::OcaLiteMediaStreamParametersAes67::CreateFromBase(connector.GetConnection().GetStreamParameters()));
        if (NULL != parameters)
        {
            UINT16 sdpVersion(static_cast<UINT16>(parameters->GetSdpVersion()));
            std::string userName(parameters->GetUserName().GetString().data(), parameters->GetUserName().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
            UINT64 sessionID(static_cast<UINT64>(parameters->GetSessionID()));
            UINT64 sessionVersion(static_cast<UINT64>(parameters->GetSessionVersion()));
            std::string originAddress(parameters->GetOriginAddress().GetDestHostOrIPAddress().GetString().data(), parameters->GetOriginAddress().GetDestHostOrIPAddress().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
            std::string sessionName(parameters->GetSessionName().GetString().data(), parameters->GetSessionName().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
            std::string destinationAddress(parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().data(), parameters->GetDestinationAddress().GetDestHostOrIPAddress().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
            UINT16 destinationPort(static_cast<UINT16>(parameters->GetDestinationAddress().GetPort()));
            UINT8 timeToLive(static_cast<UINT8>(parameters->GetTimeToLive()));
            std::string mediaLabel(parameters->GetMediaLabel().GetString().data(), parameters->GetMediaLabel().GetString().length()); // Make an explicit copy to work around GCC Copy-On-Write mechanism
            UINT64 packetTime(static_cast<UINT64>(static_cast<FLOAT>(parameters->GetPacketTime()) * OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR));
            UINT32 offset(static_cast<UINT32>(parameters->GetOffset()));
            UINT8 payloadType(static_cast<UINT8>(parameters->GetPayloadType()));
            // Determine the encoding and sample rate
            UINT8 encoding(static_cast<UINT8>(0));
            UINT32 sampleRate(0);
            rc = DetermineVerifiedEncodingAndSampleRate(false/*isSource*/, connector.GetCoding(), encoding, sampleRate);

            if (OCASTATUS_OK == rc)
            {
                if (AES67LiteHostInterfaceModifyRxStreamChannels(static_cast<UINT16>(connector.GetIDInternal()),
                                                                 connector.GetConnection().GetStreamCastMode() == OCAMEDIASTREAMCASTMODE_MULTICAST,
                                                                 encoding,
                                                                 sampleRate,
                                                                 static_cast<UINT16>(connector.GetPinCount()),
                                                                 sdpVersion,
                                                                 userName,
                                                                 sessionID,
                                                                 sessionVersion,
                                                                 originAddress,
                                                                 sessionName,
                                                                 destinationAddress,
                                                                 destinationPort,
                                                                 timeToLive,
                                                                 mediaLabel,
                                                                 packetTime,
                                                                 offset,
                                                                 payloadType,
                                                                 nrChannelsOld,
                                                                 channelsOld,
                                                                 nrChannels,
                                                                 channels))
                {
                    // Create a new channel pin map from the return values
                    ::OcaUint16 nrStreamSlots;
                    ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> newChannelPinMap;
                    ConvertToPinMap(nrChannels, channels, nrStreamSlots, newChannelPinMap);
                    connector.SetPinCount(nrStreamSlots);

                    // Check if the channel pin map has changed
                    if (channelPinMap != newChannelPinMap)
                    {
                        rc = OCASTATUS_PARTIALLY_SUCCEEDED;
                    }
                    else
                    {
                        rc = OCASTATUS_OK;
                    }

                    connector.SetChannelPinMap(newChannelPinMap);
                }
                else
                {
                    OCA_LOG_ERROR_PARAMS("Unable to modify RX stream %u", connector.GetIDInternal());
                    rc = OCASTATUS_DEVICE_ERROR;
                }
            }
        }

        delete parameters;

        delete[] channels;
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::DeleteSourceConnectorValue(const ::OcaLiteMediaSourceConnector& connector)
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    // Convert the channel pin map
    UINT16 nrChannels(static_cast<UINT16>(connector.GetPinCount()));
    UINT16* channels(new UINT16[nrChannels]);
    ConvertFromPinMap(connector.GetChannelPinMap(), nrChannels, channels);
    if (AES67LiteHostInterfaceStopTxStream(static_cast<UINT16>(connector.GetIDInternal()), nrChannels, channels))
    {
        rc = OCASTATUS_OK;
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Unable to delete TX stream %u", connector.GetIDInternal());
    }
    
    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::DeleteSinkConnectorValue(const ::OcaLiteMediaSinkConnector& connector)
{

    ::OcaUint16 maxPorts(static_cast< ::OcaUint16>(0));
    ::OcaLiteStatus rc(GetMaxPortsPerPin(maxPorts));
    if (OCASTATUS_OK != rc)
    {
        OCA_LOG_ERROR_PARAMS("Unable to get maximum number of ports per pin (rc = %u)", rc);
    }
    else
    {
        // Convert the channel pin map
        UINT16 nrChannels(static_cast<UINT16>(connector.GetPinCount() * maxPorts));
        UINT16* channels(new UINT16[nrChannels]);
        ConvertFromPinMap(connector.GetChannelPinMap(), connector.GetPinCount(), nrChannels, channels);
        if (AES67LiteHostInterfaceStopRxStream(static_cast<UINT16>(connector.GetIDInternal()), nrChannels, channels))
        {
            rc = OCASTATUS_OK;
        }
        else
        {
            OCA_LOG_ERROR_PARAMS("Unable to delete RX stream %u", connector.GetIDInternal());
        }
    }

    return rc;
}

void OcaLiteMediaTransportNetworkAes67::CreatePorts(bool isSource, UINT16 nrOfChannels)
{
    ::OcaPortMode portMode(isSource ? OCAPORTMODE_INPUT : OCAPORTMODE_OUTPUT);

    for (UINT16 channel(0); channel < nrOfChannels; channel++)
    {
        ::OcaLitePortID portID;
        ::OcaLiteString portName(AES67LiteHostInterfaceGetAES67ChannelName(channel, isSource));
        ::OcaLiteStatus rc(InternalAddPort(portName, portMode, portID));

        if (OCASTATUS_OK != rc)
        {
            OCA_LOG_WARNING_PARAMS("Failed to add port for %s channel %u (rc = %u)", isSource ? "source" : "sink", channel, rc);
        }
    }
}

void OcaLiteMediaTransportNetworkAes67::AddTransmitStream(UINT16 streamId,
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
                                                          UINT8 payloadType)
{
    // Check if the stream is already part of the administration
    ::OcaLiteMediaSourceConnector sourceConnector;
    ::OcaLiteStatus rc(GetSourceConnectorValue(static_cast< ::OcaLiteMediaConnectorID>(streamId), sourceConnector));
    if (OCASTATUS_OK != rc)
    {
        // Create the channel pin map
        ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> channelPinMap;
        ConvertToPinMap(nrChannels, channels, channelPinMap);
        assert(ArePortsInChannelPinMapValid(channelPinMap));

        // Create the codec parameters
        ::OcaLiteString codecParameters;
        if (OCASTATUS_OK == DetermineCodecParameters(encoding, codecParameters))
        {
            // Get the network's alignment level
            ::OcaDBfs level;
            ::OcaDBfs minLevel;
            ::OcaDBfs maxLevel;
            static_cast<void>(GetAlignmentLevel(level, minLevel, maxLevel));

            // Create a connector object with the data
            ::OcaLiteMediaSourceConnector connector(static_cast< ::OcaLiteMediaConnectorID>(streamId),
                                                    ::OcaLiteString(),
                                                    ::OcaLiteMediaConnection(static_cast< ::OcaBoolean>(false)/*secure*/,
                                                                         ::OcaLiteMediaStreamParametersAes67(static_cast< ::OcaUint16>(sdpVersion),
                                                                                                         ::OcaLiteString(userName),
                                                                                                         static_cast< ::OcaUint64>(sessionID),
                                                                                                         static_cast< ::OcaUint64>(sessionVersion),
                                                                                                         ::Ocp1LiteNetworkAddress(::OcaLiteString(originAddress),
                                                                                                                              static_cast< ::OcaUint16>(0)),
                                                                                                         ::OcaLiteString(sessionName),
                                                                                                         ::Ocp1LiteNetworkAddress(::OcaLiteString(destinationAddress),
                                                                                                                              static_cast< ::OcaUint16>(destinationPort)),
                                                                                                         static_cast< ::OcaUint8>(timeToLive),
                                                                                                         ::OcaLiteString(mediaLabel),
                                                                                                         static_cast< ::OcaTimeInterval>(packetTime / OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR),
                                                                                                         static_cast< ::OcaUint32>(offset),
                                                                                                         static_cast< ::OcaTimeInterval>(0),
                                                                                                         static_cast< ::OcaUint8>(payloadType)),
                                                                         multicast ? OCAMEDIASTREAMCASTMODE_MULTICAST : OCAMEDIASTREAMCASTMODE_UNICAST),
                                                    ::OcaLiteMediaCoding(static_cast< ::OcaLiteMediaCodingSchemeID>(0),
                                                                     codecParameters,
                                                                     OCA_INVALID_ONO),
                                                    static_cast< ::OcaUint16>(nrChannels),
                                                    channelPinMap,
                                                    level);

            // Add the connector to the administration
            rc = InternalAddSourceConnector(OCAMEDIACONNECTORSTATE_RUNNING, connector);
            if (OCASTATUS_OK != rc)
            {
                OCA_LOG_ERROR_PARAMS("Unable to store source connector for stream %u (rc = %u)", streamId, rc);
            }
        }
        else
        {
            OCA_LOG_ERROR("Unable to create the codec parameters");
        }
    }
    else
    {
        OCA_LOG_WARNING_PARAMS("TX stream (id = %u) already exists", streamId);
    }
}

void OcaLiteMediaTransportNetworkAes67::AddReceiveStream(UINT16 streamId,
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
                                                         UINT8 payloadType)
{
    // Check if the stream is already part of the administration
    ::OcaLiteMediaSinkConnector sinkConnector;
    ::OcaLiteStatus rc(GetSinkConnectorValue(static_cast< ::OcaLiteMediaConnectorID>(streamId), sinkConnector));
    if (OCASTATUS_OK != rc)
    {
        // Create the channel pin map
        ::OcaUint16 nrStreamSlots;
        ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> channelPinMap;
        ConvertToPinMap(nrChannels, channels, nrStreamSlots, channelPinMap);
        assert(ArePortsInChannelPinMapValid(channelPinMap));

        // Create the codec parameters
        ::OcaLiteString codecParameters;
        if (OCASTATUS_OK == DetermineCodecParameters(encoding, codecParameters))
        {
            // Get the network's alignment level
            ::OcaDBfs level;
            ::OcaDBfs minLevel;
            ::OcaDBfs maxLevel;
            static_cast<void>(GetAlignmentLevel(level, minLevel, maxLevel));

            // Get the network's alignment gain
            ::OcaDB gain;
            ::OcaDB minGain;
            ::OcaDB maxGain;
            static_cast<void>(GetAlignmentGain(gain, minGain, maxGain));

            // Create a connector object with the data
            ::OcaLiteMediaSinkConnector connector(static_cast< ::OcaLiteMediaConnectorID>(streamId),
                                                  ::OcaLiteString(),
                                                  ::OcaLiteMediaConnection(static_cast< ::OcaBoolean>(false)/*secure*/,
                                                                       ::OcaLiteMediaStreamParametersAes67(static_cast< ::OcaUint16>(sdpVersion),
                                                                                                       ::OcaLiteString(userName),
                                                                                                       static_cast< ::OcaUint64>(sessionID),
                                                                                                       static_cast< ::OcaUint64>(sessionVersion),
                                                                                                       ::Ocp1LiteNetworkAddress(::OcaLiteString(originAddress),
                                                                                                                            static_cast< ::OcaUint16>(0)),
                                                                                                       ::OcaLiteString(sessionName),
                                                                                                       ::Ocp1LiteNetworkAddress(::OcaLiteString(destinationAddress),
                                                                                                                            static_cast< ::OcaUint16>(destinationPort)),
                                                                                                       static_cast< ::OcaUint8>(0),
                                                                                                       ::OcaLiteString(mediaLabel),
                                                                                                       static_cast< ::OcaTimeInterval>(packetTime / OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR),
                                                                                                       static_cast< ::OcaUint32>(offset),
                                                                                                       static_cast< ::OcaTimeInterval>(linkOffset / OCA_TIME_INTERVAL_MICRO_SECOND_FACTOR),
                                                                                                       static_cast< ::OcaUint8>(payloadType)),
                                                                       multicast ? OCAMEDIASTREAMCASTMODE_MULTICAST : OCAMEDIASTREAMCASTMODE_UNICAST),
                                                  ::OcaLiteMediaCoding(static_cast< ::OcaLiteMediaCodingSchemeID>(0),
                                                                   codecParameters,
                                                                   OCA_INVALID_ONO),
                                                  nrStreamSlots,
                                                  channelPinMap,
                                                  level,
                                                  gain);

            // Add the connector to the administration
            rc = InternalAddSinkConnector(OCAMEDIACONNECTORSTATE_RUNNING, connector);
            if (OCASTATUS_OK != rc)
            {
                OCA_LOG_ERROR_PARAMS("Unable to store sink connector for stream %u (rc = %u)", streamId, rc);
            }
        }
        else
        {
            OCA_LOG_ERROR("Unable to create the codec parameters");
        }
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("RX stream (id = %u) already exists", streamId);
    }
}

void OcaLiteMediaTransportNetworkAes67::ConvertToPinMap(UINT16 nrChannels,
                                                        const UINT16* channels,
                                                        ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap)
{
    assert(NULL != channels);

    channelPinMap.Clear();
    for (UINT16 i(0); i < nrChannels; i++)
    {
        if (AES67_INVALID_CHANNEL != channels[i])
        {
            channelPinMap.Add(::OcaLiteMapItem< ::OcaUint16, ::OcaLitePortID>(static_cast< ::OcaUint16>(i + 1),
                                                                              ::OcaLitePortID(OCAPORTMODE_INPUT, static_cast< ::OcaUint16>(channels[i] + 1))));
        }
    }
}

void OcaLiteMediaTransportNetworkAes67::ConvertFromPinMap(const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                          UINT16 nrChannels,
                                                          UINT16* channels)
{
    assert(NULL != channels);

    for (UINT16 i(0); i < nrChannels; i++)
    {
        ::OcaUint16 keyValue(static_cast< ::OcaUint16>(i + 1));
        if (channelPinMap.ContainsKey(keyValue))
        {
            channels[i] = static_cast<UINT16>(channelPinMap.GetValue(keyValue).GetIndex()) - 1;
        }
        else
        {
            channels[i] = AES67_INVALID_CHANNEL;
        }
    }
}

void OcaLiteMediaTransportNetworkAes67::ConvertToPinMap(UINT16 nrChannels,
                                                        const UINT16* channels,
                                                        ::OcaUint16& nrStreamSlots,
                                                        ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap)
{
    assert(NULL != channels);

    channelPinMap.Clear();
    nrStreamSlots = static_cast< ::OcaUint16>(0);
    UINT16 i(0);
    while (i < nrChannels)
    {
        nrStreamSlots++;

        for (UINT16 j(0); j < channels[i]; j++)
        {
            UINT16 channelIndex(i + j + 1);
            assert(channelIndex < nrChannels);
            assert(AES67_INVALID_CHANNEL != channels[channelIndex]);
            channelPinMap.Add(::OcaLiteMapItem< ::OcaUint16, ::OcaLitePortID>(nrStreamSlots,
                                                                                   ::OcaLitePortID(OCAPORTMODE_OUTPUT, static_cast< ::OcaUint16>(channels[channelIndex] + 1))));
        }

        i += (channels[i] + 1);
    }
}

void OcaLiteMediaTransportNetworkAes67::ConvertFromPinMap(const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap,
                                                          ::OcaUint16 nrStreamSlots,
                                                          UINT16& nrChannels,
                                                          UINT16* channels)
{
    assert(NULL != channels);

#ifndef NDEBUG
    UINT16 arraySize(nrChannels);
#endif
    nrChannels = static_cast<UINT16>(0);

    for (::OcaUint16 keyValue(static_cast< ::OcaUint16>(1)); keyValue <= nrStreamSlots; keyValue++)
    {
        UINT16 countIndex(nrChannels);
        assert(countIndex < arraySize);

        if (channelPinMap.ContainsKey(keyValue))
        {
            std::pair< ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>::Iterator, ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>::Iterator> valueRange(channelPinMap.GetValueRange(keyValue));
            UINT16 portCount(0);
            for (::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>::Iterator it(valueRange.first);
                 it != valueRange.second;
                 ++it)
            {
                nrChannels++;
                assert(nrChannels < arraySize);

                channels[nrChannels] = static_cast<UINT16>(it.GetValue().GetIndex()) - 1;
                portCount++;
            }

            channels[countIndex] = portCount;
            nrChannels++;
        }
        else
        {
            channels[countIndex] = 0;
            nrChannels++;
        }
    }
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::DetermineVerifiedEncodingAndSampleRate(bool isSource,
                                                                                          const ::OcaLiteMediaCoding& coding,
                                                                                          UINT8& encoding,
                                                                                          UINT32& sampleRate)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_ERROR);

    if (coding.GetClockONo() != OCA_INVALID_ONO)
    {
        const ::OcaLiteMediaClock3* pMediaClock3(dynamic_cast<const ::OcaLiteMediaClock3*>(::OcaLiteBlock::GetRootBlock().GetObject(coding.GetClockONo())));
        if (NULL != pMediaClock3)
        {
            ::OcaLiteMediaClockRate rate;
            ::OcaONo timesourceONo;
            rc = pMediaClock3->GetCurrentRate(rate, timesourceONo);
            if (OCASTATUS_OK == rc)
            {
                sampleRate = static_cast<UINT32>(rate.GetNominalRate());
            }
        }
        else
        {
            rc = OCASTATUS_PROCESSING_FAILED;
        }
    }

    // Check the codec parameters: The codec parameters may not be empty;
    ::OcaLiteString foundScheme;
    if (OCASTATUS_OK == rc)
    {
        if (coding.GetCodecParameters().GetLength() > static_cast< ::OcaUint16>(0))
        {
            // Try to parse the codec parameters
            char schemeIdentifier('\0');
            unsigned int bitDepth(0);

            if (2 == sscanf(reinterpret_cast<const char*>(coding.GetCodecParameters().GetValue()), "%c%u", &schemeIdentifier, &bitDepth))
            {
                // set the found scheme to the one from the string.
                foundScheme = ::OcaLiteString(coding.GetCodecParameters().GetString());

                // All required data is present
                rc = OCASTATUS_OK;
            }
            else
            {
                OCA_LOG_ERROR("Codec parameters are not in the correct format");
                rc = OCASTATUS_PROCESSING_FAILED;
            }
        }
        else
        {
            OCA_LOG_ERROR("No coding scheme is found in the codec parameters");
            rc = OCASTATUS_PROCESSING_FAILED;
        }
    }

    if (OCASTATUS_OK == rc)
    {
        // Try to convert the found scheme to the required enumeration
        if (::OcaLiteString("L16") == foundScheme)
        {
            encoding = AES67_ENCODING_LPCM16;
        }
        else if (::OcaLiteString("L24") == foundScheme)
        {
            encoding = AES67_ENCODING_LPCM24;
        }
        else
        {
            rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
            OCA_LOG_ERROR_PARAMS("Requested encoding (%s) not recognized", foundScheme.GetValue());
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaTransportNetworkAes67::DetermineCodecParameters(UINT8 encoding,
                                                                            ::OcaLiteString& codecParameters)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    codecParameters = ::OcaLiteString();

    switch (encoding)
    {
    case AES67_ENCODING_LPCM16:
        codecParameters += ::OcaLiteString("L16");
        break;
    case AES67_ENCODING_LPCM24:
        codecParameters += ::OcaLiteString("L24");
        break;
    default:
        rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
        break;
    }
    return rc;
}
