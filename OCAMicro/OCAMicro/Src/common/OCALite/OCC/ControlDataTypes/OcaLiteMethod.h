/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMethod
 *
 */

#ifndef OCALITEMETHOD_H
#define OCALITEMETHOD_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteEventSubscriptionDataTypes.h"
#include "OcaLiteMethodID.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of an OCA method, i.e. the unique combination of an ONo and a MethodID.
 * @ingroup EventSubscriptionDataTypes
 */
class OcaLiteMethod : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteMethod();

    /**
     * Constructor.
     *
     * @param[in]   oNo             The object number.
     * @param[in]   methodID        The method ID.
     */
    OcaLiteMethod(::OcaONo oNo, const ::OcaLiteMethodID& methodID);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMethod(const ::OcaLiteMethod& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMethod();

    /**
     * Getter for ONo.
     *
     * @return The object number.
     */
    ::OcaONo GetONo() const
    {
        return m_oNo;
    }

    /**
     * Getter for MethodID.
     *
     * @return The method ID.
     */
    const ::OcaLiteMethodID& GetMethodID() const
    {
        return m_methodID;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMethod& operator=(const ::OcaLiteMethod& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteMethod& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMethod& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The object number. */
    ::OcaONo                m_oNo;

    /** The method ID. */
    ::OcaLiteMethodID           m_methodID;
};

#endif // OCALITEMETHOD_H
