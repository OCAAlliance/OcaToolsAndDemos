/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
    Led_Off = 0,
    Led_On
} ledState_t;

typedef enum
{
    Meter_Off = 0,
    Meter_0db,
    Meter_n3db,
    Meter_n6db,
    Meter_n12db,
    Meter_n18db,
    Meter_n42db
} meterDbSetting_t;

typedef enum 
{
    CLOSED = 0,
    OPEN
}SwitchState_t;


/* Exported constants --------------------------------------------------------*/
#define IPV4_ADDR_SIZE (4)

#define MAC_ADDR_IN_OTP 0

// Port A pin assignments
#define LED_51             GPIO_Pin_0
#define PORTA_OUT_PINS     (LED_51)

// Port B pin assignments
#define STATIC_LED0        GPIO_Pin_8
#define STATIC_LED1        GPIO_Pin_9
#define PORTB_OUT_PINS     (STATIC_LED0 | STATIC_LED1)

// Port C pin assignments
#define ENC2_A             GPIO_Pin_10
#define ENC2_B             GPIO_Pin_11
#define ENC2_SW            GPIO_Pin_12
#define ENC1_A             GPIO_Pin_13
#define ENC1_B             GPIO_Pin_14
#define ENC1_SW            GPIO_Pin_15
#define PORTC_IN_PINS      (ENC2_A | ENC2_B | ENC2_SW | ENC1_A | ENC1_B | ENC1_SW)

#define ENC2_A_EXTI_LINE   EXTI_Line10
#define ENC2_B_EXTI_LINE   EXTI_Line11
#define ENC2_A_EXTI_PIN_SRC  EXTI_PinSource10
#define ENC2_B_EXTI_PIN_SRC  EXTI_PinSource11

#define ENC1_A_EXTI_LINE   EXTI_Line13
#define ENC1_B_EXTI_LINE   EXTI_Line14
#define ENC1_A_EXTI_PIN_SRC  EXTI_PinSource13
#define ENC1_B_EXTI_PIN_SRC  EXTI_PinSource14

#define ENC_EXTI_IRQ       EXTI15_10_IRQn
#define ENC_EXTI_PORT_SRC  EXTI_PortSourceGPIOC

// Port D pin assignments
#define SW0                GPIO_Pin_0
#define SW1                GPIO_Pin_1
#define SW2                GPIO_Pin_2
#define SW3                GPIO_Pin_3
#define SW4                GPIO_Pin_4
#define SW5                GPIO_Pin_5
#define SW6                GPIO_Pin_6
#define SW7                GPIO_Pin_7
#define RELAY1             GPIO_Pin_14
#define RELAY2             GPIO_Pin_15
#define PORTD_IN_PINS      (SW0 | SW1 | SW2 | SW3 | SW4 | SW5 | SW6 | SW7)
#define PORTD_OUT_PINS     (RELAY1 | RELAY2)

// Port E pin assinments
#define COL0               GPIO_Pin_0
#define COL1               GPIO_Pin_1
#define COL2               GPIO_Pin_2
#define COL3               GPIO_Pin_3
#define COL4               GPIO_Pin_4
#define COL5               GPIO_Pin_5
#define COL6               GPIO_Pin_6
#define COL7               GPIO_Pin_7
#define ROW0               GPIO_Pin_8
#define ROW1               GPIO_Pin_9
#define ROW2               GPIO_Pin_10
#define ROW3               GPIO_Pin_11
#define ROW4               GPIO_Pin_12
#define ROW5               GPIO_Pin_13
#define ROW6               GPIO_Pin_14
#define PORTE_OUT_PINS     (COL0 | COL1 | COL2 | COL3 | COL4 | COL5 | COL6 | COL7 | \
                            ROW0 | ROW1 | ROW2 | ROW3 | ROW4 | ROW5 | ROW6)

// control functions for port A
static inline void LED51_off(void)
{
    GPIOA->BSRRH = LED_51;  // drive pin low
}

static inline void LED51_on(void)
{
    GPIOA->BSRRL = LED_51;  // drive pin high
}

// control functions for port B
static inline void STATIC_LED0_off(void)
{
    GPIOB->BSRRH = STATIC_LED0;  // drive pin low
}

static inline void STATIC_LED0_on(void)
{
    GPIOB->BSRRL = STATIC_LED0;  // drive pin high
}

static inline void STATIC_LED1_off(void)
{
    GPIOB->BSRRH = STATIC_LED1;  // drive pin low
}

static inline void STATIC_LED1_on(void)
{
    GPIOB->BSRRL = STATIC_LED1;  // drive pin high
}

// control functions for port D
static inline void RELAY1_off(void)
{
    GPIOD->BSRRH = RELAY1;  // drive pin low
}

static inline void RELAY1_on(void)
{
    GPIOD->BSRRL = RELAY1;  // drive pin high
}

static inline void RELAY2_off(void)
{
    GPIOD->BSRRH = RELAY2;  // drive pin low
}

static inline void RELAY2_on(void)
{
    GPIOD->BSRRL = RELAY2;  // drive pin high
}

#ifdef __cplusplus
extern "C" {
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  
void USART1_Init(void);
int32_t USART1_TxData(const uint8_t *Data, uint16_t length);
uint8_t USART1_GetChar(uint8_t *data);

void OCA_Pi_ReadSwitches(void *arg);

void setDbMeter(uint8_t channel, meterDbSetting_t meterSetting);
void setDbRawLED(uint8_t channel, uint8_t rawLED);
void setSwitchLed(uint8_t switchNum, ledState_t ledState);
SwitchState_t getSwitchState(uint8_t switchNum);
void processLedArray(void);
void encoder_init(void);
void serialDebugPortTask(void * pvParameters);
void vApplicationIdleHook( void );
void vApplicationTickHook( void );

int __putchar(int ch);
#ifdef Rowley_Crossworks
void __assert(const char *expression, const char *filename, int line);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

