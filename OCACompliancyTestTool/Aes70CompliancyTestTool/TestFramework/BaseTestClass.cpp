/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Base class for a test case
 */

// ---- Include system wide include files ----
#include <iostream>
#include <fstream>
using namespace std;
#include <stdarg.h>
#include <algorithm>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCP.1/Ocp1LiteConnectParameters.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "BaseTestClass.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----
/** Array to store full log messages temporary */
static char m_fullLogMessage[1024];
/** Session tracker */
BaseTestClass::SessionMap BaseTestClass::s_sessionTracker;
/** Session lost tracker */
BaseTestClass::SessionLostHandler BaseTestClass::s_sessionLostHandler;
/** Lower limit object number */
unsigned int BaseTestClass::BlockMemberInfo::m_objectNumberLowerLimit(0);
/** Upper limit object number */
unsigned int BaseTestClass::BlockMemberInfo::m_objectNumberUpperLimit(0);

// ---- Function Implementation ----

BaseTestClass::BaseTestClass(const std::string& testName) 
    : ::OcaLiteCommandHandlerController::IConnectionLostDelegate(),
    TestLogger(testName),
    m_bIsTestSuccessFull(false)
{
}

bool BaseTestClass::LoadReferenceOCCMembers()
{
    bool bResult(true);

    xml_document<> doc;

    ifstream theFile(XML_OCC_MEMBERS_FILE);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    m_rootMemberInfos.clear();
    m_blockMemberInfos.clear();
    m_managerInfos.clear();

    std::string defaultVersion("1");

    if ((NULL != doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)) &&
        (NULL != doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)->first_node("Root")))
    {
        ::BaseTestClass::RootMemberInfo info;
        if (ParseObject< ::BaseTestClass::RootMemberInfo>(doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)->first_node("Root"), info))
        {
            m_rootMemberInfos.push_back(info);

            char buffer[10] = { 0 };
            sprintf_s(buffer, "%d", info.GetVersion());
            defaultVersion = buffer;
        }
        else
        {
            AddTestResult("ERROR: Failed to parse root node");
            bResult = false;
        }
    }
    else
    {
        AddTestResult("ERROR: Root node not present in xml file");
        bResult = false;
    }

    if ((NULL != doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)) &&
        (NULL != doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)->first_node("Managers")))
    {
        xml_node<>* managers(doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)->first_node("Managers"));
        for (xml_node<> * manager = managers->first_node();
            manager;
            manager = manager->next_sibling())
        {
            ::BaseTestClass::ManagerInfo info;
            if (ParseObject< ::BaseTestClass::ManagerInfo>(manager, info, defaultVersion))
            {
                m_managerInfos.push_back(info);
            }
            else
            {
                // We don't really care, probably the manager was not available for the current version testing for.
            }
        }
    }
    else
    {
        AddTestResult("ERROR: Managers node not present in xml file");
        bResult = false;
    }

    if ((NULL != doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)) &&
        (NULL != doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)->first_node("BlockMembers")))
    {
        xml_node<>* blockMembers(doc.first_node(XML_OCC_MEMBERS_FILE_ROOT_NODE)->first_node("BlockMembers"));
        for (xml_node<> * blockMember = blockMembers->first_node();
            blockMember;
            blockMember = blockMember->next_sibling())
        {
            ::BaseTestClass::BlockMemberInfo info;
            if (ParseObject< ::BaseTestClass::BlockMemberInfo>(blockMember, info, defaultVersion))
            {
                m_blockMemberInfos.push_back(info);
            }
            else
            {
                // We don't really care, probably the object was not available for the current version testing for.
            }
        }

        if (bResult)
        {
            if ((NULL != blockMembers->first_attribute("ObjectNumberLowerLimit")) &&
                (NULL != blockMembers->first_attribute("ObjectNumberUpperLimit")))
            {
                char *ptr;

                unsigned int objectNumberLowerLimit=strtoul(blockMembers->first_attribute("ObjectNumberLowerLimit")->value(), &ptr, 10);
                unsigned int objectNumberUpperLimit=strtoul(blockMembers->first_attribute("ObjectNumberUpperLimit")->value(), &ptr, 10);
                ::BaseTestClass::BlockMemberInfo::SetObjectNumberLimits(objectNumberLowerLimit, objectNumberUpperLimit);
            }
            else
            {
                AddTestResult("ERROR: ObjectNumber Limits attributes not present on BlockMembers node");
                bResult = false;
            }
        }
    }
    else
    {
        AddTestResult("ERROR: Blockmember node not present in xml file");
        bResult = false;
    }


    AddTestLog("RefrenceOCCMembers result %d: RootMemberInfos = %d, ManagerInfos = %d, BlockMemberInfos = %d",
        bResult, m_rootMemberInfos.size(), m_managerInfos.size(), m_blockMemberInfos.size());

    return bResult;
}

bool BaseTestClass::PrepareTest()
{
    AddTestResult("");
    AddTestResult("Starting Test : %s", GetTestName().c_str());
    AddTestResult("");

    s_sessionLostHandler.RegisterForwarder(this);

    // Poll the commandhandler
    ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(1);

    return true;
}

void BaseTestClass::CleanupTest()
{
    // Poll the commandhandler
    ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(1);

    s_sessionLostHandler.UnregisterForwarder(this);

    AddTestResult("");
}

bool BaseTestClass::IsTestSuccesfull() const
{
    return m_bIsTestSuccessFull;
}

void BaseTestClass::SetTestResult(bool bTestResult)
{
    m_bIsTestSuccessFull = bTestResult;

    AddTestResult("Test %s is now %s", GetTestName().c_str(), bTestResult ? "successfull" : "unsuccesfull");
}

::OcaSessionID BaseTestClass::GetSession(const std::string& deviceName, const std::string& serviceName, UINT32 keepaliveTimeout, int retries, int timeout)
{
    BaseTestClass::SessionMap::iterator iter(s_sessionTracker.find(deviceName));
    if (iter != s_sessionTracker.end())
    {
        AddTestLog("[TRACK] Returning existing connection for %s.%s (%d)",
            deviceName.c_str(), serviceName.c_str(), iter->second);
        return iter->second;
    }
    else
    {
        AddTestLog("[TRACK] Creating new connection for %s.%s",
            deviceName.c_str(), serviceName.c_str());

        ::OcaSessionID sessionId(OCA_INVALID_SESSIONID);
        ::DNSSDResolver resolver(deviceName, serviceName, timeout);

        for (int retry(0); ((retry < retries) && (OCA_INVALID_SESSIONID == sessionId)); retry++)
        {
            ::ResolveResult resolveResult;
            if (resolver.StartResolving(resolveResult))
            {
                sessionId = ::OcaLiteCommandHandlerController::GetInstance().Connect(::Ocp1LiteConnectParameters(resolveResult.GetHostTarget(),
                                                                                                                 resolveResult.GetPort(),
                                                                                                                 keepaliveTimeout,
                                                                                                                 false),
                                                                                     GetNetworkObjectNumberFromServiceName(serviceName));
                if (sessionId == OCA_INVALID_SESSIONID)
                {
                    resolver.Reconfirm();
                }
                else
                {
                    s_sessionTracker[deviceName] = sessionId;

                    AddTestLog("[TRACK] Created new connection for %s.%s (%d)",
                        deviceName.c_str(), serviceName.c_str(), sessionId);
                }
            }
            else
            {
                resolver.Reconfirm();
            }
        }

        return sessionId;
    }
}

::OcaONo BaseTestClass::GetNetworkObjectNumberFromServiceName(const std::string& serviceName)
{
    ::OcaONo networkObjectNumber(GetContext().GetOcp1TcpNetworkObjectNumber());
    if (serviceName.find(OCA_UDP_SERVICE) != string::npos)
    {
        networkObjectNumber = GetContext().GetOcp1UdpNetworkObjectNumber();
    }
#ifdef HAS_SECURE_STACK
    else if (serviceName.find(OCA_SECURE_TCP_SERVICE) != string::npos)
    {
        networkObjectNumber = GetContext().GetOcp1SecureTcpNetworkObjectNumber();
    }
#endif
    return networkObjectNumber;
}

int BaseTestClass::GetVersionAsInt(const std::string& versionString)
{
    if (0 == strcmp(versionString.c_str(), "AES70-2015"))
    {
        return AES70_2015_TECHNICAL_VERSION;
    }
    else if (0 == strcmp(versionString.c_str(), "AES70-2018"))
    {
        return AES70_2018_TECHNICAL_VERSION;
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Invalid version string in XML (%s)", versionString.c_str());
    }
    return -1;
}

std::string BaseTestClass::GetVersionAsString(int version)
{
    if (version == AES70_2015_TECHNICAL_VERSION)
    {
        return "AES70-2015";
    }
    else if (version == AES70_2018_TECHNICAL_VERSION)
    {
        return "AES70-2018";
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Invalid version (%0x)", version);
    }
    return "INVALID-AES70-VERSION";
}

BaseTestClass::MethodInfos BaseTestClass::ParseMethods(xml_node<>* object, const std::string& classId)
{
    BaseTestClass::MethodInfos result;

    xml_node<>* methods(object->first_node("Methods"));
    if (NULL != methods)
    {
        for (xml_node<> * method = methods->first_node();
            method;
            method = method->next_sibling())
        {
            if ((NULL != method->first_attribute("Name")) &&
                (NULL != method->first_attribute("Index")))
            {
                std::string name(method->first_attribute("Name")->value());
                std::string index(method->first_attribute("Index")->value());


                int availableSince(0);
                if (NULL != method->first_attribute("AvailableSince"))
                {
                    availableSince = GetVersionAsInt(method->first_attribute("AvailableSince")->value());
                }
                int deprecatedSince(INT_MAX);
                if (NULL != method->first_attribute("DeprecatedSince"))
                {
                    deprecatedSince = GetVersionAsInt(method->first_attribute("DeprecatedSince")->value());
                }

                int currentVersion(::TestContext::GetInstance().GetTestVersion());
                bool bAvailable(currentVersion >= availableSince);

                if (bAvailable)
                {
                    std::string isMandatory("false");
                    if (NULL != method->first_node("MandatoryMap"))
                    {
                        for (xml_node<> * mandatory = method->first_node("MandatoryMap")->first_node();
                            mandatory;
                            mandatory = mandatory->next_sibling())
                        {
                            if ((NULL != mandatory->first_attribute("OcaVersion")) &&
                                (NULL != mandatory->first_attribute("Mandatory")))
                            {
                                int ocaVersion(GetVersionAsInt(mandatory->first_attribute("OcaVersion")->value()));

#ifdef _DEBUG
                                AddTestLog("            Method is mandatory %s from OcaVersion %0X (testing for version %0x)",
                                    mandatory->first_attribute("Mandatory")->value(),
                                    ocaVersion,
                                    ::TestContext::GetInstance().GetTestVersion());
#endif

                                if (ocaVersion <= ::TestContext::GetInstance().GetTestVersion())
                                {
                                    isMandatory = mandatory->first_attribute("Mandatory")->value();
                                }
                            }
                        }
                    }

                    BaseTestClass::MethodInfo methodInfo(name, classId, index, isMandatory, currentVersion >= deprecatedSince);
                    result.push_back(methodInfo);
                }
#ifdef _DEBUG
                else
                {
                    AddTestLog("            SKIPPING Method\r\n");
                }
#endif
            }
            else
            {
                OCA_LOG_ERROR("Missing attribute (Name or Index) in method element");
            }
        }
    }

    return result;
}

BaseTestClass::EventInfos BaseTestClass::ParseEvents(xml_node<>* object, const std::string& classId)
{
    BaseTestClass::EventInfos result;

    xml_node<>* events(object->first_node("Events"));
    if (NULL != events)
    {
        for (xml_node<> * event = events->first_node();
            event;
            event = event->next_sibling())
        {
            if ((NULL != event->first_attribute("Name")) &&
                (NULL != event->first_attribute("Index")))
            {
                std::string name(event->first_attribute("Name")->value());
                std::string index(event->first_attribute("Index")->value());

                std::string isMandatory("false");
                if (NULL != event->first_node("MandatoryMap"))
                {
                    for (xml_node<> * mandatory = event->first_node("MandatoryMap")->first_node();
                        mandatory;
                        mandatory = mandatory->next_sibling())
                    {
                        if ((NULL != mandatory->first_attribute("OcaVersion")) &&
                            (NULL != mandatory->first_attribute("Mandatory")))
                        {
                            int ocaVersion(GetVersionAsInt(mandatory->first_attribute("OcaVersion")->value()));

                            if (ocaVersion <= ::TestContext::GetInstance().GetTestVersion())
                            {
                                isMandatory = mandatory->first_attribute("Mandatory")->value();
                            }

#ifdef _DEBUG
                            AddTestLog("            Event is mandatory %s from OcaVersion %0X (testing for version %0x)\r\n",
                                mandatory->first_attribute("Mandatory")->value(),
                                ocaVersion,
                                ::TestContext::GetInstance().GetTestVersion());
#endif
                        }
                    }
                }

                BaseTestClass::EventInfo eventInfo(name, classId, index, isMandatory);

                result.push_back(eventInfo);
            }
            else
            {
                OCA_LOG_ERROR("Missing attribute (Name or Index) in event element");
            }
        }
    }

    return result;
}

template <class T> bool BaseTestClass::ParseObject(xml_node<> * object, T& result, const std::string& defaultVersion)
{
    if ((NULL != object->first_attribute("Name")) &&
        (NULL != object->first_attribute("ClassID")))
    {
        std::string objectName(object->first_attribute("Name")->value());
        std::string classID(object->first_attribute("ClassID")->value());
        std::string objectNumber("");
        if (NULL != object->first_attribute("ObjNumber"))
        {
            objectNumber = object->first_attribute("ObjNumber")->value();
        }
        int availableSince(0);
        if (NULL != object->first_attribute("AvailableSince"))
        {
            availableSince = GetVersionAsInt(object->first_attribute("AvailableSince")->value());
        }
        int deprecatedSince(INT_MAX);
        if (NULL != object->first_attribute("DeprecatedSince"))
        {
            deprecatedSince = GetVersionAsInt(object->first_attribute("DeprecatedSince")->value());
        }

        int currentVersion(::TestContext::GetInstance().GetTestVersion());
        bool bAvailable(currentVersion >= availableSince);

#ifdef _DEBUG
        AddTestLog("  - Running for class %s with classId %s. Fixed object number? %s, available in current version %d",
            objectName.c_str(), classID.c_str(), objectNumber.size() > 0 ? objectNumber.c_str() : "-", bAvailable);
#endif

        if (!bAvailable)
        {
            return false;
        }

        std::string isMandatory("false");
        if (NULL != object->first_node("MandatoryMap"))
        {
            for (xml_node<> * mandatory = object->first_node("MandatoryMap")->first_node();
                mandatory;
                mandatory = mandatory->next_sibling())
            {
                std::string deviceType("");
                if (NULL != mandatory->first_attribute("DeviceType"))
                {
                    deviceType = mandatory->first_attribute("DeviceType")->value();
                    std::transform(deviceType.begin(), deviceType.end(), deviceType.begin(), ::tolower);

                }
                if ((NULL != mandatory->first_attribute("OcaVersion")) &&
                    (NULL != mandatory->first_attribute("Mandatory")))
                {
                    int ocaVersion(GetVersionAsInt(mandatory->first_attribute("OcaVersion")->value()));

                    if (ocaVersion <= ::TestContext::GetInstance().GetTestVersion())
                    {
                        if (deviceType == "")
                        {
                            // There are no limitations on the device, type. Just apply the value.
                            isMandatory = mandatory->first_attribute("Mandatory")->value();
                        }
                        else if ((deviceType == "streaming") && 
                            (::TestContext::GetInstance().GetSupportedDeviceTypes() & ::TestContext::OCA_STREAMING))
                        {
                            // Device type matches ours
                            isMandatory = mandatory->first_attribute("Mandatory")->value();
                        }
                        else if ((deviceType == "secure") &&
                            (::TestContext::GetInstance().GetSupportedDeviceTypes() & ::TestContext::OCA_SECURE))
                        {
                            // Device type matches ours
                            isMandatory = mandatory->first_attribute("Mandatory")->value();
                        }
                    }

#ifdef _DEBUG
                    AddTestLog("            Class is mandatory %s from OcaVersion %0X (testing for version %0x)",
                        mandatory->first_attribute("Mandatory")->value(),
                        ocaVersion,
                        ::TestContext::GetInstance().GetTestVersion());
#endif

                }
            }
        }

        std::string classVersion(defaultVersion);
        xml_node<>* versions(object->first_node("Versions"));
        if (NULL != versions)
        {
            for (xml_node<> * version = versions->first_node();
                version;
                version = version->next_sibling())
            {
                if ((NULL != version->first_attribute("OcaVersion")) &&
                    (NULL != version->first_attribute("ClassVersion")))
                {
                    int ocaVersion(GetVersionAsInt(version->first_attribute("OcaVersion")->value()));

                    if (ocaVersion <= ::TestContext::GetInstance().GetTestVersion())
                    {
                        classVersion = version->first_attribute("ClassVersion")->value();
                    }

#ifdef _DEBUG
                    AddTestLog("            Expecting class version %s from OcaVersion %0X (testing for version %0x)\r\n",
                        version->first_attribute("OcaVersion")->value(),
                        ocaVersion,
                        ::TestContext::GetInstance().GetTestVersion());
#endif
                }
                else
                {
                    AddTestLog("Missing attribute (OcaVersion or ClassVersion) in version element");
                }
            }
        }

        std::vector< ::BaseTestClass::MethodInfo> methods(ParseMethods(object, classID));
        std::vector< ::BaseTestClass::EventInfo> events(ParseEvents(object, classID));

        result = T(objectName, classID, classVersion, isMandatory, methods, events, currentVersion >= deprecatedSince, objectNumber);
    }
    else
    {
        AddTestLog("Missing attribute (Name or ClassID) in object element");
    }

    return true;
}
// ---- MethodInfo ----
BaseTestClass::MethodInfo::MethodInfo()
{

}


BaseTestClass::MethodInfo::MethodInfo(const std::string& name, const std::string& classId, const std::string& methodIndex, const std::string& mandatory, bool bDeprecated) :
    m_name(),
    m_classId(),
    m_classLevel(-1),
    m_methodIndex(-1),
    m_bIsMandatory(false),
    m_bDeprecated(bDeprecated)
{
    m_name = name;
    m_classId = classId;
    m_classLevel = static_cast<int>(std::count(classId.begin(), classId.end(), '.') + 1);
    m_methodIndex = atoi(methodIndex.c_str());
    m_bIsMandatory = ((mandatory == "true") || (mandatory == "True")) ? true : false;

    OCA_LOG_INFO_PARAMS("#        Method %s on method index %d (classLevel %d / ClassID %s). Mandatory %d. Deprecated %d\r\n",
        m_name.c_str(),
        m_methodIndex,
        m_classLevel,
        m_classId.c_str(),
        m_bIsMandatory,
        m_bDeprecated);
}

const std::string& BaseTestClass::MethodInfo::GetName() const
{
    return m_name;
}

const std::string& BaseTestClass::MethodInfo::GetClassId() const
{
    return m_classId;
}

int BaseTestClass::MethodInfo::GetClassLevel() const
{
    return m_classLevel;
}

int BaseTestClass::MethodInfo::GetMethodIndex() const
{
    return m_methodIndex;
}

bool BaseTestClass::MethodInfo::GetIsMandatory() const
{
    return m_bIsMandatory;
}


bool BaseTestClass::MethodInfo::GetIsDeprecated() const
{
    return m_bDeprecated;
}

// ---- EventInfo ----
BaseTestClass::EventInfo::EventInfo()
{

}

BaseTestClass::EventInfo::EventInfo(const std::string& name, const std::string& classId, const std::string& eventIndex, const std::string& mandatory) :
    m_name(name),
    m_classId(classId),
    m_classLevel(-1),
    m_eventIndex(-1),
    m_bIsMandatory(false)
{
    m_classLevel = static_cast<int>(std::count(classId.begin(), classId.end(), '.') + 1);
    m_eventIndex = atoi(eventIndex.c_str());
    m_bIsMandatory = ((mandatory == "true") || (mandatory == "True")) ? true : false;
}

const std::string& BaseTestClass::EventInfo::GetName() const
{
    return m_name;
}

const std::string& BaseTestClass::EventInfo::GetClassId() const
{
    return m_classId;
}

int BaseTestClass::EventInfo::GetClassLevel() const
{
    return m_classLevel;
}

int BaseTestClass::EventInfo::GetEventIndex() const
{
    return m_eventIndex;
}

bool BaseTestClass::EventInfo::GetIsMandatory() const
{
    return m_bIsMandatory;
}

// ---- RootMemberInfo ----

BaseTestClass::RootMemberInfo::RootMemberInfo()
{

}

BaseTestClass::RootMemberInfo::RootMemberInfo(const std::string& name, const std::string& classId, const std::string& version, const std::string& isMandatory, const std::vector<MethodInfo>& methodInfos, const std::vector<EventInfo>& eventInfos, bool bDeprecated, const std::string& objectNumber) :
    m_name(name),
    m_classId(classId),
    m_version(-1),
    m_isMandatory(false),
    m_methodInfos(methodInfos),
    m_eventInfos(eventInfos),
    m_bDeprecated(bDeprecated)
{
    m_version = atoi(version.c_str());
    m_isMandatory = ((isMandatory == "true") || (isMandatory == "True")) ? true : false;
}

const std::string& BaseTestClass::RootMemberInfo::GetName() const
{
    return m_name;
}

const std::string& BaseTestClass::RootMemberInfo::GetClassId() const
{
    return m_classId;
}

int BaseTestClass::RootMemberInfo::GetVersion() const
{
    return m_version;
}

bool BaseTestClass::RootMemberInfo::GetIsMandatory() const
{
    return m_isMandatory;
}

bool BaseTestClass::RootMemberInfo::GetIsDeprecated() const
{
    return m_bDeprecated;
}

const BaseTestClass::MethodInfos& BaseTestClass::RootMemberInfo::GetMethodInfos() const
{
    return m_methodInfos;
}

const BaseTestClass::EventInfos& BaseTestClass::RootMemberInfo::GetEventInfos() const
{
    return m_eventInfos;
}

// ---- ManagerInfo ----
BaseTestClass::ManagerInfo::ManagerInfo()
{

}

BaseTestClass::ManagerInfo::ManagerInfo(const std::string& name, const std::string& classId, const std::string& version, const std::string& isMandatory, const std::vector<MethodInfo>& methodInfos, const std::vector<EventInfo>& eventInfos, bool bDeprecated, const std::string& objectNumber) :
    RootMemberInfo(name, classId, version, isMandatory, methodInfos, eventInfos, bDeprecated, objectNumber),
    m_objectNumber(OCA_INVALID_ONO)
{
    if (objectNumber.length() >= 1)
    {
        char* ptr;
        m_objectNumber = strtoul(objectNumber.c_str(), &ptr, 10);
    }
}

int BaseTestClass::ManagerInfo::GetObjectNumber() const
{
    return m_objectNumber;
}

// ---- BlockMemberInfo ----
BaseTestClass::BlockMemberInfo::BlockMemberInfo()
{

}

BaseTestClass::BlockMemberInfo::BlockMemberInfo(const std::string& name, const std::string& classId, const std::string& version, const std::string& isMandatory, const std::vector<MethodInfo>& methodInfos, const std::vector<EventInfo>& eventInfos, bool bDeprecated, const std::string& objectNumber) :
    RootMemberInfo(name, classId, version, isMandatory, methodInfos, eventInfos, bDeprecated, objectNumber),
    m_objectNumber(-1)
{
    if (objectNumber.length() >= 1)
    {
        char* ptr;
        m_objectNumber = strtoul(objectNumber.c_str(), &ptr, 10);
    }
}

unsigned int BaseTestClass::BlockMemberInfo::GetObjectNumber() const
{
    return m_objectNumber;
}
