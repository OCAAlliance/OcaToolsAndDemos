/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 */

//=============================================================================
//  File Name: led_matrix.c
//
//  Description:
//      perfroms the processing for the LED matrix connected to port E
//
//  Modification history:
//      $Id:  $
//      $Log:  $
//
//=============================================================================
#include <stdint.h>
#include "main.h"


#define NUM_LED_ROWS    7
#define NUM_LED_COLS    8

static const uint16_t c_ledColMask[NUM_LED_COLS] = {0x00FE, 0x00FD, 0x00FB, 0x00F7, 0x00EF, 0x00DF, 0x00BF, 0x007F};
static uint16_t ledRowData[NUM_LED_COLS] =         {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};


#define METER_DB_BIT_MASK   0x7E00  // rows 1..6 are used to set the db meter values
#define METER_DB_0          0x7E00
#define METER_DB_N3         0x3E00
#define METER_DB_N6         0x1E00
#define METER_DB_N12        0x0E00
#define METER_DB_N18        0x0600
#define METER_DB_N42        0x0200
#define SWITCH_LED_MASK     0x0100  // row 0 has the switch LEDs

void setDbRawLED(uint8_t channel, uint8_t rawLED)
{
    if ((channel > 0) && (channel <= NUM_LED_COLS))
    {
        // need to make channel 0..NUM_LED_COLS-1
        channel--;
 
        // led bits in raw LED are the lower order 6 bits
        // then need shifted left by 9 bits to be the correct value 
    
        // first get rid of any existing settings
        ledRowData[channel] &= SWITCH_LED_MASK;
        // or in the new meter setting
        ledRowData[channel] |= ((uint16_t)rawLED << 9);
    }
}


void setDbMeter(uint8_t channel, meterDbSetting_t meterSetting)
{    
    if ((channel > 0) && (channel <= NUM_LED_COLS))
    {
        // need to make channel 0..NUM_LED_COLS-1
        channel--;
        switch(meterSetting)
        {
            case Meter_Off:
                // only thing we need to keep is the Switch Led bit (row 0 bit) 
                ledRowData[channel] &= SWITCH_LED_MASK;
            break;
            
            case Meter_0db:
                // get rid of the meter setting, keep the switch setting
                ledRowData[channel] &= SWITCH_LED_MASK;
                // or in the new meter setting
                ledRowData[channel] |= METER_DB_0;
            break;

            case Meter_n3db:
                // get rid of the meter setting, keep the switch setting
                ledRowData[channel] &= SWITCH_LED_MASK;
                // or in the new meter setting
                ledRowData[channel] |= METER_DB_N3;
            break;
            
            case Meter_n6db: 
                // get rid of the meter setting, keep the switch setting
                ledRowData[channel] &= SWITCH_LED_MASK;
                // or in the new meter setting
                ledRowData[channel] |= METER_DB_N6;
            break;
            
            case Meter_n12db: 
                // get rid of the meter setting, keep the switch setting
                ledRowData[channel] &= SWITCH_LED_MASK;
                // or in the new meter setting
                ledRowData[channel] |= METER_DB_N12;
            break;
            
            case Meter_n18db: 
                // get rid of the meter setting, keep the switch setting
                ledRowData[channel] &= SWITCH_LED_MASK;
                // or in the new meter setting
                ledRowData[channel] |= METER_DB_N18;
            break;
            
            case Meter_n42db: 
                // get rid of the meter setting, keep the switch setting
                ledRowData[channel] &= SWITCH_LED_MASK;
                // or in the new meter setting
                ledRowData[channel] |= METER_DB_N42;
            break;
            
            default:
            break;
       }
    }
}
    
void setSwitchLed(uint8_t switchNum, ledState_t ledState)
{
    if ((switchNum > 0) && (switchNum <= NUM_LED_COLS))
    {
        // need to make switchNum 0..NUM_LED_COLS-1
        switchNum--;
        if (Led_Off == ledState)
        {
            // keep just the Meter bits
            ledRowData[switchNum] &= METER_DB_BIT_MASK;
        }
        else
        {
            ledRowData[switchNum] |= SWITCH_LED_MASK;
        }    
    }
}


void processLedArray(void)
{
    uint16_t ledOut;
    static uint8_t i = 0;
    
    ledOut = c_ledColMask[i] | (~ledRowData[i] & 0xFF00);
    GPIOE->ODR = ledOut;
   
    i++;
    if (i >= NUM_LED_COLS)
    {
        i = 0;
    }   
}

