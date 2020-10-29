/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Proxy of switch device of application 
 *
 */

// ---- Include system wide include files ----
#include <stdint.h>

// ---- Include local include files ----
#include "Switch.h"
#include "stm32f2x7_eth.h"

// ---- Helper types and constants ----

#define PHY_ADDR    0
#define PHY_CNTRL_1_REG 0x1E
#define LINK_IS_UP      0x0100


/** Class attributes. */
::Switch* Switch::m_pSingleton(NULL);

// ---- Class Implementation ----

Switch::Switch()
{
}

Switch::~Switch()
{
    Terminate();
}

Switch& Switch::GetInstance()
{
    if (NULL == Switch::m_pSingleton)
    {
        Switch::m_pSingleton = new ::Switch;
    }

    return *Switch::m_pSingleton;
}

void Switch::FreeInstance()
{
    if (Switch::m_pSingleton != NULL)
    {
        m_pSingleton->Terminate();

        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

void Switch::Initialize(bool bReset, bool& bSuccess)
{
    // the phy does not need any initialization, but lets see if it is up
    bSuccess = true;
}

void Switch::Terminate()
{
}

bool Switch::GetLinkUp()
{
    uint16_t linkStatus;
    bool bSuccess(false);
    
    linkStatus = ETH_ReadPHYRegister(PHY_ADDR, PHY_CNTRL_1_REG);
    if (linkStatus & LINK_IS_UP)
    {
        bSuccess = true;
    }

    return bSuccess;
}

bool Switch::GetFullDuplex()
{
    return true;
}

UINT32 Switch::GetLinkSpeed()
{
    return 100000;
}

UINT32 Switch::GetRxErrors()
{
    return 0;
}

void Switch::ResetRxErrors()
{
}
