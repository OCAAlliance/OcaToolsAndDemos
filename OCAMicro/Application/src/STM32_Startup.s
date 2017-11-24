/*****************************************************************************
 * Copyright (c) 2007, 2009 Rowley Associates Limited.                       *
 *                                                                           *
 * This file may be distributed under the terms of the License Agreement     *
 * provided with this software.                                              *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *****************************************************************************/

/*****************************************************************************
 *                           Preprocessor Definitions
 *                           ------------------------
 *
 * STARTUP_FROM_RESET
 *
 *   If defined, the program will startup from power-on/reset. If not defined
 *   the program will just loop endlessly from power-on/reset.
 *
 *   This definition is not defined by default on this target because the
 *   debugger is unable to reset this target and maintain control of it over the
 *   JTAG interface. The advantage of doing this is that it allows the debugger
 *   to reset the CPU and run programs from a known reset CPU state on each run.
 *   It also acts as a safety net if you accidently download a program in FLASH
 *   that crashes and prevents the debugger from taking control over JTAG
 *   rendering the target unusable over JTAG. The obvious disadvantage of doing
 *   this is that your application will not startup without the debugger.
 *
 *   We advise that on this target you keep STARTUP_FROM_RESET undefined whilst
 *   you are developing and only define STARTUP_FROM_RESET when development is
 *   complete.
 *
 * __NO_SYSTEM_INIT
 *
 *   If defined, the SystemInit() function will NOT be called. By default SystemInit()
 *   is called after reset to enable the clocks and memories to be initialised 
 *   prior to any C startup initialisation.
 *
 * VECTORS_IN_RAM
 *
 *   If defined then the exception vectors are copied from Flash to RAM
 *
 *
 * __NO_FPU
 *
 *   If defined do NOT turn on the FPU for F4xx parts
 *
 * __NO_RUNFAST_MODE
 *
 *   If defined do NOT turn on flush-to-zero and default NaN modes
 *
 *****************************************************************************/
#include "version.h"
//#define FreeRTOS

  .global reset_handler

  .extern xPortPendSVHandler
  .extern xPortSysTickHandler
  .extern vPortSVCHandler


  .section .vectors, "ax"
  .code 16
  .align 0
  .global _vectors
  .syntax unified

.macro DEFAULT_ISR_HANDLER name=
  .thumb_func
  .weak \name
\name:
1: b 1b /* endless loop */
.endm

_vectors:
  .word __stack_end__
#ifdef STARTUP_FROM_RESET
  .word reset_handler
#else
  .word reset_wait
#endif /* STARTUP_FROM_RESET */
.word NMI_Handler
.word HardFault_Handler
.word MemManage_Handler 
.word BusFault_Handler
.word UsageFault_Handler
  .word 0 // Reserved
  .word 0 // Reserved
  .word 0 // Reserved
  .word 0 // Reserved
#ifndef FreeRTOS
.word SVC_Handler
#else
  .word  vPortSVCHandler                     /* FreeRTOS SVC handler          */
#endif
.word DebugMon_Handler
  .word  0
#ifndef FreeRTOS
.word PendSV_Handler
#else
  .word  xPortPendSVHandler                  /* FreeRTOS PendSV Handler       */
#endif 
#ifndef FreeRTOS 
.word SysTick_Handler 
#else
  .word xPortSysTickHandler
#endif
.word WWDG_IRQHandler
.word PVD_IRQHandler
.word TAMP_STAMP_IRQHandler
.word RTC_WKUP_IRQHandler
.word FLASH_IRQHandler
.word RCC_IRQHandler
.word EXTI0_IRQHandler
.word EXTI1_IRQHandler
.word EXTI2_IRQHandler
.word EXTI3_IRQHandler
.word EXTI4_IRQHandler
.word DMA1_Stream0_IRQHandler
.word DMA1_Stream1_IRQHandler
.word DMA1_Stream2_IRQHandler
.word DMA1_Stream3_IRQHandler
.word DMA1_Stream4_IRQHandler
.word DMA1_Stream5_IRQHandler
.word DMA1_Stream6_IRQHandler
.word ADC_IRQHandler
.word CAN1_TX_IRQHandler
.word CAN1_RX0_IRQHandler
.word CAN1_RX1_IRQHandler
.word CAN1_SCE_IRQHandler
.word EXTI9_5_IRQHandler
.word TIM1_BRK_TIM9_IRQHandler
.word TIM1_UP_TIM10_IRQHandler
.word TIM1_TRG_COM_TIM11_IRQHandler
.word TIM1_CC_IRQHandler
.word TIM2_IRQHandler
.word TIM3_IRQHandler
.word TIM4_IRQHandler
.word I2C1_EV_IRQHandler
.word I2C1_ER_IRQHandler
.word I2C2_EV_IRQHandler
.word I2C2_ER_IRQHandler
.word SPI1_IRQHandler
.word SPI2_IRQHandler
.word USART1_IRQHandler
.word USART2_IRQHandler
.word USART3_IRQHandler
.word EXTI15_10_IRQHandler
.word RTC_Alarm_IRQHandler
.word OTG_FS_WKUP_IRQHandler
.word TIM8_BRK_TIM12_IRQHandler
.word TIM8_UP_TIM13_IRQHandler
.word TIM8_TRG_COM_TIM14_IRQHandler
.word TIM8_CC_IRQHandler
.word DMA1_Stream7_IRQHandler
.word FSMC_IRQHandler
.word SDIO_IRQHandler
.word TIM5_IRQHandler
.word SPI3_IRQHandler
.word UART4_IRQHandler
.word UART5_IRQHandler
.word TIM6_DAC_IRQHandler
.word TIM7_IRQHandler
.word DMA2_Stream0_IRQHandler
.word DMA2_Stream1_IRQHandler
.word DMA2_Stream2_IRQHandler
.word DMA2_Stream3_IRQHandler
.word DMA2_Stream4_IRQHandler
.word ETH_IRQHandler
.word ETH_WKUP_IRQHandler
.word CAN2_TX_IRQHandler
.word CAN2_RX0_IRQHandler
.word CAN2_RX1_IRQHandler
.word CAN2_SCE_IRQHandler
.word OTG_FS_IRQHandler
.word DMA2_Stream5_IRQHandler
.word DMA2_Stream6_IRQHandler
.word DMA2_Stream7_IRQHandler
.word USART6_IRQHandler
.word I2C3_EV_IRQHandler
.word I2C3_ER_IRQHandler
.word OTG_HS_EP1_OUT_IRQHandler
.word OTG_HS_EP1_IN_IRQHandler
.word OTG_HS_WKUP_IRQHandler
.word OTG_HS_IRQHandler
.word DCMI_IRQHandler
.word CRYP_IRQHandler
.word HASH_RNG_IRQHandler

_vectors_end:


// Major/Minor version number of the software - Used by TFTP to see if we need to reprogram  
// Note the offset can be found by looking for "_vectors_end", in the map 
//
  .short VERSION_MAJOR  // 18C
  .short VERSION_MINOR  // 18E  
  .word __FLASH_segment_used_end__   // 190   //Save space for code size



#ifdef VECTORS_IN_RAM
  .section .vectors_ram, "ax"
_vectors_ram:
  .space _vectors_end-_vectors, 0
#endif

  .section .init, "ax"
  .thumb_func

reset_handler:

#ifndef __NO_SYSTEM_INIT
  ldr r0, =__RAM_segment_end__
  mov sp, r0
  bl SystemInit
#endif

#ifdef VECTORS_IN_RAM
  ldr r0, =__vectors_load_start__
  ldr r1, =__vectors_load_end__
  ldr r2, =_vectors_ram
l0:
  cmp r0, r1
  beq l1
  ldr r3, [r0], #4
  str r3, [r2], #4
  b l0
l1:
#endif



#ifndef __TARGET_F0XX
  /* Configure vector table offset register */
  ldr r0, =0xE000ED08
#ifdef VECTORS_IN_RAM
  ldr r1, =_vectors_ram
#else
  ldr r1, =_vectors
#endif
  str r1, [r0]
#endif

  b _start

#ifndef __NO_SYSTEM_INIT
  .thumb_func
  .weak SystemInit
SystemInit:
  bx lr
#endif

DEFAULT_ISR_HANDLER NMI_Handler
DEFAULT_ISR_HANDLER HardFault_Handler 
DEFAULT_ISR_HANDLER MemManage_Handler 
DEFAULT_ISR_HANDLER BusFault_Handler 
DEFAULT_ISR_HANDLER UsageFault_Handler 
DEFAULT_ISR_HANDLER SVC_Handler 
DEFAULT_ISR_HANDLER DebugMon_Handler  
DEFAULT_ISR_HANDLER PendSV_Handler
DEFAULT_ISR_HANDLER SysTick_Handler 
DEFAULT_ISR_HANDLER WWDG_IRQHandler
DEFAULT_ISR_HANDLER PVD_IRQHandler
DEFAULT_ISR_HANDLER TAMP_STAMP_IRQHandler
DEFAULT_ISR_HANDLER RTC_WKUP_IRQHandler
DEFAULT_ISR_HANDLER FLASH_IRQHandler
DEFAULT_ISR_HANDLER RCC_IRQHandler
DEFAULT_ISR_HANDLER EXTI0_IRQHandler
DEFAULT_ISR_HANDLER EXTI1_IRQHandler
DEFAULT_ISR_HANDLER EXTI2_IRQHandler
DEFAULT_ISR_HANDLER EXTI3_IRQHandler
DEFAULT_ISR_HANDLER EXTI4_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream0_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream1_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream2_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream3_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream4_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream5_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream6_IRQHandler
DEFAULT_ISR_HANDLER ADC_IRQHandler
DEFAULT_ISR_HANDLER CAN1_TX_IRQHandler
DEFAULT_ISR_HANDLER CAN1_RX0_IRQHandler
DEFAULT_ISR_HANDLER CAN1_RX1_IRQHandler
DEFAULT_ISR_HANDLER CAN1_SCE_IRQHandler
DEFAULT_ISR_HANDLER EXTI9_5_IRQHandler
DEFAULT_ISR_HANDLER TIM1_BRK_TIM9_IRQHandler
DEFAULT_ISR_HANDLER TIM1_UP_TIM10_IRQHandler
DEFAULT_ISR_HANDLER TIM1_TRG_COM_TIM11_IRQHandler
DEFAULT_ISR_HANDLER TIM1_CC_IRQHandler
DEFAULT_ISR_HANDLER TIM2_IRQHandler
DEFAULT_ISR_HANDLER TIM3_IRQHandler
DEFAULT_ISR_HANDLER TIM4_IRQHandler
DEFAULT_ISR_HANDLER I2C1_EV_IRQHandler
DEFAULT_ISR_HANDLER I2C1_ER_IRQHandler
DEFAULT_ISR_HANDLER I2C2_EV_IRQHandler
DEFAULT_ISR_HANDLER I2C2_ER_IRQHandler
DEFAULT_ISR_HANDLER SPI1_IRQHandler
DEFAULT_ISR_HANDLER SPI2_IRQHandler
DEFAULT_ISR_HANDLER USART1_IRQHandler
DEFAULT_ISR_HANDLER USART2_IRQHandler
DEFAULT_ISR_HANDLER USART3_IRQHandler
DEFAULT_ISR_HANDLER EXTI15_10_IRQHandler
DEFAULT_ISR_HANDLER RTC_Alarm_IRQHandler
DEFAULT_ISR_HANDLER OTG_FS_WKUP_IRQHandler
DEFAULT_ISR_HANDLER TIM8_BRK_TIM12_IRQHandler
DEFAULT_ISR_HANDLER TIM8_UP_TIM13_IRQHandler
DEFAULT_ISR_HANDLER TIM8_TRG_COM_TIM14_IRQHandler
DEFAULT_ISR_HANDLER TIM8_CC_IRQHandler
DEFAULT_ISR_HANDLER DMA1_Stream7_IRQHandler
DEFAULT_ISR_HANDLER FSMC_IRQHandler
DEFAULT_ISR_HANDLER SDIO_IRQHandler
DEFAULT_ISR_HANDLER TIM5_IRQHandler
DEFAULT_ISR_HANDLER SPI3_IRQHandler
DEFAULT_ISR_HANDLER UART4_IRQHandler
DEFAULT_ISR_HANDLER UART5_IRQHandler
DEFAULT_ISR_HANDLER TIM6_DAC_IRQHandler
DEFAULT_ISR_HANDLER TIM7_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream0_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream1_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream2_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream3_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream4_IRQHandler
DEFAULT_ISR_HANDLER ETH_IRQHandler
DEFAULT_ISR_HANDLER ETH_WKUP_IRQHandler
DEFAULT_ISR_HANDLER CAN2_TX_IRQHandler
DEFAULT_ISR_HANDLER CAN2_RX0_IRQHandler
DEFAULT_ISR_HANDLER CAN2_RX1_IRQHandler
DEFAULT_ISR_HANDLER CAN2_SCE_IRQHandler
DEFAULT_ISR_HANDLER OTG_FS_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream5_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream6_IRQHandler
DEFAULT_ISR_HANDLER DMA2_Stream7_IRQHandler
DEFAULT_ISR_HANDLER USART6_IRQHandler
DEFAULT_ISR_HANDLER I2C3_EV_IRQHandler
DEFAULT_ISR_HANDLER I2C3_ER_IRQHandler
DEFAULT_ISR_HANDLER OTG_HS_EP1_OUT_IRQHandler
DEFAULT_ISR_HANDLER OTG_HS_EP1_IN_IRQHandler
DEFAULT_ISR_HANDLER OTG_HS_WKUP_IRQHandler
DEFAULT_ISR_HANDLER OTG_HS_IRQHandler
DEFAULT_ISR_HANDLER DCMI_IRQHandler
DEFAULT_ISR_HANDLER CRYP_IRQHandler
DEFAULT_ISR_HANDLER HASH_RNG_IRQHandler
DEFAULT_ISR_HANDLER FPU_IRQHandler


#ifndef STARTUP_FROM_RESET
  .thumb_func
reset_wait:
1: b 1b /* endless loop */
#endif /* STARTUP_FROM_RESET */
