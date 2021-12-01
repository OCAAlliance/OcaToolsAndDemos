/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 */

//=============================================================================
//  File Name: persist.c
//
//  Note:
//     If the amount of persistant data or any of the persistant data types change
//     then m_flashPersistVersion needs to be incremented.
//
//     IMPORTANT - this function is pretty much married to the device being used, 
//                 in this case the CAT24C256.  If re-using this code make sure 
//                 page size and num pages are adjusted for the device you are using.
//                 When all else fails, read the data sheet for the part you are using.
//
//  Description:
//      Persistent stoarage functions for 
//
//  Modification history:
//      $Id:  $
//      $Log:  $
//
//
//=============================================================================
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "i2c_handler.h"
#include  "main.h"

#include "persist.h"

#define FLASHADDRESS      0x50
#define INITIALCHECKSUM   0xAA

#define NUM_EEPROM_PAGES    512  // current eePROM is a CAT24C256 which has 512 64 byte pages for a total of 32K
#define EEPROM_PAGE_SIZE    64

extern uint8_t g_mac_addr[];

static const unsigned char m_flashPersistVersion = 2;

typedef struct
{
    unsigned short  size;
    unsigned long   checksum;
    unsigned char   flashVersion;
} eePromHeader_t;


persistData_t g_persistData;

         char g_RemoteHubName[SERVICE_NAME_LENGTH];
uint8_t g_static_ip_addr[IPV4_ADDR_SIZE]      = {0,0,0,0};
uint8_t g_static_netmask_addr[IPV4_ADDR_SIZE] = {0,0,0,0};
uint8_t g_static_gw_addr[IPV4_ADDR_SIZE]      = {0,0,0,0};



//Flash layout
// eePromHeader at 0
// unsigned short Size - Just the size of the data starting in the 2nd page (Does not include Size or Checksum)
// unsigned long  Checksum - calculated by adding all the bytes in the persistData structure
// unsigned char  flashVersion - this is just a variable so that if the persistent data changes possibly miss assigned
//                               data is not assigned to the wrong variables, change this if the structure of the persistent
//                               data changes
// non-vol variables from location 64 on
#define FLASH_HEADER_ADDR 0

// very important, for flash longevity and to optimize eePROM writes the data needs to be on a 64 byte
// page boundry.  See the CAT24C256 datasheet "Page Writing" and "Sequential Reading" 
#define VARSTART_ADDR (EEPROM_PAGE_SIZE * 1)        // start on the second page so data is on a 64 byte boundry

//Put the systems MAC address at end of the external flash
#define VARMAC_ADDR ((NUM_EEPROM_PAGES*EEPROM_PAGE_SIZE) - 6)  

static eePromHeader_t m_eePromHeader;

static unsigned char  sEEAddress = (FLASHADDRESS<<1);   

static unsigned long persist_WriteVars(void);
static unsigned long persist_CalcChecksum(const unsigned char *data, unsigned short length);
static xTimerHandle m_persistWriteTimer;
static void persist_WriteDataTimerCallback(xTimerHandle xTimer);

// 30 second time out
#define PERSIST_TIMER_TIMEOUT   (30000 / portTICK_RATE_MS)

//=============================================================================
// Function: persist_WriteGlobalDataTimerCallback
//
// Description:
//      Callback function for when the persistWriteTimer times out.
//
// Returns:
//  void
//
//=============================================================================
static void persist_WriteDataTimerCallback(xTimerHandle xTimer)
{
    (void) xTimer;
    
    (void)persist_WriteVars();
}

//=============================================================================
// Function: persist_SetDirtyFlag
//
// Description:
//      Will restart (or just start if never started) the persist write timer
//      if another restart is not performed within 60 seconds the non-vol 
//      parameters will be written to EEPROM
//
// Returns:
//  void
//
//=============================================================================
void persist_SetDirtyFlag(void)
{
    (void)xTimerReset(m_persistWriteTimer, 0);
}


//=============================================================================
// Function: persist_Init
//
// Description:
//  This routine reads the persistent data from EEPROM and then copies 
//  where it is needed.
//
// Returns:
//
//=============================================================================
void persist_Init(void)
{   
    unsigned long checksum;
    persistData_t *tempPersistData;

    // read the eePROM header from the first block of eePROM
    (void)I2C_1_ReadData(sEEAddress, FLASH_HEADER_ADDR, (unsigned char *)&m_eePromHeader, sizeof(eePromHeader_t), true);

    // check to see if the flash size is bogus
    // needs to be less than the size of the xfer buffer
    if (sizeof(persistData_t) == m_eePromHeader.size )
    {
        tempPersistData = malloc(sizeof(persistData_t));
        if (NULL != tempPersistData)
        {
            // read the data from flash to the temp Buffer;
            (void)I2C_1_ReadData(sEEAddress, VARSTART_ADDR, (unsigned char *)tempPersistData, sizeof(persistData_t), true);
      
            // calcualte the checksum on the data read
            checksum = persist_CalcChecksum((unsigned char *)tempPersistData, sizeof(persistData_t));
            
            if ((checksum == m_eePromHeader.checksum) && (m_eePromHeader.flashVersion == m_flashPersistVersion))
            {
                // checksum and version are OK, copy the data to the persist global.
                memcpy((void *)&g_persistData, tempPersistData, sizeof(persistData_t));
                // set up some of the data that there are "copies" of
                if (0 == g_persistData.remoteHubName[0])
                {
                    sprintf(g_RemoteHubName,"REMOTEHUB%02x%02x%02x",  g_mac_addr[3], g_mac_addr[4], g_mac_addr[5]);
                }
                else
                {
                    memcpy(g_RemoteHubName,g_persistData.remoteHubName,SERVICE_NAME_LENGTH);
                }
                                
                memcpy(g_static_ip_addr, g_persistData.staticIPaddr, IPV4_ADDR_SIZE);
                memcpy(g_static_netmask_addr, g_persistData.staticNetmask, IPV4_ADDR_SIZE);
                memcpy(g_static_gw_addr, g_persistData.staticGWaddr, IPV4_ADDR_SIZE);
            }
            else
            {
                (void)persist_WriteVars();
            }   
            free(tempPersistData);
        }
    }
    else
    {
        (void)persist_WriteVars();
    }   
     
     
     
    // create a timer for writing the global persistent data.
    // this does not start the timer, it just creates it. 
    m_persistWriteTimer = xTimerCreate((const signed char *)"PersistTimer",
                                       PERSIST_TIMER_TIMEOUT, 
                                       pdFALSE, 
                                       0,
                                       persist_WriteDataTimerCallback);
}

//=============================================================================
// Function: persist_WriteVars
//
// Description:
//  Writes all the persistant variables from SRAM to FLASH. It also writes the 
//  size and checksum.
//
// Returns:
//  Returns true if it fails
//
//=============================================================================
static unsigned long persist_WriteVars(void)
{
    unsigned short sizeSum = 0;
    unsigned short totalSize = 0;
    unsigned long checksum = INITIALCHECKSUM;
    unsigned char  *dataPtr = (unsigned char *)&g_persistData;

    // since we don't know exactly when the dog was petted last
    // we need to pet the dog just prior to writing the non-vars

    totalSize = sizeof(persistData_t);

    memcpy(g_persistData.staticIPaddr, g_static_ip_addr, IPV4_ADDR_SIZE);
    memcpy(g_persistData.staticNetmask, g_static_netmask_addr, IPV4_ADDR_SIZE);
    memcpy(g_persistData.staticGWaddr, g_static_gw_addr, IPV4_ADDR_SIZE);

    // calculate checksum
    checksum = persist_CalcChecksum((unsigned char *)&g_persistData, totalSize);
    sizeSum = 0;
    
    // write out as many EEPROM_PAGE_SIZE byte blocks as we can, this maximizes the throughput to the eePROM
    while (totalSize > EEPROM_PAGE_SIZE)
    {
        if (I2C_1_WriteData(sEEAddress, VARSTART_ADDR+sizeSum, &dataPtr[sizeSum], EEPROM_PAGE_SIZE, true))
        {
            return true;
        }
        sizeSum += EEPROM_PAGE_SIZE;
        totalSize -= EEPROM_PAGE_SIZE;
        // need to delay at least 5 msec between writes
        vTaskDelay(6);
    } 
    
    // write any data that is left
    if (totalSize > 0)
    {
        if (I2C_1_WriteData(sEEAddress, VARSTART_ADDR+sizeSum, &dataPtr[sizeSum], totalSize, true))
        {
            return true;
        }
        sizeSum += totalSize;
        vTaskDelay(6);
    }
             
    // write the eeProm header
    m_eePromHeader.flashVersion = m_flashPersistVersion;
    m_eePromHeader.checksum = checksum;
    m_eePromHeader.size = sizeSum;    
    if (I2C_1_WriteData(sEEAddress, FLASH_HEADER_ADDR, (unsigned char *)&m_eePromHeader, sizeof(eePromHeader_t), true))
    {
        return true;
    }
    vTaskDelay(6);

    return false;
}


//=============================================================================
// Function: persist_CalcChecksum
//
// Description:
//     Computes a simple additive checksum over a range of data  
//
// Returns:
//  Returns checksum 
//
//=============================================================================
static unsigned long persist_CalcChecksum(const unsigned char *data, unsigned short length)
{
    unsigned short ii;
    unsigned long checksum = INITIALCHECKSUM;
    
    for(ii = 0; ii < length; ii++)
    {
        checksum += data[ii];
    }
    return checksum;
}


//=============================================================================
// Function: persist_Erase_eeProm
//
// Description:
//  Sets the flash back to being all 0xFF
//
// Returns:
//  void
//
//=============================================================================
void persist_Erase_eeProm(void)
{
    unsigned short i;
    unsigned char *eraseData;
    
    eraseData = malloc(EEPROM_PAGE_SIZE);
    if (NULL != eraseData)
    {
        // set to 0xFF
        memset(eraseData, 0xFF, EEPROM_PAGE_SIZE);
    
        // write the erase data to all the pages
        for (i = 0; i < NUM_EEPROM_PAGES; i++)
        {
            (void)I2C_1_WriteData(sEEAddress, (uint16_t)(i*EEPROM_PAGE_SIZE), eraseData, EEPROM_PAGE_SIZE, true);
            vTaskDelay(6);        
        }
        free(eraseData);
    }
}

//=============================================================================
// Function: persist_ReadMAC
//
// Description:
//  copies the MAC address from flash at address VARMAC_ADDR to the 
//  passed in buffer, BUFFER MUST BE at least 6 bytes long no checking is done
//
// Returns:
//  void
//
//=============================================================================
void persist_ReadMAC(unsigned char *MACaddr)
{
    //READ the MAC address
    (void)I2C_1_ReadData(sEEAddress, VARMAC_ADDR, MACaddr, 6, true);
}

//=============================================================================
// Function: persist_WriteMAC
//
// Description:
//  Writes 6 bytes of data being pointed to by MACaddr into the flash at 
//  VARMAC_ADDR
//
// Returns:
//  void
//
//=============================================================================
void persist_WriteMAC(const unsigned char *MACaddr)
{
    (void)I2C_1_WriteData(sEEAddress, VARMAC_ADDR, MACaddr, 6, true);
    vTaskDelay(6);
}


