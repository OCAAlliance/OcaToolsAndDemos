/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The OcaLiteConnectParameters class.
 */

#ifndef OCALITECONNECTPARAMETERS_H
#define OCALITECONNECTPARAMETERS_H

 // ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>

 // ---- Include local include files ----

 // ---- Referenced classes and types ----

 // ---- Helper types and constants ----

 // ---- Helper functions ----

 // ---- Class Definition ----

/**
 * Base class for connect parameters.
 */
class OcaLiteConnectParameters
{
public:
    /**
     * Constructor
     *
     * @param[in] keepAliveTimeout      The keep-alive timeout
     * @param[in] keepAliveTimeoutInMs  True iff in millisecond, false iff in seconds.
     */
    OcaLiteConnectParameters(UINT32 keepAliveTimeout, bool bKeepAliveTimeoutInMs);
    
    /**
     * Get the keep-alive timeout.
     *
     * @return The keep-alive timeout.
     */
    UINT32 GetKeepAliveTimeout() const;
    
    /**
    * Get whether the keep-alive timeout is in ms or s.
    *
    * @return True iff in milli-seconds, false if in seconds.
    */
    bool GetbKeepAliveTimeoutInMs() const;

protected:
private:
    /** The keep-alive timeout */
    UINT32 m_keepAliveTimeout;
    /** True iff the keepalive timeout is in ms, false for seconds*/
    bool m_bKeepAliveTimeoutInMs;
};

#endif //OCALITECONNECTPARAMETERS_H

