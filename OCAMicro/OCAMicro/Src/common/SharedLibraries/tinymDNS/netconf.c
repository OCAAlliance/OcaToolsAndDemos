/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Network connection configuration
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "lwip/mem.h"
#include "lwip/dhcp.h"
#include "lwip/netifapi.h"
#include "lwip/ip_addr.h"
#include "ethernetif.h"
#include "main.h"
#include "netconf.h"
#include "tcpip.h"
#include "semphr.h"
#include <string.h>

#include "stm32f2x7_eth.h"
#include "mac_addr.h"

uint8_t g_mac_addr[MAC_ADDR_SIZE];

extern uint8_t g_static_ip_addr[];
extern uint8_t g_static_netmask_addr[];
extern uint8_t g_static_gw_addr[];

uint8_t g_ip_addr[IPV4_ADDR_SIZE];
static uint8_t g_netmask_addr[IPV4_ADDR_SIZE];
static uint8_t g_gw_addr[IPV4_ADDR_SIZE];
xSemaphoreHandle ip_configured;

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{ 
    DHCP_IS_OFF=0,
    DHCP_START,
    DHCP_WAIT_ADDRESS,
    DHCP_ADDRESS_ASSIGNED
} DHCP_State_TypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static struct netif xnetif; /* network interface structure */

static struct ip_addr ipaddr;
static struct ip_addr netmask;
static struct ip_addr gw;

static uint8_t is_dhcp_restart = 0;

/* Private functions ---------------------------------------------------------*/
static uint8_t check_static_ip(void);
static uint8_t check_mask_invalid(void);

void LwIP_set_link_down(void)
{
    (void) netifapi_netif_set_down(&xnetif);
}

void LwIP_set_link_up(void)
{
    (void) netifapi_netif_set_up(&xnetif);
}

void LwIP_dhcp_stop(void)
{
    (void) netifapi_dhcp_stop(&xnetif);
}

void LwIP_dhcp_restart(void)
{
    if(0 == check_static_ip())
    {
        is_dhcp_restart = 1;
    }
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void LwIP_Init(void)
{

    /* Create tcp_ip stack thread */
    tcpip_init( NULL, NULL );	
    
    /* IP address setting & display on STM32_evalboard LCD*/
    IP4_ADDR(&ipaddr, g_ip_addr[0], g_ip_addr[1], g_ip_addr[2], g_ip_addr[3]);
    IP4_ADDR(&netmask, g_netmask_addr[0], g_netmask_addr[1], g_netmask_addr[2], g_netmask_addr[3]);
    IP4_ADDR(&gw, g_gw_addr[0], g_gw_addr[1], g_gw_addr[2], g_gw_addr[3]);

    /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))
    
    Adds your network interface to the netif_list. Allocate a struct
    netif and pass a pointer to this structure as the first argument.
    Give pointers to cleared ip_addr structures when using DHCP,
    or fill them with sane numbers otherwise. The state pointer may be NULL.
    
    The init function pointer must point to a initialization function for
    your ethernet netif interface. The following code illustrates it's use.*/

    thisisjunk
    
    /* set netif MAC hardware address */
    xnetif.hwaddr[0] =  g_mac_addr[0];
    xnetif.hwaddr[1] =  g_mac_addr[1];
    xnetif.hwaddr[2] =  g_mac_addr[2];
    xnetif.hwaddr[3] =  g_mac_addr[3];
    xnetif.hwaddr[4] =  g_mac_addr[4];
    xnetif.hwaddr[5] =  g_mac_addr[5];

    (void) netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);

    /*  Registers the default network interface. */
    netif_set_default(&xnetif);
    
    /*  When the netif is fully configured this function must be called.*/
    netif_set_up(&xnetif); 
}

/**
  * @brief  LwIP_DHCP_Process_Handle
  * @param  None
  * @retval None
  */
void LwIP_DHCP_task(void * pvParameters)
{
    uint32_t IPaddress;
    uint32_t SUBaddress;
    uint32_t GWaddress;
    DHCP_State_TypeDef DHCP_state;  
    
    (void) pvParameters;

    if(1 == check_static_ip())
    {
        DHCP_state = DHCP_IS_OFF;
    }
    else
    {
        DHCP_state = DHCP_START;
    }

    for (;;)
    {
        if (is_dhcp_restart)
        {
            is_dhcp_restart = 0;
            DHCP_state = DHCP_START;
        }
    
        switch (DHCP_state)
        {
            case DHCP_IS_OFF:
            {
                memcpy(g_ip_addr, g_static_ip_addr, IPV4_ADDR_SIZE);
                memcpy(g_netmask_addr, g_static_netmask_addr, IPV4_ADDR_SIZE);
                memcpy(g_gw_addr, g_static_gw_addr, IPV4_ADDR_SIZE);
                //Check if netmask is correct
                if(0 == check_mask_invalid())
                {
                    IP4_ADDR(&netmask, g_netmask_addr[0], g_netmask_addr[1], g_netmask_addr[2], g_netmask_addr[3]);
                }
                
                IP4_ADDR(&ipaddr,g_ip_addr[0], g_ip_addr[1], g_ip_addr[2], g_ip_addr[3]);
    
                netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);
                DHCP_state = DHCP_ADDRESS_ASSIGNED;
    
                (void) xSemaphoreGive(ip_configured);
    
                //no need to keep eating CPU, if the static address changes we need re-booted.
                // this task is done
                vTaskDelete(NULL);
            }
            break;
    
            case DHCP_START:
            {
                ipaddr.addr = 0;
                netmask.addr = 0;
                gw.addr = 0;
                
                xnetif.ip_addr.addr = 0;
                xnetif.netmask.addr = 0;
                xnetif.gw.addr = 0;
                
                (void)netifapi_dhcp_start(&xnetif);
                //dhcp_start(&xnetif);
                IPaddress = 0;
                DHCP_state = DHCP_WAIT_ADDRESS;
            }
            break;
    
            case DHCP_WAIT_ADDRESS:
            {
                /* Read the new IP address */
                IPaddress = xnetif.ip_addr.addr;
                SUBaddress = xnetif.netmask.addr;
                GWaddress = xnetif.gw.addr;
                
                if (IPaddress!=0) 
                {
                    SUBaddress = xnetif.netmask.addr;
                    DHCP_state = DHCP_ADDRESS_ASSIGNED;	
                    g_ip_addr[3] = (uint8_t)(IPaddress >> 24);
                    g_ip_addr[2] = (uint8_t)(IPaddress >> 16);
                    g_ip_addr[1] = (uint8_t)(IPaddress >> 8);
                    g_ip_addr[0] = (uint8_t)(IPaddress);
                    
                    g_netmask_addr[3] = (uint8_t)(SUBaddress >> 24);
                    g_netmask_addr[2] = (uint8_t)(SUBaddress >> 16);
                    g_netmask_addr[1] = (uint8_t)(SUBaddress >> 8);
                    g_netmask_addr[0] = (uint8_t)(SUBaddress);
                    
                    g_gw_addr[3] = (uint8_t)(GWaddress >> 24);
                    g_gw_addr[2] = (uint8_t)(GWaddress >> 16);
                    g_gw_addr[1] = (uint8_t)(GWaddress >> 8);
                    g_gw_addr[0] = (uint8_t)(GWaddress);
                    
                    (void) xSemaphoreGive(ip_configured);

                    // no need to hang around, our work here is done
                    vTaskDelete(NULL);
                }
            }
            break;

            case DHCP_ADDRESS_ASSIGNED:
            default:
            break;
        }
    
        /* wait 250 ms */
        vTaskDelay(250);
    }
}

uint8_t check_static_ip(void)
{
    uint8_t i;
    uint8_t is_static = 0;

    for(i=0; i<IPV4_ADDR_SIZE; i++)
    {
        if(0 != g_static_ip_addr[i])
        {
            is_static = 1;
            break;
        }
    }

    return is_static;
}

uint8_t check_mask_invalid(void)
{
    uint8_t i;
    uint8_t is_invalid = 1;

    for(i=0; i<IPV4_ADDR_SIZE; i++)
    {
        if(0 != g_netmask_addr[i])
        {
            is_invalid = 0;
            break;
        }
    }

    return is_invalid;
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
