/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

//=============================================================================
//  File Name: persist.h
//
//  Description:
//		Header for Persistent storage routines
//
//  Modification history:
//      $Id:  $
//      $Log:  $
//
//
//
//=============================================================================
#ifndef PERSIST_H_FILE
#define PERSIST_H_FILE
#include <stdint.h>
#include "mac_addr.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void persist_Init(void);
void persist_CheckWrite(void);
void persist_ForceWrite(void);
void persist_Erase_eeProm(void);
void persist_ReadMAC(uint8_t *MACaddr);
void persist_WriteMAC(const uint8_t *MACaddr);
void persist_SetDirtyFlag(void);

#define SERVICE_NAME_LENGTH 16

typedef struct
{
	uint8_t    staticIPaddr[IPV4_ADDR_SIZE];
	uint8_t    staticNetmask[IPV4_ADDR_SIZE];
	uint8_t    staticGWaddr[IPV4_ADDR_SIZE];
	uint8_t    remoteHubName[SERVICE_NAME_LENGTH];
} persistData_t;

#ifdef __cplusplus
}
#endif

#endif

