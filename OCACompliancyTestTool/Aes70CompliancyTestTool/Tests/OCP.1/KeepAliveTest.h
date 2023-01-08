/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The keep alive test class.
 */
#ifndef KEEP_ALIVE_TEST_H
#define KEEP_ALIVE_TEST_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "../../TestFramework/BaseTestClass.h"
#include <OCF/OcaLiteCommandHandlerController.h>

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Implement the following tests:
 *
 * - OCTT must enable the device supervision mechanism on the device by 
 *   sending an OCP.1 KeepAlive message with a HeartbeatTime of 3 seconds 
 *   after establishing the TCP connection; the OCTT must verify that the 
 *   device sends a KeepAlive message every 3 seconds with a margin of +/- 0.5 
 *   seconds on an idle connection (and it should send a KeepAlive message itself 
 *   every 3 seconds with a margin of +/- 0.5 seconds)
 */
class KeepAliveTest : public BaseTestClass, public OcaLiteCommandHandlerController::IKeepAliveReceivedDelegate
{
public:
    /** Constructor */
    KeepAliveTest();
    
    /** Implement interface **/
    virtual bool PrepareTest();

    virtual bool ExecuteTest();

    virtual void CleanupTest();
     
protected:

    virtual void OnKeepAliveReceived(::OcaSessionID sessionID);

private:

    /** 
     * Verify keep alive timing.
     *
     * @param[in] protocol  The name of the protocol for logging purposes
     * 
     * return True if test succeeded, false if not.
     */
    bool VerifyKeepalives(const std::string& protocol);

    /** Active session ID */
    ::OcaSessionID m_sessionID;
    /** Received keepalives */
    int m_receivedKeepaliveCount;
    /** Received timestamps */
    std::vector<UINT32> m_keepaliveTimestamps;
};

#endif //KEEP_ALIVE_TEST_H