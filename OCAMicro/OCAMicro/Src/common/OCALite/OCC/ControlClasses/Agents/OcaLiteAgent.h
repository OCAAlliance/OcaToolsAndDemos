/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaAgent
 *
 */
#ifndef OCALITEAGENT_H
#define OCALITEAGENT_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/OcaLiteRoot.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----
class OcaLiteBlock;
class OcaLiteClassIdentification;

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 * @ingroup Agents
 */
#define OCA_AGENT_CLASSID               OCA_ROOT_CLASSID,static_cast< ::OcaUint16>(2)


// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for defining agents.
 * @ingroup Agents
 */
class OcaLiteAgent : public ::OcaLiteRoot
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetLabel() */
        GET_LABEL               = 1,
        /** SetLabel() */
        SET_LABEL               = 2,
        /** GetOwner() */
        GET_OWNER               = 3,
        /** GetPath() */
        GET_PATH                = 4
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** Property that identifies the label. */
        OCA_PROP_LABEL          = 1,
        /** Read-only property that indentifies the owner. */
        OCA_PROP_OWNER          = 2
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteAgent();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----
    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                The role of this instance.
     */
    OcaLiteAgent(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /** private copy constructor, no copying of object allowed */
    OcaLiteAgent(const ::OcaLiteAgent&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteAgent& operator=(const ::OcaLiteAgent&);
};

#endif // OCALITEAGENT_H
