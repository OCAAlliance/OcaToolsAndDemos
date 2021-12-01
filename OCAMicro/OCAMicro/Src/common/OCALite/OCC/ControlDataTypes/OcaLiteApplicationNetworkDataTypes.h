/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OCC network data types.
 *
 */

#ifndef OCALITEAPPLICATIONNETWORKDATATYPES_H
#define OCALITEAPPLICATIONNETWORKDATATYPES_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteBlob.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * Enumeration of network statuses.
 */
enum OcaLiteApplicationNetworkState /* maps onto OcaUint8 */
{
    /** State is not known. */
    OCAAPPLICATIONNETWORKSTATE_UNKNOWN      = 0,
    /** Application network is not connected to host data network and is 
        therefore not ready for connection establishng or application data transfer. */
    OCAAPPLICATIONNETWORKSTATE_NOT_READY    = 1,
    /** Application network is in the process of connecting to the host data network 
        and is therefore not ready for connection establishing or application data transfer. */
    OCAAPPLICATIONNETWORKSTATE_READYING     = 2, 
    /** Application network is connected to host data network and is ready 
        for connection establishing and application data transfer. */
    OCAAPPLICATIONNETWORKSTATE_READY        = 3,
    /** Application network is connected to host data network and is executing 
        connection establishment and application data transfer. */
    OCAAPPLICATIONNETWORKSTATE_RUNNING      = 4, 
    /** All application data transfer is paused, but connections are still in place. */
    OCAAPPLICATIONNETWORKSTATE_PAUSED       = 5,
    /** Network is in the process of stopping all media application data transport 
        activity and is deleting all media transport connections. */
    OCAAPPLICATIONNETWORKSTATE_STOPPING     = 6,
    /** No application data transport connections exist, but application network
        is still connected to host data network. */
    OCAAPPLICATIONNETWORKSTATE_STOPPED      = 7,
    /** Application network has ceased all activity due to an error, 
        but operating storage elements have not been freed. */
    OCAAPPLICATIONNETWORKSTATE_FAULT        = 8,
    /** Maximum used for range checking. */
    OCAAPPLICATIONNETWORKSTATE_MAXIMUM
};

/**
 * Enumeration of network statuses.
 */
enum OcaLiteApplicationNetworkCommand /* maps onto OcaUint8 */
{
    /** No-op.  State is not changed. */
    OCAAPPLICATIONNETWORKCOMMAND_NONE       = 1,
    /** Open a connection to the host data network, but do not make any media connections.
        Resulting state = Ready. */
    OCAAPPLICATIONNETWORKCOMMAND_PREPARE    = 2,
    /** Commence media data connection-making and data transfer. 
        Resulting state = Running. */
    OCAAPPLICATIONNETWORKCOMMAND_START      = 3,
    /** Temporarily halt all media data transfer, but preserve media connections.
        Resulting state = Paused.  
        nb To unpause, a Start command should be used. */
    OCAAPPLICATIONNETWORKCOMMAND_PAUSE      = 4,
    /** Cease data transfer and delete all media connections. 
        Resulting state = Stopped. */
    OCAAPPLICATIONNETWORKCOMMAND_STOP       = 5,
    /** Cease all media transfer, delete all media transport connections, 
        and disconnect from the host data network. 
        Resulting state = NotReady. */
    OCAAPPLICATIONNETWORKCOMMAND_RESET      = 6,
    /** Maximum used for range checking. */
    OCAAPPLICATIONNETWORKCOMMAND_MAXIMUM
};

/**
 * Discovery systems as described in [A.Annex E]. 
 */
enum OcaLiteDiscoverySystemAES67 /* maps onto OcaUint8 */
{
    /** No discovery system */
    OCADISCOVERYSYSTEMAES67_NONE                                   = 0,
    /** Bonjour discovery */
    OCADISCOVERYSYSTEMAES67_BONJOUR                                = 1,
    /** SAP discovery */
    OCADISCOVERYSYSTEMAES67_SAP                                    = 2,
    /** Axia discovery */
    OCADISCOVERYSYSTEMAES67_AXIADISCOVERYPROTOCOL                  = 3,
    /** Wheatstone wheatnet ip discovery */
    OCADISCOVERYSYSTEMAES67_WHEATSTONEWHEATNETIPDISCOVERYPROTOCOL  = 4,
    /** Maximum used for range checking. */
    OCADISCOVERYSYSTEMAES67_MAXIMUM

};

// ---- Helper functions ----

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::OcaLiteApplicationNetworkState>(const ::OcaLiteApplicationNetworkState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteApplicationNetworkCommand>(const ::OcaLiteApplicationNetworkCommand& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteDiscoverySystemAES67>(const ::OcaLiteDiscoverySystemAES67& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteApplicationNetworkState>(::OcaLiteApplicationNetworkState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteApplicationNetworkCommand>(::OcaLiteApplicationNetworkCommand& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteDiscoverySystemAES67>(::OcaLiteDiscoverySystemAES67& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteApplicationNetworkState>(const ::OcaLiteApplicationNetworkState& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteApplicationNetworkCommand>(const ::OcaLiteApplicationNetworkCommand& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteDiscoverySystemAES67>(const ::OcaLiteDiscoverySystemAES67& value, const ::IOcaLiteWriter& writer);

#endif /* OCALITEAPPLICATIONNETWORKDATATYPES_H */
