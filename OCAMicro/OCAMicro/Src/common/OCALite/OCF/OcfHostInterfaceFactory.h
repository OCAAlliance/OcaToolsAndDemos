/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The entry point to the Host Interface.
 *
 */

#ifndef OCFHOSTINTERFACEFACTORY_H
#define OCFHOSTINTERFACEFACTORY_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcfHostInterfaceDataTypes.h"

// ---- Referenced classes and types ----
class IOcfConfigure;
class IOcfTimer;
class IOcfLog;
class IOcfEventSemaphore;
class IOcfMutex;
class IOcfThread;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract class that declares an interface for operations that create Host Interface
 * objects. A concrete factory class that derives from this class needs to be defined
 * and implemented for each platform and/or operating system the Host Interface is used
 * on. This class implements the role of AbstractFactory of the GoF Abstract Factory
 * pattern.
 * @ingroup OCFHostInterface
 */
class OcfHostInterfaceFactory
{
public:
    /**
     * Destructor.
     */
    virtual ~OcfHostInterfaceFactory() {}

    // ---- Interface methods ----
    /**
     * Creates a device configuration object which can be used to configure device
     * data. The invoker of this method is responsible for disposal and destruction
     * of the created device configuration object.
     *
     * @return Pointer to the device configuration object.
     */
    virtual ::IOcfConfigure* CreateConfigurationObject() const = 0;

    /**
     * Creates a Host Interface timer. The invoker of this method is responsible for destruction
     * of the created timer.
     *
     * @return Pointer to the created host interface timer.
     */
    virtual ::IOcfTimer* CreateTimer() const = 0;

    /**
     * Creates a Host Interface logger. The invoker of this method is responsible for destruction
     * of the created logger.
     *
     * @return Pointer to the created host interface logger.
     */
    virtual ::IOcfLog* CreateLogger() const = 0;

    /**
     * Creates a Host Interface event semaphore. The invoker of this method is responsible for
     * destruction of the created event semaphore
     *
     * @return Pointer to the created host interface event semaphore.
     */
    virtual ::IOcfEventSemaphore* CreateEventSemaphore() const = 0;

    /**
     * Creates a Host Interface mutex. The invoker of this method is responsible for
     * destruction of the created mutex
     *
     * @return Pointer to the created host interface mutex.
     */
    virtual ::IOcfMutex* CreateMutex() const = 0;

    /**
     * Creates a Host Interface thread. The invoker of this method is responsible for
     * destruction of the created thread
     *
     * @return Pointer to the created host interface thread.
     */
    virtual ::IOcfThread* CreateThread() const = 0;

protected:
    /**
     * Default constructor. Made protected since this is an abstract
     * class which should not be directly created.
     */
    OcfHostInterfaceFactory() {}

private:
    /** Private copy c'tor to prevent copying. */
    OcfHostInterfaceFactory(const ::OcfHostInterfaceFactory&);

    /** Private assignment operator to prevent assignment. */
    ::OcfHostInterfaceFactory& operator=(const ::OcfHostInterfaceFactory&);
};

#endif // OCFHOSTINTERFACEFACTORY_H
