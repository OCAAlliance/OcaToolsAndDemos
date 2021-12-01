/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMediaConnectorStatus
 *
 */

#ifndef OCALITEMEDIACONNECTORSTATUS_H
#define OCALITEMEDIACONNECTORSTATUS_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteNetworkDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Represents the current status of a media (source or sink) connector.
 */
class OcaLiteMediaConnectorStatus : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteMediaConnectorStatus();

    /**
     * Constructor.
     *
     * @param[in]   connectorId     ID of the connector for which this status is valid.
     * @param[in]   state           Connector state.
     * @param[in]   errorCode       Indicates what type of error the connector is in (only relevant if the State is Fault).
     */
    OcaLiteMediaConnectorStatus(::OcaLiteMediaConnectorID connectorId, ::OcaLiteMediaConnectorState state, ::OcaUint16 errorCode);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMediaConnectorStatus(const ::OcaLiteMediaConnectorStatus& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaConnectorStatus();
   
    /** 
     * Get the ID of the connector for which this status is valid
     *
     * @return the ID of the connector for which this status is valid.
     */
    ::OcaLiteMediaConnectorID GetConnectorID() const
    {
        return m_connectorId;
    }

    /**
     * Get the Connector state.
     *
     * @return the Connector state.
     */
    ::OcaLiteMediaConnectorState GetConnectorState() const
    {
        return m_connectorState;
    }

     /**
      * Get the error in wich the connector is in (only relevant if the State is Fault).
      *
      * @return the error code.
      */
    ::OcaUint16 GetErrorCode() const
    {
        return m_errorCode;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaConnectorStatus& operator=(const ::OcaLiteMediaConnectorStatus& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteMediaConnectorStatus& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaConnectorStatus& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The ID of the media connector. */
    ::OcaLiteMediaConnectorID           m_connectorId;

    /** The state of the media connector. */
    ::OcaLiteMediaConnectorState        m_connectorState;

    /** The error code. */
    ::OcaUint16                         m_errorCode;
};

#endif // OCALITEMEDIACONNECTORSTATUS_H
