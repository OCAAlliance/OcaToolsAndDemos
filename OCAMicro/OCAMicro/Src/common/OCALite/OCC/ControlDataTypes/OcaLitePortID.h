/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLitePortID
 *
 */

#ifndef OCALITEPORTID_H
#define OCALITEPORTID_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteString.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * Enumeration that describes whether a port is for input or output.
 * @ingroup BlockMatrixDataTypes
 */
//lint -strong(AJX, OcaPortMode)
enum OcaPortMode    /* maps onto OcaUint8 */
{
    /** Input port. */
    OCAPORTMODE_INPUT               = 1,
    /** Output port. */
    OCAPORTMODE_OUTPUT              = 2,
    /** Maximum value used for range checking */
    OCAPORTMODE_MAXIMUM
};

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Unique identifier of input or output port within a given worker or block class. Port
 * numbers are ordinals starting at 1, and there are separate numbering spaces for input
 * and output ports.
 * @ingroup BlockMatrixDataTypes
 */
class OcaLitePortID : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLitePortID();

    /**
     * Constructor.
     *
     * @param[in]   mode    Indicates whether the port is for input or output.
     * @param[in]   idx     Index of the port within the given input or output set of the specified object.
     *                      This value is 1-based.
     */
    OcaLitePortID(::OcaPortMode mode, ::OcaUint16 idx);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLitePortID(const ::OcaLitePortID& source);

    /**
     * Destructor.
     */
    virtual ~OcaLitePortID();

    /**
     * Getter for Mode.
     *
     * @return Indicates whether the port is for input or output.
     */
    ::OcaPortMode GetMode() const
    {
        return m_mode;
    }

    /**
     * Getter for Index.
     *
     * @return The index of the port within the given input or output set of the specified object.
     *         This value is 1-based.
     */
    ::OcaUint16 GetIndex() const
    {
        return m_index;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLitePortID& operator=(const ::OcaLitePortID& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLitePortID& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLitePortID& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** Indicates whether the port is for input or output. */
    ::OcaPortMode                   m_mode;

    /** The index of the port within the given input or output set of the specified object. */
    ::OcaUint16                     m_index;
};

// ---- Specialized Template Function Definition ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaPortMode>(const ::OcaPortMode& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaPortMode>(::OcaPortMode& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaPortMode>(const ::OcaPortMode& value, const ::IOcaLiteWriter& writer);

//lint -restore

#endif // OCALITEPORTID_H
