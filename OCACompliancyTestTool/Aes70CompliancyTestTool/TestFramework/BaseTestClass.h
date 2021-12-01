/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The Base class for tests.
 */
#ifndef BASE_TEST_CLASS_H
#define BASE_TEST_CLASS_H

// ---- Include system wide include files ----
#include <rapidxml/rapidxml.hpp>
using namespace rapidxml;
#include <PlatformDataTypes.h>
#include <OCF/OcaLiteCommandHandlerController.h>

// ---- Include local include files ----
#include "TestContext.h"
#include "TestLogger.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/** Default resolve timeout */
#define RESOLVE_TIMEOUT 15
/** Default keepalive timeout */
#define KEEP_ALIVE_TIMEOUT 3
/** OCA TCP service name */
#define OCA_TCP_SERVICE "_oca._tcp"
/** OCA secure TCP service name */
#define OCA_SECURE_TCP_SERVICE "_ocasec._tcp"
/** OCA UDP service name */
#define OCA_UDP_SERVICE "_oca._udp"
/** Technical version number for AES70-2015 */
#define AES70_2015_TECHNICAL_VERSION 0x0103
/** Technical version number for AES70-2018 */
#define AES70_2018_TECHNICAL_VERSION 0x0104

/** XML file defines */
#define XML_CONFIGURATION_FILE "Xml/TestConfiguration.xml"
#define XML_CONFIGURATION_FILE_ROOT_NODE "TestConfiguration"
#define XML_CONFIGURATION_FILE_DEVICERESETTEST "DeviceResetTest"

#define XML_OCC_MEMBERS_FILE "Xml/ReferenceOCCMembers.xml"
#define XML_OCC_MEMBERS_FILE_ROOT_NODE "OcaDevice"

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base test class. It defines some basic functionality
 *
 * - loading and storage of the xml configuration file defined the block members
 * - Test methods 
 * - Session management
 */
class BaseTestClass : public ::OcaLiteCommandHandlerController::IConnectionLostDelegate, public TestLogger
{
public:

    /**
     * Information about a single method
     */
    class MethodInfo
    {
    public:
        /**
         * Default constructor
         */
        MethodInfo();

        /**
         * Constructor
         *
         * @param[in] name          The name of the method
         * @param[in] classId       The classId on which this method is available
         * @param[in] methodIndex   The index of this method.
         * @param[in] mandatory     "true" iff this method is mandatory.
         * @param[in] bDeprecated   "true" iff deprecated
         */
        MethodInfo(const std::string& name, const std::string& classId, const std::string& methodIndex, 
                    const std::string& mandatory, bool bDeprecated);

        /**
         * Get the name of the method
         *
         * @return the name of the method.
         */
        const std::string& GetName() const;

        /** 
         * Get the class Id
         *
         * @return The id of the class.
         */
        const std::string& GetClassId() const;
        
        /**
         * Get the class level (derived from the classId).
         *
         * @return The class level.
         */
        int GetClassLevel() const;

        /**
         * Get the index of the method
         *
         * @return The method index.
         */
        int GetMethodIndex() const;

        /**
         * Get whether this method is mandatory
         *
         * @return True iff this method is mandatory
         */
        bool GetIsMandatory() const;

        /**
         * Get whether this method is deprecated.
         *
         * @return True iff this method is deprecated
         */
        bool GetIsDeprecated() const;

    private:
    protected:
        /** The name of the method */
        std::string m_name;
        /** The class ID of the method */
        std::string m_classId;
        /** The class level */
        int m_classLevel;
        /** The method index */
        int m_methodIndex;
        /** True iff mandatory */
        bool m_bIsMandatory;
        /** True iff deprecated */
        bool m_bDeprecated;
    };

    /**
     * Information about a single event
     */
    class EventInfo
    {
    public:
        /**
         * Default constructor
         */
        EventInfo();

        /**
         * Constructor
         *
         * @param[in] name          The name of the event
         * @param[in] classId       The classId on which this event is available
         * @param[in] eventIndex    The index of this event.
         * @param[in] mandatory     "true" iff this event is mandatory.
         */
        EventInfo(const std::string& name, const std::string& classId, 
                   const std::string& eventIndex, const std::string& mandatory);

        /**
         * Get the name of the event
         *
         * @return the name of the event.
         */
        const std::string& GetName() const;

        /** 
         * Get the class Id
         *
         * @return The id of the class.
         */
        const std::string& GetClassId() const;
        
        /**
         * Get the class level (derived from the classId).
         *
         * @return The class level.
         */
        int GetClassLevel() const;

        /**
         * Get the index of the event
         *
         * @return The method index.
         */
        int GetEventIndex() const;

        /**
         * Get whether this event is mandatory
         *
         * @return True iff this event is mandatory
         */
        bool GetIsMandatory() const;

    private:
    protected:
        /** The name of the method */
        std::string m_name;
        /** The class ID of the method */
        std::string m_classId;
        /** The class level */
        int m_classLevel;
        /** The event index */
        int m_eventIndex;
        /** True iff mandatory */
        bool m_bIsMandatory;
    };

    /**
     * Information about OcaRoot
     */
    class RootMemberInfo
    {
    public:

        /**
         * Constructor
         */
        RootMemberInfo();

        /**
         * Constructor
         *
         * @param[in] name          The name of the object
         * @param[in] classId       The class ID.
         * @param[in] version       The class version.
         * @param[in] isMandatory   True iff the object is mandatory.
         * @param[in] methodInfos   Vector with the methods of this class.
         * @param[in] eventInfos    Vector with events of this class.
         * @param[in] bDeprecated   "true" iff deprecated
         * @param[in] objectNumber  The object number of this class, only when the object has a fixed objectnumber.
         */
        RootMemberInfo(const std::string& name, const std::string& classId, const std::string& version, 
                       const std::string& isMandatory, const std::vector<MethodInfo>& methodInfos, 
                       const std::vector<EventInfo>& eventInfos, bool bDeprecated, const std::string& objectNumber = "");

        /**
         * Get the name of the object
         *
         * @return The name of the object
         */
        const std::string& GetName() const;

        /** 
         * Get the class ID
         * 
         * @return The class ID
         */
        const std::string& GetClassId() const;

        /**
         * Get the expected version of the class
         * 
         * @return The expected version of the class.
         */
        int GetVersion() const;

        /**
         * Get whether this class is mandatory.
         *
         * @return True iff the class is mandatory.
         */
        bool GetIsMandatory() const;

        /** 
         * Get all methods of the class
         *
         * @return The methods of this class.
         *
         */
        const std::vector<MethodInfo>& GetMethodInfos() const;

        /**
         * Get all events of the class
         *
         * @Return The events of the class.
         */
        const std::vector<EventInfo>& GetEventInfos() const;

        /**
         * Get whether the current class is deprecated
         *
         * @return "true" iff the current class is deprecated.
         */
        bool GetIsDeprecated() const;
    private:
    protected:
        /** The name */
        std::string m_name;
        /** The class ID */
        std::string m_classId;
        /** The expected version */
        int m_version;
        /** True iff mandatory */
        bool m_isMandatory;
        /** The method information */
        std::vector<MethodInfo> m_methodInfos;
        /** The event information */
        std::vector<EventInfo> m_eventInfos;
        /** True iff deprecated */
        bool m_bDeprecated;

    };

    /**
     * Information about OcaManager
     */
    class ManagerInfo : public RootMemberInfo
    {
    public:

        /**
         * Constructor
         */
        ManagerInfo();

        /**
         * Constructor
         *
         * @param[in] name          The name of the object
         * @param[in] classId       The class ID.
         * @param[in] version       The class version.
         * @param[in] isMandatory   True iff the object is mandatory.
         * @param[in] methodInfos   Vector with the methods of this class.
         * @param[in] eventInfos    Vector with events of this class.
         * @param[in] bDeprecated   "true" iff deprecated
         * @param[in] objectNumber  The object number of this class, only when the object has a fixed objectnumber.
         */
        ManagerInfo(const std::string& name, const std::string& classId,
                    const std::string& version, const std::string& isMandatory, 
                    const std::vector<MethodInfo>& methodInfos, 
                    const std::vector<EventInfo>& eventInfos,
                    bool bDeprecated,
                    const std::string& objectNumber = "");

        /**
         * Get the object number
         *
         * @return The object number of this object
         */
        int GetObjectNumber() const;

    private:
    protected:
        /** The expected object number of this class */
        unsigned int m_objectNumber;
    };

    /**
     * Information about OcaBlock member
     */
    class BlockMemberInfo : public RootMemberInfo
    {
    public:

        /**
         * Constructor
         */
        BlockMemberInfo();
        
        /**
         * Constructor
         *
         * @param[in] name          The name of the object
         * @param[in] classId       The class ID.
         * @param[in] version       The class version.
         * @param[in] isMandatory   True iff the object is mandatory.
         * @param[in] methodInfos   Vector with the methods of this class.
         * @param[in] eventInfos    Vector with events of this class.
         * @param[in] bDeprecated   "true" iff deprecated
         * @param[in] objectNumber  The object number of this class, only when the object has a fixed objectnumber.
         */
        BlockMemberInfo(const std::string& name, const std::string& classId, const std::string& version, 
                        const std::string& isMandatory, const std::vector<MethodInfo>& methodInfos, 
                        const std::vector<EventInfo>& eventInfos, bool bDeprecated, const std::string& objectNumber);

        /**
         * Get the object number
         *
         * @return The object number of this object
         */
        unsigned int GetObjectNumber() const;

        /**
         * Get the lower limit object number of block members.
         *
         * @return the lower limit
         */
        static unsigned int GetObjectNumberLowerLimit()
        {
            return m_objectNumberLowerLimit;
        }

        /**
         * Get the uppoer limit object number of block members.
         *
         * @return the uppoer limit
         */
        static unsigned int GetObjectNumberUpperLimit()
        {
            return m_objectNumberUpperLimit;
        }

        /**
         * Set the object number limits.
         *
         * @param[in] objectNumberLowerLimit    The lower limit of the object numbers in the free range.
         * @param[in] objectNumberUpperLimit    The upper limit of the object numbers in the free range.
         */
        static void SetObjectNumberLimits(unsigned int objectNumberLowerLimit, unsigned int objectNumberUpperLimit)
        {
            m_objectNumberLowerLimit = objectNumberLowerLimit;
            m_objectNumberUpperLimit = objectNumberUpperLimit;
        }
    protected:
    private:
        /** The expected object number of this class */
        unsigned int m_objectNumber;

        /** The lower limit of all block members */
        static unsigned int m_objectNumberLowerLimit;

        /** The uppower limit of all block members */
        static unsigned int m_objectNumberUpperLimit;
    };

    /**
     * Parse and load the refenrence OCC Members
     *
     * @return True iff the file is loaded/parsed successfully.
     */
    bool LoadReferenceOCCMembers();

    /**
     * Prepare test
     *
     * @return True if preparing the test succeeds.
     */
    virtual bool PrepareTest();

    /**
     * Execute test
     *
     * @return True if executing the test succeeded, this is independent of the test result!
     */
    virtual bool ExecuteTest() = 0;

    /**
     * Cleanup the test
     */
    virtual void CleanupTest();

    /** 
     * Return whether the test result is success.
     */
    bool IsTestSuccesfull() const;

    /**
     * Get the text context.
     */
    TestContext& GetContext()
    {
        return TestContext::GetInstance();
    }

    /**
     * Convert a known OCA version string to a representation of the version as int.
     *
     * @param[in] versionString The version.
     *
     * @return The version as int.
     */
    static int GetVersionAsInt(const std::string& versionString);

    /**
     * Convert a known OCA version int to a representation of the version as string.
     *
     * @param[in] version The version.
     *
     * @return The version as string.
     */
    static std::string GetVersionAsString(int version);

protected:
    /**
     * Constructor
     *
     * @param[in] testName  The name of the test
     */
    BaseTestClass(const std::string& testName);

    /**
     * Set the test result
     *
     * @param[in] bTestResult   True iff the test succeeded, false if not (default).
     */
    void SetTestResult(bool bTestResult);

    /**
     * Get a session
     *
     * @param[in] deviceName        The device name
     * @param[in] serviceName       The service name
     * @param[in] keepaliveTimeout  The keepalive timeout (in seconds) to use
     * @param[in] retries           The number of retries when connecting
     * @param[in] timeout           Resolve timeout
     *
     * @return The session ID
     */
    OcaSessionID GetSession(const std::string& deviceName, const std::string& serviceName, UINT32 keepaliveTimeout = KEEP_ALIVE_TIMEOUT, int retries = 3, int timeout = RESOLVE_TIMEOUT);

    /**
     * Virtual (empty) connection lost handler, can be overriden by a test class if required.
     *
     * @param[in] sessionID The sessionID of the lost connection
     */
    virtual void OnConnectionLost(::OcaSessionID sessionID) { }

    /**
     * Get the network object number to use for the passed service name.
     *
     * @param[in] serviceName   The service name to look for.
     *
     * @return The object number, by default the TCP object number.
     */
    ::OcaONo GetNetworkObjectNumberFromServiceName(const std::string& serviceName);

    /**
     * Parse an array of methods.
     *
     * @param[in] object    Pointer to the xml object.
     * @param[in] classId   The classId of the class holding the methods.
     *
     * @return vector with the method information
     */
    typedef std::vector<BaseTestClass::MethodInfo> MethodInfos;
    MethodInfos ParseMethods(xml_node<>* object, const std::string& classId);

    /**
     * Parse an array of events.
     *
     * @param[in] object    Pointer to the xml object.
     * @param[in] classId   The classId of the class holding the events.
     *
     * @return vector with the method information
     */
    typedef std::vector<BaseTestClass::EventInfo> EventInfos;
    EventInfos ParseEvents(xml_node<>* object, const std::string& classId);

    /**
     * Parse an object.
     *
     * @param[in] object            Pointer to the xml object.
     * @param[in] result            Template to store the result.
     * @param[in] defaultVersion    The default version.
     *
     * @return True iff parsing succeeded.
     */
    template <class T> bool ParseObject(xml_node<> * object, T& result, const std::string& defaultVersion = "1");

    /** Information about the root members */
    typedef vector< ::BaseTestClass::RootMemberInfo> RootMemberInfos;
    RootMemberInfos  m_rootMemberInfos;
    /** Information about the block members */
    typedef vector< ::BaseTestClass::BlockMemberInfo> BlockMemberInfos;
    BlockMemberInfos m_blockMemberInfos;
    /** Informaiton about the managers */
    typedef vector< ::BaseTestClass::ManagerInfo> ManagerInfos;
    ManagerInfos m_managerInfos;

private:

    /**
     * Session lost handler
     */
    class SessionLostHandler : public ::OcaLiteCommandHandlerController::IConnectionLostDelegate
    {
    public:
        /** Constructor */
        SessionLostHandler()
            : OcaLiteCommandHandlerController::IConnectionLostDelegate(),
            m_pCallback(NULL)
        {
            ::OcaLiteCommandHandlerController::GetInstance().RegisterConnectionLostEventHandler(this);
        }
        /** Destructor */
        ~SessionLostHandler()
        {
            ::OcaLiteCommandHandlerController::GetInstance().UnregisterConnectionLostEventHandler(this);
        }

        /**
         * Register a callback to forward connection lost events
         *
         * @param[in] pCallback The callback to invoke
         */
        void RegisterForwarder(::OcaLiteCommandHandlerController::IConnectionLostDelegate* pCallback)
        {
            assert(m_pCallback == NULL);
            m_pCallback = pCallback;
        }

        /**
         * Unregister a previously registered connection lost handler
         *
         * @param[in] pCallback The currently registered handler
         */
        void UnregisterForwarder(const ::OcaLiteCommandHandlerController::IConnectionLostDelegate* pCallback)
        {
            assert((m_pCallback != NULL) && (m_pCallback == pCallback));
            m_pCallback = NULL;
        }

    protected:

        // Implement interface ::OcaLiteCommandHandlerController::IConnectionLostDelegate
        void OnConnectionLost(::OcaSessionID sessionID)
        {
            SessionMap::iterator iter(s_sessionTracker.begin());
            while (iter != s_sessionTracker.end())
            {
                if (iter->second == sessionID)
                {
                    OCA_LOG_INFO_PARAMS("[TRACK] Deleting session for %d", sessionID);
                    s_sessionTracker.erase(iter->first);
                    break;
                }
                ++iter;
            }

            if (NULL != m_pCallback)
            {
                m_pCallback->OnConnectionLost(sessionID);
            }
        }

    private:
        /** Forwarder callback */
        ::OcaLiteCommandHandlerController::IConnectionLostDelegate* m_pCallback;
    };

    /** Session tracker (Shared between all tests) */
    typedef std::map<std::string, ::OcaSessionID> SessionMap;
    static SessionMap                       s_sessionTracker;
    /** Session lost tracker */
    static SessionLostHandler               s_sessionLostHandler;

    /** Is the test successfull */
    bool                                    m_bIsTestSuccessFull;

    /** private copy constructor, no copying of object allowed */
    BaseTestClass(const ::BaseTestClass&);
    /** private assignment operator, no assignment of object allowed */
    BaseTestClass& operator=(const ::BaseTestClass&);
};

#endif //BASE_TEST_CLASS_H
