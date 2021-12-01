/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 */

//=========================================================================
//  File Name: mac_addr.c
//
//  Description:
//      functions to handle storing and getting the MAC address.
//      depending on the application the MAC may be stored in OTP,
//      or it could be in an external EEPROM.
//  
//  Modification History:
//      $Id: mac_addr.c,v 1.1 2014/06/24 18:37:56 dvaughn Exp $
//      $Log: mac_addr.c,v $
//      Revision 1.1  2014/06/24 18:37:56  dvaughn
//      Initial commit
//
//      Revision 1.1  2014/06/20 12:35:02  dvaughn
//      Initial Commit
//
//
//=========================================================================
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f2xx.h"
#include "mac_addr.h"
#include "persist.h"
#include "main.h"

// In the STM32F2xx parts there are 512 bytes of OTP memory, these are organized as 
// 16 blocks of 32 bytes each.  For each block there is a lock byte which prohibits 
// any future changes.  As with any FLASH memory, the bits start of as all ones and
// bits can be written to a 0.  For a full description see ST Doc PM0059, section 1.7
// see also stm32f2xx_flash.h and stm32f2xx_flash.c

// For the MAC we use the first block of OTP, we only
// have 6 bytes to store, but we use the entire 32 byte block.  
#define MAC_OTP_ADDR    0x1fff7800
#define MAC_LOCK_BYTE   0x1fff7A00

// mac for AtteroTech:00:00:00
static const uint8_t tempMAC[6] = {0x00, 0x1c, 0xe2, 0x00, 0x00, 0x00};

bool g_defaultMACinUse = false;

// function to read the MAC address
static void readMAC(uint8_t *mac_addr_data)
{
#if MAC_ADDR_IN_OTP
    uint8_t i;
    uint32_t MAC_Addr = MAC_OTP_ADDR;

    for (i = 0; i < MAC_ADDR_SIZE; i++)
    {
        mac_addr_data[i] = *(__IO uint8_t*)MAC_Addr++; 
    }
#else
    persist_ReadMAC(mac_addr_data);
#endif
}

// function to write a MAC to non volatile store
int writeMAC(const uint8_t *mac_addr_data)
{
#if MAC_ADDR_IN_OTP
    FLASH_Status flashOK;
    uint32_t OTP_MAC_Addr = MAC_OTP_ADDR;
    uint8_t i;

    // first need to unlock the Flash control register
    FLASH_Unlock();
    // write the data to the OTP bytes 
    for (i = 0; i < MAC_ADDR_SIZE; i++)
    {
        flashOK = FLASH_ProgramByte(OTP_MAC_Addr++, mac_addr_data[i] );
        if (FLASH_COMPLETE != flashOK)
        {
            // something bad happened, just lock the flash and leave
            FLASH_Lock();
            return -1;
        }
    }
    
    // now set lock byte to make the MAC un-modifiable for evermore
    flashOK = FLASH_ProgramByte(MAC_LOCK_BYTE, 0 );
    if (FLASH_COMPLETE != flashOK)
    {
        // something bad happened, just lock the flash and leave
        FLASH_Lock();
        return -2;
    }
    
    // we are done, lock the flash and return
    FLASH_Lock();
#else
    persist_WriteMAC(mac_addr_data);
#endif
    return 0;
}


static void testMACandFix(uint8_t *macData)
{
    uint16_t macTotal;
    uint8_t i;

    // determine if the box has ever had a MAC loaded.
    // if no MAC is present then it will be all ones (oxFF)
    // if the MAC is all ones jam a known MAC (AtteroTech:000000)
    // the production tool will detect this and then send a control Packet to
    // write a real MAC
    macTotal = 0;
    for (i = 0; i < MAC_ADDR_SIZE; i++)
    {
        macTotal += macData[i];
    }
    
    // 0xFF * 6 = 0x5FA
    if (0x5FA == macTotal)
    {
        memcpy(macData, tempMAC, MAC_ADDR_SIZE);
        g_defaultMACinUse = true;
    }
}

// function used by netconf (or any other external caller) to get the MAC
// returns either the MAC stored in non volatile memory, or if one is not preset
// returns the AtteroTech default MAC
void getMAC(uint8_t *mac_addr_data)
{
    readMAC(mac_addr_data);
    testMACandFix(mac_addr_data);
}

