/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteConnectParameters
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteConnectParameters.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteConnectParameters::OcaLiteConnectParameters(UINT32 keepAliveTimeout, bool bKeepAliveTimeoutInMs) :
    m_keepAliveTimeout(keepAliveTimeout),
    m_bKeepAliveTimeoutInMs(bKeepAliveTimeoutInMs)
{
}

UINT32 OcaLiteConnectParameters::GetKeepAliveTimeout() const
{
    return m_keepAliveTimeout;
}

bool OcaLiteConnectParameters::GetbKeepAliveTimeoutInMs() const
{
    return m_bKeepAliveTimeoutInMs;
}
