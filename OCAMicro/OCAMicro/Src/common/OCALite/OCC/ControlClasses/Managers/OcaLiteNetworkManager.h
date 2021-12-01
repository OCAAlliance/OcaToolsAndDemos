/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteNetworkManager
 *
 */
#ifndef OCALITENETWORKMANAGER_H
#define OCALITENETWORKMANAGER_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBlobFixedLen.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlClasses/Agents/OcaLiteNetwork.h>

// ---- Include local include files ----
#include "OcaLiteDeviceManager.h"

// ---- Referenced classes and types ----
class OcaLiteNetwork;
class OcaLiteMediaTransportNetwork;

// ---- Helper types and constants ----

/**
 * The classID used for initialization.
 */
#define OCA_NETWORKMANAGER_CLASSID      OCA_MANAGER_CLASSID,static_cast< ::OcaUint16>(6)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Mandatory manager for all media transport and control networks to which the device belongs.
 */
class OcaLiteNetworkManager : public ::OcaLiteManager
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetNetworks() */
        GET_NETWORKS                    = 1,
        /** GetStreamNetworks() */
        GET_STREAM_NETWORKS             = 2,
        /** GetControlNetworks() */
        GET_CONTROL_NETWORKS            = 3,
        /** GetMediaTransportNetworks() */
        GET_MEDIA_TRANSPORT_NETWORKS    = 4
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Object numbers of OcaLiteNetwork objects, one for each network to which this device belongs. */
        OCA_PROP_NETWORKS           = 1,
        /** Object numbers of OcaStreamNetwork objects, one for each stream network to which this device belongs. */
        OCA_PROP_STREAM_NETWORKS    = 2,
        /** Object numbers of OcaControlNetwork objects, one for each control network to which this device belongs. */
        OCA_PROP_CONTROL_NETWORKS = 3,
        /** Object numbers of OcaMediaTransportNetwork objects, one for each media transport network to which this device belongs. */
        OCA_PROP_MEDIA_TRANSPORT_NETWORKS = 4
    };

    /**
     * The object number of the network manager
     */
    static const ::OcaONo OBJECT_NUMBER;

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    /**
     * Getter for the OcaLiteNetworkManager singleton.
     *
     * @return Reference to the OcaLiteNetworkManager.
     */
    static ::OcaLiteNetworkManager& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    // ---- Interface methods ----
    /**
     * Gets the value of the networks property.
     *
     * @param[out] networks     Output parameter that holds the value of
     *                          network list
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetNetworks(::OcaLiteList< ::OcaONo>& networks) const;

    /**
     * Gets the value of the stream networks property.
     *
     * @param[out] networks     Output parameter that holds the value of
     *                          network list
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetStreamNetworks(::OcaLiteList< ::OcaONo>& networks) const;

    /**
     * Gets the value of the media transport networks property.
     *
     * @param[out] networks     Output parameter that holds the value of
     *                          media transport network list
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetMediaTransportNetworks(::OcaLiteList< ::OcaONo>& networks) const;

    // ---- Miscellaneous methods ----
    /**
     * Gets the pointer of the OcaLiteNetwork by object number
     *
     * @param[in] networkONo     Object number of the network
     *
     * @return A valid pointer when the network is found, otherwise NULL.
     */
    ::OcaLiteNetwork* GetNetwork(::OcaONo networkONo) const;

    /**
     * Adds a network to the network object list.
     * Only for internal use.
     *
     * @param[in] network   The network to add
     *
     * @return Indicates whether the network object number was added successfully; if adding
     *         the network object number failed or the object number was already in use.
     */
    ::OcaBoolean AddNetwork(const ::OcaLiteAgent& network);

    /**
     * Deletes the network object number.
     * Only for internal use.
     *
     * @param[in] network   The network that is removed
     */
    void RemoveNetwork(const ::OcaLiteAgent& network);

    /**
     * Adds a network to the network object list.
     * Only for internal use.
     *
     * @param[in] network   The network to add
     *
     * @return Indicates whether the network object number was added successfully; if adding
     *         the network object number failed or the object number was already in use.
     */
    ::OcaBoolean AddStreamNetwork(const ::OcaLiteAgent& network);

    /**
     * Deletes the network object number.
     * Only for internal use.
     *
     * @param[in] network   The network that is removed
     */
    void RemoveStreamNetwork(const ::OcaLiteAgent& network);

    /**
     * Adds a network to the network object list.
     * Only for internal use.
     *
     * @param[in] network   The network to add
     *
     * @return Indicates whether the network object number was added successfully; if adding
     *         the network object number failed or the object number was already in use.
     */
    ::OcaBoolean AddMediaTransportNetwork(const ::OcaLiteMediaTransportNetwork& network);

    /**
     * Deletes the network object number.
     * Only for internal use.
     *
     * @param[in] network   The network that is removed
     */
    void RemoveMediaTransportNetwork(const ::OcaLiteMediaTransportNetwork& network);


    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     */
    OcaLiteNetworkManager();

    /**
     * Destructor.
     */
    virtual ~OcaLiteNetworkManager();

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /** Singleton instance of the server */
    static ::OcaLiteNetworkManager*             m_pSingleton;
    /** Object numbers of the networks */
    ::OcaLiteList< ::OcaONo>                    m_networks;
    /** Object number of the one and only stream network */
    ::OcaONo                                    m_streamNetwork;
    /** Object number of the one and only media transport network */
    ::OcaONo                                    m_mediaTransportNetwork;
};

#endif // OCALITENETWORKMANAGER_H
