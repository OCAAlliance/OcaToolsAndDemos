/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 */

#include <string.h>
#include <stdio.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mac_addr.h"


#define TASK_DELAY  50          // task delay of 50 millisecond
#define CMD_BUF_SIZE    64

static void DbgPortHndlr(void);

static uint8_t  m_dbgInputBuffer[CMD_BUF_SIZE];
static uint16_t m_dbgInBufidx = 0;
static uint8_t  m_dbgResponseBuffer[CMD_BUF_SIZE];


//----------------------------------------------------------------------------
//  Function: serialDebugPortTask
//
//  Description:
//      This task monitors the debug port
//
//  Params:
//      pvParameters - required by FreeRTOS but not used
//
//  Returns:
//	    void (hopefully never returns, that would be bad)
//----------------------------------------------------------------------------
void serialDebugPortTask(void * pvParameters)
{

     // there were no parameters passed, but this is required by FreeRTOS (function signature)
     // void it out so the compiler will not whine (or stop, since all warnings are set as errors)
    (void)pvParameters;
    
    // bring up the debug port
    //USART1_Init();      // port to the BKII

    // we loop forever
    for(;;)
    {
        // let the rest of the world run for a bit.
        vTaskDelay(TASK_DELAY);


         // check for any input data
        DbgPortHndlr();

    }
    
}


//-------------------------------------------------------------------
// Function: DbgPortHndlr
//
// Description: 
//      Handler for "debug" control via the MCU Debug port on the board
//
//      the input from the UART is just the ascii commands, 
//      i.e. "setmac 00:00:00:00:00:00<CR>", etc  no special framing
//
// Returns: void
//
//-------------------------------------------------------------------
static void DbgPortHndlr(void)
{
    uint8_t dataIn;

    uint8_t int8_tAvail = 1;

    while (int8_tAvail)
    {
        dataIn = 0;
        int8_tAvail = USART1_GetChar(&dataIn);
        if (int8_tAvail)
        {
        	// echo typed characters
        	USART1_TxData(&dataIn, int8_tAvail);
            m_dbgInputBuffer[m_dbgInBufidx++] = dataIn;
            if (m_dbgInBufidx >= 30)
            {
                // this is a hack, but it keeps us from overflowing the buffer
                // currently the longest command is 25 bytes or so, none are 30
                m_dbgInBufidx = 0;
            }
        }
    }
    
    // commands are delimited by a <CR>
    if (0x0d == m_dbgInputBuffer[m_dbgInBufidx-1])
    {
        int len;
        // echo LF
        uint8_t dataIn = 0x0a;
    	USART1_TxData(&dataIn, 1);
        // currently we only handle two command
        // "getmac<CR>" and "setmac 00:00:00:00:00:00<CR>"

        if (0 == strncmp((char *)m_dbgInputBuffer,"getmac",6))
        {
            uint8_t macaddr[6];
            getMAC(macaddr);

            len = sprintf((char *)m_dbgResponseBuffer,"macaddr = %02x:%02x:%02x:%02x:%02x:%02x\n\r",
                          macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
        }
        else if (0 == strncmp((char *)m_dbgInputBuffer,"setmac",6)) 
        {
            int data;
            uint8_t i;
            uint8_t itsBad = 0;
            char  *nextDigit;
            uint8_t macaddr[6];

            nextDigit = strtok((char *)&m_dbgInputBuffer[6],":\n\r");
            for (i=0; i < 6; i++)
            {
                if (NULL != nextDigit)
                {
                    len = sscanf(nextDigit,"%02x", &data);
                    if (1 != len)
                    {
                        itsBad = 1;
                        break;
                    }
                    else
                    {
                        macaddr[i] = data;
                    }
                    nextDigit = strtok(NULL,":\n\r");                
                }
                else
                {
                    itsBad = 1;
                    break;
                }
            }
            
            if (0 == itsBad)
            {
                writeMAC(macaddr);
                len = sprintf((char *)m_dbgResponseBuffer,"mac written\n\r");
            }
            else
            {
                len = sprintf((char *)m_dbgResponseBuffer,"BAD try \"setmac xx:xx:xx:xx:xx:xx\"\n\r");
            }
        }
        else
        {
            len = sprintf((char *)m_dbgResponseBuffer,"BAD try setmac or getmac\n\r");
        }
        USART1_TxData(m_dbgResponseBuffer, len);
        // zero everything out for the next command
        m_dbgInBufidx = 0;
        memset(m_dbgInputBuffer,0,CMD_BUF_SIZE);
    }
}// end DbgPortHndlr


