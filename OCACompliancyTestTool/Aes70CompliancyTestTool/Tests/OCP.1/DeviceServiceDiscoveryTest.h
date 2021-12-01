/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located in the LICENSE file in the root of this project
*  as an original contracting party.
*
*  Description         : The Device service registration test class.
*/
#ifndef DEVICE_SERVICE_DISCOVERY_TEST_H
#define DEVICE_SERVICE_DISCOVERY_TEST_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "../../TestFramework/BaseTestClass.h"

// ---- Referenced classes and types ----
class OcaLiteObjectIdentification;
class GeneralProxy;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Implement the following tests:
 *
 * - OCTT must verify that an OCA device registers either the insecure OCA service, 
 *   or the secure OCA service, or both
 * 
 * - OCTT must verify that the (insecure and/or secure) OCA service uses a port of 
 *   the dynamic port range (49152-65535); if both services are registered the tool 
 *   must verify that different ports are used for the services 
 * 
 * - OCTT must verify that the TXT record of the (insecure and/or secure) OCA service
 *   contains at least the entries txtvers=x and protovers=y where x is equal to or 
 *   lower than the version specified in [OCP.1], and y is equal to or lower than 
 *   the version specified in the OcaDeviceManager description in [OCC] 
 * 
 * - OCTT must verify that the TXT record of the (insecure and/or secure) OCA service
 *   is formatted according the rules of the TXT version as specified in the version
 *   of the OCP.1 specification that defined the value of x 
 * 
 * - OCTT must use the protocol version y in its following verifications; this means 
 *   it must verify all features that are specified in version y of the OCA 
 *   specifications and it means that the OCTT must be aware of all different OCA 
 *   versions and their specifications
 * 
 * - OCTT must verify that it can make a TCP connection to the OCA device on its 
 *   insecure port if the device had registered the insecure OCA service
 * 
 * - OCTT must verify the control capability discovery of the device by 
 *   querying the root block and device manager using OCP.1 commands after it has 
 *   established the TCP connection; the root block query must return status OK, 
 *   the device manager query must return status OK or NotImplemented
 * 
 * - OCTT must verify that it can make a TCP connection to the OCA device on its 
 *   secure port if the device had registered the secure OCA service 
 *   
 * - OCTT must verify the secure OCA protocol by executing the TLS handshake and 
 *   verifying the handshake succeeds if the PSK of the device has been supplied 
 *   to the OCTT; if the PSK has not been supplied the OCTT should use a dummy 
 *   PSK and verify that the TLS handshake fails
 */
class DeviceServiceDiscoveryTest : public BaseTestClass
{
public:
    /** Constructor */
    DeviceServiceDiscoveryTest();

    /** Implement interface **/
    virtual bool ExecuteTest();

protected:

    /**
     * Perform a test on the resolve result.
     *
     * @param[in] resolveResult The resolve result.
     *
     * @return True iff the test succeeded, false if not.
     */
    bool TestResolveResult(const ::ResolveResult& resolveResult);

    /**
     * Perform a basic connection test.
     *
     * @param[in] resolveResult The resolve result.
     * @param[in] serviceName   The service name.
     */
    bool TestBasicConnection(const ::ResolveResult& resolveResult, const std::string& serviceName);

    /**
     * Handle members, find the blocks and query them.
     *
     * @param[in] members   List of members to parse
     * @param[in] pProxy    Proxy to use for new requests.
     *
     */
    ::OcaLiteStatus HandleMembers(::OcaLiteList< ::OcaLiteObjectIdentification> members, ::GeneralProxy* pProxy);

private:
};

#endif //DEVICE_SERVICE_DISCOVERY_TEST_H