/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : LwIPAdapterOcaLite
 *
 */

// ---- Include system wide include files ----
#include <algorithm>

#include "stm32f2xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ethernetif.h"
#include <common/OcaLite/LogicalDevices/Switch/Switch.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <lwip/netif.h>
#include <lwip/tcpip.h>
#include <lwip/dhcp.h>
#include <Stm32CortexM3/lib/UtilLib/Rs232Debug.h>
//#include <Stm32CortexM4/lib/UtilLib/Gpio.h>
#include <Stm32CortexM3/lib/UtilLib/Timer.h>

// ---- Include local include files ----
#include "LwIPAdapterOcaLite.h"
#include "mac_addr.h"

// ---- Helper types and constants ----

/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1 //0
#define IP_ADDR3   12 //10
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1

#define MAX_DHCP_TRIES  4

uint8_t g_mac_addr[MAC_ADDR_SIZE];

extern uint8_t g_static_ip_addr[];
extern uint8_t g_static_netmask_addr[];
extern uint8_t g_static_gw_addr[];


/** The singleton reference to the adapter. */
::LwIPAdapterOcaLite* LwIPAdapterOcaLite::m_pSingleton(NULL);

/** LwIP network interface structure. */
static struct netif s_netif;

// ---- Local data ----

// ---- Helper functions ----
/**
  * @brief This function is the LwIpAdapterTask task, it runs on a timer. 
  * It calls SwitchDriver::GetStatus() to trigger a fetch of the link status.
  * When the status has been fetched the semaphore is released.
  *
  * @param NULL
  */
static void LwIpAdapterTask(void *argument)
{
    UINT32 delay;
    portTickType ticks;

    (void)argument;

    for(;;)
    {
        delay = LwIPAdapterOcaLite::GetInstance().Run();
        ticks = portTickType(delay / portTICK_RATE_MS);
        vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
    }
}

static void CancelDhcpOperation(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;	

    /* Stop DHCP */
    dhcp_stop(&s_netif);

    /* Static address used */
    IP4_ADDR(&ipaddr, g_static_ip_addr[0], g_static_ip_addr[1], g_static_ip_addr[2], g_static_ip_addr[3]);
    IP4_ADDR(&netmask, g_static_netmask_addr[0], g_static_netmask_addr[1], g_static_netmask_addr[2], g_static_netmask_addr[3]);
    IP4_ADDR(&gw, g_static_gw_addr[0], g_static_gw_addr[1], g_static_gw_addr[2], g_static_gw_addr[3]);
    netif_set_addr(&s_netif, &ipaddr , &netmask, &gw);

    /* Log new address */
    Rs232DebugPrint ("   Static IP address: %d.%d.%d.%d\r\n", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
}

// ---- Class Implementation ----
LwIPAdapterOcaLite::LwIPAdapterOcaLite()
    : m_bInitialized(false),
      m_latestLinkUpState(false),
      m_linkStateRefreshTime(0),
      m_SwitchQueryState(STATUS_UNKNOWN),
      m_DhcpState(DHCP_LINK_DOWN),
      m_StopDhcpService(false)
{
    static_cast<void>(m_members.insert(MemberMap::value_type(static_cast<UINT32>(MEMBER_ALWAYS), MemberList())));
    static_cast<void>(m_members.insert(MemberMap::value_type(static_cast<UINT32>(MEMBER_LINK_STATUS), MemberList())));
}

LwIPAdapterOcaLite::~LwIPAdapterOcaLite()
{
}

::LwIPAdapterOcaLite& LwIPAdapterOcaLite::GetInstance()
{
    if (NULL ==::LwIPAdapterOcaLite::m_pSingleton)
    {
        LwIPAdapterOcaLite::m_pSingleton = new ::LwIPAdapterOcaLite;
    }

    return *LwIPAdapterOcaLite::m_pSingleton;
}

void LwIPAdapterOcaLite::FreeInstance()
{
    if (::LwIPAdapterOcaLite::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}
static const int8_t dhcpClient[] = "DHCPClient";

bool LwIPAdapterOcaLite::Initialize()
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;	

    // depending on implementation the MAC may be stored in the 207 OTP, or
    // it could be stored in an I2C EEPROM.  Make sure the I2C has been init'ed and the 
    // persistent variables loaded prior to calling getMAC
    getMAC(g_mac_addr);

    // Initialize tcp_ip service (has to be done from a task)
    tcpip_init(NULL, NULL);

    // Configure netif (has to be done from a task)
    // - IP address default setting
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    /* set netif MAC hardware address */
    s_netif.hwaddr[0] =  g_mac_addr[0];
    s_netif.hwaddr[1] =  g_mac_addr[1];
    s_netif.hwaddr[2] =  g_mac_addr[2];
    s_netif.hwaddr[3] =  g_mac_addr[3];
    s_netif.hwaddr[4] =  g_mac_addr[4];
    s_netif.hwaddr[5] =  g_mac_addr[5];

    // - Add the network interface to the netif_list, passing MAC device init and input method.
    netif_add(&s_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
    // - Registers the default network interface.
    netif_set_default(&s_netif);

    // - Starting up the interface is done by the adapter task ...
//    osThreadDef(LwIpAdapter, LwIpAdapterTask, osPriorityBelowNormal, 1, configMINIMAL_STACK_SIZE * 5 + RS232_DEBUG_WORDS);
//    osThreadCreate (osThread(LwIpAdapter), NULL);
    (void) xTaskCreate(LwIpAdapterTask, dhcpClient , configMINIMAL_STACK_SIZE * 3, NULL,(tskIDLE_PRIORITY + 3), NULL);

    m_bInitialized = true;
    return m_bInitialized;
}

void LwIPAdapterOcaLite::Teardown() const
{
}

void LwIPAdapterOcaLite::AddMember(MemberType memberType, ::IMember* pMember)
{
    ::IMember::MemberList::iterator listIter(std::find(m_members[memberType].begin(), m_members[memberType].end(), pMember));

    if (m_members[memberType].end() == listIter)
    {
        m_members[memberType].push_back(pMember);
    }
}

void LwIPAdapterOcaLite::RemoveMember(MemberType memberType, ::IMember* pMember)
{
    ::IMember::MemberList::iterator listIter(std::find(m_members[memberType].begin(), m_members[memberType].end(), pMember));

    if (m_members[memberType].end() != listIter)
    {
        static_cast<void>(m_members[memberType].erase(listIter));
    }
}

UINT8* LwIPAdapterOcaLite::GetMacAddress()
{
    return s_netif.hwaddr;
}

UINT32 LwIPAdapterOcaLite::GetIpAddress()
{
    return s_netif.ip_addr.addr;
}

UINT32 LwIPAdapterOcaLite::GetNetMask()
{
    return s_netif.netmask.addr;
}

UINT32 LwIPAdapterOcaLite::GetDefaultGateway()
{
    return s_netif.gw.addr;
}

bool LwIPAdapterOcaLite::IsLinkUpAndAddressAssigned()
{
    return((m_latestLinkUpState) &&
           ((m_DhcpState == DHCP_ADDRESS_ASSIGNED) || (m_DhcpState == DHCP_INACTIVE) ||(m_DhcpState == DHCP_TIMEOUT)));
}

void LwIPAdapterOcaLite::SetDhcpMode(bool active)
{
    if (active)
    {
        if (m_DhcpState == DHCP_INACTIVE)
        {
            m_DhcpState = DHCP_START;
        }
    }
    else
    {
        if (m_DhcpState != DHCP_INACTIVE)
        {
            m_StopDhcpService = true;
        }
    }
}

UINT32 LwIPAdapterOcaLite::Run()
{
    UINT32 delay(2000); // seconds
    bool networkStatusChanged(false);
    
    // Check user input
    if (m_StopDhcpService)
    {
        m_DhcpState = DHCP_INACTIVE;
    }
    
    // Check Switch actions
    switch(m_SwitchQueryState)
    {
    case STATUS_REFRESHED:
        {
            bool bLinkUp = Switch::GetInstance().GetLinkUp();

            if (m_latestLinkUpState != bLinkUp)
            {
                // State changed, inform netif.
                if (bLinkUp)
                {
                    /* Set link up state: the Orange LED. */
//                    ::Gpio_LedOn(LED2);

                    /* Update netif */
                    netif_set_link_up(&s_netif);
                    printf("Network: Link Up\r\n");

                    /* Start DHCP server (if configured) */
                    if (m_DhcpState != DHCP_INACTIVE)
                    {
                        m_DhcpState = DHCP_START;
                    }
                }
                else
                {
                    /* Set link down state: the Orange LED. */
//                    ::Gpio_LedOff(LED2);

                    /* Update netif */
                    netif_set_link_down(&s_netif);
                    printf("Network: Link Down\r\n");

                    /* Stop DHCP server (if active) */
                    if (m_DhcpState != DHCP_INACTIVE)
                    {
                        m_DhcpState = DHCP_LINK_DOWN;
                    }
                }
                networkStatusChanged = true;
                m_latestLinkUpState = bLinkUp;
            }
            m_linkStateRefreshTime = TimerGetSysTick();
            m_SwitchQueryState = STATUS_KNOWN;
        } 
        break;
    case STATUS_KNOWN:
        {
            if (INTERVAL_ELAPSED(m_linkStateRefreshTime, 2000))
            {
                m_SwitchQueryState = QUERY_STATUS;
            }
        } 
        break; 
    case QUERY_STATUS:
        break; 
    default:
        m_SwitchQueryState = QUERY_STATUS;
        break;
    }
    
    // Check DHCP actions
    switch(m_DhcpState)
    {
    case DHCP_INACTIVE:
        {
            if (m_StopDhcpService)
            {
                // Cancel DHCP operation
                CancelDhcpOperation();
                
                networkStatusChanged = true;
                m_StopDhcpService = false;
            }
        } 
        break;
    case DHCP_START:
        {
            // Start DHCP client
            s_netif.ip_addr.addr = 0;
            s_netif.netmask.addr = 0;
            s_netif.gw.addr = 0;
            dhcp_start(&s_netif);

            m_DhcpState = DHCP_WAIT_ADDRESS;
            delay = 250; // Check status after 250 msec.
        } break;
    case DHCP_WAIT_ADDRESS:
        {
            // Watch progress by checking IP address */
            if (s_netif.ip_addr.addr != 0) 
            {
                // Stop DHCP client
                dhcp_stop(&s_netif);

                // Log new address
                {
                    uint32_t ipAddres = s_netif.ip_addr.addr;
                    uint8_t iptab[4];

                    iptab[0] = (uint8_t)(ipAddres >> 24);
                    iptab[1] = (uint8_t)(ipAddres >> 16);
                    iptab[2] = (uint8_t)(ipAddres >> 8);
                    iptab[3] = (uint8_t)(ipAddres);
                	bool bLinkLocal = (iptab[3] == 169);
                    if(!bLinkLocal)
                    	Rs232DebugPrint ("IP address assigned by a DHCP server: ");
                    else
                    	Rs232DebugPrint ("No DHCP server found, using link local address: ");
					Rs232DebugPrint ("%d.%d.%d.%d\r\n", iptab[3], iptab[2], iptab[1], iptab[0]);
                }
                networkStatusChanged = true;
                m_DhcpState = DHCP_ADDRESS_ASSIGNED;	
            }
            else
            {
                /* DHCP timeout */
                if (s_netif.dhcp->tries > MAX_DHCP_TRIES)
                {
                    // Cancel DHCP operation
                    CancelDhcpOperation();

                    networkStatusChanged = true;
                    m_DhcpState = DHCP_TIMEOUT;
                }
                else
                {
                    // Log status
                    {
                        static UINT8 s_num = 0;
                        if (s_num != s_netif.dhcp->tries)
                        {
                            s_num = s_netif.dhcp->tries;
                        }
                    }
                    delay = 250; // Check status after 250 msec.
                }
            }
        } 
        break; 
    case DHCP_ADDRESS_ASSIGNED:
    case DHCP_TIMEOUT:
    case DHCP_LINK_DOWN:
    default:
        { } 
        break;
    }
    
    // Call members as requested ...
    IMember::MemberMap::const_iterator mapIter(m_members.find(MEMBER_ALWAYS));
    if (m_members.end() != mapIter)
    {
        InformMembers(mapIter);
    }
    if (networkStatusChanged)
    {
        mapIter = m_members.find(MEMBER_LINK_STATUS);
        if (m_members.end() != mapIter)
        {
            InformMembers(mapIter);
        }
    }
    
    // Query for link status ...
    if (m_SwitchQueryState == QUERY_STATUS)
    {
        delay = 100; // Check response after 100 msec.
        m_SwitchQueryState = STATUS_REFRESHED;
//        ::Switch::GetInstance().QueryLinkState(this);
    }

    return delay;
}

void LwIPAdapterOcaLite::InformMembers(IMember::MemberMap::const_iterator &mapIter)
{
    IMember::MemberList memberList(mapIter->second);

    MemberList::iterator memberIter(memberList.begin());
    while (memberIter != memberList.end())
    {
        if (NULL != *memberIter)
        {
            static_cast<void>((*memberIter)->MemberCallback(0));
        }
        memberIter++;
    }
}

bool LwIPAdapterOcaLite::MemberCallback(UINT32 data)
{
    (void)data;
    m_SwitchQueryState = STATUS_REFRESHED;
    return true;
}
