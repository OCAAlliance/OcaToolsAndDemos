/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaManager
 *
 */
#ifndef OCALITEMANAGER_H
#define OCALITEMANAGER_H

// ---- Include system wide include files --
#include <OCC/ControlClasses/OcaLiteRoot.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----
class OcaLiteClassIdentification;

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 * @ingroup Managers
 */
#define OCA_MANAGER_CLASSID            OCA_ROOT_CLASSID,static_cast< ::OcaUint16>(3)


// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for classes that represent non-audio (i.e. control and monitoring) functions.
 * All concrete manager objects are lockable (the constructor of this class initializes the Root object
 * with the property Lockable true).
 * @ingroup Managers
 */
class OcaLiteManager : public ::OcaLiteRoot
{
public:
    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    /**
     * Get the name of the manager.
     *
     * @return The name of the manager.
     */
    const ::OcaLiteString& GetName() const;

    /**
     * Initialize the manager.
     *
     * @return true if succesfully initialized, false otherwise.
     */
    virtual ::OcaBoolean Initialize();

    /**
     * Shutdown the manager.
     *
     * @return true if succesfully shutdown, false otherwise.
     */
    virtual ::OcaBoolean Shutdown();

    // ---- Interface methods ----

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   role                The role of this instance.
     * @param[in]   name                The name of this manager.
     */
    OcaLiteManager(::OcaONo objectNumber, const ::OcaLiteString& role, const ::OcaLiteString& name);

    /**
     * Destructor.
     */
    virtual ~OcaLiteManager() {}

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /**
     * The name of the manager.
     */
    ::OcaLiteString                m_name;

    /** private copy constructor, no copying of object allowed */
    OcaLiteManager(const ::OcaLiteManager&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteManager& operator=(const ::OcaLiteManager&);
};

#endif // OCALITEMANAGER_H
