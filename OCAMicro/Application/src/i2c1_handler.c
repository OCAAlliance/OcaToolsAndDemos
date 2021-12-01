/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 */

//=========================================================================
//  File Name: i2c1_handler.c
//
//  Description:
//      Implementation of the I2C 1 driver for the STM32F2xx
//  
//  Modification History:
//      $Id: i2c1_handler.c,v 1.1 2014/06/24 18:37:56 dvaughn Exp $
//      $Log: i2c1_handler.c,v $
//      Revision 1.1  2014/06/24 18:37:56  dvaughn
//      Initial commit
//
//      Revision 1.2  2014/04/30 11:57:51  dvaughn
//      removed unused NVIC_Init structure
//
//      Revision 1.1  2014/03/21 14:38:29  dvaughn
//      Initial commit
//
//
//==========================================================================
#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "i2c_handler.h"

//----------------------------------------------------------------------------
//  Local definitions
//----------------------------------------------------------------------------
static xSemaphoreHandle I2C1Semaphore = NULL; //Protection semaphore

//----------------------------------------------------------------------------
// Global objects
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Externs
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Local objects
//-----------------------------------------------------------------------------
static __IO uint32_t  i2cTimeout = I2C1_LONG_TIMEOUT;
static uint8_t i2c1_TIMEOUT_UserCallback(void);


//-----------------------------------------------------------------------------
// Function: I2C_1_Init
//
// Description:
//    Initialization routine for I2C peripheral
//
// Parameters:
//      void
//
// Returns:
//      void
//
//-----------------------------------------------------------------------------
void I2C_1_Init(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //Create the protection semaphore
    vSemaphoreCreateBinary( I2C1Semaphore );

    /* RCC Configuration */
    /*I2C Peripheral clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /*SDA GPIO clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /*SCL GPIO clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Reset I2Cx IP */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* Release reset signal of I2Cx IP */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

    /* GPIO Configuration */
    /*Configure I2C SCL pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*Configure I2C SDA pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect PXx to I2C_SCL */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

    /* Connect PXx to I2C_SDA */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);



    I2C_DeInit(I2C1);
    /* I2C peripheral configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C1_SPEED;

    /* Initialize the I2C peripheral */
    I2C_Init(I2C1, &I2C_InitStructure);

    /* Enable the I2C peripheral */
    I2C_Cmd(I2C1, ENABLE);  
}

//-----------------------------------------------------------------------------
// Function: I2C_1_WriteData
//
// Description:
//    Routine to send data through the the I2C
//
// Parameters:
//      uint8_t       device address
//      uint8_t       register address to write to
//      uint8_t*      pointer to data [1 or 2 bytes]
//      uint16_t      length of data
//
// Returns:
//      uint8_t       0 on success, 1 on fail
//
//-----------------------------------------------------------------------------
uint8_t I2C_1_WriteData(uint8_t dev, uint16_t reg, const uint8_t* data, uint16_t len, uint8_t eeprom)
{
    uint8_t retVal = 0;
    uint16_t i = 0;

    (void) xSemaphoreTake( I2C1Semaphore, portMAX_DELAY );

    /*!< While the bus is busy */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    /* Start the config sequence */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(I2C1, dev, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }


    if (eeprom)
    {       
        /*!< Send the EEPROM's internal address to write to : MSB of the address first */
        I2C_SendData(I2C1, (uint8_t)((reg & 0xFF00) >> 8));

        /* Test on EV8 and clear it */
        i2cTimeout = I2C1_LONG_TIMEOUT;
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        {
            if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
        }
        
        /*!< Send the EEPROM's internal address to write to : LSB of the address */
        I2C_SendData(I2C1, (uint8_t)(reg & 0x00FF));
    }
    else
    {
        /* Transmit the first address for write operation */
        I2C_SendData(I2C1, (uint8_t)(reg & 0x00FF));
    }

    /* Test on EV8 and clear it */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    for(i=0; i<len; i++)
    {
        /* Prepare the register value to be sent */
        I2C_SendData(I2C1, data[i]);

        /*!< Wait till all data have been physically transferred on the bus */
        i2cTimeout = I2C1_LONG_TIMEOUT;
        while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF))
        {
            if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
        }
    }

    /* End the configuration sequence */
    I2C_GenerateSTOP(I2C1, ENABLE);  

    (void) xSemaphoreGive( I2C1Semaphore );
    return retVal;
}

//-----------------------------------------------------------------------------
// Function: I2C_1_ReadData
//
// Description:
//    Routine to send data through the the I2C
//
// Parameters:
//      uint8_t       device address
//      uint8_t       register address to read from
//      uint8_t*      pointer to hold data [1 or 2 bytes]
//      uint16_t      length of data
//
// Returns:
//      uint8_t       0 on success, 1 on fail
//
//-----------------------------------------------------------------------------
uint8_t I2C_1_ReadData(uint8_t dev, uint16_t reg, uint8_t* data, uint16_t len, uint8_t eeprom)
{
    uint8_t retVal = 0;
    uint16_t i = 0;

    (void) xSemaphoreTake( I2C1Semaphore, portMAX_DELAY );

    /*!< While the bus is busy */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    /* Start the config sequence */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(I2C1, dev, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    if (eeprom)
    {       
        /*!< Send the EEPROM's internal address to write to : MSB of the address first */
        I2C_SendData(I2C1, (uint8_t)((reg & 0xFF00) >> 8));

        /* Test on EV8 and clear it */
        i2cTimeout = I2C1_LONG_TIMEOUT;
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        {
            if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
        }
        
        /*!< Send the EEPROM's internal address to write to : LSB of the address */
        I2C_SendData(I2C1, (uint8_t)(reg & 0x00FF));
    }
    else
    {
        /* Transmit the register address to be read */
        I2C_SendData(I2C1, (uint8_t)(reg & 0x00FF));
    }

    /* Test on EV8 and clear it */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    /*!< Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);

    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    } 

    if (len == 1)
    {
        I2C_AcknowledgeConfig(I2C1, DISABLE);
    }

    /*!< Send Codec address for read */
    I2C_Send7bitAddress(I2C1, dev, I2C_Direction_Receiver);  

    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }     

    for(i=0; i<len-1; i++)
    {
        /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
        (void)I2C1->SR2;

        /* Wait for the byte to be received */
        i2cTimeout = I2C1_LONG_TIMEOUT;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
        }

        if (i == len-2)
        {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
        }
        data[i] = I2C_ReceiveData(I2C1);
    }


    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)I2C1->SR2;

    /* Wait for the byte to be received */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    /*!< Send STOP Condition */
    I2C_GenerateSTOP(I2C1, ENABLE);

    /*!< Read the byte received from the Codec */
    data[i] = I2C_ReceiveData(I2C1);

    /*!< While the bus is busy */
    i2cTimeout = I2C1_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        if((i2cTimeout--) == 0) return i2c1_TIMEOUT_UserCallback();
    }

    /*!< Re-Enable Acknowledgment to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);  

    /* Clear AF flag for next communication */
    I2C_ClearFlag(I2C1, I2C_FLAG_AF); 

    /* Return the byte read from Codec */
    (void) xSemaphoreGive( I2C1Semaphore );
    return retVal;
}

//-----------------------------------------------------------------------------
// Function: i2c1_TIMEOUT_UserCallback
//
// Description:
//    Routine to handle I2C Timeouts.
//    Currently the routine only resets the I2C peripheral
//
// Parameters:
//      void
//
// Returns:
//      uint32_t       0 on success, 1 on fail
//
//-----------------------------------------------------------------------------
static uint8_t i2c1_TIMEOUT_UserCallback(void)
{
    /* The following code allows I2C error recovery and return to normal communication
       if the error source doesn\92t still exist (ie. hardware issue..) */
    I2C_InitTypeDef I2C_InitStructure;


    I2C_GenerateSTOP(I2C1, ENABLE);
    (void)I2C_ReceiveData(I2C1);
    I2C_SoftwareResetCmd(I2C1, ENABLE);
    I2C_SoftwareResetCmd(I2C1, DISABLE);


    I2C_DeInit(I2C1);
    /* I2C peripheral configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C1_SPEED;

    /* Initialize the I2C peripheral */
    I2C_Init(I2C1, &I2C_InitStructure);

    /* Enable the I2C peripheral */
    I2C_Cmd(I2C1, ENABLE);  
    /* At this stage the I2C error should be recovered and device can communicate
       again (except if the error source still exist).
       User can implement mechanism (ex. test on max trial number) to manage situation
       when the I2C can't recover from current error. */

    (void) xSemaphoreGive( I2C1Semaphore );
    return 1;
}



