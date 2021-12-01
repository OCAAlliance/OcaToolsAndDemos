/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

//=========================================================================
//  File Name: mac_addr.h
//
//  Description:
//      header file for Mac_addr functions
//  
//  Modification History:
//      $Id: mac_addr.h,v 1.1 2014/06/24 18:36:05 dvaughn Exp $
//      $Log: mac_addr.h,v $
//      Revision 1.1  2014/06/24 18:36:05  dvaughn
//      Initial commit
//
//      Revision 1.1  2014/06/20 12:35:19  dvaughn
//      Initial Commit
//
//
//=========================================================================
#ifndef MAC_ADDR_H
#define MAC_ADDR_H

#define MAC_ADDR_SIZE  (6)

#ifdef __cplusplus
extern "C" {
#endif

// function to read the MAC address, passed pointer needs to point to
// an arrray of MAC_ADDR_SIZE
void getMAC(uint8_t *mac_addr_data);

// function to write a MAC passed in via an array to OTP
// the passed in array is assumed to be of MAC_ADDR_SIZE
int writeMAC(const uint8_t *mac_addr_data);

#ifdef __cplusplus
}
#endif

#endif // MAC_ADDR_H