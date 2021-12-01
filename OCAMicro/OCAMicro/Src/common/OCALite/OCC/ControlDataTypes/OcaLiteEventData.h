/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteEventData
 *
 */

#ifndef OCALITEEVENTDATA_H
#define OCALITEEVENTDATA_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteEvent.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Base class for classes containing event data. This base class holds the event data that every
 * event must pass. If an event has additional parameters they must be added in derived classes, and
 * the derived classes must take care of marshaling and unmarshaling the additional data.
 */
class OcaLiteEventData : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteEventData();

    /**
     * Constructor.
     *
     * @param[in]   event           The event that was raised.
     */
    explicit OcaLiteEventData(const ::OcaLiteEvent& event);

    /**
     * Destructor.
     */
    virtual ~OcaLiteEventData();

    /**
     * Getter for Event.
     *
     * @return The event that was raised.
     */
    const ::OcaLiteEvent& GetEvent() const
    {
        return m_event;
    }

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteEventData& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteEventData& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

protected:
    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteEventData(const ::OcaLiteEventData& source);

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteEventData& operator=(const ::OcaLiteEventData& source);

private:
    /** The event that was raised. */
    ::OcaLiteEvent              m_event;
};

#endif // OCALITEEVENTDATA_H
