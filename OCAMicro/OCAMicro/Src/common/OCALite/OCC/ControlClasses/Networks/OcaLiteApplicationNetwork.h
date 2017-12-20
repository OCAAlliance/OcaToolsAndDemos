/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteApplicationNetwork
 *
 */

#ifndef OCALITEAPPLICATIONNETWORK_H
#define OCALITEAPPLICATIONNETWORK_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>
#include <OCC/ControlDataTypes/OcaLiteApplicationNetworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkSystemInterfaceDescriptor.h>

// ---- Include local include files ----
#include "../OcaLiteRoot.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * The classID used for initialization.
 */
#define OCA_APPLICATION_NETWORK_CLASSID      OCA_ROOT_CLASSID,static_cast< ::OcaUint16>(4)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class from which the application network classes inherit.
 */
class OcaLiteApplicationNetwork : public ::OcaLiteRoot
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetLabel() */
        GET_LABEL               = 1,
        /** SetLabel() */
        SET_LABEL               = 2,
        /** GetOwner() */
        GET_OWNER               = 3,
        /** GetServiceID() */
        GET_SERVICE_ID          = 4,
        /** SetServiceID() */
        SET_SERVICE_ID          = 5,
        /** GetSystemInterfaces() */
        GET_SYSTEM_INTERFACES   = 6,
        /** GetState() */
        GET_STATE               = 7,
        /** GetErrorCode() */
        GET_ERROR_CODE          = 8,
        /** Control() */
        CONTROL                 = 9,
        /** GetPath() */
        GET_PATH                = 10
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Specific label of the network. Can be used to provide human readable information about the network.
            The label can be get and set over any network. */
        OCA_PROP_LABEL              = 1,
        /** Object number of block that contains this network. */
        OCA_PROP_OWNER              = 2,
        /** Name or GUID that this device publishes in the network's directory/discovery system to designate the services
            offered via this application network object. This may or may not be the same as the device's host name, if any.
            For data network types that have host names (e.g. IP networks), the authoritative copy of the host name is in
            the system interface ID. */
        OCA_PROP_SERVICE_ID         = 3,
        /** Collection of identifiers of system interface(s) used by the network. A "system interface" is the system
            service through which network traffic passes into and out of the device -- e.g. a socket. The identifier
            format is system and network dependent; for OCA purposes, it is maintained as a variable-length blob which
            the protocol does not inspect. */
        OCA_PROP_SYSTEM_INTERFACES  = 4,
        /** Operational state of the network. */
        OCA_PROP_STATE              = 5,
        /** Most recent error code. 0 = no error. */
        OCA_PROP_ERROR_CODE         = 6
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteApplicationNetwork();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the network's IDAdvertised.
     *
     * @param[out]   name   The value of the id advertised property
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetServiceID(::OcaLiteApplicationNetworkServiceID& name) const;

    /**
     * Gets the list of system interface IDs that this network is using.
     *
     * @param[out]   interfaces     The value of the system interfaces property
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetSystemInterfaces(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces) const;


    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);


    void SystemInterfaceIDsChanged(const ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces);

    void ServiceIDChanged(const ::OcaLiteApplicationNetworkServiceID& serviceId);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                Read-only text property that describes object's role
     *                                  in the device. Particularly useful for workers, e.g. "Input 1 Gain".
     */
    OcaLiteApplicationNetwork(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the list of system interface IDs that this network is using.
     *
     * @param[in]   interfaces  List of system interfaces that this network will use
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces) const;

    /**
     * Get the value of the service id.
     *
     * @param[in]   interfaces  List of system interfaces that this network will use
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetServiceIDValue(::OcaLiteApplicationNetworkServiceID& nodeId) const;

private:
    /** The current service id. */
    ::OcaLiteApplicationNetworkServiceID m_serviceId;

    /** The sytem interface. */
    ::OcaLiteNetworkSystemInterfaceDescriptor m_interface;

    /** private copy constructor, no copying of object allowed */
    OcaLiteApplicationNetwork(const ::OcaLiteApplicationNetwork&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteApplicationNetwork& operator=(const ::OcaLiteApplicationNetwork&);
};

#endif //OCALITEAPPLICATIONNETWORK_H
