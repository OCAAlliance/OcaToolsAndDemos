/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The TestContext class.
 */
#ifndef TEST_CONTEXT_H
#define TEST_CONTEXT_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteFrameworkDataTypes.h>
#include <DNSSDResolver.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Context for the tests to use.
 */
class TestContext
{
public:

    /**
     * Getter for the TestSuite singleton.
     *
     * @return  Reference to the TestSuite.
     */
    static ::TestContext& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    /**
     * The supported layer 3 protocols
     */
    enum SupportedProtocols
    {
        NONE = 0,
        /** OCA over TCP is supported */
        OCA_TCP = 1 << 0,
        OCA_TCP_SEC = 1 << 1,
        /** OCA over UDP is supported */
        OCA_UDP = 1 << 2
    };

    /**
     * The supported device types
     */
    enum SupportedDeviceTypes
    {
        /** General device */
        ALL = 0,
        /** Streaming device */
        OCA_STREAMING = 1 << 0,
        /** Secure device */
        OCA_SECURE = 1 << 1
    };

    /** 
     * Set the name of the device under test 
     *
     * @param[in] deviceName    The name of the device under test
     */
    void SetTestDeviceName(const std::string& deviceName);

    /**
     * Set the version of the protocol to test for
     *
     * @param[in] testVersion   The version.
     */
    void SetTestVersion(int testVersion);

    /**
     * Set the protocols which are expected to be supported by the device.
     *
     * @param[in] supportedProtocols    Bit flag with the protocols supported.
     */
    void SetSupportedProtocols(SupportedProtocols supportedProtocols);

    /**
     * Set the types which are expected to be supported by the device.
     *
     * @param[in] supportedDeviceTypes    Bit flag with the device types supported.
     */
    void SetSupportedDeviceTypes(SupportedDeviceTypes supportedDeviceTypes);

    /**
     * Set the object number of the TCP network
     *
     * @param[in] ocp1NetworkObjectNumber   The object number.
     */
    void SetOcp1TcpNetworkObjectNumber(::OcaONo ocp1NetworkObjectNumber);

#ifdef HAS_SECURE_STACK
    /**
     * Set the object number of the secure TCP network
     *
     * @param[in] ocp1NetworkObjectNumber   The object number.
     */
    void SetOcp1SecureTcpNetworkObjectNumber(::OcaONo ocp1NetworkObjectNumber);
#endif

    /**
     * Set the object number of the UDP network
     *
     * @param[in] ocp1NetworkObjectNumber   The object number.
     */
    void SetOcp1UdpNetworkObjectNumber(::OcaONo ocp1NetworkObjectNumber);

    /**
     * Get the name of the device under test.
     *
     * @return The name
     */
    const std::string& GetTestDeviceName() const
    {
        return m_deviceName;
    }
    
    /**
     * Get the specification version to test for.
     *
     * @Return the version
     */
    int GetTestVersion() const
    {
        return m_testVersion;
    }

    /**
     * Get the protocols which are expected to work
     *
     * @return the protocols
     */
    SupportedProtocols GetSupportedProtocols() const
    {
        return m_supportedProtocols;
    }

    /**
     * Get the device types which are expected to work
     *
     * @return the supported device types
     */
    SupportedDeviceTypes GetSupportedDeviceTypes() const
    {
        return m_supportedDeviceTypes;
    }

    /**
     * Get the object number of the TCP network
     *
     * @return the object number
     */
    ::OcaONo GetOcp1TcpNetworkObjectNumber() const
    {
        return m_ocp1TcpNetworkObjectNumber;
    }

#ifdef HAS_SECURE_STACK
    /**
     * Get the object number of the secure TCP network
     *
     * @return the object number
     */
    ::OcaONo GetOcp1SecureTcpNetworkObjectNumber() const
    {
        return m_ocp1SecureTcpNetworkObjectNumber;
    }
#endif

    /**
     * Get the object number of the UDP network
     *
     * @return the object number
     */
    ::OcaONo GetOcp1UdpNetworkObjectNumber() const
    {
        return m_ocp1UdpNetworkObjectNumber;
    }
    
protected:
    /** Constructor */
    TestContext();

    /** Destructor. */
    virtual ~TestContext();

private:
    /** The name of the device under test */
    std::string m_deviceName;
    /** The version of the spec to test with */
    int m_testVersion;
    /** The supported protocols */
    SupportedProtocols m_supportedProtocols;
    /** The supported device types */
    SupportedDeviceTypes m_supportedDeviceTypes;
    /** Object number of the TCP network */
    ::OcaONo m_ocp1TcpNetworkObjectNumber;
#ifdef HAS_SECURE_STACK
    /** Object number of the secure TCP network */
    ::OcaONo m_ocp1SecureTcpNetworkObjectNumber;
#endif
    /** Object number of the UDP network */
    ::OcaONo m_ocp1UdpNetworkObjectNumber;

    /** Singleton instance of the server */
    static ::TestContext* m_pSingleton;

    /** private copy constructor, no copying of object allowed */
    TestContext(const ::TestContext&);
    /** private assignment operator, no assignment of object allowed */
    ::TestContext& operator=(const ::TestContext&);
};

#endif //TEST_CONTEXT_H
