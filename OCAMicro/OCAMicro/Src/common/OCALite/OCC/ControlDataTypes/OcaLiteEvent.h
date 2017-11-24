/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteEvent
 *
 */

#ifndef OCALITEEVENT_H
#define OCALITEEVENT_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteEventSubscriptionDataTypes.h"
#include "OcaLiteEventID.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of an OCA event, i.e. the unique combination of emitter ONo and the EventID.
 * @ingroup EventSubscriptionDataTypes
 */
class OcaLiteEvent : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteEvent();

    /**
     * Constructor.
     *
     * @param[in]   emitterONo      The object number of the emitter.
     * @param[in]   eventID         The event ID of the subscribed event.
     */
    OcaLiteEvent(::OcaONo emitterONo, const ::OcaLiteEventID& eventID);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteEvent(const ::OcaLiteEvent& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteEvent();

    /**
     * Getter for EmitterONo.
     *
     * @return The object number of the emitter.
     */
    ::OcaONo GetEmitterONo() const
    {
        return m_emitterONo;
    }

    /**
     * Getter for EventID.
     *
     * @return The event ID of the subscribed event.
     */
    const ::OcaLiteEventID& GetEventID() const
    {
        return m_eventID;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteEvent& operator=(const ::OcaLiteEvent& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteEvent& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteEvent& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The object number of the emitter. */
    ::OcaONo                m_emitterONo;

    /** The event ID of the subscribed event. */
    ::OcaLiteEventID            m_eventID;
};

#endif // OCALITEEVENT_H
