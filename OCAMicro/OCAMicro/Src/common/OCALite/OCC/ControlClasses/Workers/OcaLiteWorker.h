/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaWorker
 *
 */
#ifndef OCALITEWORKER_H
#define OCALITEWORKER_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLitePort.h>

// ---- Include local include files ----
#include "../OcaLiteRoot.h"

// ---- Referenced classes and types ----
class OcaLiteBlock;

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_WORKER_CLASSID      OCA_ROOT_CLASSID,static_cast< ::OcaUint16>(1)

// ---- Helper functions ----

/** Frequency value in Hertz. */
typedef ::OcaFloat32   OcaLiteFrequency;

// ---- Class Definition ----
/**
 * Abstract base class for classes that represent the device's application and support functions.
 */
class OcaLiteWorker : public ::OcaLiteRoot
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetEnabled() */
        GET_ENABLED         = 1,
        /** SetEnabled() */
        SET_ENABLED         = 2,
        /** AddPort() */
        ADD_PORT            = 3,
        /** DeletePort() */
        DELETE_PORT         = 4,
        /** GetPorts() */
        GET_PORTS           = 5,
        /** GetPortName() */
        GET_PORT_NAME       = 6,
        /** SetPortName() */
        SET_PORT_NAME       = 7,
        /** GetLabel() */
        GET_LABEL           = 8,
        /** SetLabel() */
        SET_LABEL           = 9,
        /** GetOwner() */
        GET_OWNER           = 10,
        /** GetLatency() */
        GET_LATENCY         = 11,
        /** SetLatency() */
        SET_LATENCY         = 12,
        /** GetPath() */
        GET_PATH            = 13
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Read/write property that indicates whether the worker object is enabled in the device. If an object
            is disabled it cannot be used by the application. Note that the behavior of a disabled object
            depends on the object itself (e.g. a disabled chime generator is silent, a disabled equalizer is flat,
            etc.). */
        OCA_PROP_ENABLED        = 1,
        /** The list of (input and output) OCA ports the worker object has. This read-only property must be
            initialized at object creation. Note that a worker object can have no ports (in which case the list is
            empty). */
        OCA_PROP_PORTS          = 2,
        /** Specific label of the worker. Can be used to provide human readable information about the
            worker. The label can be get and set over the network. */
        OCA_PROP_LABEL          = 3,
        /** Object number of block that contains this worker. */
        OCA_PROP_OWNER          = 4,
        /** Processing latency of this object. Optional. Readonly or readwrite, depending on
            implementation. */
        OCA_PROP_LATENCY        = 5
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteWorker();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets if the worker object is enabled
     *
     * @param[out] enabled  Indicates if enabled
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetEnabled(::OcaBoolean& enabled) const;

    /**
     * Set the worker object to enabled or disabled
     *
     * @param[in]  bEnabled     Input parameter that holds the value of
     *                          the enabled property
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus SetEnabled(::OcaBoolean bEnabled);

    /**
     * Gets the OcaPorts
     *
     * @param[out]  ports       Output parameter that holds the value of
     *                          the OcaPorts
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetPorts(::OcaLiteList< ::OcaLitePort>& ports) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                Read-only text property that describes object's role
     *                                  in the device. Particularly useful for workers, e.g. "Input 1 Gain".
     * @param[in]   ports               The list of (input and output) OCA ports the worker object
     *                                  has. Note that a worker object can have no ports, in which case
     *                                  the list is empty.
     */
    OcaLiteWorker(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets if the worker object is enabled. Can be overridden by derived classes
     * that actually implement the worker.
     *
     * @param[out] enabled  Indicates if enabled
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetEnabledValue(::OcaBoolean& enabled) const;

    /**
     * Set the worker object to enabled or disabled. Can be overridden by derived classes
     * that actually implement the worker. Usually setters must be overridden,
     * but as this enabled property is actually an optional property a
     * default implementation is given where "Not implemented" is returned.
     *
     * @param[in]  bEnabled     Input parameter that holds the value of
     *                          the enabled property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetEnabledValue(::OcaBoolean bEnabled);

 private:

    /**
     * Read/write property that indicates whether the worker object is enabled in the device. If an object
     * is disabled it cannot be used by the application.
     * @note The behavior of a disabled object depends on the object itself (e.g. a disabled chime generator
     * is silent, a disabled equalizer is flat, etc.).
     */
    ::OcaBoolean                            m_bEnabled;

    /**
     * The list of (input and output) OCA ports the worker object has.
     * @note A worker object can have no ports (in which case the list is empty).
     */
    ::OcaLiteList< ::OcaLitePort>                   m_ocaPorts;

    /** private copy constructor, no copying of object allowed */
    OcaLiteWorker(const ::OcaLiteWorker&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteWorker& operator=(const ::OcaLiteWorker&);
};

#endif // OCALITEWORKER_H
