/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Contains common gpio functionality.
 *
 */

#ifndef GPIO_H
#define GPIO_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- Definitions ----

typedef enum 
{
    /* LED0 is B1 in the schematic (RED) */
    LED0 = 0,
    /* LED1 is B2 in the schematic (GREEN) */
    LED1 = 1,
    /* LED2 is B3 in the schematic (YELLOW) */
    LED2 = 2,
    /* LED3 is the GREEN led of B4 in the schematic */
    LED3 = 3,
    /* LED4 is the RED led of B4 in the schematic */
    LED4 = 4,
    /* LED5 is the GREEN led of B5 in the schematic */
    LED5 = 5,
    /* LED6 is the RED led of B5 in the schematic */
    LED6 = 6,
    /* LED7 is the GREEN led of B6 in the schematic */
    LED7 = 7,
    /* LED8 is the RED led of B7 in the schematic */
    LED8 = 8
} Led_TypeDef;

typedef enum
{
    /* DATA_READY is a handshake line of the SPI port */
    DATA_READY_SPI = 0,
    /* HOLD_OFF is a handshake line of the SPI port */
    HOLD_OFF_SPI = 1,
    /* OUTPUT_ENABLE enables the output of the clock signals */
    OUTPUT_ENABLE_CLK_GEN = 2
} ApplicationProcessor_TypeDef;

typedef enum 
{  
  BUTTON_WAKEUP = 0,
  BUTTON_TAMPER = 1,
  BUTTON_KEY = 2
}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

typedef enum 
{  
  STATUS_KIND_SUCCESS = 0,
  STATUS_KIND_PROGRESS = 1,
  STATUS_KIND_ERROR = 2
}StatusKind_TypeDef;

typedef enum 
{  
  STATUS_VALUE_TIMER = 1,
  STATUS_VALUE_I2C = 2,
  STATUS_VALUE_SPI = 3,
  STATUS_VALUE_UART = 4,
  STATUS_VALUE_FLASH = 5,
  STATUS_VALUE_DMA = 6,
  STATUS_VALUE_7 = 7
}StatusValue_TypeDef;

// ---- Type declaration ----

// ---- Global variables ----

// ---- Function declarations ----

/**
  * @brief  Configures LED GPIO.
  *
  * @param  Led: LED to be configured. 
  *
  * @retval None
  */
void Gpio_LedInit(Led_TypeDef Led);

/**
  * @brief  Turns selected LED On.
  *
  * @param  Led: LED to be set on 
  *
  * @retval None
  */
void Gpio_LedOn(Led_TypeDef Led);

/**
  * @brief  Turns selected LED Off. 
  *
  * @param  Led: LED to be set off
  *
  * @retval None
  */
void Gpio_LedOff(Led_TypeDef Led);

/**
  * @brief  Toggles the selected LED.
  *
  * @param  Led: LED to be toggled
  *
  * @retval None
  */
void Gpio_LedToggle(Led_TypeDef Led);

/**
  * @brief  Configures Application Processor GPIO.
  *
  * @param  line: Application Processor line to be configured. 
  * @param  set:  Set or reset after configuration. 
  *
  * @retval None
  */
void Gpio_PeriphInit(ApplicationProcessor_TypeDef line, bool set);

/**
  * @brief  Turns selected line On.
  *
  * @param  line: Application Processor line to be set on 
  *
  * @retval None
  */
void Gpio_PeriphOn(ApplicationProcessor_TypeDef line);

/**
  * @brief  Turns selected line Off. 
  *
  * @param  line: Application Processor line to be set off
  *
  * @retval None
  */
void Gpio_PeriphOff(ApplicationProcessor_TypeDef line);

/**
  * @brief  Show system status with LEDs 3 - 8.
  * @param  kind:   Kind of status to be shown: error, progress, success
  * @param  status: Value of the status
  */
void Gpio_ShowStatus(StatusKind_TypeDef kind, StatusValue_TypeDef status);

#ifdef __cplusplus
}
#endif

#endif // GPIO_H
