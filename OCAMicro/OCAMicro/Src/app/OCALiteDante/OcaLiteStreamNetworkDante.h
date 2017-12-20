/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteStreamNetwork
 *
 */
#ifndef OCALITESTREAMNETWORKDANTE_H
#define OCALITESTREAMNETWORKDANTE_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Agents/OcaLiteStreamNetwork.h>
#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_STREAMNETWORKDANTE_CLASSID OCA_STREAMNETWORK_CLASSID,(OCA_CLASS_ID_PROPRIETARY_CLASS_FIELD_MASK | static_cast< ::OcaUint16>(0x01))

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for defining stream network.
 */
class OcaLiteStreamNetworkDante : public ::OcaLiteStreamNetwork
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetMaxRxLatency() */
        GET_MAX_RX_LATENCY              = 1,
        /** GetRxLatency() */
        GET_RX_LATENCY                  = 2,
        /** SetRxLatency() */
        SET_RX_LATENCY                  = 3,
        /** CreateMultiCastSourceFlow() */
        CREATE_MULTICAST_SOURCE_FLOW    = 4,
        /** DeleteFlow() */
        DELETE_FLOW                     = 5
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {   
        /** Maximum rx latency */
        OCA_PROP_MAX_RX_LATENCY = 1,
        /** Rx latency */
        OCA_PROP_RX_LATENCY     = 2
    };

    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   idAdvertised            Default name by which this device
     *                                      advertises itself on this network.
     */
    OcaLiteStreamNetworkDante(::OcaONo objectNumber,
                              ::OcaBoolean lockable,
                              const ::OcaLiteString& role,
                              const ::OcaLiteNetworkDanteNodeID& idAdvertised);

    /**
     * Destructor.
     */
    virtual ~OcaLiteStreamNetworkDante();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----
    /**
     * Initialize the signal channels.
     *
     * @param[in]   startONo    The first available object number
     * @param[out]  endONo      The first available object number after initialization
     * @return True if the initialization succeeded, false otherwise.
     */
    virtual ::OcaBoolean InitSignalChannels(::OcaONo startONo, ::OcaONo& endONo);

    // ---- Interface methods ----
    /**
     * Check a channel subscription for a change.
     *
     * @param[in]   Channel                The channel number to check
     * @param[out]  NewSubscription     The channel [potentially] new subscription
     * @return True if the initialization succeeded, false otherwise.
     */
    virtual ::OcaBoolean CheckforNewSubscription(UINT32 Channel, const char *NewSubscription);
    /**
     * Teardown this network.
     */
    virtual void Teardown();

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);


protected:

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /** private copy constructor, no copying of object allowed */
    OcaLiteStreamNetworkDante(const ::OcaLiteStreamNetworkDante&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteStreamNetworkDante& operator=(const ::OcaLiteStreamNetworkDante);

    ::OcaONo m_RxChannelsStartONo;
    ::OcaONo m_RxChannelsEndONo;
    ::OcaONo m_TxChannelsStartONo;
    ::OcaONo m_TxChannelsEndONo;
    ::OcaBoolean m_Initialized;
};

#endif // OCALITESTREAMNETWORKDANTE_H
