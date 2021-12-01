/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteManagerDescriptor
 *
 */

#ifndef OCALITEMANAGERDESCRIPTOR_H
#define OCALITEMANAGERDESCRIPTOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteClassID.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteManagementDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Description of a manager instance.
 */
class OcaLiteManagerDescriptor : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteManagerDescriptor();

    /**
     * Constructor.
     *
     * @param[in]   objectNumber    The object number of this manager instance.
     * @param[in]   name            The name of the manager instance.
     * @param[in]   classID         The class ID of the class from which the manager instance was created.
     * @param[in]   classVersion    The version number of the class from which the instance was created.
     */
    OcaLiteManagerDescriptor(::OcaONo objectNumber,
                         const ::OcaLiteString& name,
                         const ::OcaLiteClassID& classID,
                         ::OcaClassVersionNumber classVersion);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteManagerDescriptor(const ::OcaLiteManagerDescriptor& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteManagerDescriptor();

    /**
     * Getter for ObjectNumber.
     *
     * @return The object number of this manager instance.
     */
    ::OcaONo GetObjectNumber() const
    {
        return m_objectNumber;
    }

    /**
     * Getter for Name.
     *
     * @return The name of the manager instance.
     */
    const ::OcaLiteString& GetName() const
    {
        return m_name;
    }

    /**
     * Getter for ClassID.
     *
     * @return The class ID of the class from which the manager instance was created.
     */
    const ::OcaLiteClassID& GetClassID() const
    {
        return m_classID;
    }

    /**
     * Getter for ClassVersion.
     *
     * @return The version number of the class from which this instance was created.
     */
    ::OcaClassVersionNumber GetClassVersion() const
    {
        return m_classVersion;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteManagerDescriptor& operator=(const ::OcaLiteManagerDescriptor& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteManagerDescriptor& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteManagerDescriptor& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The object number of this manager instance. */
    ::OcaONo                m_objectNumber;

    /** The name of the manager instance. */
    ::OcaLiteString             m_name;

    /** The clas ID of the class from which the manager instance was created. */
    ::OcaLiteClassID            m_classID;

    /** The version number of the class from which this instance was created. */
    ::OcaClassVersionNumber m_classVersion;
};

#endif // OCALITEMANAGERDESCRIPTOR_H
