/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : Module for debugging via Rs232
 *
 */

// ---- Include system wide include files ----
#include <stm32f2xx.h>
#include <stdio.h>
#include <stdarg.h>
//#include <sys/stat.h>

// ---- Include local include files ----
#include "Rs232Debug.h"

#define USE_PRINTF

#ifndef USE_PRINTF
//----- Definitions ----
#define COMn                (1)

/** @brief Definition for COM port1, connected to USART1 */ 
#define NP_COM1                          USART1
#define NP_COM1_CLK_ENABLE()             __USART1_CLK_ENABLE()   
#define NP_COM1_CLK_DISABLE()            __USART1_CLK_DISABLE()

#define NP_COM1_TX_PIN                   GPIO_PIN_9
#define NP_COM1_TX_GPIO_PORT             GPIOA
#define NP_COM1_TX_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()   
#define NP_COM1_TX_GPIO_CLK_DISABLE()    __GPIOA_CLK_DISABLE()  
#define NP_COM1_TX_AF                    GPIO_AF7_USART1

#define NP_COM1_RX_PIN                   GPIO_PIN_10
#define NP_COM1_RX_GPIO_PORT             GPIOA
#define NP_COM1_RX_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()   
#define NP_COM1_RX_GPIO_CLK_DISABLE()    __GPIOA_CLK_DISABLE()  
#define NP_COM1_RX_AF                    GPIO_AF7_USART1

#define NP_COM1_IRQn                     USART1_IRQn

#define NP_COMx_CLK_ENABLE(__INDEX__)            (((__INDEX__) == 0) ? NP_COM1_CLK_ENABLE() : 0)
#define NP_COMx_CLK_DISABLE(__INDEX__)           (((__INDEX__) == 0) ? NP_COM1_CLK_DISABLE() : 0)

#define NP_COMx_TX_GPIO_CLK_ENABLE(__INDEX__)    (((__INDEX__) == 0) ? NP_COM1_TX_GPIO_CLK_ENABLE() : 0)
#define NP_COMx_TX_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? NP_COM1_TX_GPIO_CLK_DISABLE() : 0)

#define NP_COMx_RX_GPIO_CLK_ENABLE(__INDEX__)    (((__INDEX__) == 0) ? NP_COM1_RX_GPIO_CLK_ENABLE() : 0)
#define NP_COMx_RX_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? NP_COM1_RX_GPIO_CLK_DISABLE() : 0)

//----- Type declaration ----
typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;

//----- Global variables ----

//----- Local variables ----
USART_TypeDef* COM_USART[COMn] = {NP_COM1};
GPIO_TypeDef* COM_TX_PORT[COMn] = {NP_COM1_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {NP_COM1_RX_GPIO_PORT};
const uint16_t COM_TX_PIN[COMn] = {NP_COM1_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {NP_COM1_RX_PIN};
const uint16_t COM_TX_AF[COMn] = {NP_COM1_TX_AF};
const uint16_t COM_RX_AF[COMn] = {NP_COM1_RX_AF};

static UART_HandleTypeDef s_uartHandle;

//----- Local Function declarations ----
static void BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef *huart);

//****************************************************************************
void Rs232DebugInit()
{
    if( s_uartHandle.State == HAL_UART_STATE_RESET)
    {
        s_uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
		s_uartHandle.Init.BaudRate     = 115200;
		s_uartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
		s_uartHandle.Init.StopBits     = UART_STOPBITS_1;
		s_uartHandle.Init.Parity       = UART_PARITY_NONE;
		s_uartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
		s_uartHandle.Init.Mode         = UART_MODE_TX_RX;

		BSP_COM_Init(COM1, &s_uartHandle);
    }
}
#endif // inndef USE_PRINTF

void Rs232DebugPrint(const char* formatString, ...)
{
    char charbuf[RS232_DEBUG_BUFFER];
	uint16_t size;
    va_list argList;
    va_start(argList, formatString);
    size = vsnprintf(charbuf, 150, formatString, argList);
    va_end(argList);

#ifdef USE_PRINTF
    (void)size;
    printf("%s", charbuf);
#else
	if(HAL_UART_Transmit(&s_uartHandle, (uint8_t *)charbuf, (uint16_t)size, 0xFFFF) != HAL_OK)
	{
		Gpio_ShowStatus(STATUS_KIND_ERROR, STATUS_VALUE_UART);
	}
#endif
}

void Rs232DebugPrintString(char *pString, int strLen)
{
#ifdef USE_PRINTF
    printf("%s", pString);
#else
	if(HAL_UART_Transmit(&s_uartHandle, (uint8_t *)pString, (uint16_t)((strLen == 0)? strlen(pString): strLen), 0xFFFF) != HAL_OK)
	{
		Gpio_ShowStatus(STATUS_KIND_ERROR, STATUS_VALUE_UART);
	}
#endif
}

void Rs232DebugPrintChar(char character)
{
#ifdef USE_PRINTF
    printf("%c", character);
#else
	if(HAL_UART_Transmit(&s_uartHandle, (uint8_t *)&character, 1, 0xFFFF) != HAL_OK)
	{
		Gpio_ShowStatus(STATUS_KIND_ERROR, STATUS_VALUE_UART);
	}
#endif
}

void Rs232DebugPutChar(char character)
{
#ifdef USE_PRINTF
#else
    while (!(s_uartHandle.Instance->SR & ((uint16_t)0x0080)));
    s_uartHandle.Instance->DR = ((char)character & 0x1FF);
#endif
}

INT32 Rs232DebugCheckAndGetChar(char* pCharacter)
{
	INT32 retVal = 0;
#ifdef USE_PRINTF
#else
    if (__HAL_UART_GET_FLAG(&s_uartHandle, UART_FLAG_RXNE))
    {
        *pCharacter = (char)s_uartHandle.Instance->DR & (uint16_t)0x01FF;
		retVal = 1;
	}
#endif
	return retVal;
}

#ifndef USE_PRINTF

static void BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Ensure a clean initialization */
	NP_COMx_CLK_DISABLE(COM);

	/* Enable GPIO clock */
	NP_COMx_TX_GPIO_CLK_ENABLE(COM);
	NP_COMx_RX_GPIO_CLK_ENABLE(COM);

	/* Enable USART clock */
	NP_COMx_CLK_ENABLE(COM);

	/* Configure USART Tx as alternate function */
	GPIO_InitStruct.Pin = COM_TX_PIN[COM];
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = COM_TX_AF[COM];
	HAL_GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStruct);

	/* Configure USART Rx as alternate function */
	GPIO_InitStruct.Pin = COM_RX_PIN[COM];
	GPIO_InitStruct.Alternate = COM_RX_AF[COM];
	HAL_GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStruct);

	/* USART configuration */
	huart->Instance = COM_USART[COM];
	if(HAL_UART_Init(huart) != HAL_OK)
	{
		Gpio_ShowStatus(STATUS_KIND_ERROR, STATUS_VALUE_UART);
	}
}
#endif
