/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteTimePTP
 *
 */

#ifndef OCALITETIMEPTP_H
#define OCALITETIMEPTP_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLitePTPSeconds.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Media clock nominal rate and associated parameters.
 */
class OcaLiteTimePTP : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteTimePTP();

    /**
     * Constructor.
     *
     * @param[in]   negative        TRUE if and only if time value is negative. Absolute times are always positive.
     * @param[in]   seconds         48 bits of seconds.
     * @param[in]   nanoSeconds     32 bits of nano seconds.
     */
    OcaLiteTimePTP(::OcaBoolean negative,
        const ::OcaLitePTPSeconds& seconds,
        ::OcaUint32 nanoSeconds);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteTimePTP(const ::OcaLiteTimePTP& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteTimePTP();

    /**
     * Getter for Negative.
     *
     * @return TRUE if and only if time value is negative. Absolute times are always positive.
     */
    ::OcaBoolean GetNegative() const
    {
        return m_negative;
    }

    /**
     * Getter for Seconds.
     *
     * @return 48 bits of seconds.
     */
    const ::OcaLitePTPSeconds& GetSeconds() const
    {
        return m_seconds;
    }

    /**
     * Getter for NanoSeconds.
     *
     * @return 32 bits of nano seconds.
     */
    ::OcaUint32 GetNanoSeconds() const
    {
        return m_nanoSeconds;
    }

    /**
     * Addition operator.
     * @note The code will assert when the resulting seconds will exceed that of a 48-bit unsigned integer.
     *
     * @param[in]   timePTP     The PTP time to add.
     * @return A reference to this object.
     */
    ::OcaLiteTimePTP& operator+=(const ::OcaLiteTimePTP& timePTP);

    /**
     * Subtraction operator.
     * @note The code will assert when the resulting seconds will be less than 0.
     *
     * @param[in]   timePTP     The PTP time to subtract.
     * @return A reference to this object.
     */
    ::OcaLiteTimePTP& operator-=(const ::OcaLiteTimePTP& timePTP);

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteTimePTP& operator=(const ::OcaLiteTimePTP& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteTimePTP& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteTimePTP& rhs) const;

    /**
     * Creates an OcaTimePTP value based on the date/time components in local time.
     * @note Conversion will give unpredicted results for a date before January 1, 1970.
     *       In a debug build, the code will break on a date prior to January 1, 1970.
     *
     * @param[in]   year            The year in four digit notation.
     * @param[in]   month           The month (1-12).
     * @param[in]   day             The day of the month (1-31).
     * @param[in]   hour            The hour (0-23).
     * @param[in]   minute          The minute (0-59).
     * @param[in]   second          The second (0-59).
     * @param[in]   nanoSeconds     The nano seconds.
     * @return The OcaLiteTimePTP representing the date and time.
     */
    static ::OcaLiteTimePTP CreateTime(::OcaUint16 year, ::OcaUint8 month, ::OcaUint8 day, ::OcaUint8 hour, ::OcaUint8 minute, ::OcaUint8 second, ::OcaUint32 nanoSeconds);

    /**
     * Parses an OcaTimePTP value into the date/time different components in local time.
     *
     * @param[in]   timePTP         The PTP time value to parse.
     * @param[out]  year            The year in four digit notation.
     * @param[out]  month           The month (1-12).
     * @param[out]  day             The day of the month (1-31).
     * @param[out]  hour            The hour (0-23).
     * @param[out]  minute          The minute (0-59).
     * @param[out]  second          The second (0-59).
     * @param[out]  nanoSeconds     The nano seconds.
     * @return True if parsing succeeded, false if not.
     */
    static bool ParseTime(const ::OcaLiteTimePTP& timePTP, ::OcaUint16& year, ::OcaUint8& month, ::OcaUint8& day, ::OcaUint8& hour, ::OcaUint8& minute, ::OcaUint8& second, ::OcaUint32& nanoSeconds);


    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /**
     * Performs the binary addition or subtraction operation.
     *
     * @param[in]   simpleAddition      True if simple addition of the components suffices. This
     *                                  is the case if the sign will not change due to the operation.
     * @param[in]   timePTP             The right hand side of the operation to work with.
     * @return A reference to this object.
     */
    ::OcaLiteTimePTP& PerformBinaryOperation(bool simpleAddition, const ::OcaLiteTimePTP& timePTP);

    /** TRUE if and only if time value is negative. Absolute times are always positive. */
    ::OcaBoolean                    m_negative;

    /** 48 bits of seconds. */
    ::OcaLitePTPSeconds             m_seconds;

    /** 32 bits of nano seconds. */
    ::OcaUint32                     m_nanoSeconds;
};

// Make sure the class can be referenced across .NET assembly borders
#if defined(__cplusplus_cli) && defined(OCCDATATYPES_BUILD_DLL)
#pragma make_public(::OcaLiteTimePTP)
#endif

#endif // OCALITETIMEPTP_H
