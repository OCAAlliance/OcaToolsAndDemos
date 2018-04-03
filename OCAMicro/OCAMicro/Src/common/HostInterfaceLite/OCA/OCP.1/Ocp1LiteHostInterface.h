/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The entry point to the OCP.1 Host Interface.
 *
 */

#ifndef OCP1LITEHOSTINTERFACE_H
#define OCP1LITEHOSTINTERFACE_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----
#include "Ocp1LiteHostInterfaceDataTypes.h"

// ---- Referenced classes and types ----

/**
 * Initialize the host interface layer.
 *
 * @return The result of the operation.
 */
bool Ocp1LiteHostInterfaceInitialize(void);

/**
 * Retrieves a Host Interface socket.
 *
 * @param[in]  networkProtocolType      The network protocol type the socket should
 *                                      make use of.
 * @param[in]  transportProtocolType    Indicates the transport protocol type.
 * @return Pointer to the retrieved host interface socket object.
 */
INT32 Ocp1LiteHostInterfaceRetrieveSocket(::SocketNetworkProtocolType networkProtocolType, ::SocketTransportProtocolType transportProtocolType);

/**
 * Convert a UINT64 from host byte order to network order.
 *
 * @param[in] hostlonglong  A 64-bit number in host bytes order
 *
 * @return The value supplied in network byte order.
 */
UINT64 ConvertFromHostByteOrderToNetworkOrder(UINT64 hostlonglong);

/**
 * Convert a UINT32 from host byte order to network order.
 *
 * @param[in] hostlong  A 32-bit number in host bytes order
 *
 * @return The value supplied in network byte order.
 */
UINT32 ConvertFromHostByteOrderToNetworkOrder(UINT32 hostlong);

/**
 * Convert a UINT16 from host byte order to network order.
 *
 * @param[in] hostshort  A 16-bit number in host bytes order
 *
 * @return The value supplied in network byte order.
 */
UINT16 ConvertFromHostByteOrderToNetworkOrder(UINT16 hostshort);

/**
 * Convert a UINT64 from TCP/IP network order to host byte order.
 *
 * @param[in] netlonglong   A 64-bit number in network byte order.
 *
 * @return The value supplied in host byte order.
 */
UINT64 ConvertFromNetworkOrderToHostByteOrder(UINT64 netlonglong);

/**
 * Convert a UINT32 from TCP/IP network order to host byte order.
 *
 * @param[in] netlong   A 32-bit number in network byte order.
 *
 * @return The value supplied in host byte order.
 */
UINT32 ConvertFromNetworkOrderToHostByteOrder(UINT32 netlong);

/**
 * Convert a UINT16 from TCP/IP network order to host byte order.
 *
 * @param[in] netshort   A 16-bit number in network byte order.
 *
 * @return The value supplied in host byte order.
 */
UINT16 ConvertFromNetworkOrderToHostByteOrder(UINT16 netshort);

#endif //OCP1LITEHOSTINTERFACE_H
