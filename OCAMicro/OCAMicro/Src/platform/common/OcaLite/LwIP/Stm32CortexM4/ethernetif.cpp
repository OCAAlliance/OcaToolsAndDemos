/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/ethernetif.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014
  * @brief   This file implements Ethernet network interface drivers for lwIP
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <Stm32CortexM4/lib/StdPeriphLib/StdPeriphLib.h>
#include <common/OcaLite/LogicalDevices/Switch/Switch.h>
#include <Stm32CortexM4/lib/UtilLib/Gpio.h>
#include <Stm32CortexM4/lib/UtilLib/Rs232Debug.h>

#include "lwip/opt.h"
#include "lwip/lwip_timers.h"
#include "netif/etharp.h"
#include "ethernetif.h"

/* Private typedef -----------------------------------------------------------*/
#ifndef RS232_DEBUG_BUFFER
    #define RS232_DEBUG_BUFFER (0)
#endif

/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT                 ( 100 )
/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE            ( 350 )

/* Define those to better describe your network interface. */
#define IFNAME0 'B' 
#define IFNAME1 'S' 

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END;/* Ethernet Rx MA Descriptor */
__ALIGN_BEGIN ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END;/* Ethernet Tx DMA Descriptor */
__ALIGN_BEGIN uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */
__ALIGN_BEGIN uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */

/* Semaphore to informe ethernetif of frame reception */
static osSemaphoreId s_xMacSemaphore = NULL;

/* Global Ethernet handle */
static ETH_HandleTypeDef s_xEthHandle;

/* Private function prototypes -----------------------------------------------*/
static void low_level_init(struct netif *netif);
static void MacProcessDataTask(void const * argument);
static void MacHandleLinkStatusChange(struct netif *netif);

/*******************************************************************************
                       Ethernet MSP Routines
*******************************************************************************/

/**
  * @brief  Initializes the ETH MSP.
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
  
    /* Enable GPIOs clocks */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();

    /* Ethernet pins configuration ************************************************/
    /*
        ETH_MII_REF_CLK ------------------> PA1   (ETH_MII_CLK)
        ETH_MII_RX_DV --------------------> PA7   (ETH_MII_RX_EN)
        ETH_MII_RXD0 ---------------------> PC4
        ETH_MII_RXD1 ---------------------> PC5
        ETH_MII_TX_EN --------------------> PB11  (ETH_MII_TX_DV)
        ETH_MII_TXD0 ---------------------> PB12
        ETH_MII_TXD1 ---------------------> PB13
    */

    /* Configure PA1 and PA7 */
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL ; 
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB11, PB13 and PB14 */
    GPIO_InitStructure.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC4 and PC5 */
    GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Enable the Ethernet global Interrupt */
    HAL_NVIC_SetPriority(ETH_IRQn, 0x8, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
  
    /* Enable ETHERNET clock  */
    __ETH_CLK_ENABLE();
}

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    osSemaphoreRelease(s_xMacSemaphore);
}

/**
  * @brief  Ethernet IRQ Handler
  * @param  None
  * @retval None
  */
void ETHERNET_IRQHandler(void)
{
    HAL_ETH_IRQHandler(&s_xEthHandle);
}

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH) 
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
static void low_level_init(struct netif *netif)
{
    ETH_MACInitTypeDef configuration = {0, };

    // To be checked how this works (passing the address of a stack variable ...)
    // Passing netif->hwaddr fails !!
    uint8_t macaddress[6]= { MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5 };
  
    s_xEthHandle.Instance = ETH;  
    s_xEthHandle.Init.MACAddr = macaddress;
    s_xEthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    s_xEthHandle.Init.Speed = ETH_SPEED_100M;
    s_xEthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    s_xEthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    s_xEthHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
    s_xEthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    s_xEthHandle.Init.PhyAddress = DP83848_PHY_ADDRESS; // Note: not used.
  
    /* configure ethernet peripheral (GPIOs, clocks, MAC, DMA) */
    if (HAL_ETH_Init(&s_xEthHandle) == HAL_OK)
    {
        /* Set netif link flag */
        netif->flags |= NETIF_FLAG_LINK_UP;
    }
  
    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&s_xEthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
     
    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&s_xEthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
  
    /* set netif MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set netif MAC hardware address */
    netif->hwaddr[0] =  MAC_ADDR0;
    netif->hwaddr[1] =  MAC_ADDR1;
    netif->hwaddr[2] =  MAC_ADDR2;
    netif->hwaddr[3] =  MAC_ADDR3;
    netif->hwaddr[4] =  MAC_ADDR4;
    netif->hwaddr[5] =  MAC_ADDR5;

    /* set netif maximum transfer unit */
    netif->mtu = 1500;

    /* Accept broadcast address and ARP traffic, and enable IGMP capability. */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;

    /* create a binary semaphore used for informing ethernetif of frame reception */
    osSemaphoreDef(MacSem);
    s_xMacSemaphore = osSemaphoreCreate(osSemaphore(MacSem), 1);

    /* create the task that handles the ETH_MAC */
    osThreadDef(MacTask, MacProcessDataTask, osPriorityRealtime, 0, INTERFACE_THREAD_STACK_SIZE + RS232_DEBUG_WORDS);
    osThreadCreate (osThread(MacTask), netif);

    /* Set the link callback function, this function is called on change of link status*/
    netif_set_link_callback(netif, MacHandleLinkStatusChange);

    /* ETHERNET MAC Re-Configuration */
    configuration.PassControlFrames = ETH_PASSCONTROLFRAMES_BLOCKALL;
    configuration.ReceiveAll = ETH_RECEIVEALL_ENABLE;
    HAL_ETH_ConfigMAC(&s_xEthHandle, &configuration);

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(&s_xEthHandle); 
}

/**
  * @brief This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become availale since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t errval;
    struct pbuf *q;
    uint8_t *buffer = (uint8_t *)(s_xEthHandle.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc;
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;
    HAL_StatusTypeDef retVal;
    int bufCount = 0;

    DmaTxDesc = s_xEthHandle.TxDesc;
    bufferoffset = 0;
  
    /* copy frame from pbufs to driver buffers */
    for(q = p; q != NULL; q = q->next)
    {
        /* Is this buffer available? If not, goto error */
        if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
        {
            errval = ERR_USE;
            goto error;
        }
    
        /* Get bytes in current lwIP buffer */
        byteslefttocopy = q->len;
        payloadoffset = 0;
    
        /* Check if the length of data to copy is bigger than Tx buffer size*/
        while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
        {
            /* Copy data to Tx buffer*/
            memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );
      
            /* Point to next descriptor */
            DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);
      
            /* Check if the buffer is available */
            if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
            {
                errval = ERR_USE;
                goto error;
            }
      
            buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);
      
            byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
            payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
            framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
            bufferoffset = 0;
        }
    
        /* Copy the remaining bytes */
        memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy );
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength = framelength + byteslefttocopy;
    }

    /* Prepare transmit descriptors to give to DMA */ 
    retVal = HAL_ETH_TransmitFrame(&s_xEthHandle, framelength);
  
    errval = ERR_OK;
  
error:
  
    /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
    if ((s_xEthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
    {
        /* Clear TUS ETHERNET DMA flag */
        s_xEthHandle.Instance->DMASR = ETH_DMASR_TUS;
    
        /* Resume DMA transmission*/
        s_xEthHandle.Instance->DMATPDR = 0;
    }
    return errval;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf * low_level_input(struct netif *netif)
{
    struct pbuf *p = NULL, *q = NULL;
    uint16_t len = 0 ;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;
    uint32_t bufferoffset = 0;
    uint32_t payloadoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t i=0;
    int bufCount = 0;

    /* get received frame */
    if (HAL_ETH_GetReceivedFrame_IT(&s_xEthHandle) != HAL_OK)
        return NULL;
  
    /* Obtain the size of the packet and put it into the "len" variable. */
    len = s_xEthHandle.RxFrameInfos.length;
    buffer = (uint8_t *)s_xEthHandle.RxFrameInfos.buffer;
  
    if (len > 0)
    {
        /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }
  
    if (p != NULL)
    {
        dmarxdesc = s_xEthHandle.RxFrameInfos.FSRxDesc;
        bufferoffset = 0;

        for(q = p; q != NULL; q = q->next)
        {
            byteslefttocopy = q->len;
            payloadoffset = 0;
      
            /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
            while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
            {
                /* Copy data to pbuf */
                memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));
        
                /* Point to next descriptor */
                dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
                buffer = (uint8_t *)(dmarxdesc->Buffer1Addr);
        
                byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
                payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
                bufferoffset = 0;
            }
      
            /* Copy remaining data in pbuf */
            memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }

    /* Release descriptors to DMA */
    /* Point to first descriptor */
    dmarxdesc = s_xEthHandle.RxFrameInfos.FSRxDesc;
    /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
    for(i = 0; i < s_xEthHandle.RxFrameInfos.SegCount; i++)
    {  
        dmarxdesc->Status |= ETH_DMARXDESC_OWN;
        dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
    }
    
    /* Clear Segment_Count */
    s_xEthHandle.RxFrameInfos.SegCount =0;
  
    /* When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((s_xEthHandle.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
    {
        /* Clear RBUS ETHERNET DMA flag */
        s_xEthHandle.Instance->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        s_xEthHandle.Instance->DMARPDR = 0;
    }
    return p;
}

/**
  * @brief This function is the MacProcessDataTask task, it is processed when a packet 
  * is ready to be read from the interface. It uses the function low_level_input() 
  * that should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param netif the lwip network interface structure for this ethernetif
  */
static void MacProcessDataTask( void const * argument )
{
    struct netif *netif = (struct netif *) argument;
    struct pbuf *p;

    for( ;; )
    {
        if (osSemaphoreWait( s_xMacSemaphore, TIME_WAITING_FOR_INPUT) == osOK)
        {
            p = low_level_input( netif );
            if   (p != NULL)
            {
                if (netif->input( p, netif) != ERR_OK )
                {
                    pbuf_free(p);
                    p = NULL;
                }
            }
        }
    }
}

/**
  * @brief  Mac callback function, this function is called on change of link status
  *         to update low level driver configuration.
  * @param  netif: The network interface
  * @retval None
  */
static void MacHandleLinkStatusChange(struct netif *netif)
{
    if (netif_is_link_up(netif))
    { 
        // Switch runs in auto negotiationmode.

        /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
        if (Switch::GetInstance().GetFullDuplex())
        {
            /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
            s_xEthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;  
        }
        else
        {
            /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
            s_xEthHandle.Init.DuplexMode = ETH_MODE_HALFDUPLEX;           
        }
        /* Configure the MAC with the speed fixed by the auto-negotiation process */
        if (Switch::GetInstance().GetLinkSpeed() == 10)
        {  
            /* Set Ethernet speed to 10M following the auto-negotiation */
            s_xEthHandle.Init.Speed = ETH_SPEED_10M; 
        }
        else
        {   
            /* Set Ethernet speed to 100M following the auto-negotiation */ 
            s_xEthHandle.Init.Speed = ETH_SPEED_100M;
        }
        ::Rs232DebugPrint("EthernetIf: Link up (speed=%s, duplex=%s)\r\n",
                          s_xEthHandle.Init.Speed == 0? "10": "100",
                          s_xEthHandle.Init.DuplexMode == 0? "HALF": "FULL");
                
        /* ETHERNET MAC Re-Configuration */
        HAL_ETH_ConfigMAC(&s_xEthHandle, (ETH_MACInitTypeDef *) NULL);

        /* Restart MAC interface */
        HAL_ETH_Start(&s_xEthHandle);   
        netif_set_up(netif); 
    }
    else
    {
        /* Stop MAC interface */
        HAL_ETH_Stop(&s_xEthHandle);

        ::Rs232DebugPrint("EthernetIf: Link down\r\n");

       netif_set_down(netif); 
    }
}

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}
