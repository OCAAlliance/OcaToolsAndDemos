/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteConnectParameters
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "Ocp1LiteConnectParameters.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteConnectParameters::Ocp1LiteConnectParameters(const std::string& hostOrIp, UINT16 port, UINT32 keepAliveTimeout, bool bKeepAliveTimeoutInMs)
    : ::OcaLiteConnectParameters(keepAliveTimeout, bKeepAliveTimeoutInMs),
    m_hostOrIp(hostOrIp),
    m_port(port)
{
}

const std::string& Ocp1LiteConnectParameters::GetHostOrIp() const
{
    return m_hostOrIp;
}

UINT16 Ocp1LiteConnectParameters::GetPort() const
{
    return m_port;
}
