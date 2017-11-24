/* File: startup_ARMCM3.S
 * Purpose: startup file for Cortex-M3 devices. Should use with
 *   GCC for ARM Embedded Processors
 * Version: V2.01
 * Date: 12 June 2014
 *
 */
/* Copyright (c) 2011 - 2014 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/
	.syntax	unified
	.arch	armv7-m

	.section .stack
	.align	3
#ifdef __STACK_SIZE
	.equ	Stack_Size, __STACK_SIZE
#else
	.equ	Stack_Size, 0x00000400
#endif
	.globl	__StackTop
	.globl	__StackLimit
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

	.section .heap
	.align	3
#ifdef __HEAP_SIZE
	.equ	Heap_Size, __HEAP_SIZE
#else
	.equ	Heap_Size, 0x00000C00
#endif
	.globl	__HeapBase
	.globl	__HeapLimit
__HeapBase:
	.if	Heap_Size
	.space	Heap_Size
	.endif
	.size	__HeapBase, . - __HeapBase
__HeapLimit:
	.size	__HeapLimit, . - __HeapLimit

	.section .vectors
	.align	2
	.globl	__Vectors
__Vectors:
	.long	__StackTop            /* Top of Stack */                        
	.long	Reset_Handler         /* Reset Handler */                       
	.long	NMI_Handler           /* NMI Handler */                         
	.long	HardFault_Handler     /* Hard Fault Handler */                  
	.long	MemManage_Handler     /* MPU Fault Handler */                   
	.long	BusFault_Handler      /* Bus Fault Handler */                   
	.long	UsageFault_Handler    /* Usage Fault Handler */                 
	.long	0                     /* Reserved */                            
	.long	0                     /* Reserved */                            
	.long	0                     /* Reserved */                            
	.long	0                     /* Reserved */                            
	.long	vPortSVCHandler       /* SVCall Handler */                  
	.long	0                     /* Debug Monitor Handler */               
	.long	0                     /* Reserved */                            
	.long	xPortPendSVHandler    /* PendSV Handler */                  
    .long	xPortSysTickHandler   /*      __cs3_isr_systick               */  
    .long	0                     /*      __cs3_isr_wwdg                  */     
    .long	0                     /*      __cs3_isr_pvd                   */     
    .long	0                     /*      __cs3_isr_tamp_stamp            */     
    .long	0                     /*      __cs3_isr_rtc_wkup              */     
    .long	0                     /*      __cs3_isr_flash                 */     
    .long	0                     /*      __cs3_isr_rcc                   */     
    .long	0                     /*      __cs3_isr_exti0                 */     
    .long	0                     /*      __cs3_isr_exti1                 */     
    .long	0                     /*      __cs3_isr_exti2                 */     
    .long	0                     /*      __cs3_isr_exti3                 */     
    .long	0                     /*      __cs3_isr_exti4                 */     
    .long	0                     /*      __cs3_isr_dma1_stream0          */     
    .long	0                     /*      __cs3_isr_dma1_stream1          */     
    .long	0                     /*      __cs3_isr_dma1_stream2          */     
    .long	0                     /*      __cs3_isr_dma1_stream3          */     
    .long	0                     /*      __cs3_isr_dma1_stream4          */     
    .long	0                     /*      __cs3_isr_dma1_stream5          */     
    .long	0                     /*      __cs3_isr_dma1_stream6          */     
    .long	0                     /*      __cs3_isr_adc                   */     
    .long	0                     /*      __cs3_isr_can1_tx               */     
    .long	0                     /*      __cs3_isr_can1_rx0              */     
    .long	0                     /*      __cs3_isr_can1_rx1              */     
    .long	0                     /*      __cs3_isr_can1_sce              */     
    .long	0                     /*      __cs3_isr_EXTI9_5               */     
    .long	0                     /*      __cs3_isr_tim1_brk_tim9         */     
    .long	0                     /*      __cs3_isr_tim1_up_tim10         */     
    .long	0                     /*      __cs3_isr_tim1_trg_com_tim11    */     
    .long	0                     /*      __cs3_isr_tim1_cc               */     
    .long	0                     /*      __cs3_isr_tim2                  */     
    .long	0                     /*      __cs3_isr_tim3                  */     
    .long	0                     /*      __cs3_isr_tim4                  */     
    .long	0                     /*      __cs3_isr_i2c1_ev               */     
    .long	0                     /*      __cs3_isr_i2c1_er               */     
    .long	0                     /*      __cs3_isr_i2c2_ev               */     
    .long	0                     /*      __cs3_isr_i2c2_er               */     
    .long	0                     /*      __cs3_isr_spi1                  */     
    .long	0                     /*      __cs3_isr_spi2                  */     
    .long	USART1_IRQHandler     /*      __cs3_isr_usart1      */   
    .long	0                     /*      __cs3_isr_usart2                */     
    .long	0                     /*      __cs3_isr_usart3                */     
    .long	EXTI15_10_IRQHandler  /*      __cs3_isr_exti15_10 */     
    .long	0                     /*      __cs3_isr_rtc_alarm             */     
    .long	0                     /*      __cs3_isr_otg_fs_wkup           */     
    .long	0                     /*      __cs3_isr_tim8_brk_tim12        */     
    .long	0                     /*      __cs3_isr_tim8_up_tim13         */     
    .long	0                     /*      __cs3_isr_tim8_trg_com_tim144   */     
    .long	0                     /*      __cs3_isr_tim8_cc               */     
    .long	0                     /*      __cs3_isr_dma1_stream7          */     
    .long	0                     /*      __cs3_isr_fsmc                  */     
    .long	0                     /*      __cs3_isr_sdio                  */     
    .long	0                     /*      __cs3_isr_tim5                  */     
    .long	0                     /*      __cs3_isr_spi3                  */     
    .long	0                     /*      __cs3_isr_uart4                 */     
    .long	0                     /*      __cs3_isr_uart5                 */     
    .long	0                     /*      __cs3_isr_tim6_dac              */     
    .long	0                     /*      __cs3_isr_tim7                  */     
    .long	0                     /*      __cs3_isr_dma2_stream0          */     
    .long	0                     /*      __cs3_isr_dma2_stream1          */     
    .long	0                     /*      __cs3_isr_dma2_stream2          */     
    .long	0                     /*      __cs3_isr_dma2_stream3          */     
    .long	0                     /*      __cs3_isr_dma2_stream4          */     
    .long	ETH_IRQHandler        /*      __cs3_ethernet    /*
    .long	0                     /*      __cs3_ethernet_wk_exti          */     
    .long	0                     /*      __cs3_isr_can2_tx               */     
    .long	0                     /*      __cs3_isr_can2_rx0              */     
    .long	0                     /*      __cs3_isr_can2_rx1              */     
    .long	0                     /*      __cs3_isr_can2_sce              */     
    .long	0                     /*      __cs3_usb_otg_fs                */     
    .long	0                     /*      __cs3_dma2_channel5             */     
    .long	0                     /*      __cs3_dma2_channel6             */     
    .long	0                     /*      __cs3_dma2_channel7             */     
    .long	0                     /*      __cs3_uart6                     */     
    .long	0                     /*      __cs3_i2c3_ev                   */     
    .long	0                     /*      __cs3_i2c3_er                   */     
    .long	0                     /*      __cs3_usb_otg_hs_ep1_out        */     
    .long	0                     /*      __cs3_usb_otg_hs_ep1_in         */     
    .long	0                     /*      __cs3_usb_otg_hs_ep1_wk         */     
    .long	0                     /*      __cs3_usb_otg_hs                */     
    .long	0                     /*      __cs3_dcmmi                     */     
    .long	0                     /*      __cs3_cryp crypto               */     
    .long	0                     /*      __cs3_Hash and Rng              */     

	/* External interrupts */
	.long	Default_Handler

	.size	__Vectors, . - __Vectors

	.text
	.thumb
	.thumb_func
	.align	2
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
/*  It copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r4, =__copy_table_start__
	ldr	r5, =__copy_table_end__

.L_loop0:
	cmp	r4, r5
	bge	.L_loop0_done
	ldr	r1, [r4]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]

.L_loop0_0:
	subs	r3, #4
	ittt	ge
	ldrge	r0, [r1, r3]
	strge	r0, [r2, r3]
	bge	.L_loop0_0

	adds	r4, #12
	b	.L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__etext
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__

.L_loop1:
	cmp	r2, r3
	ittt	lt
	ldrlt	r0, [r1], #4
	strlt	r0, [r2], #4
	blt	.L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
	ldr	r3, =__zero_table_start__
	ldr	r4, =__zero_table_end__

.L_loop2:
	cmp	r3, r4
	bge	.L_loop2_done
	ldr	r1, [r3]
	ldr	r2, [r3, #4]
	movs	r0, 0

.L_loop2_0:
	subs	r2, #4
	itt	ge
	strge	r0, [r1, r2]
	bge	.L_loop2_0

	adds	r3, #8
	b	.L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__

	movs	r0, 0
.L_loop3:
	cmp	r1, r2
	itt	lt
	strlt	r0, [r1], #4
	blt	.L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
	bl	SystemInit
#endif

#ifndef __START
#define __START _start
#endif
	bl	__START

	.pool
	.size	Reset_Handler, . - Reset_Handler

	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.set	\handler_name, Default_Handler
	.endm

	.end
