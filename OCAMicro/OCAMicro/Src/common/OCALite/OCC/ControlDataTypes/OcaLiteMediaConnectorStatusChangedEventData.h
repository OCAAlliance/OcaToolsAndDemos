/*  
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaConnectorStatusChangedEventData
 *
 */

#ifndef OCALITEMEDIACONNECTORSTATUSCHANGEDEVENTDATA_H
#define OCALITEMEDIACONNECTORSTATUSCHANGEDEVENTDATA_H

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- Include local include files ----
#include "OcaLiteEventData.h"
#include "OcaLiteMediaConnectorStatus.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 *
 */
class OcaLiteMediaConnectorStatusChangedEventData : public ::OcaLiteEventData
{
public:

    /** Default constructor */
    OcaLiteMediaConnectorStatusChangedEventData();

    /**
     * Constructor.
     *
     * @param[in]   emitterONo      The object number of the emitter of the property changed event.
     * @param[in]   propertyID      The property ID of the property that has changed.
     * @param[in]   propertyValue   New value of the property. Type of property value parameter
     *                              is defined by template parameter DataType.
     * @param[in]   changeType      Type of change.
     */
    OcaLiteMediaConnectorStatusChangedEventData(const ::OcaLiteEvent& event,
                                                const ::OcaLiteMediaConnectorStatus& connectorStatus);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMediaConnectorStatusChangedEventData(const ::OcaLiteMediaConnectorStatusChangedEventData& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaConnectorStatusChangedEventData();

        /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaConnectorStatusChangedEventData& operator=(const ::OcaLiteMediaConnectorStatusChangedEventData& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteMediaConnectorStatusChangedEventData& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaConnectorStatusChangedEventData& rhs) const;


    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    ::OcaLiteMediaConnectorStatus m_connectorStatus;
};

#endif // OCALITEMEDIACONNECTORSTATUSCHANGEDEVENTDATA_H
