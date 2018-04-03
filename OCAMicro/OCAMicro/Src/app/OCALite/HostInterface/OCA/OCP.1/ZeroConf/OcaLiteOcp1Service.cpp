/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLite Ocp1Service implementation.
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/ZeroConf/IOcp1LiteService.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <../../platform/common/OcaLite/BonjourSDK/dns_sd.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----
/** The maximum length of a TXT record. */
#define MAXIMUM_TXT_RECORD_LENGTH 255
// ---- Class Implementation ----

static DNSServiceRef m_dnsService = NULL;

/**
 * Registration reply callback. Dummy implementation.
 */ 
static void DNSSD_API DNSServiceRegisterReply2(DNSServiceRef sdRef, DNSServiceFlags flags, DNSServiceErrorType errorCode, const char* name, const char* regtype, const char* domain, void* context)
{
}

bool Ocp1LiteServiceRegister(const std::string& name, const std::string& registrationType,
                             UINT16 port, const std::vector<std::string>& txtRecordList, const std::string& domain)
{
    OCA_LOG_TRACE_PARAMS("Register(name = %s, registrationType = %s, port = %u, txtRecordList.size() = %u, domain = %s)",
            name.c_str(), registrationType.c_str(), port, txtRecordList.size(), domain.c_str());


    DNSServiceErrorType error((NULL != m_dnsService) ? kDNSServiceErr_Invalid : kDNSServiceErr_NoError);

    if (kDNSServiceErr_NoError == error)
    {
        UINT8 txtRecord[MAXIMUM_TXT_RECORD_LENGTH] = {0};
        UINT32 recordLength(0);

        if (!txtRecordList.empty())
        {
            //Calculate the TXT record total length
            std::vector<std::string>::const_iterator txtRecordIter(txtRecordList.begin());
            while (txtRecordList.end() != txtRecordIter)
            {
                recordLength += static_cast<UINT32>(txtRecordIter->length() + 1 /* Length byte */);
                ++txtRecordIter;
            }

            if (recordLength < MAXIMUM_TXT_RECORD_LENGTH)
            {
                UINT16 currentPos(0);
                txtRecordIter = txtRecordList.begin();
                while (txtRecordList.end() != txtRecordIter)
                {
                    // Write the length byte
                    txtRecord[currentPos] = static_cast<UINT8>(txtRecordIter->length());
                    currentPos++;
                    INT32 result(::memcpy_s(&(txtRecord[currentPos]),
                                            static_cast<size_t>(MAXIMUM_TXT_RECORD_LENGTH - currentPos),
                                            txtRecordIter->c_str(),
                                            txtRecordIter->length()));
                    if (0 != result)
                    {
                        OCA_LOG_ERROR_PARAMS("Error copying TXT Record, errorcode=%d", result);
                        error = kDNSServiceErr_Unknown;
                        break;
                    }
                    else
                    {
                        currentPos += static_cast<UINT8>(txtRecordIter->length());
                    }
                    ++txtRecordIter;
                }
            }
            else
            {
                OCA_LOG_ERROR("TXT Record is too large");
                error = kDNSServiceErr_BadParam;
            }
        }

        if (kDNSServiceErr_NoError == error)
        {
            error = ::DNSServiceRegister(&m_dnsService, 0, 0, name.c_str(),
                registrationType.c_str(), domain.c_str(), NULL, htons(port), static_cast<UINT16>(recordLength),
                txtRecord, &DNSServiceRegisterReply2, NULL);
        }
    }
    return (kDNSServiceErr_NoError == error) ? true : false;
}

int Ocp1LiteServiceGetSocket()
{
    int socketFd = -1;
    if (NULL != m_dnsService)
    {
        socketFd = static_cast<int>(::DNSServiceRefSockFD(m_dnsService));
    }
    return socketFd;
}

void Ocp1LiteServiceRunWithFdSet(fd_set* readSet)
{
    if (NULL != m_dnsService)
    {
        int dnsServiceSocket(static_cast<int>(::DNSServiceRefSockFD(m_dnsService)));

        if (FD_ISSET(dnsServiceSocket, readSet))
        {
            DNSServiceErrorType error(::DNSServiceProcessResult(m_dnsService));
            if (error != kDNSServiceErr_NoError)
            {
                OCA_LOG_ERROR_PARAMS("Failed to process dns service. Result %d", error);
            }
        }
    }
}

void Ocp1LiteServiceRun()
{
    if (NULL != m_dnsService)
    {
        fd_set readFds;
        struct timeval tv = { 0 , 0 };
        SOCKET dnsServiceSocket(static_cast<SOCKET>(::DNSServiceRefSockFD(m_dnsService)));

        FD_ZERO(&readFds);
        FD_SET(dnsServiceSocket, &readFds);


        int result = ::select(0, &readFds, NULL, NULL, &tv);
        if (1 == result)
        {
            DNSServiceErrorType error(::DNSServiceProcessResult(m_dnsService));
            if (error != kDNSServiceErr_NoError)
            {
                OCA_LOG_ERROR_PARAMS("Failed to process dns service. Result %d", error);
            }
        }
    }
}

void Ocp1LiteServiceDispose(void)
{
    if (NULL != m_dnsService)
    {
        DNSServiceRefDeallocate(m_dnsService);
        m_dnsService = NULL;
    }

}
