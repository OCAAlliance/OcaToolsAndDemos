/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 */

//=============================================================================
//  File Name: switch_processing.c
//
//  Description:
//      performs the OCA_Pi board switch scanning functions
//
//  Modification history:
//      $Id:  $
//      $Log:  $
//
//=============================================================================
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include <limits.h>
#include <app/Stm32CortexM3/OCALitePrototype/PISwitchBitStringSensor.h>
#include <app/Stm32CortexM3/OCALitePrototype/PIencoderInt8Sensor.h>

extern ::PISwitchBitStringSensor*  g_PI_Switches;
extern ::PIencoderInt8Sensor*      g_PI_encoders[];

extern volatile int32_t     encoder1_Cnt;
extern volatile int32_t     encoder2_Cnt;
static int32_t              prev_encoder1Cnt;
static int32_t              prev_encoder2Cnt;
extern int32_t              g_encoder1_Min;
extern int32_t              g_encoder1_Max;
extern int32_t              g_encoder2_Min;
extern int32_t              g_encoder2_Max;

// 10 ms periodicity (NOTE: Not guaranteed w/out preemptive task scheduling)
#define TMR_PERIODIC_TIME_MS (10)
#define NUMBER_OF_SWITCHES    8


typedef enum 
{
    RELEASED,
    PRESSED,
    HOLD,
    HOLDING
}DebounceState_t;

typedef struct
{
    GPIO_TypeDef *PortAddress;
    uint16_t Pin;
    void (*callbackOnPress)(uint8_t keyNum);
    uint8_t PressThreshHold;
    void (*callbackOnHold)(uint8_t keyNum);
    uint8_t HoldThreshHold;
    void (*callbackOnRelease)(uint8_t keyNum);
}Switch_Activity_t;

typedef struct
{
    uint16_t switchNum;
    uint32_t closedCount;
    uint32_t openCount;
    DebounceState_t debounceState;
    Switch_Activity_t switchActivity;
}Switch_Scan_t;


static void SW_KeyPressed(uint8_t keyNum);
static void SW_KeyHeld(uint8_t keyNum);
static void SW_KeyReleased(uint8_t keyNum);

static void ReadAndDebounce(Switch_Scan_t *p_switch);

// Switch scan table
// Times are in TMR_PERIODIC_TIME_MS
static Switch_Scan_t m_switches[NUMBER_OF_SWITCHES] =
{
  {1, 0, 0, RELEASED, {GPIOD, SW0, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} },
  {2, 0, 0, RELEASED, {GPIOD, SW1, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} },
  {3, 0, 0, RELEASED, {GPIOD, SW2, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} },
  {4, 0, 0, RELEASED, {GPIOD, SW3, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} },
  {5, 0, 0, RELEASED, {GPIOD, SW4, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} }, 
  {6, 0, 0, RELEASED, {GPIOD, SW5, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} },
  {7, 0, 0, RELEASED, {GPIOD, SW6, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} },
  {8, 0, 0, RELEASED, {GPIOD, SW7, SW_KeyPressed, 6, SW_KeyHeld, 50, SW_KeyReleased} }
};







//----------------------------------------------------------------------------
//  Function: ReadAndDebounce
//
//  Description:
//      This function will read and debounce a switch input.
//
//  Input Parameters:
//
//      Switch_Scan_t *p_switch- description below
//
//      SwitchState_t switchState - indicates the state of current switch, i.e.
//      OPEN or CLOSED.
//
//      callbackOnPress - function pointer to the callback function to call
//                        when the number of consecutive down presses reaches
//                        the
//                        PressThreshHold limit.
//
//      PressThreshHold - number of consecutive CLOSED events to indicate a
//                        valid press - fires callbackOnPress().
//
//      HoldThreshHold - number of consecutive CLOSED events to indicate that
//                       a hold condition occurs - fires callbackOnHold()
//
//
//  Returns: void
//
//-----------------------------------------------------------------------------
static void ReadAndDebounce(Switch_Scan_t *p_switch)
{
    uint8_t value;
    SwitchState_t state = OPEN;

    value =  GPIO_ReadInputDataBit(p_switch->switchActivity.PortAddress,
                                   p_switch->switchActivity.Pin);
    if ( 1 == value )
    {
        state = CLOSED;
    }
    switch(state)
    {
        case CLOSED:
        {
            if (RELEASED == p_switch->debounceState)
            {
               p_switch->debounceState = PRESSED;
            }
            else if (PRESSED == p_switch->debounceState)
            {
                p_switch->closedCount++;
                if ( p_switch->closedCount > p_switch->switchActivity.PressThreshHold ) 
                {
                    p_switch->debounceState = HOLD;
                    p_switch->openCount     = 0;
                    p_switch->closedCount   = 0;
                    if (NULL != p_switch->switchActivity.callbackOnPress)
                    {
                        p_switch->switchActivity.callbackOnPress(p_switch->switchNum);
                    }
                } 
            }
            else if (HOLD == p_switch->debounceState)
            {
                p_switch->closedCount++;
                if ( p_switch->closedCount > p_switch->switchActivity.HoldThreshHold ) 
                {
                    p_switch->debounceState = HOLDING;
                    p_switch->openCount     = 0;
                    p_switch->closedCount   = 0;
                    if ( NULL != p_switch->switchActivity.callbackOnHold )
                    {
                        p_switch->switchActivity.callbackOnHold(p_switch->switchNum);
                    }
                } 
            }
            break;
        }// case CLOSED:

        case OPEN:
        {
            if ( (HOLD == p_switch->debounceState) || (HOLDING == p_switch->debounceState)  )
            {
                p_switch->openCount++;
                if ( p_switch->openCount > p_switch->switchActivity.PressThreshHold )
                {
                    p_switch->debounceState = RELEASED;
                    p_switch->openCount     = 0;
                    p_switch->closedCount   = 0;
                    p_switch->switchActivity.callbackOnRelease(p_switch->switchNum);
                }
            }//  if ( (HOLD == p_switch->debounceState) || (HOLDING == p_switch->debounceState)  )

            break;
        }//  case OPEN:
          
        default:
        {
            break;
        }// default:
        
    }// switch()
}


//-----------------------------------------------------------------------------
//  Function:  OCA_Pi_ReadSwitches
//
//  Description:
//      This is a task that runs every 10 msec to read and debounce keys.
//
//  Parameters:
//      void *arg - not used - .
//
//  Returns:
//      void
//
//-----------------------------------------------------------------------------
void OCA_Pi_ReadSwitches(void *arg)
{
    
    (void)arg;  // quiet the compiler on the unused arg
    
    uint8_t             swIndex = 0;
    portTickType        xLastWakeTime;
    const portTickType  xFrequency = (TMR_PERIODIC_TIME_MS / portTICK_RATE_MS);
    OcaInt8             encoder1_min;
    OcaInt8             encoder1_max;
    OcaInt8             encoder2_min;
    OcaInt8             encoder2_max;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();

    while ((NULL == g_PI_encoders[0]) || (NULL == g_PI_encoders[1]))
    {
        vTaskDelay(5);
    }

    g_PI_encoders[0]->GetEncoderLimits(encoder1_min, encoder1_max);
    g_PI_encoders[1]->GetEncoderLimits(encoder2_min, encoder2_max);

    g_encoder1_Min = (int32_t)encoder1_min;
    g_encoder1_Max = (int32_t)encoder1_max;
    g_encoder2_Min = (int32_t)encoder2_min;
    g_encoder2_Max = (int32_t)encoder2_max;

    prev_encoder1Cnt = INT_MAX;
    prev_encoder2Cnt = INT_MAX;

    for (;;)
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    
        for ( swIndex = 0; swIndex < NUMBER_OF_SWITCHES; swIndex++ )
        {
            ReadAndDebounce(&m_switches[swIndex]);
        }

        // test to see if the encoders have changed
        if (encoder1_Cnt != prev_encoder1Cnt)
        {
            prev_encoder1Cnt = encoder1_Cnt;
            g_PI_encoders[0]->SetEncoderValue(encoder1_Cnt);
        }
        
        if (encoder2_Cnt != prev_encoder2Cnt)
        {
            prev_encoder2Cnt = encoder2_Cnt;
            g_PI_encoders[1]->SetEncoderValue(encoder2_Cnt);
        }

    } // end while
}



SwitchState_t getSwitchState(uint8_t switchNum)
{
    SwitchState_t switchState = OPEN;
    
    // call is for switchNum 1..8, index is 0..7
    switchNum--;
    if (switchNum < NUMBER_OF_SWITCHES)
    {
        if ((HOLD    == m_switches[switchNum].debounceState) ||
            (HOLDING == m_switches[switchNum].debounceState))
        {
            switchState = CLOSED;
        }
    }
    
    return switchState;
}

//-----------------------------------------------------------------------------
//  Function:  SW0_KeyPressed
//
//  Description:
//      Function stub to handle the SW0 key pressed event.  
//
//  Parameters:
//      None.
//
//  Returns:
//      None.
//
//-----------------------------------------------------------------------------
static void SW_KeyPressed(uint8_t keyNum )
{
   (void) g_PI_Switches->SetSwitchValue(::OcaUint16(keyNum-1), true);
}// static void SW0_KeyPressed(void)



//-----------------------------------------------------------------------------
//  Function:  SW0_KeyHeld
//
//  Description:
//      Function stub to handle the SW0 key held event.  
//
//  Parameters:
//      None.
//
//  Returns:
//      None.
//
//-----------------------------------------------------------------------------
static void SW_KeyHeld(uint8_t keyNum)
{
}// static void SW0_KeyHeld(void)


//-----------------------------------------------------------------------------
//  Function:  SW0_KeyReleased
//
//  Description:
//      Function stub to handle the SW0 key released event.  
//
//  Parameters:
//      None.
//
//  Returns:
//      None.
//
//-----------------------------------------------------------------------------
static void SW_KeyReleased(uint8_t keyNum)
{
   (void) g_PI_Switches->SetSwitchValue(::OcaUint16(keyNum-1), false);
}// static void SW0_KeyReleased(void)





