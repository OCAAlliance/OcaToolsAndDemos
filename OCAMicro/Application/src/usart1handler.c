/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 */

//=========================================================================
//  File Name: usart1handler.c
//
//  Description:
//      Implementation of the control Port UART
//  
//  Modification History:
//      $Id: usart1handler.c,v 1.1 2014/06/24 18:37:56 dvaughn Exp $
//      $Log: usart1handler.c,v $
//      Revision 1.1  2014/06/24 18:37:56  dvaughn
//      Initial commit
//
//      Revision 1.1  2014/03/21 14:38:29  dvaughn
//      Initial commit
//
//
//
//
//
//==========================================================================
#include <string.h>
#include "stm32f2xx_usart.h"
#include "misc.h"

#define RXBUFFERSIZE    256
#define TXBUFFERSIZE    256

//----------------------------------------------------------------------------
// Global objects
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Externs
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Local objects
//-----------------------------------------------------------------------------
 uint8_t  TxBuffer[TXBUFFERSIZE];
 uint8_t  RxBuffer [RXBUFFERSIZE];
static uint16_t RxWrIndx = 0;
static uint16_t RxRdIndx = 0;
static uint16_t TxRdIndx = 0;
static uint16_t TxWrIndx = 0;


//-----------------------------------------------------------------------------
// Function: USART1_Init
//
// Description:
//    Initialization routine for USART1
//
// Parameters:
//      void
//
// Returns:
//      void
//
//-----------------------------------------------------------------------------
void USART1_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  // Configure UART
  // Uses AF7 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  /* NVIC configuration */  
  /* Enable the USARTx Interrupt */
  //lint -e641
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  //lint -e641
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;  // lowest priority
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  USART_Cmd(USART1, ENABLE);
}


//-----------------------------------------------------------------------------
// Function: USART1_TxData
//
// Description:
//    Routine to send data through the UART.
//
// Parameters:
//      uint8_t*      data to send
//      uint8_t       length of data
//
// Returns:
//      int       0 if no room, length transferred on success, -1 on fail
//                can only fail if the passed in length is greater than buff size
//
//-----------------------------------------------------------------------------
int32_t USART1_TxData(const uint8_t *Data, uint16_t length)
{
    uint16_t spaceAvail  = 0;
    uint16_t spaceAvail1 = 0;
    uint16_t spaceAvail2 = 0;
    uint16_t Xfer1 = 0;
    uint16_t Xfer2 = 0;
    uint8_t  retVal = 0;
      
    if (TxWrIndx >= TxRdIndx)
    {
        // buffer has possible space in front of wrIndx, 
        // possible space at the beginning of buffer
        spaceAvail1 = TXBUFFERSIZE - TxWrIndx;
        spaceAvail2 = TxRdIndx;
        spaceAvail = spaceAvail1 + spaceAvail2;
        // always use one less so with data in buffer we leave one
        // byte
        if (spaceAvail)
        {
            spaceAvail--;
        }
    }
    else
    {
        // read index is greater than write index
        spaceAvail1 = (TxRdIndx - TxWrIndx) - 1;
        spaceAvail = spaceAvail1;
    }

    if (spaceAvail >= length)
    {
        retVal = length;
        
        // calulate xfer sizes
         if (spaceAvail1 >= length)
         {
            // one part Xfer
            Xfer1 = length;
         }
         else
         {
            // two part Xfer
            Xfer1 = spaceAvail1;
            Xfer2 = length - Xfer1;
         }
         
        // move first part
        if (Xfer1)
        {
            memcpy(&TxBuffer[TxWrIndx], Data, Xfer1);
            TxWrIndx += Xfer1;
        }

        // test to see if TxWrIndx is maxed out
        if (TXBUFFERSIZE == TxWrIndx)
        {
            TxWrIndx = 0;
        }
     
        // move second part (if needed)
        if (Xfer2)
        {
            memcpy(&TxBuffer[0], &Data[Xfer1], Xfer2);
            TxWrIndx += Xfer2;
        }   
  
        // enable the transmit interrupt
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
    else if (length >= TXBUFFERSIZE)
    {
        retVal = -1;
    }
    else
    {
        retVal = 0;
    }
    
    return retVal;
}


//-----------------------------------------------------------------------------
// Function: USART1_GetChar
//
// Description:
//    Routine to recieve a character from the UART.
//
// Parameters:
//      uint8_t*      pointer to hold the character
//
// Returns:
//      uint8_t       0 on fail, 1 on success
//
//-----------------------------------------------------------------------------
uint8_t USART1_GetChar(uint8_t *data)
{
    uint8_t retVal = 0;
    if (RxWrIndx != RxRdIndx)
    {
        retVal = 1;
        *data = RxBuffer[RxRdIndx++];
        if (RxRdIndx == RXBUFFERSIZE)
        {
            RxRdIndx = 0;
        }
    }
    return retVal;
}



//lint -e830 e765
/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/*  or in C:\Documents and Settings\YourName\Local Settings\Application Data\Rowley Associates Limited\CrossWorks for ARM\packages\targets\STM32      */
/*  STM32_Startup.s                                                           */
/******************************************************************************/
//-----------------------------------------------------------------------------
// Function: USART1_IRQHandler
//
// Description:
//    IRQ handler for the UART
//
// Parameters:
//      void
//
// Returns:
//      void
//
//-----------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
  /* USART Transmit Int -------------------------------------------------*/
  if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
  { 
        USART_SendData(USART1, TxBuffer[TxRdIndx++]);
        if (TxRdIndx == TXBUFFERSIZE)
        {
            TxRdIndx = 0;
        }                   
  
        if (TxRdIndx == TxWrIndx)
        {
          /* Disable the USARTx transmit data register empty interrupt */
          USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        }
  }
  
  /* USART in mode Receiver --------------------------------------------------*/
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
      {
        RxBuffer[RxWrIndx++] = (uint8_t)USART_ReceiveData(USART1);
        if (RxWrIndx == RXBUFFERSIZE)
        {
            RxWrIndx = 0;
        }
      }
  }     
}
