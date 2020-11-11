/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The DNSSDResolver class.
 */
#ifndef DNSSD_RESOLVER_H
#define DNSSD_RESOLVER_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>
#define _WINSOCKAPI_
#include <../platform/common/BonjourSDK/dns_sd.h>

// ---- Include local include files ----
#include "ResolveResult.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

class DNSSDResolver
{
public:

    /**
     * Constructor
     *
     * @param[in] deviceName    The device name to resolve.
     * @param[in] serviceType   The service type to resolve.
     * @param[in] timeout       The timeout in seconds to use for the different calls.
     */
    DNSSDResolver(const std::string& deviceName,
                  const std::string& serviceType,
                  UINT32 timeout);

    /**
     * Check whether bonjour is installed on the current system
     *
     * @return True if the Bonjour service is found with the installed services. False if not.
     */
    static bool bIsBonjourInstalled();

    /**
     * Start resolving the device for the service.
     *
     * @param[out] result   The found information.
     * 
     * @return True iff resolving the device succeeded.
     */
    bool StartResolving(::ResolveResult& result);


    /**
     * Reconfirm the record.
     * This call will only succeed if the device is already resolved.
     *
     * @return True iff successfull. After this a new call can be made to StartResolving.
     */
    bool Reconfirm();

protected:
private:

    /** DNS-SD callback handler */
    void OnDNSServiceResolveReply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char* fullname, const char* hosttarget,
        uint16_t port, uint16_t txtLen, const unsigned char* txtRecord, void* context);

    /** DNS-SD callback handler */
    static void DNSSD_API DNSServiceResolveReply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char* fullname, const char* hosttarget,
        uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void* context);

    /** DNS-SD callback handler */
    static void DNSSD_API DNSServiceQueryRecordReply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname,
                                                     uint16_t rrtype, uint16_t rrclass, uint16_t rdlen, const void *rdata, uint32_t ttl, void *context);

    /** The device name to resolve */
    std::string     m_deviceName;
    /** The service type to resolve */
    std::string     m_serviceType;
    /** The timeout to use when resolving */
    UINT32          m_timeout;
    /** Track whether the resolve result is valid */
    bool            m_bResolved;
    /** Resolve result */
    ::ResolveResult m_resolveResult;
    /** True iff bonjour is installed */
    static bool     m_bInstalled;
    
    /** private copy constructor, no copying of object allowed */
    DNSSDResolver(const ::DNSSDResolver&);
    /** private assignment operator, no assignment of object allowed */
    DNSSDResolver& operator=(const ::DNSSDResolver&);
};

#endif