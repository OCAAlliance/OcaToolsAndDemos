/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
*
*  Description         : The Object compliancy test class.
*/
#ifndef OBJECT_COMPLIANCY_TEST_H
#define OBJECT_COMPLIANCY_TEST_H

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
 * - OCTT must verify the ONo for all objects of the device;
 *   objects with a reserved ONo must have this reserved ONo and objects with a freely allocated ONo must have an 
 *   ONo in the freely allocated range 
 * 
 * - OCTT must for all objects of the device verify the 
 *   implementation of the required OcaRoot methods (as specified in [OCAMIN]) by invoking the methods and 
 *   verifying that they return a valid status
 *
 * - OCTT must for all objects  of the device verify the 
 *   implementation of the required OcaRoot PropertyChanged event by subscribing to the event and verifying
 *   that subscription succeeds 
 *
 * - OCTT must for all objects of the device verify the existence 
 *   of the optional methods documented in [OCC] by invoking the methods and verifying that they return a valid status
 *
 * - OCTT must for all objects with specific event subscriptions of the device verify the implementation of the 
 *   event by subscribing to the event and verifying that subscription succeeds
 *
 */
class ObjectCompliancyTest : public BaseTestClass
{
public:
    /** Constructor */
    ObjectCompliancyTest();
    
    /** Implement interface **/
    virtual bool ExecuteTest();

protected:
    /**
     * Perform checks on the passed methods.
     *
     * @param[in] methodInfos       List of methods from the configuration file.
     * @param[in] rootMemberInfo    Information about the rootmember we have under test (can be a manager or blockmember).
     * @param[in] objectNumber      The object number of the object under test.
     * @param[in] proxy             The proxy to use.
     *
     * @return True if the methods are succesfully tested, false if not.
     */
    bool CheckMethods(const BaseTestClass::MethodInfos& methodInfos,
                      const RootMemberInfo& rootMemberInfo, 
                      ::OcaONo objectNumber, 
                      ::GeneralProxy& proxy);

    /**
     * Perform checks on the passed events.
     *
     * @param[in] eventInfos        List of event from the configuration file.
     * @param[in] rootMemberInfo    Information about the rootmember we have under test (can be a manager or blockmember).
     * @param[in] objectNumber      The object number of the object under test.
     * @param[in] proxy             The proxy to use.
     *
     * @return True if the events are succesfully tested, false if not.
     */
    bool CheckEvents(const BaseTestClass::EventInfos& eventInfos, 
                     const RootMemberInfo& rootMemberInfo, 
                     ::OcaONo objectNumber,
                     ::GeneralProxy& proxy);

    /**
     * Query all managers from the device.
     *
     * @param[in]  proxy                    The proxy to use.
     * @param[out] deviceReportedManagers   Output paramater that holds the available managers. Only valid if result is OCASTATUS_OK
     *
     * @return The result of the operation.
     */
    ::OcaLiteStatus GetManagers(::GeneralProxy& proxy, 
                                ::OcaLiteList< ::OcaLiteManagerDescriptor>& deviceReportedManagers);

    /**
     * Handle all members, when a block is found, query for new members.
     * @note Should only be called by GetObjects.
     *
     * @param[in]  members          The member to look in.
     * @param[in]  proxy            The proxy to use.
     * @param[out] outputMembers    Output paramater that holds the available members. Only valid if result is OCASTATUS_OK
     *
     * @return The result of the operation.
     */
    ::OcaLiteStatus HandleMembers(::OcaLiteList< ::OcaLiteObjectIdentification> members, 
                                  ::GeneralProxy& proxy, 
                                  ::OcaLiteList< ::OcaLiteObjectIdentification>& outputMembers);

    /**
     * Get all objects of the connected device.
     *
     * @param[in] proxy             The proxy to use.
     * @param[in] outputMembers     Output parameter that hold the available members. Only valid if result is OCASTATUS_OK
     *
     * @return The result of the operation.
     */
    ::OcaLiteStatus GetObjects(::GeneralProxy& proxy, 
                               ::OcaLiteList< ::OcaLiteObjectIdentification>& outputMembers);

    /**
     * Test all managers.
     *
     * @param[in] deviceReportedManagers    The managers reported by the device.
     * @param[in] proxy                     The proxy to use.
     *
     * @return True iff all tests went ok, false if not.
     */
    bool TestManagers(const ::OcaLiteList< ::OcaLiteManagerDescriptor>& deviceReportedManagers, 
                      ::GeneralProxy& proxy);

    /**
     * Test all objects.
     *
     * @param[in] deviceReportedObjects    The objects reported by the device.
     * @param[in] proxy                    The proxy to use.
     *
     * @return True iff all tests went ok, false if not.
     */
    bool TestObjects(const ::OcaLiteList< ::OcaLiteObjectIdentification>& deviceReportedObjects, ::GeneralProxy& proxy);

private:

    /** private copy constructor, no copying of object allowed */
    ObjectCompliancyTest(const ::ObjectCompliancyTest&);
    /** private assignment operator, no assignment of object allowed */
    ObjectCompliancyTest& operator=(const ::ObjectCompliancyTest&);
};

#endif //OBJECT_COMPLIANCY_TEST_H