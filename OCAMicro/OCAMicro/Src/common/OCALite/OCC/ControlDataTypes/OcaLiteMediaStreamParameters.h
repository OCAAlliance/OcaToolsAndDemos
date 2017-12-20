/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaStreamParameters
 *
 */

#ifndef OCALITEMEDIASTREAMPARAMETERS_H
#define OCALITEMEDIASTREAMPARAMETERS_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <OCALite/OCC/ControlDataTypes/IOcaLiteMarshal.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteBlob.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteString.h>

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Media stream parameters
 */
class OcaLiteMediaStreamParameters : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteMediaStreamParameters();

    /**
     * Constructor.
     *
     * @param[in]   value       The value.
     */
    explicit OcaLiteMediaStreamParameters(const ::OcaLiteBlob& value);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMediaStreamParameters(const ::OcaLiteMediaStreamParameters& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaStreamParameters();

    /**
     * Getter for the value
     *
     * @return The value
     */
    const ::OcaLiteBlob& GetValue() const
    {
        return m_value;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaStreamParameters& operator=(const ::OcaLiteMediaStreamParameters& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteMediaStreamParameters& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaStreamParameters& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

protected:
    /** Set value */
    void SetValue(const ::OcaLiteBlob& value)
    {
        m_value = value;
    }

private:
    /** The value. */
    ::OcaLiteBlob    m_value;
};

#endif // OCALITEMEDIASTREAMPARAMETERS_H
