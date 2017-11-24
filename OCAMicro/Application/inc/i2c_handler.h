/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

//=========================================================================
//  File Name: i2c1_handler.h
//
//  Description:
//      Definitions for the I2C_1 driver
//  
//  Modification History:
//      $Id: i2c_handler.h,v 1.1 2014/06/24 18:36:05 dvaughn Exp $
//      $Log: i2c_handler.h,v $
//      Revision 1.1  2014/06/24 18:36:05  dvaughn
//      Initial commit
//
//      Revision 1.1  2014/03/21 14:37:48  dvaughn
//      Initial commit
//

//
//
//==========================================================================
#ifndef I2C_HANDLER
#define I2C_HANDLER
#include <stdint.h>
#include "stm32f2xx_i2c.h"
#include "misc.h"

#ifdef __cplusplus
extern "C" {
#endif


#define I2C1_FLAG_TIMEOUT       ((uint32_t)0x1000)
#define I2C1_LONG_TIMEOUT       ((uint32_t)(300 * I2C1_FLAG_TIMEOUT))
#define I2C1_SPEED              100000


void I2C_1_Init(void);
uint8_t I2C_1_WriteData(uint8_t dev, uint16_t reg, const uint8_t* data, uint16_t len, uint8_t eeprom);
uint8_t I2C_1_ReadData (uint8_t dev, uint16_t reg, uint8_t* data, uint16_t len, uint8_t eeprom);

#ifdef __cplusplus
}
#endif



#endif
