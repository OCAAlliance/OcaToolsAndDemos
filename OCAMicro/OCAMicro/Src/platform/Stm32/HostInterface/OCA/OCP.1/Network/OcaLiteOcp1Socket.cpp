/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLite Ocp1Socket implementation.
 *
 */

// ---- Include system wide include files ----
#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Network/IOcp1LiteSocket.h>
#include <StandardLib/StandardLib.h>
#include <lwip/opt.h> 
#include <lwip/arch.h>
#include <lwip/api.h>
#include <lwip/inet.h>
#include <lwip/sockets.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----
/** Invalid socket ID */
#define INVALID_SOCKET -1

// ---- Helper functions ----

// ---- Local data ----

#ifdef ERRNO
   int err;
#endif
// ---- Class Implementation ----


INT32 OcfLiteHostInterfaceSelect(INT32 highest, OcfLiteSelectableSet& readset,  OcfLiteSelectableSet& writeset, OcfLiteSelectableSet& exceptset, INT32 timeout)
{
    struct timeval timeValue = {0};

    if (timeout > 0)
    {
        timeValue.tv_sec = timeout / 1000;
        timeValue.tv_usec = timeout % 1000;
    }
    
    return select(highest + 1, &readset, &writeset, &exceptset, &timeValue);
}

INT32 Ocp1LiteHostInterfaceRetrieveSocket(::SocketNetworkProtocolType networkProtocolType, ::SocketTransportProtocolType transportProtocolType)
{
    return lwip_socket(AF_INET, transportProtocolType == PROTOCOL_TCP ? SOCK_STREAM : SOCK_DGRAM , static_cast<INT32>(transportProtocolType));
}

bool Ocp1LiteSocketOpen(INT32 socket)
{
    return true;
}

bool Ocp1LiteSocketBind(INT32 socket, UINT16 port)
{
    assert(socket != INVALID_SOCKET);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

    return (lwip_bind(socket, (struct sockaddr *)&sin, sizeof(sin)) == 0);
}

bool Ocp1LiteSocketListen(INT32 socket, UINT8 backlog)
{
    assert(socket != INVALID_SOCKET);

    return (lwip_listen(socket, static_cast<int>(backlog)) == 0);
}

bool Ocp1LiteSocketAccept(INT32 socket, INT32& newsocket)
{
    assert(socket != INVALID_SOCKET);
    
    struct sockaddr_in sin;
    socklen_t sinLen(sizeof(struct sockaddr_in));

    newsocket = lwip_accept(socket, (struct sockaddr *)&sin, &sinLen);

    int on = true;
    setsockopt(newsocket, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(on));

    return ((newsocket > 0) && (sinLen == sizeof(struct sockaddr_in)));
}

bool Ocp1LiteSocketReject(INT32 socket)
{
    assert(socket != INVALID_SOCKET);
    
    struct sockaddr_in sin;
    socklen_t sinLen(sizeof(struct sockaddr_in));
    INT32 newSocket = lwip_accept(socket, (struct sockaddr *)&sin, &sinLen);

    if ((newSocket > INVALID_SOCKET) && (sinLen == sizeof(struct sockaddr_in)))
    {
        return (lwip_close(newSocket) == 0);
    }

    return false;
}

INT32 Ocp1LiteSocketSend(INT32 socket, const void* buffer, INT32 length)
{
    assert(socket != INVALID_SOCKET);
    return lwip_send(socket, buffer, length, 0);
}

INT32 Ocp1LiteSocketSendTo(INT32 socket, const void* buffer, INT32 length, const std::string& hostOrIp, UINT16 port)
{
    assert((INVALID_SOCKET != socket) && !hostOrIp.empty());
    
    int b[sizeof(UINT32)];

    if (sizeof(UINT32) == ::ScanIpAddress(hostOrIp.c_str(), b))
    {
        struct sockaddr_in sin;
        ip_addr ip_addr;

        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        IP4_ADDR(&ip_addr, b[0], b[1], b[2], b[3]);
        sin.sin_addr.s_addr = ip_addr.addr;

        return lwip_sendto(socket, buffer, length, 0, (struct sockaddr *)&sin, sizeof(sin));
    }

    return ISOCKET_ERROR;
}

INT32 Ocp1LiteSocketReceive(INT32 socket, void* buffer, INT32 length)
{
    assert(socket != INVALID_SOCKET);

    return lwip_recv(socket, buffer, length, 0);
}

bool Ocp1LiteSocketShutdown(INT32 socket)
{
    assert(socket != INVALID_SOCKET);
    return (lwip_shutdown(socket, SHUT_RDWR) == 0);
}

bool Ocp1LiteSocketClose(INT32 socket)
{
    assert(socket != INVALID_SOCKET);
    return (lwip_close(socket) == 0);
}
