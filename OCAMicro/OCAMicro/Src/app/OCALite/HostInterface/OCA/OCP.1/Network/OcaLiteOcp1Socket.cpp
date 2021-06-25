/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLite Ocp1Socket implementation.
 *
 */

// ---- Include system wide include files ----
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <assert.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Network/IOcp1LiteSocket.h>
#include <StandardLib/StandardLib.h>
#ifdef _WIN32
#include <Ws2tcpip.h>
#else
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
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
    assert(networkProtocolType == IPV4);

    int socketType = (transportProtocolType == PROTOCOL_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    int socketFd = socket(AF_INET, socketType, transportProtocolType);
    if (SOCKET_ERROR != -1)
    {
        bool optionOn(true);

        // Set SO_REUSEADDR option
        int result(::setsockopt(static_cast<int>(socketFd), SOL_SOCKET, SO_REUSEADDR,
                               reinterpret_cast<const char*>(&optionOn), sizeof(optionOn)));
        if (0 != result)
        {
            OCA_LOG_ERROR_PARAMS("setsockopt SO_REUSEADDR failed, errorcode=%d",
                errno);
        }

        // Set TCP_NODELAY option for TCP sockets
        if ((0 == result) &&
            (PROTOCOL_TCP == transportProtocolType))
        {
            result = ::setsockopt(static_cast<int>(socketFd), IPPROTO_TCP, TCP_NODELAY,
                                   reinterpret_cast<const char*>(&optionOn), sizeof(optionOn));
            if (0 != result)
            {
                OCA_LOG_ERROR_PARAMS("setsockopt TCP_NODELAY failed, errorcode=%d",
                    errno);
            }
        }

        // Set the TTL for UDP sockets
        if ((0 == result) &&
            (PROTOCOL_UDP == transportProtocolType))
        {
            UINT32 ttl(32);
            result = ::setsockopt(static_cast<int>(socketFd), IPPROTO_IP, IP_MULTICAST_TTL,
                                    reinterpret_cast<const char*>(&ttl), sizeof(ttl));

            if (0 != result)
            {
                OCA_LOG_ERROR_PARAMS("setsockopt IP_MULTICAST_TTL failed, errorcode=%d",
                    errno);
            }
        }
    }
    return socketFd;
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

#ifndef __APPLE__
    return (bind(socket, (struct sockaddr *)&sin, sizeof(sin)) == 0);
#else
    bind(socket, (struct sockaddr *)&sin, sizeof(sin));
    return true;
#endif
}

bool Ocp1LiteSocketListen(INT32 socket, UINT8 backlog)
{
    assert(socket != INVALID_SOCKET);

    return (listen(socket, static_cast<int>(backlog)) == 0);
}

bool Ocp1LiteSocketAccept(INT32 socket, INT32& newsocket)
{
    int result;
    int optionOn(1);
#ifdef _WIN32
    SOCKADDR_INET newSocketAddress;

    PSOCKADDR pNewSocketAddress(PSOCKADDR(&newSocketAddress.Ipv4));
    INT32 newSocketAddressLength(sizeof(newSocketAddress.Ipv4));

    // It's important to clear the complete address structure including
    // alignment bytes, before setting each data member. The Socket API
    // uses one basic addressing structure with different overlays,
    // dependending on the address family.
    ::memset(&newSocketAddress, 0, sizeof(newSocketAddress));

    newsocket = static_cast<INT32>(static_cast<SOCKET>(::accept(static_cast<int>(socket), pNewSocketAddress, &newSocketAddressLength)));

    if (INVALID_SOCKET == newsocket)
    {
        INT32 error(errno);
        if ((WSAEINTR != error) && (WSAENOTSOCK != error))
        {
            OCA_LOG_ERROR_PARAMS("Socket error on accept, errorcode=%d", errno);
            return false;
        }
    }
#else 
    struct sockaddr_in socketAddr;
    int socketAddrLength = sizeof(socketAddr);
    memset(&socketAddr, 0, sizeof(socketAddr));
    newsocket = ::accept(socket, (struct sockaddr*)&socketAddr, (socklen_t*)&socketAddrLength);
#endif
    
    result = ::setsockopt(newsocket, SOL_SOCKET, SO_REUSEADDR, (char *)&optionOn, sizeof(optionOn));
    if (0 != result)
    {
        OCA_LOG_ERROR_PARAMS("setsockopt SO_REUSEADDR failed, errorcode=%d",
            errno);
    }

    // Set TCP_NODELAY option for TCP sockets
    if (0 == result)
    {
        result = ::setsockopt(newsocket, IPPROTO_TCP, TCP_NODELAY, (char *)&optionOn, sizeof(optionOn));
        if (0 != result)
        {
            OCA_LOG_ERROR_PARAMS("setsockopt TCP_NODELAY failed, errorcode=%d",
                errno);
        }
    }
    
    return (newsocket != INVALID_SOCKET);

}

bool Ocp1LiteSocketReject(INT32 socket)
{
    assert(socket != INVALID_SOCKET);
    
    struct sockaddr_in sin;
    socklen_t sinLen(sizeof(struct sockaddr_in));
    INT32 newSocket = accept(socket, (struct sockaddr *)&sin, &sinLen);

    if ((newSocket > INVALID_SOCKET) && (sinLen == sizeof(struct sockaddr_in)))
    {
#ifdef _WIN32
        return (closesocket(newSocket) == 0);
#else
        return (close(newSocket) == 0);
#endif
    }

    return false;
}

INT32 Ocp1LiteSocketSend(INT32 socket, const void* buffer, INT32 length)
{
    assert(socket != INVALID_SOCKET);
    return send(socket, reinterpret_cast<const char*>(buffer), length, 0);
}

INT32 Ocp1LiteSocketSendTo(INT32 socket, const void* buffer, INT32 length, const std::string& hostOrIp, UINT16 port)
{
    assert(socket != SOCKET_ERROR);
    struct sockaddr_in sockAddr;

    int socketAddressSize(sizeof(sockAddr));

    struct addrinfo hints;
    struct addrinfo* pAddressList(NULL);
    INT32 error;

    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;

    /* Get Address Info */
    error = getaddrinfo(hostOrIp.c_str(), NULL, &hints, &pAddressList);
    if (-1 != error)
    {
        struct sockaddr_in* resolvedAddress = (struct sockaddr_in*)pAddressList->ai_addr;
        sockAddr.sin_addr = resolvedAddress->sin_addr;

        error = sendto(socket, (char*)buffer, (int)length, 0, (struct sockaddr*)&sockAddr, socketAddressSize);
    }

    if (NULL != pAddressList)
    {
        freeaddrinfo(pAddressList);
    }
    return error;
}

INT32 Ocp1LiteSocketReceiveFrom(INT32 socket, void* buffer, INT32 length, std::string& fromIp, UINT16& recvFromPort)
{
#ifdef _WIN32
    SOCKADDR_INET remoteSocketAddress;
    PSOCKADDR pSocketAddress = PSOCKADDR(&remoteSocketAddress.Ipv4);
    INT32 socketAddressSize = sizeof(remoteSocketAddress.Ipv4);
    int result(recvfrom(socket, static_cast<char*>(buffer), length, 0, pSocketAddress, &socketAddressSize));

    recvFromPort = ntohs(remoteSocketAddress.Ipv4.sin_port);
    char address[25] = { 0 };
    sprintf(address, "%d.%d.%d.%d",
        (remoteSocketAddress.Ipv4.sin_addr.S_un.S_addr >> 0) & 0xFF,
        (remoteSocketAddress.Ipv4.sin_addr.S_un.S_addr >> 8) & 0xFF,
        (remoteSocketAddress.Ipv4.sin_addr.S_un.S_addr >> 16) & 0xFF,
        (remoteSocketAddress.Ipv4.sin_addr.S_un.S_addr >> 24) & 0xFF);
    static_cast<void>(fromIp.assign(address));
    return result;
#else
    struct sockaddr_in remoteSocketAddress;
    socklen_t sockaddr_size = sizeof(struct sockaddr_in);

    int result(recvfrom(socket, static_cast<char*>(buffer), length, 0, (struct sockaddr *) &remoteSocketAddress, &sockaddr_size));
    
    recvFromPort = ntohs(remoteSocketAddress.sin_port);
    char address[25] = { 0 };
    sprintf(address, "%d.%d.%d.%d",
        (remoteSocketAddress.sin_addr.s_addr >> 0) & 0xFF,
        (remoteSocketAddress.sin_addr.s_addr >> 8) & 0xFF,
        (remoteSocketAddress.sin_addr.s_addr >> 16) & 0xFF,
        (remoteSocketAddress.sin_addr.s_addr >> 24) & 0xFF);
    static_cast<void>(fromIp.assign(address));
    return result;
#endif
}

INT32 Ocp1LiteSocketReceive(INT32 socket, void* buffer, INT32 length)
{
    assert(socket != INVALID_SOCKET);

    return recv(socket, static_cast<char*>(buffer), length, 0);
}

bool Ocp1LiteSocketShutdown(INT32 socket)
{
    assert(socket != INVALID_SOCKET);
#ifdef _WIN32
    return (shutdown(socket, SD_SEND) == 0);
#else
    return (shutdown(socket, SHUT_WR) == 0);
#endif
}

bool Ocp1LiteSocketClose(INT32 socket)
{
    assert(socket != INVALID_SOCKET);
#ifdef _WIN32
    return (closesocket(socket) == 0);
#else
    return (close(socket) == 0);
#endif
}
