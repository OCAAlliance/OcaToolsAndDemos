/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Test Object Compliancy
 */

// ---- Include system wide include files ----
#include <OCALite/Proxy/GeneralProxy.h>
#include <OCALite/OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteMethod.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteManagerDescriptor.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteEvent.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteBlockMember.h>
#include <OCALite/OCP.1/Ocp1LiteNetworkAddress.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "ObjectCompliancyTest.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Function Implementation ----
ObjectCompliancyTest::ObjectCompliancyTest() :
    BaseTestClass("OCC Object Compliancy Tests")
{

}

bool ObjectCompliancyTest::CheckMethods(const BaseTestClass::MethodInfos& methodInfos, const RootMemberInfo& objectInfo, ::OcaONo objectNumber, ::GeneralProxy& proxy)
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
        else
        {
            // Invoke the method at the device.
            ::OcaLiteMethodID methodId(static_cast< ::OcaUint16>(methodIter->GetClassLevel()), static_cast< ::OcaUint16>(methodIter->GetMethodIndex()));
            ::OcaLiteStatus callResult(proxy.OcaCompliancyTestTool_InvalidCall(objectNumber, methodId));

            if ((OCASTATUS_BAD_METHOD != callResult) &&
                (OCASTATUS_BAD_ONO != callResult))
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
                AddTestResult("ERROR: Device must implement method %s (index %d) for object %s (ONo %d, result %d). It may return not implemented",
                    methodIter->GetName().c_str(),
                    methodIter->GetMethodIndex(),
                    objectInfo.GetName().c_str(),
                    objectNumber,
                    callResult);

                // We don't need to break, just set the result to false.
                bTestResult = false;
            }
        }

        ++methodIter;
    }

    return bTestResult;
}

bool ObjectCompliancyTest::CheckEvents(const BaseTestClass::EventInfos& eventInfos, const RootMemberInfo& objectInfo, ::OcaONo objectNumber, ::GeneralProxy& proxy)
{
    // Assume all goes well.
    bool bTestResult(true);

    EventInfos::const_iterator eventIter(eventInfos.begin());
    while (eventIter != eventInfos.end())
    {
        ::OcaLiteStatus callResult(OCASTATUS_PROCESSING_FAILED);

        // If the event is mandatory, the result must be OK.
        if (eventIter->GetIsMandatory())
        {
            // Add the subscription at the device.
            callResult = proxy.OcaSubscriptionManager_AddSubscription(::OcaLiteEvent(objectNumber,
                                                                                     ::OcaLiteEventID(static_cast< ::OcaUint16>(eventIter->GetClassLevel()),
                                                                                                      static_cast< ::OcaUint16>(eventIter->GetEventIndex()))),
                                                                      ::OcaLiteMethod(static_cast< ::OcaONo>(1234),
                                                                                      ::OcaLiteMethodID(static_cast< ::OcaUint16>(1),
                                                                                                        static_cast< ::OcaUint16>(1))),
                                                                      ::OcaLiteBlob(),
                                                                      ::OcaLiteNotificationDeliveryMode::OCANOTIFICATIONDELIVERYMODE_RELIABLE,
                                                                      ::OcaLiteNetworkAddress());
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
        }
        else
        {
            // Add the subscription at the device.
            ::OcaLiteStatus callResult(proxy.OcaSubscriptionManager_AddSubscription(::OcaLiteEvent(objectNumber,
                                                                                                   ::OcaLiteEventID(static_cast<::OcaUint16>(eventIter->GetClassLevel()),
                                                                                                                    static_cast<::OcaUint16>(eventIter->GetEventIndex()))),
                                                                                    ::OcaLiteMethod(static_cast<::OcaONo>(1234),
                                                                                                    ::OcaLiteMethodID(static_cast<::OcaUint16>(1),
                                                                                                                      static_cast<::OcaUint16>(1))),
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
            else if (OCASTATUS_NOT_IMPLEMENTED == callResult)
            {
                AddTestLog("         Device does not implement event %s for object %u (result %d)",
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
                else if (OCASTATUS_NOT_IMPLEMENTED == callResult)
                {
                    AddTestLog("         Device does not implement event %s for object %u (result %d)",
                        eventIter->GetName().c_str(),
                        objectNumber,
                        callResult);
                }
                else
                {
                    // Event should have been present or not implemented.
                    AddTestResult("ERROR: Device must implement event %s (index %d) for object %u (result %d). It may return not implmented.",
                        eventIter->GetName().c_str(),
                        eventIter->GetEventIndex(),
                        objectNumber,
                        callResult);

                    // We don't need to break, just set the result to false.
                    bTestResult = false;
                }
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

        ++eventIter;
    }

    return bTestResult;
}

::OcaLiteStatus ObjectCompliancyTest::GetManagers(::GeneralProxy& proxy, ::OcaLiteList< ::OcaLiteManagerDescriptor>& deviceReportedManagers)
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

::OcaLiteStatus ObjectCompliancyTest::HandleMembers(::OcaLiteList< ::OcaLiteObjectIdentification> members, ::GeneralProxy& proxy, ::OcaLiteList< ::OcaLiteObjectIdentification>& outputMembers)
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

::OcaLiteStatus ObjectCompliancyTest::GetObjects(::GeneralProxy& proxy, ::OcaLiteList< ::OcaLiteObjectIdentification>& outputMembers)
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

bool ObjectCompliancyTest::TestManagers(const ::OcaLiteList< ::OcaLiteManagerDescriptor>& deviceReportedManagers, ::GeneralProxy& proxy)
{
    bool bTestResult(true);

    AddTestResult("   Testing %d manager(s) for test device", deviceReportedManagers.GetCount());

    // We can only test what we know, therefore use that as main for loop (remark this does include base classes!)
    ManagerInfos::iterator iter(m_managerInfos.begin());
    while (iter != m_managerInfos.end())
    {
        // Loop through our reported manager to test against.
        for (::OcaUint16 counter(0); counter < deviceReportedManagers.GetCount(); counter++)
        {
            // Cache it
            ::OcaLiteManagerDescriptor currentDeviceReportedManager(deviceReportedManagers.GetItem(counter));

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
                AddTestLog("Test MANDATORY manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), iter->GetName().c_str());

                if (iter->GetIsDeprecated())
                {
                    AddTestResult("WARNING: manager %s (Name=%s) is deprecated! (Testing ClassID %s, ObjectNumber %d, Class version %d)",
                        iter->GetName().c_str(),
                        currentDeviceReportedManager.GetName().GetString().c_str(),
                        iter->GetClassId().c_str(),
                        currentDeviceReportedManager.GetObjectNumber(),
                        currentDeviceReportedManager.GetClassVersion());
                }

                // Check whether this object has a fixed object number
                if (static_cast< ::OcaONo>(iter->GetObjectNumber()) != OCA_INVALID_ONO)
                {
                    // This object has a fixed object number in the specification, check the object number and class version
                    if ((currentDeviceReportedManager.GetObjectNumber() == iter->GetObjectNumber()) &&
                        (currentDeviceReportedManager.GetClassVersion() >= iter->GetVersion()))
                    {
                        AddTestLog("Attributes of manager %s (Name=%s) verified! ClassID %s, ObjectNumber %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(), 
                            currentDeviceReportedManager.GetName().GetString().c_str(), 
                            iter->GetClassId().c_str(),
                            iter->GetObjectNumber(), currentDeviceReportedManager.GetClassVersion(), 
                            iter->GetVersion());
                    }
                    else
                    {
                        AddTestResult("ERROR: Attributes of manager %s (Name=%s) NOT verified! ClassID %s, ObjectNumber %d -> Expected %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(),
                            currentDeviceReportedManager.GetName().GetString().c_str(),
                            iter->GetClassId().c_str(),
                            currentDeviceReportedManager.GetObjectNumber(),
                            iter->GetObjectNumber(),
                            currentDeviceReportedManager.GetClassVersion(),
                            iter->GetVersion());

                        bTestResult = false;
                    }
                }
                else
                {
                    // We only need to check the class version.
                    if (currentDeviceReportedManager.GetClassVersion() >= iter->GetVersion())
                    {
                        AddTestLog("Attributes of manager %s (Name=%s) verified! ClassID %s, ObjectNumber %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(),
                            currentDeviceReportedManager.GetName().GetString().c_str(),
                            iter->GetClassId().c_str(),
                            iter->GetObjectNumber(),
                            currentDeviceReportedManager.GetClassVersion(),
                            iter->GetVersion());
                    }
                    else
                    {
                        AddTestResult("ERROR: Attributes of manager %s (Name=%s) NOT verified! ClassID %s, ObjectNumber %d -> Expected %d, Class version %d -> expected %d ",
                            iter->GetName().c_str(),
                            currentDeviceReportedManager.GetName().GetString().c_str(),
                            iter->GetClassId().c_str(),
                            currentDeviceReportedManager.GetObjectNumber(),
                            iter->GetObjectNumber(),
                            currentDeviceReportedManager.GetClassVersion(),
                            iter->GetVersion());

                        bTestResult = false;
                    }
                }

                // Test all methods
                if (!CheckMethods(iter->GetMethodInfos(), *iter, currentDeviceReportedManager.GetObjectNumber(), proxy))
                {
                    AddTestResult("ERROR: Failed to test the methods of manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), iter->GetName().c_str());
                    bTestResult = false;
                }

                // Test all events
                if (!CheckEvents(iter->GetEventInfos(), *iter, currentDeviceReportedManager.GetObjectNumber(), proxy))
                {
                    AddTestResult("ERROR: Failed to test the events of manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), iter->GetName().c_str());
                    bTestResult = false;
                }
            }
        }

        ++iter;
    }

    // Also the for OcaRoot compliancy, OcaManager compliancy is already tested.
    for (::OcaUint16 counter(0); counter < deviceReportedManagers.GetCount(); counter++)
    {
        // Cache it
        ::OcaLiteManagerDescriptor currentDeviceReportedManager(deviceReportedManagers.GetItem(counter));

        AddTestLog("Test manager %u for type %s", currentDeviceReportedManager.GetObjectNumber(), m_rootMemberInfos[0].GetName().c_str());

        // Test all methods
        if (!CheckMethods(m_rootMemberInfos[0].GetMethodInfos(), m_rootMemberInfos[0], currentDeviceReportedManager.GetObjectNumber(), proxy))
        {
            AddTestResult("ERROR: Failed to test the methods of manager %u of type %s",
                currentDeviceReportedManager.GetObjectNumber(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }

        // Test all events
        if (!CheckEvents(m_rootMemberInfos[0].GetEventInfos(), m_rootMemberInfos[0], currentDeviceReportedManager.GetObjectNumber(), proxy))
        {
            AddTestResult("ERROR: Failed to test the events of manager %u of type %s",
                currentDeviceReportedManager.GetObjectNumber(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }
    }

    return bTestResult;
}

bool ObjectCompliancyTest::TestObjects(const ::OcaLiteList< ::OcaLiteObjectIdentification>& deviceReportedObjects, ::GeneralProxy& proxy)
{
    bool bTestResult(true);

    AddTestResult("   Testing %d object(s) for test device", deviceReportedObjects.GetCount());

    // We can only test what we know, therefore use that as main for loop (remark this does include base classes!)
    BlockMemberInfos::iterator iter(m_blockMemberInfos.begin());
    while (iter != m_blockMemberInfos.end())
    {
        // Loop through our reported object to test against.
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
                AddTestLog("Test MANDATORY object %u for type %s", currentDeviceReportedObjects.GetONo(), iter->GetName().c_str());

                if (iter->GetIsDeprecated())
                {
                    AddTestResult("WARNING: Object %s (ONo=%d) is deprecated! (ClassID %s. Class version %d)",
                        iter->GetName().c_str(),
                        currentDeviceReportedObjects.GetONo(),
                        iter->GetClassId().c_str(),
                        currentDeviceReportedObjects.GetClassIdentification().GetClassVersion());
                }

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

    // Also the for OcaRoot compliancy
    for (::OcaUint16 counter(0); counter < deviceReportedObjects.GetCount(); counter++)
    {
        // Cache it
        ::OcaLiteObjectIdentification currentDeviceReportedObjects(deviceReportedObjects.GetItem(counter));

        AddTestLog("Test object %u for type %s", currentDeviceReportedObjects.GetONo(), m_rootMemberInfos[0].GetName().c_str());

        // Test all methods
        if (!CheckMethods(m_rootMemberInfos[0].GetMethodInfos(), m_rootMemberInfos[0], currentDeviceReportedObjects.GetONo(), proxy))
        {
            AddTestResult("ERROR: Failed to test the methods of object %u of type %s",
                currentDeviceReportedObjects.GetONo(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }

        // Test all events
        if (!CheckEvents(m_rootMemberInfos[0].GetEventInfos(), m_rootMemberInfos[0], currentDeviceReportedObjects.GetONo(), proxy))
        {
            AddTestResult("ERROR: Failed to test the events of object %u of type %s",
                currentDeviceReportedObjects.GetONo(), m_rootMemberInfos[0].GetName().c_str());
            bTestResult = false;
        }
    }

    return bTestResult;
}

bool ObjectCompliancyTest::ExecuteTest()
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
                    if (TestManagers(deviceReportedManagers, proxy))
                    {
                        AddTestLog("Succesfully tested managers.");
                    }
                    else
                    {
                        AddTestResult("ERROR: Failed to test managers");
                        bTestResult = false;
                    }
                }
                else
                {
                    AddTestResult("ERROR: Failed to get managers");
                    bTestResult = false;
                }

                ::OcaLiteList< ::OcaLiteObjectIdentification> deviceReportedObjects;
                if (GetObjects(proxy, deviceReportedObjects) == OCASTATUS_OK)
                {
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
                ::GeneralProxy proxy(sessionId, ::TestContext::GetInstance().GetOcp1SecureTcpNetworkObjectNumber());

                ::OcaLiteList< ::OcaLiteManagerDescriptor> deviceReportedManagers;
                if (GetManagers(proxy, deviceReportedManagers) == OCASTATUS_OK)
                {
                    if (TestManagers(deviceReportedManagers, proxy))
                    {
                        AddTestLog("Succesfully tested managers.");
                    }
                    else
                    {
                        AddTestResult("ERROR: Failed to test managers");
                        bTestResult = false;
                    }
                }
                else
                {
                    AddTestResult("ERROR: Failed to get managers");
                    bTestResult = false;
                }

                ::OcaLiteList< ::OcaLiteObjectIdentification> deviceReportedObjects;
                if (GetObjects(proxy, deviceReportedObjects) == OCASTATUS_OK)
                {
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
                    if (TestManagers(deviceReportedManagers, proxy))
                    {
                        AddTestLog("Succesfully tested managers.");
                    }
                    else
                    {
                        AddTestResult("ERROR: Failed to test managers");
                        bTestResult = false;
                    }
                }
                else
                {
                    AddTestResult("ERROR: Failed to get managers");
                    bTestResult = false;
                }

                ::OcaLiteList< ::OcaLiteObjectIdentification> deviceReportedObjects;
                if (GetObjects(proxy, deviceReportedObjects) == OCASTATUS_OK)
                {
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
