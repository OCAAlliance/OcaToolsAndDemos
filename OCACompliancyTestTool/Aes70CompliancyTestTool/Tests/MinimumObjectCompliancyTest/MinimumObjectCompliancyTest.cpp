/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Test Minimum Object Compliancy
 */

// ---- Include system wide include files ----
#include <OCALite/Proxy/GeneralProxy.h>
#include <OCALite/OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCALite/OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCALite/OCC/ControlClasses/Managers/OcaLiteMediaClockManager.h>
#include <OCALite/OCC/ControlClasses/Agents/OcaLiteTimeSource.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteMethod.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteManagerDescriptor.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteEvent.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteBlockMember.h>
#include <OCALite/OCP.1/Ocp1LiteNetworkAddress.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "MinimumObjectCompliancyTest.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Function Implementation ----

MinimumObjectCompliancyTest::MinimumObjectCompliancyTest() :
    BaseTestClass("Minimum object compliancy test")
{

}

bool MinimumObjectCompliancyTest::CheckMethods(const BaseTestClass::MethodInfos& methodInfos, const RootMemberInfo& objectInfo, ::OcaONo objectNumber, ::GeneralProxy& proxy)
{
    // Assume all goes well.
    bool bTestResult(true);

    // Loop through all methods we know.
    MethodInfos::const_iterator methodIter(methodInfos.begin());
    while (methodIter != methodInfos.end())
    {
        // If the method is mandatory, the result must not be bad method, bad ono, or not implemented.
        if (methodIter->GetIsMandatory())
        {
            // Invoke the method at the device.
            ::OcaLiteMethodID methodId(static_cast< ::OcaUint16>(methodIter->GetClassLevel()), static_cast< ::OcaUint16>(methodIter->GetMethodIndex()));
            ::OcaLiteStatus callResult(proxy.OcaCompliancyTestTool_InvalidCall(objectNumber, methodId));
     
            if ((OCASTATUS_BAD_METHOD != callResult) &&
                (OCASTATUS_BAD_ONO != callResult) && 
                (OCASTATUS_NOT_IMPLEMENTED != callResult))
            {
                // Success
                AddTestLog("      Device implements method %s for object %s (ONo %u, result %d)", 
                                methodIter->GetName().c_str(), 
                                objectInfo.GetName().c_str(), 
                                objectNumber, 
                                callResult);
            }
            else
            {
                // Failed
                AddTestResult("ERROR: Device must implement method %s (index %d) for object %s (ONo %d, result %d)", 
                                    methodIter->GetName().c_str(), 
                                    methodIter->GetMethodIndex(), 
                                    objectInfo.GetName().c_str(), 
                                    objectNumber, 
                                    callResult);

                // We don't need to break, just set the result to false.
                bTestResult = false;
            }
        }
        // Else we don't care for this test

        ++methodIter;
    }

    return bTestResult;
}

bool MinimumObjectCompliancyTest::CheckEvents(const BaseTestClass::EventInfos& eventInfos, const RootMemberInfo& objectInfo, ::OcaONo objectNumber, ::GeneralProxy& proxy)
{
    // Assume all goes well.
    bool bTestResult(true);

    EventInfos::const_iterator eventIter(eventInfos.begin());
    while (eventIter != eventInfos.end())
    {

        // If the event is mandatory, the result must be OK.
        if (eventIter->GetIsMandatory())
        {
            // Add the subscription at the device.
            ::OcaLiteStatus callResult(proxy.OcaSubscriptionManager_AddSubscription(::OcaLiteEvent(objectNumber,
                                                                                                   ::OcaLiteEventID(static_cast< ::OcaUint16>(eventIter->GetClassLevel()),
                                                                                                                    static_cast< ::OcaUint16>(eventIter->GetEventIndex()))),
                                                                                    ::OcaLiteMethod(static_cast< ::OcaONo>(1234),
                                                                                                    ::OcaLiteMethodID(static_cast< ::OcaUint16>(1), 
                                                                                                                      static_cast< ::OcaUint16>(1))),
                                                                                    ::OcaLiteBlob(),
                                                                                    ::OcaLiteNotificationDeliveryMode::OCANOTIFICATIONDELIVERYMODE_RELIABLE,
                                                                                    ::OcaLiteNetworkAddress()));
            if (OCASTATUS_OK == callResult)
            {
                AddTestLog("         Device implements event %s for object %u (result %d)",
                                eventIter->GetName().c_str(), 
                                objectNumber, 
                                callResult);
            }
            else
            {

                // Add the subscription at the device.
                callResult = proxy.OcaSubscriptionManager_AddSubscription(::OcaLiteEvent(objectNumber,
                                                                                         ::OcaLiteEventID(static_cast< ::OcaUint16>(eventIter->GetClassLevel()),
                                                                                                         static_cast< ::OcaUint16>(eventIter->GetEventIndex()))),
                                                                         ::OcaLiteMethod(static_cast< ::OcaONo>(1234),
                                                                                         ::OcaLiteMethodID(static_cast< ::OcaUint16>(1), 
                                                                                                             static_cast< ::OcaUint16>(1))),
                                                                         ::OcaLiteBlob(),
                                                                         ::OcaLiteNotificationDeliveryMode::OCANOTIFICATIONDELIVERYMODE_FAST,
                                                                         ::OcaLiteNetworkAddress(::Ocp1LiteNetworkAddress(::OcaLiteString("239.0.0.1"),
                                                                                                                          static_cast< ::OcaUint16>(54354))));
                if (OCASTATUS_OK == callResult)
                {
                    AddTestLog("         Device implements event %s for object %u (result %d)",
                                    eventIter->GetName().c_str(), 
                                    objectNumber, 
                                    callResult);
                }
                else
                {
                    // Event should have been present.
                    AddTestResult("ERROR: Device must implement event %s (index %d) for object %u (result %d).",
                                        eventIter->GetName().c_str(), 
                                        eventIter->GetEventIndex(), 
                                        objectNumber, 
                                        callResult);

                    // We don't need to break, just set the result to false.
                    bTestResult = false;
                }
            }

            // Cleanup
            if (OCASTATUS_OK == callResult)
            {
                static_cast<void>(proxy.OcaSubscriptionManager_RemoveSubscription(::OcaLiteEvent(objectNumber,
                                                                                                 ::OcaLiteEventID(static_cast< ::OcaUint16>(eventIter->GetClassLevel()),
                                                                                                                  static_cast< ::OcaUint16>(eventIter->GetEventIndex()))),
                                                                                  ::OcaLiteMethod(static_cast< ::OcaONo>(1234),
                                                                                                  ::OcaLiteMethodID(static_cast< ::OcaUint16>(1), 
                                                                                                                    static_cast< ::OcaUint16>(1)))));
            }
            else
            {
                AddTestLog("WARNING: Failed to remove subscription for event %s (index %d) for object %u (result %d). It may return NotImplemented.",
                    eventIter->GetName().c_str(), eventIter->GetEventIndex(), objectNumber, callResult);
            }
        }

        ++eventIter;
    }

    return bTestResult;
}

::OcaLiteStatus MinimumObjectCompliancyTest::GetManagers(::GeneralProxy& proxy, ::OcaLiteList< ::OcaLiteManagerDescriptor>& deviceReportedManagers)
{
    // Get the managers from the device.
    ::OcaLiteStatus getManagersResult(proxy.OcaDeviceManager_GetManagers(deviceReportedManagers));
    if (OCASTATUS_OK != getManagersResult)
    {
        AddTestResult("ERROR: Failed to get managers from the device. status = %d", getManagersResult);


        deviceReportedManagers.Clear();
    }
    return getManagersResult;
}

::OcaLiteStatus MinimumObjectCompliancyTest::HandleMembers(::OcaLiteList< ::OcaLiteObjectIdentification> members, ::GeneralProxy& proxy, ::OcaLiteList< ::OcaLiteObjectIdentification>& outputMembers)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    // For all the members, we have to do something, we bail out if something fails.
    for (::OcaUint16 i(0); ((i < members.GetCount()) && (OCASTATUS_OK == rc)); i++)
    {
        // See if the member is a block kind of object.
        ::OcaLiteClassID classId(members.GetItem(i).GetClassIdentification().GetClassID());
        if ((classId.GetFieldCount() >= ::OcaLiteBlock::CLASS_ID.GetFieldCount()) &&
            (0 == memcmp(classId.GetFields(),
                ::OcaLiteBlock::CLASS_ID.GetFields(),
                ::OcaLiteBlock::CLASS_ID.GetFieldCount() * sizeof(::OcaLiteBlock::CLASS_ID.GetFields()[0]))))
        {
            // We query the new members
            ::OcaLiteList< ::OcaLiteObjectIdentification> newMembers;
            rc = proxy.OcaBlock_GetMembers(members.GetItem(i).GetONo(), newMembers);
            if (OCASTATUS_OK == rc)
            {
                // Add the members to our output list.
                for (::OcaUint16 counter(0); counter < newMembers.GetCount(); counter++)
                {
                    outputMembers.Add(newMembers.GetItem(counter));
                }

                // We call ourselves, blocks may contain blocks main contain blocks ..
                rc = HandleMembers(newMembers, proxy, outputMembers);
            }
            else
            {
                AddTestResult("ERROR: Failed to get members from block object %u. Status %d",
                    members.GetItem(i).GetONo(), rc);
            }
        }
    }

    return rc;
}


static int CountObjectsOfType(const ::OcaLiteList< ::OcaLiteObjectIdentification>& members, const ::OcaLiteClassID& classId)
{
    int count(0);
    for (::OcaUint16 counter(0); counter < members.GetCount(); counter++)
    {
        // Cache it
        const ::OcaLiteObjectIdentification& currentMember(members.GetItem(counter));
        if ((classId.GetFieldCount() <= currentMember.GetClassIdentification().GetClassID().GetFieldCount()) &&
            (0 == memcmp(classId.GetFields(), currentMember.GetClassIdentification().GetClassID().GetFields(),
                sizeof(classId.GetFields()[0]) * classId.GetFieldCount())))
        {
            count++;
        }
    }
    return count;
}

 ::OcaLiteStatus MinimumObjectCompliancyTest::GetObjects(::GeneralProxy& proxy, ::OcaLiteList< ::OcaLiteObjectIdentification>& outputMembers)
{
    outputMembers.Clear();

    // Get information about the root block
    ::OcaLiteClassIdentification rootBlockClassInfo;
    ::OcaLiteStatus rc(proxy.OcaRoot_GetClassIdentification(OCA_ROOT_BLOCK_ONO, rootBlockClassInfo));
    if (OCASTATUS_OK == rc)
    {
        outputMembers.Add(::OcaLiteObjectIdentification(OCA_ROOT_BLOCK_ONO, rootBlockClassInfo));

        // Get information about all other objects, either on the root block recursice, or for devices which doesn't
        // implement that, perform ourselves a recursive search
        ::OcaLiteList< ::OcaLiteBlockMember> membersRecursive;
        ::OcaLiteStatus rc = proxy.OcaBlock_GetMembersRecursive(OCA_ROOT_BLOCK_ONO, membersRecursive);
        bool bTestRootBlockResult((rc == OCASTATUS_OK) && (membersRecursive.GetCount() > 0));
        if (bTestRootBlockResult)
        {
            AddTestLog("OcaBlock.GetMembersRecursive returns status %d, nr members %d. Expecting OK. Test %s",
                rc, membersRecursive.GetCount(), bTestRootBlockResult ? "Passed" : "Failed");

            for (::OcaUint16 counter(0); counter < membersRecursive.GetCount(); counter++)
            {
                outputMembers.Add(membersRecursive.GetItem(counter).GetMemberObjectIdentification());
            }
        }
        else
        {
            // GetMembersRecursive failed, this is acceptable if we can query the members ourselves recursive
            ::OcaLiteList< ::OcaLiteObjectIdentification> members;
            rc = proxy.OcaBlock_GetMembers(OCA_ROOT_BLOCK_ONO, members);
            if (OCASTATUS_OK == rc)
            {
                outputMembers = members;
                rc = HandleMembers(members, proxy, outputMembers);
            }

            bTestRootBlockResult = (rc == OCASTATUS_OK);

            AddTestLog("OcaBlock.GetMembers returns status %d. Expecting OK. Test %s",
                rc, bTestRootBlockResult ? "Passed" : "Failed");
        }
    }

    return rc;
}

bool MinimumObjectCompliancyTest::TestManagers(const ::OcaLiteList< ::OcaLiteManagerDescriptor>& deviceReportedManagers, 
                                               const ::OcaLiteList< ::OcaLiteObjectIdentification>& members,
                                               ::GeneralProxy& proxy)
{
    bool bTestResult(true);

    ::OcaLiteList< ::OcaLiteManagerDescriptor> mandatoryManagers;

    AddTestResult("   Testing mandatory manager(s) for test device");

    // First filter the mandatory managers.
    ManagerInfos::iterator iter(m_managerInfos.begin());
    while (iter != m_managerInfos.end())
    {
        // Track whether we have found the current iter class.
        bool bFound(false);

        // Loop through our reported manager to test against.
        for (::OcaUint16 counter(0); counter < deviceReportedManagers.GetCount(); counter++)
        {
            // Cache it
            const ::OcaLiteManagerDescriptor& currentDeviceReportedManager(deviceReportedManagers.GetItem(counter));

            // This is a bit tricky, we want to fall through this is when:
            // - The classID matches exactly (e.g. 1.2.3.4 == 1.2.3.4).
            // - The classID from the iter is a base class of reported (e.g. 1.2.3 == 1.2.3.4)
            ::OcaLiteClassID xmlClassId(iter->GetClassId());
            if (((xmlClassId.GetFieldCount() == currentDeviceReportedManager.GetClassID().GetFieldCount()) ||
                (xmlClassId.GetFieldCount() < currentDeviceReportedManager.GetClassID().GetFieldCount())) &&
                0 == memcmp(xmlClassId.GetFields(),
                    currentDeviceReportedManager.GetClassID().GetFields(),
                    xmlClassId.GetFieldCount() > currentDeviceReportedManager.GetClassID().GetFieldCount() ?
                    currentDeviceReportedManager.GetClassID().GetFieldCount() * sizeof(currentDeviceReportedManager.GetClassID().GetFields()[0]) :
                    xmlClassId.GetFieldCount() * sizeof(xmlClassId.GetFields()[0])))
            {
                bFound = true;

                if (iter->GetIsMandatory())
                {
                    if (!mandatoryManagers.Contains(currentDeviceReportedManager))
                    {
                        mandatoryManagers.Add(currentDeviceReportedManager);
                    }
                }
                else if (::TestContext::GetInstance().GetTestVersion() >= AES70_2018_TECHNICAL_VERSION)
                {
                    // For AES70-2018 we have some special rules:
                    // - OcaNetworkManager (6) Mandatory if a device contains more than one network object.
                    // - OcaMediaClockManager (7) Mandatory if a device contains more than one clock object.
                    // - OcaDeviceTimeManager (10) Mandatory if a device contains more than one time source object.
                    // - OcaCodingManager (12) Mandatory if a device implements more than one encoding schemes or more than one decoding schemes.

                    if (::OcaLiteNetworkManager::OBJECT_NUMBER == iter->GetObjectNumber())
                    {
                        int numberOfNetworks(CountObjectsOfType(members, ::OcaLiteNetwork::CLASS_ID));
                        AddTestLog("Device contains %d networks", numberOfNetworks);
                        if (numberOfNetworks > 1)
                        {
                            if (!mandatoryManagers.Contains(currentDeviceReportedManager))
                            {
                                mandatoryManagers.Add(currentDeviceReportedManager);
                            }
                        }
                    }
                    else if (::OcaLiteMediaClockManager::OBJECT_NUMBER == iter->GetObjectNumber())
                    {
                        int numberOfClocks(CountObjectsOfType(members, ::OcaLiteMediaClock::CLASS_ID));
                        int numberOfClocks3(CountObjectsOfType(members, ::OcaLiteMediaClock3::CLASS_ID));
                        AddTestLog("Device contains %d clocks (clock %d, clock3 %d)", numberOfClocks + numberOfClocks3, numberOfClocks, numberOfClocks3);
                        if ((numberOfClocks + numberOfClocks3) > 1)
                        {
                            if (!mandatoryManagers.Contains(currentDeviceReportedManager))
                            {
                                mandatoryManagers.Add(currentDeviceReportedManager);
                            }
                        }
                    }
                    else if (/**::OcaLiteDeviceTimeManager::OBJECT_NUMBER**/static_cast< ::OcaONo>(10) == iter->GetObjectNumber())
                    {
                        int numberOfTimeSources(CountObjectsOfType(members, ::OcaLiteTimeSource::CLASS_ID));
                        AddTestLog("Device contains %d timesources", numberOfTimeSources);

                        if (numberOfTimeSources > 1)
                        {
                            if (!mandatoryManagers.Contains(currentDeviceReportedManager))
                            {
                                mandatoryManagers.Add(currentDeviceReportedManager);
                            }
                        }
                    }
                }
            }
        }
        if (!bFound && iter->GetIsMandatory())
        {
            AddTestResult("ERROR: Missing mandatory manager %d", iter->GetName().c_str());
            // Did not found mandatory manager!
            bTestResult = false;
        }
        ++iter;
    }

    AddTestResult("   Testing %d mandatory manager(s) for compliancy", mandatoryManagers.GetCount());

    // We can only test what we know, therefore use that as main for loop (remark this does include base classes!)
    iter = m_managerInfos.begin();
    while (iter != m_managerInfos.end())
    {
        // Track whether we have found the current iter class.
        bool bFound(false);

        // Loop through our mandatory manager to test against.
        for (::OcaUint16 counter(0); counter < mandatoryManagers.GetCount(); counter++)
        {
            // Cache it
            ::OcaLiteManagerDescriptor currentDeviceReportedManager(mandatoryManagers.GetItem(counter));

            // This is a bit tricky, we want to fall through this is when:
            // - The classID matches exactly (e.g. 1.2.3.4 == 1.2.3.4).
            // - The classID from the iter is a base class of reported (e.g. 1.2.3 == 1.2.3.4)
            ::OcaLiteClassID xmlClassId(iter->GetClassId());
            if (((xmlClassId.GetFieldCount() == currentDeviceReportedManager.GetClassID().GetFieldCount()) ||
                (xmlClassId.GetFieldCount() < currentDeviceReportedManager.GetClassID().GetFieldCount())) &&
                0 == memcmp(xmlClassId.GetFields(),
                    currentDeviceReportedManager.GetClassID().GetFields(),
                    xmlClassId.GetFieldCount() > currentDeviceReportedManager.GetClassID().GetFieldCount() ?
                    currentDeviceReportedManager.GetClassID().GetFieldCount() * sizeof(currentDeviceReportedManager.GetClassID().GetFields()[0]) :
                    xmlClassId.GetFieldCount() * sizeof(xmlClassId.GetFields()[0])))
            {
                bFound = true;

                AddTestLog("Test MANDATORY manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), iter->GetName().c_str());

                // Check whether this object has a fixed object number
                if (static_cast< ::OcaONo>(iter->GetObjectNumber()) != OCA_INVALID_ONO)
                {
                    // This object has a fixed object number in the specification, check the object number and class version
                    if ((currentDeviceReportedManager.GetObjectNumber() == iter->GetObjectNumber()) &&
                        (currentDeviceReportedManager.GetClassVersion() >= iter->GetVersion()))
                    {
                        AddTestLog("   Attributes of manager %s (Name=%s) verified! ClassID %s, ObjectNumber %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(), currentDeviceReportedManager.GetName().GetString().c_str(), iter->GetClassId().c_str(),
                            iter->GetObjectNumber(), currentDeviceReportedManager.GetClassVersion(), iter->GetVersion());
                    }
                    else
                    {
                        AddTestResult("ERROR: Attributes of manager %s (Name=%s) NOT verified! ClassID %s, ObjectNumber %d -> Expected %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(), currentDeviceReportedManager.GetName().GetString().c_str(), iter->GetClassId().c_str(),
                            currentDeviceReportedManager.GetObjectNumber(), iter->GetObjectNumber(), currentDeviceReportedManager.GetClassVersion(), iter->GetVersion());

                        bTestResult = false;
                    }
                }
                else
                {
                    // We only need to check the class version.
                    if (currentDeviceReportedManager.GetClassVersion() >= iter->GetVersion())
                    {
                        AddTestLog("   Attributes of manager %s (Name=%s) verified! ClassID %s, ObjectNumber %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(), currentDeviceReportedManager.GetName().GetString().c_str(), iter->GetClassId().c_str(),
                            iter->GetObjectNumber(), currentDeviceReportedManager.GetClassVersion(), iter->GetVersion());
                    }
                    else
                    {
                        AddTestResult("ERROR: Attributes of manager %s (Name=%s) NOT verified! ClassID %s, ObjectNumber %d -> Expected %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(), currentDeviceReportedManager.GetName().GetString().c_str(), iter->GetClassId().c_str(),
                            currentDeviceReportedManager.GetObjectNumber(), iter->GetObjectNumber(), currentDeviceReportedManager.GetClassVersion(), iter->GetVersion());

                        bTestResult = false;
                    }
                }

                // Test all methods
                if (!CheckMethods(iter->GetMethodInfos(), *iter, currentDeviceReportedManager.GetObjectNumber(), proxy))
                {
                    AddTestResult("ERROR: Failed to test the methods of MANDATORY manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), iter->GetName().c_str());
                    bTestResult = false;
                }

                // Test all events
                if (!CheckEvents(iter->GetEventInfos(), *iter, currentDeviceReportedManager.GetObjectNumber(), proxy))
                {
                    AddTestResult("ERROR: Failed to test the events of MANDATORY manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), iter->GetName().c_str());
                    bTestResult = false;
                }
            }
        }

        ++iter;
    }

    // Also the for OcaRoot compliancy, OcaManager compliancy is already tested.
    for (::OcaUint16 counter(0); counter < mandatoryManagers.GetCount(); counter++)
    {
        // Cache it
        ::OcaLiteManagerDescriptor currentDeviceReportedManager(mandatoryManagers.GetItem(counter));

        AddTestLog("Test MANDATORY manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), m_rootMemberInfos[0].GetName().c_str());

        // Test all methods
        if (!CheckMethods(m_rootMemberInfos[0].GetMethodInfos(), m_rootMemberInfos[0], currentDeviceReportedManager.GetObjectNumber(), proxy))
        {
            AddTestResult("ERROR: Failed to test the methods of MANDATORY manager %u of type %s", 
                currentDeviceReportedManager.GetObjectNumber(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }

        // Test all events
        if (!CheckEvents(m_rootMemberInfos[0].GetEventInfos(), m_rootMemberInfos[0], currentDeviceReportedManager.GetObjectNumber(), proxy))
        {
            AddTestResult("ERROR: Failed to test the events of MANDATORY manager %u of type %s", 
                currentDeviceReportedManager.GetObjectNumber(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }
    }

    return bTestResult;
}

bool MinimumObjectCompliancyTest::TestObjects(const ::OcaLiteList< ::OcaLiteObjectIdentification>& deviceReportedObjects, ::GeneralProxy& proxy)
{
    bool bTestResult(true);

    AddTestResult("Testing mandatory object(s) for test device");

    ::OcaLiteList< ::OcaLiteObjectIdentification> mandatoryObjects;

    // We can only test what we know, therefore use that as main for loop (remark this does include base classes!)
    BlockMemberInfos::iterator iter(m_blockMemberInfos.begin());
    while (iter != m_blockMemberInfos.end())
    {
        // Track whether we have found the current iter class.
        bool bFound(false);

        // Loop through our reported manager to test against.
        for (::OcaUint16 counter(0); counter < deviceReportedObjects.GetCount(); counter++)
        {
            // Cache it
            ::OcaLiteObjectIdentification currentDeviceReportedObjects(deviceReportedObjects.GetItem(counter));

            // This is a bit tricky, we want to fall through this is when:
            // - The classID matches exactly (e.g. 1.2.3.4 == 1.2.3.4).
            // - The classID from the iter is a base class of reported (e.g. 1.2.3 == 1.2.3.4)
            ::OcaLiteClassID xmlClassId(iter->GetClassId());
            if (((xmlClassId.GetFieldCount() == currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount()) ||
                (xmlClassId.GetFieldCount() < currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount())) &&
                0 == memcmp(xmlClassId.GetFields(),
                    currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFields(),
                    xmlClassId.GetFieldCount() > currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount() ?
                    currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount() * sizeof(currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFields()[0]) :
                    xmlClassId.GetFieldCount() * sizeof(xmlClassId.GetFields()[0])))
            {
                bFound = true;

                if (iter->GetIsMandatory())
                {
                    if (!mandatoryObjects.Contains(currentDeviceReportedObjects))
                    {
                        mandatoryObjects.Add(currentDeviceReportedObjects);
                    }
                }
            }
        }

        if (!bFound && iter->GetIsMandatory())
        {
            // Did not found mandatory object!
            AddTestResult("ERROR: Missing mandatory object %s", iter->GetName().c_str());
            bTestResult = false;
        }
        ++iter;
    }

    AddTestResult("   Testing %d mandatory object(s) for compliancy", mandatoryObjects.GetCount());

    // We can only test what we know, therefore use that as main for loop (remark this does include base classes!)
    iter = m_blockMemberInfos.begin();
    while (iter != m_blockMemberInfos.end())
    {
        // Track whether we have found the current iter class.
        bool bFound(false);

        // Loop through our reported manager to test against.
        for (::OcaUint16 counter(0); counter < mandatoryObjects.GetCount(); counter++)
        {
            // Cache it
            ::OcaLiteObjectIdentification currentDeviceReportedObjects(mandatoryObjects.GetItem(counter));

            // This is a bit tricky, we want to fall through this is when:
            // - The classID matches exactly (e.g. 1.2.3.4 == 1.2.3.4).
            // - The classID from the iter is a base class of reported (e.g. 1.2.3 == 1.2.3.4)
            ::OcaLiteClassID xmlClassId(iter->GetClassId());
            if (((xmlClassId.GetFieldCount() == currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount()) ||
                (xmlClassId.GetFieldCount() < currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount())) &&
                0 == memcmp(xmlClassId.GetFields(),
                    currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFields(),
                    xmlClassId.GetFieldCount() > currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount() ?
                    currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFieldCount() * sizeof(currentDeviceReportedObjects.GetClassIdentification().GetClassID().GetFields()[0]) :
                    xmlClassId.GetFieldCount() * sizeof(xmlClassId.GetFields()[0])))
            {
                bFound = true;

                AddTestLog("Test MANDATORY object %u for type %s", currentDeviceReportedObjects.GetONo(), iter->GetName().c_str());

                // Check the class version and object number range
                if (((currentDeviceReportedObjects.GetONo() >= iter->GetObjectNumberLowerLimit()) && 
                    (currentDeviceReportedObjects.GetONo() <= iter->GetObjectNumberUpperLimit())) || 
                    currentDeviceReportedObjects.GetONo() == 100 &&
                    (currentDeviceReportedObjects.GetClassIdentification().GetClassVersion() >= iter->GetVersion()))
                {
                    AddTestLog("   Attributes of object type %s, ONo=%d verified! ClassID %s. Class version %d -> expected %d ",
                        iter->GetName().c_str(), currentDeviceReportedObjects.GetONo(), iter->GetClassId().c_str(),
                        currentDeviceReportedObjects.GetClassIdentification().GetClassVersion(), iter->GetVersion());
                }
                else
                {
                    if (!(currentDeviceReportedObjects.GetClassIdentification().GetClassVersion() >= iter->GetVersion()))
                    {
                        AddTestResult("ERROR: Attributes of object type %s, ONo=%u NOT verified! ClassID %s, Class version %d -> expected %d ",
                            iter->GetName().c_str(), currentDeviceReportedObjects.GetONo(), iter->GetClassId().c_str(),
                            currentDeviceReportedObjects.GetClassIdentification().GetClassVersion(), iter->GetVersion());
                    }

                    if (!((currentDeviceReportedObjects.GetONo() >= iter->GetObjectNumberLowerLimit()) &&
                        (currentDeviceReportedObjects.GetONo() <= iter->GetObjectNumberUpperLimit())))
                    {
                        AddTestResult("ERROR: Attributes of object type %s, ONo=%u NOT verified! ClassID %s, Class version %d. ONo should be with range >= %u and <= %u",
                            iter->GetName().c_str(), currentDeviceReportedObjects.GetONo(), iter->GetClassId().c_str(),
                            currentDeviceReportedObjects.GetClassIdentification().GetClassVersion(),
                            iter->GetObjectNumberLowerLimit(), iter->GetObjectNumberUpperLimit());
                    }

                    bTestResult = false;
                }

                // Test for the current level
                // Test all methods
                if (!CheckMethods(iter->GetMethodInfos(), *iter, currentDeviceReportedObjects.GetONo(), proxy))
                {
                    AddTestResult("ERROR: Failed to test methods of MANDATORY object %u of type %s", 
                        currentDeviceReportedObjects.GetONo(), iter->GetName().c_str());
                    bTestResult = false;
                }

                // Test all events
                if (!CheckEvents(iter->GetEventInfos(), *iter, currentDeviceReportedObjects.GetONo(), proxy))
                {
                    AddTestResult("ERROR: Failed to test events of MANDATORY object %u of type %s", 
                        currentDeviceReportedObjects.GetONo(), iter->GetName().c_str());
                    bTestResult = false;
                }
            }
        }

        ++iter;
    }

    // Loop through our reported manager to test against.
    for (::OcaUint16 counter(0); counter < mandatoryObjects.GetCount(); counter++)
    {
        // Cache it
        ::OcaLiteObjectIdentification currentDeviceReportedObjects(mandatoryObjects.GetItem(counter));

        AddTestLog("Test MANDATORY object %u for type %s", currentDeviceReportedObjects.GetONo(), m_rootMemberInfos[0].GetName().c_str());

        // Also test on Root level
        // Test all methods
        if (!CheckMethods(m_rootMemberInfos[0].GetMethodInfos(), m_rootMemberInfos[0], currentDeviceReportedObjects.GetONo(), proxy))
        {
            AddTestResult("ERROR: Failed to test the events of MANDATORY object %u of type %s",
                currentDeviceReportedObjects.GetONo(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }

        // Test all events
        if (!CheckEvents(m_rootMemberInfos[0].GetEventInfos(), m_rootMemberInfos[0], currentDeviceReportedObjects.GetONo(), proxy))
        {
            AddTestResult("ERROR: Failed to test the events of MANDATORY object %u of type %s",
                currentDeviceReportedObjects.GetONo(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }

    }
    return bTestResult;
}

bool MinimumObjectCompliancyTest::ExecuteTest()
{
    bool bTestResult(LoadReferenceOCCMembers());
    if (bTestResult)
    {
        if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP)
        {
            // Setup a connection
            ::OcaSessionID sessionId(GetSession(::TestContext::GetInstance().GetTestDeviceName(), OCA_TCP_SERVICE));
            if (OCA_INVALID_SESSIONID != sessionId)
            {
                // Create a proxy
                ::GeneralProxy proxy(sessionId, ::TestContext::GetInstance().GetOcp1TcpNetworkObjectNumber());

                ::OcaLiteList< ::OcaLiteManagerDescriptor> deviceReportedManagers;
                if (GetManagers(proxy, deviceReportedManagers) == OCASTATUS_OK)
                {
                    ::OcaLiteList< ::OcaLiteObjectIdentification> deviceReportedObjects;
                    if (GetObjects(proxy, deviceReportedObjects) == OCASTATUS_OK)
                    {
                        if (TestManagers(deviceReportedManagers, deviceReportedObjects, proxy))
                        {
                            AddTestLog("Succesfully tested managers.");
                        }
                        else
                        {
                            AddTestResult("ERROR: Failed to test managers");
                            bTestResult = false;
                        }

                        if (TestObjects(deviceReportedObjects, proxy))
                        {
                            AddTestLog("Succesfully tested objects.");
                        }
                        else
                        {
                            AddTestResult("ERROR: Failed to test objects");
                            bTestResult = false;
                        }
                    }
                    else
                    {
                        AddTestResult("ERROR: Failed to get objects");
                        bTestResult = false;
                    }
                }
                else
                {
                    AddTestResult("ERROR: Failed to get managers");
                    bTestResult = false;
                }
            }
            else
            {
                AddTestResult("ERROR: Failed to setup TCP connection to test device");
                bTestResult = false;
            }
        }

#ifdef HAS_SECURE_STACK
        if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP_SEC)
        {
            // Setup a connection
            ::OcaSessionID sessionId(GetSession(::TestContext::GetInstance().GetTestDeviceName(), OCA_SECURE_TCP_SERVICE));
            if (OCA_INVALID_SESSIONID != sessionId)
            {
                // Create a proxy
                ::GeneralProxy proxy(sessionId, ::TestContext::GetInstance().GetOcp1TcpSecureNetworkObjectNumber());

                ::OcaLiteList< ::OcaLiteManagerDescriptor> deviceReportedManagers;
                if (GetManagers(proxy, deviceReportedManagers) == OCASTATUS_OK)
                {
                    ::OcaLiteList< ::OcaLiteObjectIdentification> deviceReportedObjects;
                    if (GetObjects(proxy, deviceReportedObjects) == OCASTATUS_OK)
                    {
                        if (TestManagers(deviceReportedManagers, deviceReportedObjects, proxy))
                        {
                            AddTestLog("Succesfully tested managers.");
                        }
                        else
                        {
                            AddTestResult("ERROR: Failed to test managers");
                            bTestResult = false;
                        }

                        if (TestObjects(deviceReportedObjects, proxy))
                        {
                            AddTestLog("Succesfully tested objects.");
                        }
                        else
                        {
                            AddTestResult("ERROR: Failed to test objects");
                            bTestResult = false;
                        }
                    }
                    else
                    {
                        AddTestResult("ERROR: Failed to get objects");
                        bTestResult = false;
                    }
                }
                else
                {
                    AddTestResult("ERROR: Failed to get managers");
                    bTestResult = false;
                }
            }
            else
            {
                AddTestResult("ERROR: Failed to setup (secure) TCP connection to test device");
                bTestResult = false;
            }
        }
#endif

        if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_UDP)
        {
            // Setup a connection
            ::OcaSessionID sessionId(GetSession(::TestContext::GetInstance().GetTestDeviceName(), OCA_UDP_SERVICE));
            if (OCA_INVALID_SESSIONID != sessionId)
            {
                // Create a proxy
                ::GeneralProxy proxy(sessionId, ::TestContext::GetInstance().GetOcp1UdpNetworkObjectNumber());

                ::OcaLiteList< ::OcaLiteManagerDescriptor> deviceReportedManagers;
                if (GetManagers(proxy, deviceReportedManagers) == OCASTATUS_OK)
                {
                    ::OcaLiteList< ::OcaLiteObjectIdentification> deviceReportedObjects;
                    if (GetObjects(proxy, deviceReportedObjects) == OCASTATUS_OK)
                    {
                        if (TestManagers(deviceReportedManagers, deviceReportedObjects, proxy))
                        {
                            AddTestLog("Succesfully tested managers.");
                        }
                        else
                        {
                            AddTestResult("ERROR: Failed to test managers");
                            bTestResult = false;
                        }

                        if (TestObjects(deviceReportedObjects, proxy))
                        {
                            AddTestLog("Succesfully tested objects.");
                        }
                        else
                        {
                            AddTestResult("ERROR: Failed to test objects");
                            bTestResult = false;
                        }
                    }
                    else
                    {
                        AddTestResult("ERROR: Failed to get objects");
                        bTestResult = false;
                    }
                }
                else
                {
                    AddTestResult("ERROR: Failed to get managers");
                    bTestResult = false;
                }
            }
            else
            {
                AddTestResult("ERROR: Failed to setup UDP connection to test device");
                bTestResult = false;
            }
        }
    }
    else
    {
        AddTestResult("ERROR: Failed to load ReferenceOCCMembers xml file");
    }

    SetTestResult(bTestResult);
    return bTestResult;
}
