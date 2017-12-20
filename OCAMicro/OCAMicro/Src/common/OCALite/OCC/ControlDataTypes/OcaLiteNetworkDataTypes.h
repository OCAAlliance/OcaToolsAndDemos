/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCC network data types.
 *
 */

#ifndef OCALITENETWORKDATATYPES_H
#define OCALITENETWORKDATATYPES_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteBlob.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

/**
 * Identifier of an Connector pin index.
 */
typedef ::OcaUint16     OcaLiteStreamConnectorPinIndex;

// ---- Helper types and constants ----

/**
 * Enumeration of network statuses.
 */
enum OcaLiteNetworkStatus   /* maps onto OcaUint8 */
{
    /** Status is not known for some reason. */
    OCANETWORKSTATUS_UNKNOWN        = 0,
    /** Network is ready for data transfer. */
    OCANETWORKSTATUS_READY          = 1,
    /** Network is starting up. */
    OCANETWORKSTATUS_STARTING_UP    = 2,    
    /**
     * Network has been stopped by a call to the Stop() method. All media connections and/or
     * control sessions have been closed.
     */
    OCANETWORKSTATUS_STOPPED        = 3,
    /** Maximum value used for range checking */
    OCANETWORKSTATUS_MAXIMUM
};

/**
* Status options for OcaStreamNetwork.
*/
enum OcaLiteStreamStatus  /* maps onto OcaUint8 */
{
    /** Connection is not ready to transfer data. */
    OCASTREAMSTATUS_NOT_CONNECTED = 0,
    /** Connection is ready for data transfer. */
    OCASTREAMSTATUS_CONNECTED = 1,
    /** Connection is set up, but data transfer has been halted. */
    OCASTREAMSTATUS_PAUSED = 2,
    /** Maximum value used for range checking */
    OCASTREAMSTATUS_MAXIMUM
};

/**
 * Status options for a media connection.
 */
enum OcaLiteNetworkConnectionStatus     /* maps onto OcaUint8 */
{
    /** Connection is not ready to transfer data. */
    OCANETWORKCONNECTIONSTATUS_NOT_CONNECTED    = 0,
    /** Connection is ready for data transfer. */
    OCANETWORKCONNECTIONSTATUS_CONNECTED        = 1,    
    /** Maximum value used for range checking */
    OCANETWORKCONNECTIONSTATUS_MAXIMUM
};

/**
 * Types of layer 2 networks
 */
enum OcaLiteNetworkLinkType    /* maps onto OcaUint8 */
{
    /** No network */
    OCANETWORKLINKTYPE_NONE                 = 0,
    /** Wired ethernet */
    OCANETWORKLINKTYPE_ETHERNET_WIRED       = 1,
    /** Wireless ethernet */
    OCANETWORKLINKTYPE_ETHERNET_WIRELESS    = 2,
    /** USB */
    OCANETWORKLINKTYPE_USB                  = 3,
    /** Low-speed serial point-to-point */
    OCANETWORKLINKTYPE_SERIAL_P2P           = 4,
    /** Maximum value used for range checking */
    OCANETWORKLINKTYPE_MAXIMUM
};

/**
 * Enumeration of encoding.
 */
enum OcaLiteEncoding                     /* maps onto OcaUint8 */
{
    /** None. */
    OCAENCODING_NONE                        = 0,
    /** PCM16 */
    OCAENCODING_PCM16                       = 1,
    /** PCM24 */
    OCAENCODING_PCM24                       = 2,
    /** PCM32 */
    OCAENCODING_PCM32                       = 3,
    /** The extension point */
    OCAENCODING_EXTENSION_POINT             = 65
};

/**
 * Network control protocols available
 */
enum OcaLiteNetworkControlProtocol    /* maps onto OcaUint8 */
{
    /** No control protocol - the network does not do control */
    OCANETWORKCONTROLPROTOCOL_NONE      = 0,
    /** OCP.1 - OCA protocol for TCP/IP networks */
    OCANETWORKCONTROLPROTOCOL_OCP01     = 1,
    /** OCP.2 - OCA protocol for USB links */
    OCANETWORKCONTROLPROTOCOL_OCP02     = 2,
    /** OCP.3 - Character XML or JSON (tbd) version of OCA protocol, for serial links and other purposes */
    OCANETWORKCONTROLPROTOCOL_OCP03     = 3,
    /** Maximum value used for range checking */
    OCANETWORKCONTROLPROTOCOL_MAXIMUM
};

/**
 * Media transport protocols available
 */
enum OcaLiteNetworkMediaProtocol
{
    /** No media protocol - the network does not do media transport. */
    OCANETWORKMEDIAPROTOCOL_NONE            = 0,
    /** AVnu AV3 - RTP over AVB */
    OCANETWORKMEDIAPROTOCOL_AV3             = 1,
    /** IEEE 1722 / 1722.1 */
    OCANETWORKMEDIAPROTOCOL_AVBTP           = 2,
    /** Pre-AV3 Dante with ATP transport */
    OCANETWORKMEDIAPROTOCOL_DANTE           = 3,
    /** Cobranet */
    OCANETWORKMEDIAPROTOCOL_COBRANET        = 4,
    /** AES67 network. */
    OCANETWORKMEDIAPROTOCOL_AES67           = 5,
    /** SMPTE 2022?  Or 2071?  (TBD) */
    OCANETWORKMEDIAPROTOCOL_SMPTEAUDIO      = 6,
    /** LiveWire media transport */
    OCANETWORKMEDIAPROTOCOL_LIVEWIRE        = 7,
    /** Maximum used for range checking */
    OCANETWORKMEDIAPROTOCOL_MAXIMUM,
    /** Base value for addition of nonstandard (e.g. proprietary) protocol options */
    OCANETWORKMEDIAPROTOCOL_EXTENSION_POINT = 65
};
/**
* Identifier of an OcaStream index.
*/
typedef ::OcaUint16     OcaLiteStreamIndex;

/**
 * Identifier of an Connector pin index.
 */
typedef ::OcaUint16     OcaLiteStreamConnectorPinIndex;

/**
 * Enumeration of media source or sink types.
 */
enum OcaLiteNetworkMediaSourceOrSink    /* maps onto OcaUint8 */
{
    /** None. */
    OCANETWORKMEDIASOURCEORSINK_NONE        = 0,
    /** Media source. */
    OCANETWORKMEDIASOURCEORSINK_SOURCE      = 1,
    /** Media sink. */
    OCANETWORKMEDIASOURCEORSINK_SINK        = 2,
    /** Maximum value used for range checking */
    OCANETWORKMEDIASOURCEORSINK_MAXIMUM
};

/**
 * Identifier of media connector
 */
typedef ::OcaUint16     OcaLiteMediaConnectorID; 

/**
* Identifier of media coding
*/
typedef ::OcaUint16     OcaLiteMediaCodingSchemeID;

/**
 * Enumeration of media connector states
 */
enum OcaLiteMediaConnectorState /* maps onto OcaUint8 */
{
    /** Connector has no media connection and no media data is
        being transferred. */
    OCAMEDIACONNECTORSTATE_STOPPED          = 0,
    /** Stream connection is being set up.  Media data is 
        not flowing. */
    OCAMEDIACONNECTORSTATE_SETTING_UP       = 1,
    /** Media data is flowing since the connection is 
        established. */
    OCAMEDIACONNECTORSTATE_RUNNING          = 2,
    /** Media transfer is stopped. Existing connection 
        is intact. */
    OCAMEDIACONNECTORSTATE_PAUSED           = 3,
    /** Data transfer has been halted due to errors. 
        Working storage has not been freed. */
    OCAMEDIACONNECTORSTATE_FAULT            = 4,
    /** Used for range checking */
    OCAMEDIACONNECTORSTATE_MAXIMUM
};

/**
 * Type of media endpoint:  unicast or multicast.
 */
enum OcaLiteMediaStreamCastMode    /* maps onto OcaUint8 */
{
    /** Undefined streamcast mode.*/
    OCAMEDIASTREAMCASTMODE_NONE         = 0,
    /** Unicast stream. */
    OCAMEDIASTREAMCASTMODE_UNICAST      = 1,
    /** Multicast stream */
    OCAMEDIASTREAMCASTMODE_MULTICAST    = 2,
    /** Maximum value used for range checking */
    OCAMEDIASTREAMCASTMODE_MAXIMUM
};

/**
 * Command values for OcaMediaNetwork.ControlConnector(...)
 */
enum OcaLiteMediaConnectorCommand    /* maps onto OcaUint8 */
{
    /** No-op.  State is not changed. */
    OCAMEDIACONNECTORCOMMAND_NONE   = 0,
    /** Commence media data connection-making and data transfer. 
        Resulting state = Running. */
    OCAMEDIACONNECTORCOMMAND_START  = 1,
    /** Commence media data connection-making and data transfer.
        Resulting state = Running. */
    OCAMEDIACONNECTORCOMMAND_PAUSE  = 2,
    /** Maximum value used for range checking */
    OCAMEDIACONNECTORCOMMAND_MAXIMUM
};

/**
 * Media connector element
 */
enum OcaLiteMediaConnectorElement     /* maps onto OcaUint16 */
{
    /** Channel pin map has changed. */
    OCAMEDIACONNECTORELEMENT_PINMAP     = 0x0001,
    /** Connection Descriptor has changed. */
    OCAMEDIACONNECTORELEMENT_CONNECTION = 0x0002,
    /** Coding descriptor of the current connection descriptor has changed. */
    OCAMEDIACONNECTORELEMENT_CODING     = 0x0004,
    /** Connector element is added */
    OCAMEDIACONNECTORELEMENT_ADDED      = (OCAMEDIACONNECTORELEMENT_PINMAP | OCAMEDIACONNECTORELEMENT_CONNECTION | OCAMEDIACONNECTORELEMENT_CONNECTION),
    /** Connector element is deleted */
    OCAMEDIACONNECTORELEMENT_DELETED    = (OCAMEDIACONNECTORELEMENT_PINMAP | OCAMEDIACONNECTORELEMENT_CONNECTION | OCAMEDIACONNECTORELEMENT_CONNECTION)
};

// ---- Helper functions ----

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteStreamStatus>(const ::OcaLiteStreamStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteNetworkConnectionStatus>(const ::OcaLiteNetworkConnectionStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteNetworkLinkType>(const ::OcaLiteNetworkLinkType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteEncoding>(const ::OcaLiteEncoding& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteNetworkControlProtocol>(const ::OcaLiteNetworkControlProtocol& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteNetworkMediaProtocol>(const ::OcaLiteNetworkMediaProtocol& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteNetworkMediaSourceOrSink>(const ::OcaLiteNetworkMediaSourceOrSink& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteMediaConnectorState>(const ::OcaLiteMediaConnectorState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteMediaStreamCastMode>(const ::OcaLiteMediaStreamCastMode& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteMediaConnectorCommand>(const ::OcaLiteMediaConnectorCommand& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteMediaConnectorElement>(const ::OcaLiteMediaConnectorElement& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteNetworkStatus>(::OcaLiteNetworkStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteStreamStatus>(::OcaLiteStreamStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteNetworkConnectionStatus>(::OcaLiteNetworkConnectionStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteNetworkLinkType>(::OcaLiteNetworkLinkType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteEncoding>(::OcaLiteEncoding& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteNetworkControlProtocol>(::OcaLiteNetworkControlProtocol& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteNetworkMediaProtocol>(::OcaLiteNetworkMediaProtocol& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteNetworkMediaSourceOrSink>(::OcaLiteNetworkMediaSourceOrSink& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteMediaConnectorState>(::OcaLiteMediaConnectorState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteMediaStreamCastMode>(::OcaLiteMediaStreamCastMode& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteMediaConnectorCommand>(::OcaLiteMediaConnectorCommand& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteMediaConnectorElement>(::OcaLiteMediaConnectorElement& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteStreamStatus>(const ::OcaLiteStreamStatus& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkConnectionStatus>(const ::OcaLiteNetworkConnectionStatus& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkLinkType>(const ::OcaLiteNetworkLinkType& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteEncoding>(const ::OcaLiteEncoding& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkControlProtocol>(const ::OcaLiteNetworkControlProtocol& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkMediaProtocol>(const ::OcaLiteNetworkMediaProtocol& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkMediaSourceOrSink>(const ::OcaLiteNetworkMediaSourceOrSink& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaConnectorState>(const ::OcaLiteMediaConnectorState& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaStreamCastMode>(const ::OcaLiteMediaStreamCastMode& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaConnectorCommand>(const ::OcaLiteMediaConnectorCommand& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaConnectorElement>(const ::OcaLiteMediaConnectorElement& value, const ::IOcaLiteWriter& writer);

#endif /* OCALITENETWORKDATATYPES_H */
