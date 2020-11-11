/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The Ocp1LiteConnectParameters class.
 */

#ifndef OCP1LITECONNECTPARAMETERS_H
#define OCP1LITECONNECTPARAMETERS_H

 // ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteConnectParameters.h>

 // ---- Include local include files ----

 // ---- Referenced classes and types ----

 // ---- Helper types and constants ----

 // ---- Helper functions ----

 // ---- Class Definition ----

/**
 * Helper class to set connection parameters for Ocp.1 based connections (TCP and UDP).
 */
class Ocp1LiteConnectParameters : public ::OcaLiteConnectParameters
{
public:
    /**
     * Constructor
     *
     * @param[in] hostOrIp              The host or IP address.
     * @param[in] port                  The port number.
     * @param[in] keepAliveTimeout      The keepalive timeout.
     * @param[in] bKeepAliveTimeoutInMs True iff the keepalive timeout is in milli-seconds, false for seconds.
     */
    Ocp1LiteConnectParameters(const std::string& hostOrIp, UINT16 port, UINT32 keepAliveTimeout = 3, bool bKeepAliveTimeoutInMs = false);

    /**
     * Get the host or ip address to use in the connection.
     *
     * @return The host or ip to use.
     */
    const std::string& GetHostOrIp() const;

    /**
     * Get the port.
     */
    UINT16 GetPort() const;

protected:
private:
    /** The host or IP to connect to */
    std::string m_hostOrIp;
    /** The port to connect to */
    UINT16        m_port;

};

#endif //OCP1LITECONNECTPARAMETERS_H