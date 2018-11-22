/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The device reset test class.
 */
#ifndef DEVICE_RESET_TEST_H
#define DEVICE_RESET_TEST_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "../../TestFramework/BaseTestClass.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Implement the following tests:
 * 
 * - OCTT must try to activate the OCP.1 Device Reset mechanism by calling
 *   the SetResetKey method of the device manager of the device supplying a 
 *   reset key k1 and an Address containing only a UDP port u; if the device 
 *   responds to this command with a status OK the OCTT must perform the 
 *   tests specified; if the device responds to 
 *   the command with a status NotImplemented the OCTT must skip these tesonnects
 *   
 * - OCTT must send a unicast OCP.1 DeviceReset message containing 
 *   reset key k1 to the device on UDP port u and verify that the connection 
 *   to the device is lost (by checking that the KeepAlive mechanism reports 
 *   connection loss and by checking that, after reconnecting to the device, 
 *   the GetResetCause method of the OcaDeviceManager returns reset cause 
 *   ExternalRequest if the return value of this method is status OK) 
 *   
 * - OCTT must reconnect to the device, activate the KeepAlive mechanism with a 
 *   HeartbeatTime of 3 seconds and activate the OCP.1 Device Reset mechanism 
 *   by calling the SetResetKey method supplying a reset key k1 and an Address 
 *   containing UDP port u and multicast IP address m; OCTT must then send a 
 *   multicast OCP.1 DeviceReset message containing reset key k1 to the multicast 
 *   IP address m and UDP port u and verify that the connection to the device is 
 *   lost (by checking that the KeepAlive mechanism reports connection loss and 
 *   by checking that, after reconnecting to the device, the GetResetCause method 
 *   of the OcaDeviceManager returns reset cause ExternalRequest if the return 
 *   value of this method is status OK) 
 *   
 * - OCTT must reconnect to the device, activate the KeepAlive mechanism with a 
 *   HeartbeatTime of 3 seconds and activate the OCP.1 Device Reset mechanism by 
 *   calling the SetResetKey method supplying a reset key k1 and an Address
 *   containing only UDP port u; OCTT must then again send the SetResetKey command 
 *   supplying a reset key k2 and an Address containing only UDP port u2; OCTT must 
 *   send a unicast OCP.1 DeviceReset message containing reset key k1 to the device 
 *   on UDP port u and verify that the connection to the device is not lost (by 
 *   verifying the device keeps on sending KeepAlive messages); OCTT must finally 
 *   send a unicast OCP.1 DeviceReset message containing reset key k2 to the device 
 *   on UDP port u2 and verify that the connection to the device is lost (by checking 
 *   that the KeepAlive mechanism reports connection loss and by checking that, 
 *   after reconnecting to the device, the GetResetCause method of the 
 *   OcaDeviceManager returns reset cause ExternalRequest if the return value of 
 *   this method is status OK1) 
 *   
 * - OCTT must reconnect to the device, activate the KeepAlive mechanism with a 
 *   HeartbeatTime of 3 seconds and activate the OCP.1 Device Reset mechanism by 
 *   calling the SetResetKey method supplying a reset key k1 and an Address 
 *   containing UDP port u1 and multipcast IP address m1; OCTT must then send a 
 *   unicast OCP.1 DeviceReset message containing reset key k2 to the device on 
 *   UDP port u1 and verify that the connection to the device is not lost (by 
 *   verifying the device keeps on sending KeepAlive messages); OCTT must then 
 *   send a unicast OCP.1 DeviceReset message containing reset key k1 to the 
 *   device on UDP port u2 and verify that the connection to the device is not 
 *   lost ((by verifying the device keeps on sending KeepAlive messages); OCTT 
 *   must finally send a multicast OCP.1 DeviceReset message containing reset key 
 *   k1 to a multicast IP address m2 and UDP port u1 and verify that the connection 
 *   to the device is not lost (by verifying the device keeps on sending KeepAlive 
 *   messages)
 *   
 */
class DeviceResetTest : public BaseTestClass
{
public:
    /** Constructor */
    DeviceResetTest();

    /** Implement interface **/
    virtual bool PrepareTest();

    virtual bool ExecuteTest();

protected:

    virtual void OnConnectionLost(::OcaSessionID sessionID);

    /**
     * Perfrom the reset key test.
     *
     * @param[in] serviceName The service name (e.g. _oca._tcp or _oca._udp).
     *
     * @ return The result of the test.
     */
    bool PerformResetKeyTest(const std::string& serviceName);

    /**
     * Wait for session lost.
     *
     * @param[in] timeout   The timeout in seconds.
     *
     * @return True if the m_sessionId session was lost withing the timeout, false if not.
     */
    bool WaitForSessionLost(int timeout);

    /**
     * Perform a parameterized reset test.
     *
     * @param[in] serviceName       The serviceName
     * @param[in] unicast           True for an unicast reset message, false for multicast.
     * @param[in] tryWithWrongKey   True if a wrong key should be tested, false if not.
     *
     * @return True if the test succeeded, false if not.
     */
    bool SingleResetTest(const std::string& serviceName, bool unicast, bool tryWithWrongKey);

private:
    /** The current session Id */
    ::OcaSessionID m_sessionId;
};

#endif //DEVICE_RESET_TEST_H