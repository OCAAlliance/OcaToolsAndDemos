/*  
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaSinkConnectorChangedEventData
 *
 */

#ifndef OCALITEMEDIASINKCONNECTORCHANGEDEVENTDATA_H
#define OCALITEMEDIASINKCONNECTORCHANGEDEVENTDATA_H

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- Include local include files ----
#include "OcaLiteEventData.h"
#include "OcaLiteMediaSinkConnector.h"
#include "OcaLiteMediaConnectorStatus.h"
#include "OcaLitePropertyChangedEventData.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Event data raised whenever a sink connector is changed.
 */
class OcaLiteMediaSinkConnectorChangedEventData : public ::OcaLiteEventData
{
public:
    /** Constructor */
    OcaLiteMediaSinkConnectorChangedEventData();

    /**
     * Constructor.
     *
     * @param[in]   event           The event which occured.
     * @param[in]   sinkConnector   The sink connector.
     * @param[in]   changeType      The changed type.
     * @param[in]   changedElement  The elements which are changed in the connector.
     */
    OcaLiteMediaSinkConnectorChangedEventData(const ::OcaLiteEvent& event,
                                              const ::OcaLiteMediaSinkConnector& sinkConnector,
                                              ::OcaLitePropertyChangeType changeType,
                                              ::OcaLiteMediaConnectorElement changedElement);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMediaSinkConnectorChangedEventData(const ::OcaLiteMediaSinkConnectorChangedEventData& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaSinkConnectorChangedEventData();

        /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaSinkConnectorChangedEventData& operator=(const ::OcaLiteMediaSinkConnectorChangedEventData& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteMediaSinkConnectorChangedEventData& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaSinkConnectorChangedEventData& rhs) const;


    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The media sink connector for which the changed event holds (i.e. that is added, deleted or changed). */
    ::OcaLiteMediaSinkConnector m_sinkConnector;
    /** Indicates what type of change occurred. Only ItemAdded, ItemChanged and ItemDeleted can be used in this event data. */
    ::OcaLitePropertyChangeType m_changeType;
    /** Indicates which element(s) of the connector changed. */
    ::OcaLiteMediaConnectorElement m_changedElement;
};

#endif // OCALITEMEDIASINKCONNECTORCHANGEDEVENTDATA_H
