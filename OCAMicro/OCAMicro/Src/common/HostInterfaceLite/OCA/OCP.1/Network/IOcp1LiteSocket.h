/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The Socket interface of the Host Interface.
 *
 */

#ifndef IOCP1LITESOCKET_H
#define IOCP1LITESOCKET_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "../../OCP.1/Ocp1LiteHostInterfaceDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * Generic error value for a IOcp1LiteSocket error.
 * @ingroup OCP1HostInterface
 */
#define ISOCKET_ERROR        -1

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Open the socket by allocation resources for this socket to the OS.
 *
 * @param[in] socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @return True if succeeded, false if not.
 */
bool Ocp1LiteSocketOpen(INT32 socket);

/**
 * Bind the socket to the given (TCP or UDP) port. Needed for TCP listening
 * sockets to identifty the port clients can connect on. Needed for UDP sockets
 * to identify the port datagrams can be received on.
 *
 * @param[in] socket  The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @param[in] port    The port number the socket is bound to. Use 0 to let the
 *                    operating system select an available port.
 * @return Indicates whether the socket was successfully bound.
 */
bool Ocp1LiteSocketBind(INT32 socket, UINT16 port);

/**
 * Places the socket in a listening state. Only TCP sockets can be placed
 * in a listening state, and the socket must have been bound to a port
 * before calling this method.
 *
 * @param[in] socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @param[in] backlog   The maximum length of the pending connections queue.
 * @return Indicates whether the socket was successfully placed in a listening state.
 */
bool Ocp1LiteSocketListen(INT32 socket, UINT8 backlog);

/**
 * Accepts a connection request on the listening socket and fills the passed socket
 * with the information required to handle the connection. If an error occurs false
 * is returned.
 *
 * @param[in]  socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @param[out] newSocket The accepted socket. Only valid if the return value indicates so.
 * @return True if the accept succeeded, false otherwise.
 */
bool Ocp1LiteSocketAccept(INT32 socket, INT32& newSocket);

/**
 * Rejects a connection requist on the listening socket. If an error occurs false is returned.
 *
 * @param[in] socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @return True of the reject succeeded, false otherwise.
 */
bool Ocp1LiteSocketReject(INT32 socket);

/**
 * Sends data present in a send buffer to a connected socket.
 *
 * @param[in]  socket  The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @param[in]  buffer  Pointer to the send buffer.
 * @param[in]  length  The length of the data (in number of bytes).
 * @return Indicates the number of bytes that were sent. A return value of ISOCKET_ERROR
 *         indicates an error occurred when trying to send (e.g. when the socket
 *         was not connected before this method is called).
 */
INT32 Ocp1LiteSocketSend(INT32 socket, const void* buffer, INT32 length);

/**
 * Sends data to the specified remote host. This method can be used on
 * UDP sockets to send multicast and/or broadcast traffic. If a UDP socket
 * is connected this method can be used to send traffic to another host
 * than its default remote host. This method is not meant for TCP sockets;
 * if it is used for such a socket the hostOrIp and port parameters are ignored.
 *
 * @param[in]  socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @param[in]  buffer    Pointer to the send buffer.
 * @param[in]  length    The length of the data (in number of bytes).
 * @param[in]  hostOrIp  The hostname or IP address of the remote host. The
 *                       local host is assumed if an empty string is passed.
 * @param[in]  port      The port number of the remote host.
 * @return Indicates the number of bytes that were sent. A return value of ISOCKET_ERROR
 *         indicates an error occurred when trying to send.
 */
INT32 Ocp1LiteSocketSendTo(INT32 socket, const void* buffer, INT32 length, const std::string& hostOrIp, UINT16 port);

/**
 * Receives data from a bound socket into a receive buffer.
 *
 * @param[in]   socket  The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @param[out]  buffer  Pointer to the receive buffer.
 * @param[in]   length  The length of the receive buffer (in number of bytes).
 * @return Indicates the number of bytes that were received. A return
 *         value of ISOCKET_ERROR indicates an error occurred when trying to receive.
 *         A return value of 0 indicates the remote host has performed an
 *         orderly shutdown.
 */
INT32 Ocp1LiteSocketReceive(INT32 socket, void* buffer, INT32 length);

/**
 * Retrieves the remote address the socket is connected to.
 *
 * @param[in] socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @return The remote address the socket is connected to (or an empty string if there is no
 *         connection).
 */
std::string Ocp1LiteSocketGetRemoteAddress(INT32 socket);

/**
 * Retrieves the remote port the socket is connected to.
 *
 * @param[in] socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @return The remote port the socket is connected to (or 0 if there is no connection).
 */
UINT16 Ocp1LiteSocketGetRemotePort(INT32 socket);

/**
 * Shuts down the socket. For connected TCP sockets a FIN message will be sent
 * after all pending data is sent and acknowledged by the remote host. The socket
 * can then be safely closed when the Receive method returns 0. Shutting down a
 * socket is only needed for gracefully closing a connected TCP socket, it has no
 * effect for UDP sockets or listening TCP sockets.
 *
 * @param[in] socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @return Indicates whether the socket was succesfully shut down.
 */
bool Ocp1LiteSocketShutdown(INT32 socket);

/**
 * Closes the socket and releases the resources associated with the socket.
 * The socket may not be reused after it has been closed. TCP sockets should
 * be shutdown before they are closed (unless the TCP session was already
 * shut down by the remote host).
 *
 * @param[in] socket    The socket (previously retreived via Ocp1LiteHostInterfaceRetrieveSocket)
 * @return Indicates whether the socket was succesfully closed.
 */
bool Ocp1LiteSocketClose(INT32 socket);

#endif // IOCP1LITESOCKET_H
