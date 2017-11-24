/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Contains common gpio functionality.
 *
 */

// ---- Include system wide include files ----
#include <stm32f2xx.h>
#include <Stm32CortexM3/lib/UtilLib/Rs232Debug.h>

// ---- Include local include files ----
#include "Gpio.h"

                      3 

//----- Type declaration ----

//----- Global variables ----

//----- Local variables ----




//----- Local Function declarations ----

//******************************* API ****************************************

void Gpio_LedInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_LED clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = LED_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(LED_PORT[Led], &GPIO_InitStruct);

  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

void Gpio_LedOn(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

void Gpio_LedOff(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET); 
}

void Gpio_LedToggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

void Gpio_PeriphInit(ApplicationProcessor_TypeDef line, bool set)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_LED clock */
  PERIPFx_GPIO_CLK_ENABLE(line);

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = PERIPF_PIN[line];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(PERIPF_PORT[line], &GPIO_InitStruct);

  HAL_GPIO_WritePin(PERIPF_PORT[line], PERIPF_PIN[line], set? GPIO_PIN_SET: GPIO_PIN_RESET);
}

void Gpio_PeriphOn(ApplicationProcessor_TypeDef line)
{
  HAL_GPIO_WritePin(PERIPF_PORT[line], PERIPF_PIN[line], GPIO_PIN_SET);
}

void Gpio_PeriphOff(ApplicationProcessor_TypeDef line)
{
  HAL_GPIO_WritePin(PERIPF_PORT[line], PERIPF_PIN[line], GPIO_PIN_RESET); 
}

void Gpio_ShowStatus( StatusKind_TypeDef kind, StatusValue_TypeDef status)
{
    bool led3 = false;
    bool led4 = false;
    bool led5 = false;
    bool led6 = false;
    bool led7 = false;
    bool led8 = false;

    if ((kind == STATUS_KIND_SUCCESS) || (kind == STATUS_KIND_PROGRESS))
    {
        if ( (status & 0x01) != 0)
        {
            led3 = true;
        }
        if ( (status & 0x02) != 0)
        {
            led5 = true;
        }
        if ( (status & 0x04) != 0)
        {
            led7 = true;
        }
    }
    if ((kind == STATUS_KIND_ERROR) || (kind == STATUS_KIND_PROGRESS))
    {
        if ( (status & 0x01) != 0)
        {
            led4 = true;
        }
        if ( (status & 0x02) != 0)
        {
            led6 = true;
        }
        if ( (status & 0x04) != 0)
        {
            led8 = true;
        }
    }
    (led3)? Gpio_LedOn(LED3): Gpio_LedOff(LED3);
    (led4)? Gpio_LedOn(LED4): Gpio_LedOff(LED4);
    (led5)? Gpio_LedOn(LED5): Gpio_LedOff(LED5);
    (led6)? Gpio_LedOn(LED6): Gpio_LedOff(LED6);
    (led7)? Gpio_LedOn(LED7): Gpio_LedOff(LED7);
    (led8)? Gpio_LedOn(LED8): Gpio_LedOff(LED8);
}
