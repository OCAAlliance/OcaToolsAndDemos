/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : sDNSd on LwIP for OCALite
 *
 */

// ---- Include system wide include files ----
#include <lwip/netif.h>
#include <lwip/tcpip.h>
#include <lwip/sockets.h>
#include <Stm32/HostInterface/OCA/OCP.1/LwIPAdapterOcaLite.h>
#include <SharedLibraries/tinymDNS/mdns.h>
#include <SharedLibraries/tinymDNS/mdnsd.h>
#include <Stm32/lib/UtilLib/Rs232Debug.h>

// ---- Include local include files ----
#include "FreeRTOS.h"
#include "semphr.h"
#include "tinymDNSWrapper.h"

// ---- Helper types and constants ----

// ---- Local data ----
extern xSemaphoreHandle mDNS_started;

/** The singleton reference to the adapter. */
::tinymDNSWrapper* tinymDNSWrapper::m_pSingleton(NULL);

// ---- Helper functions ----

// ---- Class Implementation ----
tinymDNSWrapper::tinymDNSWrapper()
    : m_myState(Uninitialized),
      m_name(),
      m_registrationType(),
      m_txtRecordList(),
      m_domain(),
      m_port(0),
      m_ipAddress(0),
      m_svc(NULL)
{
}

tinymDNSWrapper::~tinymDNSWrapper()
{
}

::tinymDNSWrapper& tinymDNSWrapper::GetInstance()
{
    if (NULL ==::tinymDNSWrapper::m_pSingleton)
    {
        tinymDNSWrapper::m_pSingleton = new ::tinymDNSWrapper;
    }

    return *tinymDNSWrapper::m_pSingleton;
}

void tinymDNSWrapper::FreeInstance()
{
    if (::tinymDNSWrapper::m_pSingleton != NULL)
    {
        m_pSingleton->Teardown();

        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

bool tinymDNSWrapper::Initialize()
{
    LwIPAdapterOcaLite::GetInstance().AddMember(LwIPAdapterOcaLite::MEMBER_LINK_STATUS, this);

    m_myState = Idle;

    return true;
}

void tinymDNSWrapper::Teardown()
{
    LwIPAdapterOcaLite::GetInstance().RemoveMember(LwIPAdapterOcaLite::MEMBER_LINK_STATUS, this);

    DeleteService();

    m_myState = Uninitialized;
}

bool tinymDNSWrapper::Register(const std::string& name, const std::string& registrationType,
                             UINT16 port, const std::vector<std::string>& txtRecordList, const std::string& domain)
{
    bool bSuccess(HandleEvent(RegisterEvt));

    if (bSuccess)
    {
        m_name = name;
        m_registrationType = registrationType;
        m_port = port;
        m_txtRecordList = txtRecordList;
        m_domain = domain;
    }
    
    return bSuccess;
}

bool tinymDNSWrapper::Unregister()
{
    m_name.clear();
    m_registrationType.clear();
    m_port = 0;
    m_txtRecordList.clear();
    m_domain.clear();

    return HandleEvent(UnregisterEvt);
}

bool tinymDNSWrapper::HandleEvent(mDNSdEvent event)
{
    bool bSuccess(false);

    switch(m_myState)
    {
    case Idle:
        bSuccess = HandleIdleState(event);
        break;
    case NewAndUp:
        bSuccess = HandleNewAndUpState(event);
        break;
    case NewAndDown:
        bSuccess = HandleNewAndDownState(event);
        break;
    case RunAndUp:
        bSuccess = HandleRunAndUpState(event);
        break;
    case RunAndDown:
        bSuccess = HandleRunAndDownState(event);
        break;
    case StopAndDown:
        bSuccess = HandleStopAndDownState(event);
        break;
    default:
        break;
    }

    return bSuccess;
}

bool tinymDNSWrapper::HandleIdleState(mDNSdEvent event)
{
    bool bSuccess(true);

    switch(event)
    {
    case RegisterEvt:
        if (LwIPAdapterOcaLite::GetInstance().IsLinkUpAndAddressAssigned())
        {
            m_myState = NewAndUp;
        }
        else
        {
            m_myState = NewAndDown;
        }
        break;
    case UnregisterEvt:
    case LinkUpEvt:
    case LinkDownEvt:
    case PublishEvt:
    default:
        break;
    }
    return bSuccess;
}

bool tinymDNSWrapper::HandleNewAndUpState(mDNSdEvent event)
{
    bool bSuccess(true);

    switch(event)
    {
    case RegisterEvt:
        bSuccess = false; //First unregister current service!
        break;
    case UnregisterEvt:
        m_myState = Idle;
        break;
    case LinkDownEvt:
        m_myState = NewAndDown;
        break;
    case PublishEvt:
        m_myState = RunAndUp;
        break;
    case LinkUpEvt:
    default:
        break;
    }
    return bSuccess;
}

bool tinymDNSWrapper::HandleNewAndDownState(mDNSdEvent event)
{
    bool bSuccess(true);

    switch(event)
    {
    case RegisterEvt:
        bSuccess = false; //First unregister current service!
        break;
    case UnregisterEvt:
        m_myState = Idle;
        break;
    case LinkUpEvt:
        m_myState = NewAndUp;
        break;
    case LinkDownEvt:
    case PublishEvt:
    default:
        break;
    }
    return bSuccess;
}

bool tinymDNSWrapper::HandleRunAndUpState(mDNSdEvent event)
{
    bool bSuccess(true);

    switch(event)
    {
    case RegisterEvt:
        bSuccess = false; //First unregister current service!
        break;
    case UnregisterEvt:
        DeleteService();
        m_myState = Idle;
        break;
    case LinkDownEvt:
        m_myState = RunAndDown;
        break;
    case LinkUpEvt:
		CreateService();
        if (NULL != m_svc)
        {
            m_myState = RunAndUp;
        }
        break;
    case PublishEvt:
    default:
        break;
    }
    return bSuccess;
}

bool tinymDNSWrapper::HandleRunAndDownState(mDNSdEvent event)
{
    bool bSuccess(true);

    switch(event)
    {
    case RegisterEvt:
        bSuccess = false; //First unregister current service!
        break;
    case UnregisterEvt:
        m_myState = StopAndDown;
        break;
    case LinkUpEvt:
		CreateService();
        if (NULL != m_svc)
        {
            m_myState = RunAndUp;
        }
        break;
    case LinkDownEvt:
    case PublishEvt:
    default:
        // Ignore (NOP)
        break;
    }
    return bSuccess;
}

bool tinymDNSWrapper::HandleStopAndDownState(mDNSdEvent event)
{
    bool bSuccess(true);

    switch(event)
    {
    case RegisterEvt:
        m_myState = RunAndDown;
        break;
    case LinkUpEvt:
        DeleteService();
        m_myState = Idle;
        break;
    case UnregisterEvt:
    case LinkDownEvt:
    case PublishEvt:
    default:
        break;
    }
    return bSuccess;
}

bool reg_empty(false);

void tinymDNSWrapper::CreateService()
{    
    reg_empty = m_registrationType.empty();
//    if (!m_registrationType.empty() && (NULL == m_svc))
    if (!reg_empty && (NULL == m_svc))
     {
        // Start and wait for the semaphore
        ::mdnsd_start();
        (void)::xSemaphoreTake(mDNS_started, portMAX_DELAY);

        // Format all stuff
        char hostname[64] = {0};
        ::strcpy(hostname, std::string(m_name.c_str() + std::string(".local")).c_str());

        m_ipAddress = ::LwIPAdapterOcaLite::GetInstance().GetIpAddress();
        ::mdnsd_set_hostname(server, hostname, m_ipAddress);

        const char** txt = new const char*[m_txtRecordList.size() + 1];
        for (size_t i(0); i < m_txtRecordList.size(); i++)
        {
            txt[i] = m_txtRecordList[i].c_str();
        }
        txt[m_txtRecordList.size()] = NULL;
        
        // Create the registration
        m_svc = ::mdnsd_register_svc(server, m_name.c_str(), (m_registrationType + std::string(".") + m_domain).c_str(), m_port, NULL, txt);

        // Cleanup
        delete txt;
    }
}

void tinymDNSWrapper::DeleteService()
{
    if (NULL != m_svc)
    {
        ::mdns_service_destroy(m_svc);
        m_svc = NULL;
    }
}

UINT32 tinymDNSWrapper::Run()
{
    if (m_pSingleton->m_myState == NewAndUp)
    {
		CreateService();
        if (NULL != m_svc)
        {
            HandleEvent(PublishEvt);
        }
    }
    return 0;
}

bool tinymDNSWrapper::MemberCallback(UINT32 data)
{
    (void)data;

    if (LwIPAdapterOcaLite::GetInstance().IsLinkUpAndAddressAssigned())
    {
        static_cast<void>(HandleEvent(LinkUpEvt));
    }
    else
    {
        static_cast<void>(HandleEvent(LinkDownEvt));
    }

    return true;
}
