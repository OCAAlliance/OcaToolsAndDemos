/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Clock driver
 *
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "ClockGenerator.h"

// ---- Helper types and constants ----

/** The singleton reference to the driver. */
ClockGenerator* ClockGenerator::m_pSingleton(NULL);

// ---- Class Implementation ----
ClockGenerator::ClockGenerator():
      m_bInitialized(false),
      m_bConfigured(false)
{
}

ClockGenerator::~ClockGenerator()
{
}

ClockGenerator& ClockGenerator::GetInstance()
{
    if (NULL == ClockGenerator::m_pSingleton)
    {
        ClockGenerator::m_pSingleton = new ::ClockGenerator;
    }

    return *ClockGenerator::m_pSingleton;
}

void ClockGenerator::FreeInstance()
{
    if (ClockGenerator::m_pSingleton != NULL)
    {
        m_pSingleton->Teardown();

        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

void ClockGenerator::Initialize()
{
    if (!m_bInitialized)
    {
        m_bInitialized = true;
    }
}

void ClockGenerator::Teardown()
{
    if (m_bConfigured)
    {
        m_bConfigured = false;
    }
}

bool ClockGenerator::Configure(const UINT8 configuration)
{
    bool success(true);

    if (m_bInitialized & !m_bConfigured)
    {
		m_bConfigured = true;
    }

    return success;
}
