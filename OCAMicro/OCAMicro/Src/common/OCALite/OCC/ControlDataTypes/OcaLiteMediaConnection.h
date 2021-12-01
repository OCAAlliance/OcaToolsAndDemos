/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
*
*  Description         : OcaLiteMediaConnection
*
*/

#ifndef OCALITEMEDIACONNECTION_H
#define OCALITEMEDIACONNECTION_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteMediaStreamParameters.h"
#include "OcaLiteNetworkDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----


/**
 * A single-channel or multichannel connection between a local media connector (i.e. OcaMedia(Source/Sink)Connector instance) of 
 * an OcaMediaTransportNetwork object in this node and another ("remote") media source or sink.  Normally, the remote source or sink is in another node. 
 * The remote end may or may not be an OCA-compliant device.
 *
 * A connection is unidirectional. Its direction is determined by the connector that owns the connection.   Its direction is either:
 *  - Outbound:  A signal flow from a source connector to an external destination; OR
 *  - Inbound:  A signal flow from an external source to a sink connector.
 *
 * An OcaMediaConnection object may represent a connection to either a unicast or a multicast stream.
 * Any given OcaMedia(Source/Sink)Connector object will only have one media connection.
 * In non-OCA documents, connections are sometimes referred to as streams or flows.
 */
class OcaLiteMediaConnection : public ::IOcaLiteMarshal
{
public:
    /** Default constructor */
    OcaLiteMediaConnection();
    
    /*
     * Constructor 
     * @param[in] secure            True iff connection is secure.
     * @param[in] streamParameters  Stream parameters(encoding, sampling, etc). Details TBD
     * @param[in] streamCastMode    Unicast or multicast
     */
    OcaLiteMediaConnection(::OcaBoolean secure, 
                           ::OcaLiteMediaStreamParameters streamParameters, 
                           ::OcaLiteMediaStreamCastMode streamCastMode);

    /**
     * Copy constructor
     */
    OcaLiteMediaConnection(const ::OcaLiteMediaConnection& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaConnection();

    /**
     * Get the secure state.
     *
     * @return True iff connection is secure.
     */
    ::OcaBoolean GetSecure() const
    {
        return m_secure;
    }

    /**
     * Get the stream parameters.
     *
     * @return The stream parameters.
     */
    const ::OcaLiteMediaStreamParameters& GetStreamParameters() const
    {
        return m_streamParameters;
    }

    /**
     * Get the stream case mode.
     *
     * @return Unicast or multicast.
     */
    ::OcaLiteMediaStreamCastMode GetStreamCastMode() const
    {
        return m_streamCastMode;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaConnection& operator=(const ::OcaLiteMediaConnection& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteMediaConnection& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaConnection& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** True iff connection is secure. */
    ::OcaBoolean m_secure;
    /** Stream parameters(encoding, sampling, etc).  */
    ::OcaLiteMediaStreamParameters m_streamParameters;
    /** Unicast or multicast */
    ::OcaLiteMediaStreamCastMode m_streamCastMode;
};

#endif //OCALITEMEDIACONNECTION_H
