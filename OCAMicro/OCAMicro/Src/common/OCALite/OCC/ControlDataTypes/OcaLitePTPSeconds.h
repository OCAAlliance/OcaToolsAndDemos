/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLitePTPSeconds
 *
 */

#ifndef OCALITEPTPSECONDS_H
#define OCALITEPTPSECONDS_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/** The maximum value of the PTP seconds, defined as the maximum value of a 48-bit unsigned integer. */
#define MAX_PTP_SECONDS_VALUE   static_cast< ::OcaUint64>(0x0000FFFFFFFFFFFFULL)

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * PTP seconds. Defined as a 64 bit unsigned integer for ease of programming;
 * however, the value shall not exceed that of a 48-bit unsigned integer.
 * @ingroup ClockingDataTypes
 */
class OcaLitePTPSeconds : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLitePTPSeconds();

    /**
     * Constructor.
     * @note The code will assert when the value exceeds that of a 48-bit unsigned integer.
     *
     * @param[in]   seconds         The value of the seconds.
     */
    explicit OcaLitePTPSeconds(::OcaUint64 seconds);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLitePTPSeconds(const ::OcaLitePTPSeconds& source);

    /**
     * Destructor.
     */
    virtual ~OcaLitePTPSeconds();

    /**
     * Getter for Seconds.
     *
     * @return The value of the seconds.
     */
    ::OcaUint64 GetSeconds() const
    {
        return m_seconds;
    }

    /**
     * Cast operator to cast to an OcaUint64 with the seconds value.
     *
     * @return An OcaUint64 with the seconds value.
     */
    operator ::OcaUint64() const;

    /**
     * Addition operator.
     * @note The code will assert when the result will exceed that of a 48-bit unsigned integer.
     *
     * @param[in]   seconds     The seconds to add.
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator+=(const ::OcaLitePTPSeconds& seconds);

    /**
     * Addition operator.
     * @note The code will assert when the result will exceed that of a 48-bit unsigned integer.
     *
     * @param[in]   seconds     The seconds to add.
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator+=(::OcaUint64 seconds);

    /**
     * Pre-increment operator.
     * @note The code will assert when the result will exceed that of a 48-bit unsigned integer.
     *
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator++();

    /**
     * Subtraction operator.
     * @note The code will assert when the result will be less than 0.
     *
     * @param[in]   seconds     The seconds to subtract.
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator-=(const ::OcaLitePTPSeconds& seconds);

    /**
     * Subtraction operator.
     * @note The code will assert when the result will be less than 0.
     *
     * @param[in]   seconds     The seconds to subtract.
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator-=(::OcaUint64 seconds);

    /**
     * Pre-decrement operator.
     * @note The code will assert when the result will be less than 0.
     *
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator--();

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator=(const ::OcaLitePTPSeconds& source);

    /**
     * Assignment operator.
     * @note The code will assert when the value exceeds that of a 48-bit unsigned integer.
     *
     * @param[in]   seconds     The new value of the seconds.
     * @return A reference to this object.
     */
    ::OcaLitePTPSeconds& operator=(::OcaUint64 seconds);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLitePTPSeconds& rhs) const;

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(::OcaUint64 rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLitePTPSeconds& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(::OcaUint64 rhs) const;

    /**
     * The less-than operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is less than rhs.
     */
    bool operator<(const ::OcaLitePTPSeconds& rhs) const;

    /**
     * The less-than operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is less than rhs.
     */
    bool operator<(::OcaUint64 rhs) const;

    /**
     * The less-than-or-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is less than or equal to rhs.
     */
    bool operator<=(const ::OcaLitePTPSeconds& rhs) const;

    /**
     * The less-than-or-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is less than or equal to rhs.
     */
    bool operator<=(::OcaUint64 rhs) const;

    /**
     * The greater-than operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is greater than rhs.
     */
    bool operator>(const ::OcaLitePTPSeconds& rhs) const;

    /**
     * The greater-than operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is greater than rhs.
     */
    bool operator>(::OcaUint64 rhs) const;

    /**
     * The greater-than-or-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is greater than or equal to rhs.
     */
    bool operator>=(const ::OcaLitePTPSeconds& rhs) const;

    /**
     * The greater-than-or-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is greater than or equal to rhs.
     */
    bool operator>=(::OcaUint64 rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The value of the seconds. */
    ::OcaUint64                     m_seconds;
};

// ---- Additional operators ----

/**
 * Addition operator.
 * @note The code will assert when the result will exceed that of a 48-bit unsigned integer.
 *
 * @param[in]   lhs     The left hand side of the operator.
 * @param[in]   rhs     The right hand side of the operator.
 * @return The value after the application of the operation.
 * @ingroup DataTypes
 */
::OcaLitePTPSeconds operator+(::OcaLitePTPSeconds lhs, const ::OcaLitePTPSeconds& rhs);

/**
 * Addition operator.
 * @note The code will assert when the result will exceed that of a 48-bit unsigned integer.
 *
 * @param[in]   lhs     The left hand side of the operator.
 * @param[in]   rhs     The right hand side of the operator.
 * @return The value after the application of the operation.
 * @ingroup DataTypes
 */
::OcaLitePTPSeconds operator+(::OcaLitePTPSeconds lhs, ::OcaUint64 rhs);

/**
 * Subtraction operator.
 * @note The code will assert when the result will be less than 0.
 *
 * @param[in]   lhs     The left hand side of the operator.
 * @param[in]   rhs     The right hand side of the operator.
 * @return The value after the application of the operation.
 * @ingroup DataTypes
 */
::OcaLitePTPSeconds operator-(::OcaLitePTPSeconds lhs, const ::OcaLitePTPSeconds& rhs);

/**
 * Subtraction operator.
 * @note The code will assert when the result will be less than 0.
 *
 * @param[in]   lhs     The left hand side of the operator.
 * @param[in]   rhs     The right hand side of the operator.
 * @return The value after the application of the operation.
 * @ingroup DataTypes
 */
::OcaLitePTPSeconds operator-(::OcaLitePTPSeconds lhs, ::OcaUint64 rhs);

// Make sure the class can be referenced across .NET assembly borders
#if defined(__cplusplus_cli) && defined(OCCDATATYPES_BUILD_DLL)
#pragma make_public(::OcaLitePTPSeconds)
#endif

#endif // OcaLitePTPSeconds_H
