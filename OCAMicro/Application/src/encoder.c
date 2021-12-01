/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 */


#include "main.h"
#include <limits.h>

volatile int32_t     encoder1_Cnt = 0;
volatile int32_t     encoder2_Cnt = 0;

int32_t              g_encoder1_Min = INT_MIN;
int32_t              g_encoder1_Max = INT_MAX;
int32_t              g_encoder2_Min = INT_MIN;
int32_t              g_encoder2_Max = INT_MAX;


///////////////////////////////////////////////////////////////////////////////
//EXTIx IRQ Handlers
///////////////////////////////////////////////////////////////////////////////
void EXTI15_10_IRQHandler(void)
{
    // we will need multiple bits from the GPIOC IDR, read the whole register.
    uint16_t GPIOC_idr = GPIOC->IDR;
    
    // are we getting ENC2A interrupt
    if(EXTI_GetITStatus(ENC2_A_EXTI_LINE) == SET)
    {
        // Clear the pending bit
        EXTI_ClearITPendingBit(ENC2_A_EXTI_LINE);
        // what state is enc2A in
        if (GPIOC_idr & ENC2_A)
        {
            // this was a low to high transistion on ENC2A
            // what state is enc2B in?
            if (GPIOC_idr & ENC2_B)
            {
                // enc2A went high and enc2B is high, decrement the count
                encoder2_Cnt--;
            }
            else
            {
                // enc2A went high and enc2B is low, inccrement the count
                encoder2_Cnt++;               
            }
        }
        else
        {
            // this was a high to low transistion on ENC2A
            // what state is enc2B in?
            if (GPIOC_idr & ENC2_B)
            {
                // enc2A went low and enc2B is high, inccrement the count
                encoder2_Cnt++;               
            }
            else
            {
                // enc2A went low and enc2B is low, decrement the count
                encoder2_Cnt--;               
            }
        }
    }

    // are we getting a ENC2B interrupt?
    if(EXTI_GetITStatus(ENC2_B_EXTI_LINE) == SET)
    {
         // Clear the ENC2B pending bit
        EXTI_ClearITPendingBit(ENC2_B_EXTI_LINE);
        // what state is enc2B in
        if (GPIOC_idr & ENC2_B)
        {
            // this was a low to high transistion on ENC2B
            // what state is enc2A in?
            if (GPIOC_idr & ENC2_A)
            {
                // enc2B went high and enc2A is high, increment the count
                encoder2_Cnt++;
            }
            else
            {
                 // enc2B went high and enc2A is low, decrement the count
                encoder2_Cnt--;                      
            }
        }
        else
        {
            // this was a high to low transistion on ENC2B
            // what state is enc2A in?
            if (GPIOC_idr & ENC2_A)
            {
                 // enc2B went low and enc2A is high, decrement the count
                encoder2_Cnt--;               
            }
            else
            {
                 // enc2B went low and enc2A is low, increment the count
                encoder2_Cnt++;               
            }
        }
    }

    // are we getting ENC1A interrupt
    if(EXTI_GetITStatus(ENC1_A_EXTI_LINE) == SET)
    {
        // Clear the pending bit
        EXTI_ClearITPendingBit(ENC1_A_EXTI_LINE);
        // what state is enc1A in
        if (GPIOC_idr & ENC1_A)
        {
            // this was a low to high transistion on ENC1A
            // what state is enc1B in?
            if (GPIOC_idr & ENC1_B)
            {
                // enc1A went high and enc1B is high, decrement the count
                encoder1_Cnt--;
            }
            else
            {
                 // enc1A went high and enc1B is low, inccrement the count
                encoder1_Cnt++;               
            }
        }
        else
        {
            // this was a high to low transistion on ENC1A
            // what state is enc1B in?
            if (GPIOC_idr & ENC1_B)
            {
                 // enc1A went low and enc1B is high, inccrement the count
                encoder1_Cnt++;               
            }
            else
            {
                 // enc1A went low and enc1B is low, decrement the count
                encoder1_Cnt--;               
            }
        }
    }

    // are we getting a ENC1B interrupt?
    if(EXTI_GetITStatus(ENC1_B_EXTI_LINE) == SET)
    {
         // Clear the ENC1B pending bit
        EXTI_ClearITPendingBit(ENC1_B_EXTI_LINE);
        // what state is enc1B in
        if (GPIOC_idr & ENC1_B)
        {
            // this was a low to high transistion on ENC1B
            // what state is enc1A in?
            if (GPIOC_idr & ENC1_A)
            {
                // enc1B went high and enc1A is high, increment the count
                encoder1_Cnt++;
            }
            else
            {
                 // enc1B went high and enc1A is low, decrement the count
                encoder1_Cnt--;                              
            }
        }
        else
        {
            // this was a high to low transistion on ENC1B
            // what state is enc1A in?
            if (GPIOC_idr & ENC1_A)
            {
                 // enc1B went low and enc1A is high, decrement the count
                encoder1_Cnt--;               
            }
            else
            {
                 // enc1B went low and enc1A is low, increment the count
                encoder1_Cnt++;               
            }
        }
    }

    if (encoder1_Cnt > g_encoder1_Max)
    {
        encoder1_Cnt = g_encoder1_Min;
    }
    else if (encoder1_Cnt < g_encoder1_Min)
    {
        encoder1_Cnt = g_encoder1_Max;
    }
    
    if (encoder2_Cnt > g_encoder2_Max)
    {
        encoder2_Cnt = g_encoder2_Min;
    }
    else if (encoder2_Cnt < g_encoder2_Min)
    {
        encoder2_Cnt = g_encoder2_Max;
    }

}



////////////////////////////////////////////////////////////////////////////////
// initialize the encoder pins and set them up to interrupt
///////////////////////////////////////////////////////////////////////////////
void encoder_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = PORTC_IN_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, & GPIO_InitStructure);

    /* Connect EXTI Lines to INT Pin */
    SYSCFG_EXTILineConfig(ENC_EXTI_PORT_SRC, ENC2_A_EXTI_PIN_SRC);
    SYSCFG_EXTILineConfig(ENC_EXTI_PORT_SRC, ENC2_B_EXTI_PIN_SRC);
    SYSCFG_EXTILineConfig(ENC_EXTI_PORT_SRC, ENC1_A_EXTI_PIN_SRC);
    SYSCFG_EXTILineConfig(ENC_EXTI_PORT_SRC, ENC1_B_EXTI_PIN_SRC);

    /* Configure EXTI line */
    EXTI_InitStructure.EXTI_Line = (ENC2_A_EXTI_LINE | ENC2_B_EXTI_LINE | ENC1_A_EXTI_LINE | ENC1_B_EXTI_LINE);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set the EXTI interrupt to the highest priority */
    NVIC_InitStructure.NVIC_IRQChannel = ENC_EXTI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

