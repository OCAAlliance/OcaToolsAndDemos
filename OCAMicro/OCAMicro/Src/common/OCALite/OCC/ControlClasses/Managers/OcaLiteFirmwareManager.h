/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaFirmwareManager
 *
 */
#ifndef OCALITEFIRMWAREMANAGER_H
#define OCALITEFIRMWAREMANAGER_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteManagementDataTypes.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>

// ---- Include local include files ----
#include "OcaLiteManager.h"

// ---- Referenced classes and types ----
class OcaLiteBlob;
class OcaLiteNetworkAddress;
class OcaLiteVersion;

// ---- Helper types and constants ----

/**
 * The classID used for initialization.
 * @ingroup Managers
 */
#define OCA_FIRMWAREMANAGER_CLASSID      OCA_MANAGER_CLASSID,static_cast< ::OcaUint16>(3)

 // ---- Helper functions ----

// ---- Class Definition ----
/**
 * Mandatory class that manages versions of the different firmware and software images of the device.
 * @ingroup Managers
 */
class OcaLiteFirmwareManager : public ::OcaLiteManager
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetComponentVersions() */
        GET_COMPONENT_VERSIONS          = 1,
        /** StartUpdateProcess() */
        START_UPDATE_PROCESS            = 2,
        /** BeginActiveImageUpdate() */
        BEGIN_ACTIVE_IMAGE_UPDATE       = 3,
        /** AddImageData() */
        ADD_IMAGE_DATA                  = 4,
        /** VerifyImage() */
        VERIFY_IMAGE                    = 5,
        /** EndActiveImageUpdate() */
        END_ACTIVE_IMAGE_UPDATE         = 6,
        /** BeginPassiveComponentUpdate() */
        BEGIN_PASSIVE_COMPONENT_UPDATE  = 7,
        /** EndUpdateProcess() */
        END_UPDATE_PROCESS              = 8
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** List of the versions of the components of the device. */
        OCA_PROP_COMPONENT_VERSIONS     = 1
    };

    /**
     * The object number of the firmware manager
     */
    static const ::OcaONo OBJECT_NUMBER;

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    /**
     * Getter for the OcaLiteFirmwareManager singleton.
     *
     * @return Reference to the OcaLiteFirmwareManager.
     */
    static ::OcaLiteFirmwareManager& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    // ---- Interface methods ----
    /**
     * Gets the value of the ComponentVersions property.
     *
     * @param[out] componentVersions    Output parameter that holds the value of
     *                                  the ComponentVersions if the method succeeds.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetComponentVersions(::OcaLiteList< ::OcaLiteVersion>& componentVersions) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /** Constructor */
    OcaLiteFirmwareManager();

    /** Destructor. */
    virtual ~OcaLiteFirmwareManager();

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:

    /** Singleton instance of the server */
    static ::OcaLiteFirmwareManager*          m_pSingleton;

    /** private copy constructor, no copying of object allowed */
    OcaLiteFirmwareManager(const ::OcaLiteFirmwareManager&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteFirmwareManager& operator=(const ::OcaLiteFirmwareManager&);
};

#endif // OCALITEFIRMWAREMANAGER_H
