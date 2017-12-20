/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaClockRate
 *
 */

#ifndef OCALITEMEDIACLOCKRATE_H
#define OCALITEMEDIACLOCKRATE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteWorkerDataTypes.h"
#include "OcaLiteTemplateHelpers.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Media clock nominal rate and associated parameters.
 */
class OcaLiteMediaClockRate : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteMediaClockRate();

    /**
     * Constructor.
     *
     * @param[in]   nominalRate     Nominal clock rate, in Hertz.
     * @param[in]   pullRange       Pull range in Hertz. Not all clock types will specify this.
     *                              Use IEEE NaN for unspecified value.
     * @param[in]   accuracy        Accuracy in ppm. Not all clock types will specify this.
     *                              Use IEEE NaN for unspecified value.
     * @param[in]   jitterMax       Maximum jitter in ppm. Not all clock types will specify this.
     *                              Use IEEE NaN for unspecified value.
     */
    OcaLiteMediaClockRate(::OcaFrequency nominalRate,
                      ::OcaFrequency pullRange,
                      ::OcaFloat32 accuracy,
                      ::OcaFloat32 jitterMax);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMediaClockRate(const ::OcaLiteMediaClockRate& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaClockRate();

    /**
     * Getter for NominalRate.
     *
     * @return The nominal clock rate, in Hertz.
     */
    ::OcaFrequency GetNominalRate() const
    {
        return m_nominalRate;
    }

    /**
     * Getter for PullRange.
     *
     * @return The pull range in Hertz, IEEE NaN for unspecified value.
     */
    ::OcaFrequency GetPullRange() const
    {
        return m_pullRange;
    }

    /**
     * Getter for Accuracy.
     *
     * @return The accuracy in ppm, IEEE NaN for unspecified value.
     */
    ::OcaFloat32 GetAccuracy() const
    {
        return m_accuracy;
    }

    /**
     * Getter for JitterMax.
     *
     * @return The maximum jitter in ppm, IEEE NaN for unspecified value.
     */
    ::OcaFloat32 GetJitterMax() const
    {
        return m_jitterMax;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaClockRate& operator=(const ::OcaLiteMediaClockRate& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteMediaClockRate& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaClockRate& rhs) const;

    /**
     * Returns a string that represents the current object.
     *
     * @return A string that represents the current object.
     */
    virtual ::OcaLiteString ToString() const;

    // ---- IOcaMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The nominal clock rate, in Hertz. */
    ::OcaFrequency                  m_nominalRate;

    /** The pull range in Hertz. */
    ::OcaFrequency                  m_pullRange;

    /** The accuracy in ppm. */
    ::OcaFloat32                    m_accuracy;

    /** The maximum jitter in ppm. */
    ::OcaFloat32                    m_jitterMax;
};

// Make sure the class can be referenced across .NET assembly borders
#if defined(__cplusplus_cli) && defined(OCCDATATYPES_BUILD_DLL)
#pragma make_public(::OcaLiteMediaClockRate)
#endif

#endif // OCALITEMEDIACLOCKRATE_H
