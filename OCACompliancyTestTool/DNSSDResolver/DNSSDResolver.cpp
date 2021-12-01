/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLite DNSSDResolver implementation.
 *
 */

 // ---- Include system wide include files ----
#include <../common/HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <../common/HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>
#include <../common/OCALite/OCF/OcaLiteCommandHandlerController.h>
#include <assert.h>

 // ---- FileInfo Macro ----

 // ---- Include local include files ----
#include "DNSSDResolver.h"

 // ---- Helper types and constants ----
#define LOCAL_DOMAIN "local."
 // ---- Helper functions ----

 // ---- Local data ----
bool DNSSDResolver::m_bInstalled(false);

 // ---- Class Implementation ----

DNSSDResolver::DNSSDResolver(const std::string& deviceName,
                             const std::string& serviceType,
                             UINT32 timeout) 
    : m_deviceName(deviceName),
      m_serviceType(serviceType),
      m_timeout(timeout <= 1 ? 2 : timeout),
      m_bResolved(false),
      m_resolveResult()
{   
}

bool DNSSDResolver::bIsBonjourInstalled()
{
    if (!m_bInstalled)
    {
        ::SC_HANDLE scmHandle(::OpenSCManager(0, 0, GENERIC_READ));
        if (scmHandle)
        {
            ::SC_HANDLE serviceHandle(::OpenService(scmHandle, L"Bonjour Service", GENERIC_READ));
            if (!serviceHandle)
            {
                OCA_LOG_ERROR_PARAMS("[DNSSD] Bonjour service does not exist. (Error = %d)", GetLastError());
            }
            else
            {
                m_bInstalled = true;

                // TODO We could use Query Status to see whether the service is also running.
                OCA_LOG_TRACE("[DNSSD] Bonjour service does exist.");
                ::CloseServiceHandle(serviceHandle);
            }
            ::CloseServiceHandle(scmHandle);
        }
        else
        {
            OCA_LOG_ERROR_PARAMS("[DNSSD] Failed to OpenSCManager: %d", GetLastError());
        }
    }

    return m_bInstalled;
}

bool DNSSDResolver::StartResolving(ResolveResult& result)
{
    if (bIsBonjourInstalled() &&
        !m_deviceName.empty() &&
        !m_serviceType.empty() &&
        !m_bResolved)
    {
        ::DNSServiceRef serviceRef;
        ::DNSServiceErrorType result(::DNSServiceResolve(&serviceRef,
                                                         0, // Flags
                                                         kDNSServiceInterfaceIndexAny,
                                                         m_deviceName.c_str(),
                                                         m_serviceType.c_str(),
                                                         LOCAL_DOMAIN,
                                                         DNSSDResolver::DNSServiceResolveReply,
                                                         this));

        if (kDNSServiceErr_NoError == result)
        {
            // Process the socket till we have the result.
            int socket(DNSServiceRefSockFD(serviceRef));

            UINT32 beginTime(::OcfLiteTimerGetTimerTickCount());

            while (!m_bResolved &&
                ((beginTime + ((m_timeout - 1) * 1000)) > ::OcfLiteTimerGetTimerTickCount()) &&
                (kDNSServiceErr_NoError == result))
            {
                fd_set readSet;
                FD_ZERO(&readSet);
                FD_SET(socket, &readSet);

                timeval timeout = { 0, 100 };
                int selectResult(::select(socket + 1, &readSet, NULL, NULL, &timeout));
                if (selectResult > 0)
                {
                    result = ::DNSServiceProcessResult(serviceRef);
                    if (kDNSServiceErr_NoError != result)
                    {
                        OCA_LOG_ERROR_PARAMS("[DNSSD] Failed to DNSServiceProcessResult for DNSServiceResolve. Error=%d", result);
                    }
                }

                ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(1);
            }
        }

        ::DNSServiceRefDeallocate(serviceRef);
    }

    if (m_bResolved)
    {
        result = m_resolveResult;
    }
    return m_bResolved;
}

bool DNSSDResolver::Reconfirm()
{
    bool bResult(false);
    if (m_bResolved)
    {
        ::DNSServiceRef srvRecord;
        ::DNSServiceErrorType result(::DNSServiceQueryRecord(&srvRecord,
                                                             kDNSServiceFlagsForce,
                                                             m_resolveResult.GetInterfaceIndex(),
                                                             m_resolveResult.GetFullName().c_str(),
                                                             kDNSServiceType_SRV,
                                                             kDNSServiceClass_IN,
                                                             DNSSDResolver::DNSServiceQueryRecordReply,
                                                             this));

        if (kDNSServiceErr_NoError == result)
        {
            // Process the socket till we have the result.
            int socket(::DNSServiceRefSockFD(srvRecord));

            UINT32 beginTime(::OcfLiteTimerGetTimerTickCount());
            while (((beginTime + ((m_timeout - 1) * 1000)) > ::OcfLiteTimerGetTimerTickCount()) &&
                (kDNSServiceErr_NoError == result))
            {
                fd_set readSet;
                FD_ZERO(&readSet);
                FD_SET(socket, &readSet);

                timeval timeout = { 1, 0 };
                int selectResult(::select(socket + 1, &readSet, NULL, NULL, &timeout));
                if (selectResult > 0)
                {
                    result = ::DNSServiceProcessResult(srvRecord);
                    if (kDNSServiceErr_NoError == result)
                    {
                        bResult = true;
                        break;
                    }
                    else
                    {
                        OCA_LOG_ERROR_PARAMS("Failed to DNSServiceProcessResult for DNSServiceQueryRecord. Error=%d", result);
                    }
                }
            }

            ::DNSServiceRefDeallocate(srvRecord);
        }
    }
    return bResult;
}

void DNSSDResolver::OnDNSServiceResolveReply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname, const char *hosttarget,
                                             uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void *context)
{
    OCA_LOG_INFO("[DNSSD] OnDNSServiceResolveReply:");
    OCA_LOG_INFO_PARAMS("[DNSSD]  - errorCode      = %d", errorCode);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - interfaceIndex = %d", interfaceIndex);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - flags          = %d", flags);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - fullname       = %s", fullname);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - hosttarget     = %s", hosttarget);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - port           = %d", port);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - txtLen         = %d", txtLen);

    m_resolveResult.SetResolveResult(interfaceIndex, fullname, hosttarget, port, txtLen, txtRecord);

    m_bResolved = true;
}


void DNSSDResolver::DNSServiceResolveReply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname, const char *hosttarget, 
                                           uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void *context)
{
    if (errorCode != kDNSServiceErr_NoError)
    {
        OCA_LOG_ERROR_PARAMS("[DNSSD] DNSServiceResolveReply. errorCode=%d", errorCode);
    }
    else
    {
        OCA_LOG_INFO_PARAMS("[DNSSD] DNSServiceResolveReply. errorCode=%d", errorCode);
    }

    DNSSDResolver* pDNSSDResolver(static_cast<DNSSDResolver*>(context));
    assert(NULL != pDNSSDResolver);
    pDNSSDResolver->OnDNSServiceResolveReply(sdRef, flags, interfaceIndex, errorCode, fullname, hosttarget, port, txtLen, txtRecord, context);
}

void DNSSDResolver::DNSServiceQueryRecordReply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname,
                                               uint16_t rrtype, uint16_t rrclass, uint16_t rdlen, const void *rdata, uint32_t ttl, void *context)
{
    OCA_LOG_INFO("[DNSSD] DNSServiceQueryRecordReply:");
    OCA_LOG_INFO_PARAMS("[DNSSD]  - errorCode      = %d", errorCode);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - interfaceIndex = %d", interfaceIndex);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - flags          = %d", flags);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - fullname       = %s", fullname);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - rrtype         = %d", rrtype);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - rrclass        = %d", rrclass);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - rdlen          = %d", rdlen);
    OCA_LOG_INFO_PARAMS("[DNSSD]  - ttl            = %d", ttl);
    if (errorCode == kDNSServiceErr_NoError)
    {
        ::DNSServiceErrorType result(::DNSServiceReconfirmRecord(kDNSServiceFlagsForce, interfaceIndex, fullname, rrtype, rrclass, rdlen, rdata));
        OCA_LOG_INFO_PARAMS("[DNSSD] DNSServiceReconfirmRecord(fullname=%s). Result=%d", fullname, result);

        DNSSDResolver* pDNSSDResolver(static_cast<DNSSDResolver*>(context));
        assert(NULL != pDNSSDResolver);

        pDNSSDResolver->m_bResolved = false;
    }
}
