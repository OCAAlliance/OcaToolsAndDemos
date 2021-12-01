/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteEventID
 *
 */

#ifndef OCALITEEVENTID_H
#define OCALITEEVENTID_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of an OCA event ID.
 */
class OcaLiteEventID : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteEventID();

    /**
     * Constructor.
     *
     * @param[in]   defLevel        Level in the tree of classes which defines this event (1=root).
     * @param[in]   eventIndex      Index of the event (in the class description).
     */
    OcaLiteEventID(::OcaUint16 defLevel, ::OcaUint16 eventIndex);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteEventID(const ::OcaLiteEventID& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteEventID();

    /**
     * Getter for DefLevel.
     *
     * @return The level in the tree of classes which defines this event (1=root).
     */
    ::OcaUint16 GetDefLevel() const
    {
        return m_defLevel;
    }

    /**
     * Getter for EventIndex.
     *
     * @return The index of the event (in the class description).
     */
    ::OcaUint16 GetEventIndex() const
    {
        return m_eventIndex;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteEventID& operator=(const ::OcaLiteEventID& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteEventID& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteEventID& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The level in the tree of classes which defines this event (1=root). */
    ::OcaUint16     m_defLevel;

    /** The index of the event (in the class description). */
    ::OcaUint16     m_eventIndex;
};

#endif // OCALITEEVENTID_H
