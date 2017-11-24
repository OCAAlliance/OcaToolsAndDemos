/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Host Interface specific data types.
 *
 */

#ifndef OCP1LITEHOSTINTERFACEDATATYPES_H
#define OCP1LITEHOSTINTERFACEDATATYPES_H

// ---- Include system wide include files ----

/**
 * @addtogroup OCP1HostInterface
 * @{
 */

/**
 * Enumeration of Host Interface network protocol types.
 */
 //lint -strong(AJX, SocketNetworkProtocolType)
enum SocketNetworkProtocolType
{
    /** IP Version 4 */
    IPV4 = 0,
    /** IP Version 6 */
    IPV6 = 1
};

/**
 * Enumeration of Host Interface socket protocol types.
 */
//lint -strong(AJX, SocketTransportProtocolType)
enum SocketTransportProtocolType
{
    /** TCP protocol. */
    PROTOCOL_TCP     = 6,
    /** UDP protocol. */
    PROTOCOL_UDP     = 17
};

/** @} */ // end addtogroup Ocp1HostInterface

#endif /* OCP1LITEHOSTINTERFACEDATATYPES_H */
