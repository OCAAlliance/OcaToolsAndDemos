/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The Ocp1 Host Interface implementation for a
 *                        Bultimo enabled platform.
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----
#ifdef _WIN32
#define htons(A) (((static_cast<UINT16>(A) & 0xff00) >> 8) | \
                  ((static_cast<UINT16>(A) & 0x00ff) << 8))
#define htonl(A) (((static_cast<UINT32>(A) & 0xff000000) >> 24) | \
                  ((static_cast<UINT32>(A) & 0x00ff0000) >> 8) | \
                  ((static_cast<UINT32>(A) & 0x0000ff00) << 8) | \
                  ((static_cast<UINT32>(A) & 0x000000ff) << 24))
#define htonll(A)(((static_cast<UINT64>(A) & 0xff00000000000000) >> 56) | \
                  ((static_cast<UINT64>(A) & 0x00ff000000000000) >> 40) | \
                  ((static_cast<UINT64>(A) & 0x0000ff0000000000) >> 24) | \
                  ((static_cast<UINT64>(A) & 0x000000ff00000000) >> 8) | \
                  ((static_cast<UINT64>(A) & 0x00000000ff000000) << 8) | \
                  ((static_cast<UINT64>(A) & 0x0000000000ff0000) << 24) | \
                  ((static_cast<UINT64>(A) & 0x000000000000ff00) << 40) | \
                  ((static_cast<UINT64>(A) & 0x00000000000000ff) << 56))

#define ntohs htons
#define ntohl htonl
#define ntohll htonll
#else
#include <netinet/in.h>

#define htonll(A)(A)
#define ntohll htonll
#endif

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

UINT64 ConvertFromNetworkOrderToHostByteOrder(UINT64 hostlonglong)
{
    return ntohll(hostlonglong);
}

UINT32 ConvertFromNetworkOrderToHostByteOrder(UINT32 hostlong)
{
    return ntohl(hostlong);
}

UINT16 ConvertFromNetworkOrderToHostByteOrder(UINT16 hostshort)
{
    return ntohs(hostshort);
}

UINT64 ConvertFromHostByteOrderToNetworkOrder(UINT64 netlonglong)
{
    return htonll(netlonglong);
}

UINT32 ConvertFromHostByteOrderToNetworkOrder(UINT32 netlong)
{
    return htonl(netlong);
}

UINT16 ConvertFromHostByteOrderToNetworkOrder(UINT16 netshort)
{
    return htons(netshort);
}
