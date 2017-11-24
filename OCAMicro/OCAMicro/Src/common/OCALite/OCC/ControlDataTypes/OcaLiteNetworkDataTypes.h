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
 * @ingroup MediaNetworkDataTypes
 */
/*lint -strong(AJX, OcaStreamConnectorPinIndex) */
typedef ::OcaUint16     OcaLiteStreamConnectorPinIndex;

// ---- Helper types and constants ----

/**
 * Enumeration of network statuses.
 * @ingroup NetworkDataTypes
 */
//lint -strong(AJX, OcaLiteNetworkStatus)
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
 * Status options for a media connection.
 * @ingroup NetworkDataTypes
 */
//lint -strong(AJX, OcaNetworkConnectionStatus)
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
 * @ingroup NetworkDataTypes
 */
//lint -strong(AJX, OcaLiteNetworkLinkType)
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
 * @ingroup MediaNetworkDataTypes
 */
//lint -strong(AJX, OcaLiteEncoding)
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
 * @ingroup ControlNetworkDataTypes
 */
//lint -strong(AJX, OcaLiteNetworkControlProtocol)
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
 * @ingroup MediaNetworkDataTypes
 */
//lint -strong(AJX, OcaLiteNetworkMediaProtocol)
enum OcaLiteNetworkMediaProtocol    /* maps onto OcaUint8 */
{
    /** No media protocol - the network does not do media transport */
    OCANETWORKMEDIAPROTOCOL_NONE         = 0,
    /** AVnu AV3 - RTP over AVB */
    OCANETWORKMEDIAPROTOCOL_AV3          = 1,
    /** IEEE 1722 / 1722.1 */
    OCANETWORKMEDIAPROTOCOL_AVBTP        = 2,
    /** Pre-AV3 Dante with ATP transport */
    OCANETWORKMEDIAPROTOCOL_DANTE        = 3,
    /** Cobranet */
    OCANETWORKMEDIAPROTOCOL_COBRANET     = 4,
    /** Maximum value used for range checking */
    OCANETWORKMEDIAPROTOCOL_MAXIMUM      = 0xFF
};

/**
 * Enumeration of media source or sink types.
 * @ingroup MediaNetworkDataTypes
 */
//lint -strong(AJX, OcaLiteNetworkMediaSourceOrSink)
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

// ---- Helper functions ----

// ---- Specialized Template Function Definition ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

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
bool UnmarshalValue< ::OcaLiteNetworkStatus>(::OcaLiteNetworkStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

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
::OcaUint32 GetSizeValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, const ::IOcaLiteWriter& writer);

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

//lint -restore

#endif /* OCALITENETWORKDATATYPES_H */
