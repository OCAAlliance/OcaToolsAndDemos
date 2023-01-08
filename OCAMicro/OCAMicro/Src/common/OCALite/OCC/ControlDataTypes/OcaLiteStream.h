/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteStream
 *
 */

#ifndef OCALITESTREAM_H
#define OCALITESTREAM_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteNetworkDataTypes.h"
#include "OcaLiteStreamParameters.h"
#include "OcaLiteStreamConnectorID.h"
#include "OcaLiteStreamID.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * Media transport protocols available
 */
enum OcaLiteStreamType    /* maps onto OcaUint8 */
{
    /** Unknown media endpoint type.*/
    OCASTREAMTYPE_NONE          = 0,
    /** Unicast stream. */
    OCASTREAMTYPE_UNICAST       = 1,
    /** Multicast stream */
    OCASTREAMTYPE_MULTICAST     = 2,
    /** Maximum value used for range checking */
    OCASTREAMTYPE_MAXIMUM
};

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * A single-channel or multichannel signal flow between a local stream connector (i.e.
 * OcaStreamConnector instance) of an OcaStreamNetwork object in this node and another ("remote")
 * stream connector. Normally, the remote stream connector is in another node.
 * Each stream is unidirectional. With respect to the OcaStreamNetwork object in question, a stream is
 * either: <br />
 * <b>-Outbound:</b> A signal flow from an output connector port in the OcaStreamNetwork object to an
 *                   external destination;<br />
 * or<br />
 * <b>-Inbound:</b> A signal flow from an external source to an input connector in the OcaStreamNetwork
 *                  object.<br />
 * An OcaLiteStream object may represent either a unicast or a multicast stream. Any given OcaStreamConnector
 * object may support multiple outbound flows, but not multiple inbound flows.
 */
class OcaLiteStream : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteStream();

    /**
     * Constructor.
     * 
     * @param[in]   id                  Public identifier of this stream.
     * @param[in]   label               Arbitrary user-settable name for this stream.
     * @param[in]   index               Index of this stream. Unique within owner OcaStreamNetwork object.
     * @param[in]   type                Type of this stream.
     * @param[in]   localConnector      Object number of OcaStreamConnector object to which this stream is connected.
     * @param[in]   remoteConnectorId   Full identifier of the connector at the far end of this stream.
     * @param[in]   priority            The traffic priority of the connection. Values are network
     *                                  implementation dependent.
     * @param[in]   secure              True iff connection is secure.
     * @param[in]   streamParameters    The stream parameters (encoding, sampling, etc.)
     * @param[in]   errorNumber         The index of the most recent error encountered.
     * @param[in]   status              The stream connection status.
     */
    OcaLiteStream(::OcaLiteStreamID id,
                  const ::OcaLiteString& label,
                  ::OcaUint16 index,
                  ::OcaLiteStreamType type,
                  ::OcaONo localConnector,
                  ::OcaLiteStreamConnectorID remoteConnectorId,
                  ::OcaUint16 priority,
                  ::OcaBoolean secure,
                  const ::OcaLiteStreamParameters& streamParameters,
                  ::OcaUint16 errorNumber,
                  ::OcaLiteStreamStatus status);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteStream(const ::OcaLiteStream& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteStream();

    /**
     * Getter for ID.
     *
     * @return The unique identifier of this connection within the device.
     */
    ::OcaLiteStreamID GetID() const
    {
        return m_id;
    }

    /**
     * Getter for Label.
     *
     * @return The arbitrary user-settable name for this connection.
     */
    const ::OcaLiteString& GetLabel() const
    {
        return m_label;
    }

    /**
     * Getter for Index.
     *
     * @return The index of this stream. Unique within the owner stream network.
     */
    ::OcaLiteStreamIndex GetIndex() const
    {
        return m_index;
    }

    /**
     * Setter for Index.
     *
     * @param[in] streamIndex  The new stream index
     */
    void SetIndex(::OcaLiteStreamIndex streamIndex)
    {
        m_index = streamIndex;
    }

    /**
     * Getter for Stream Type.
     *
     * @return The stream type.
     */
    ::OcaLiteStreamType GetStreamType() const
    {
        return m_type;
    }

     /**
     * Getter for Local connector object number.
     *
     * @return Object number of the local connector to which this stream is connected. 
     */
    ::OcaONo GetLocalConnectorONo() const
    {
        return m_localConnectorONo;
    }

    /**
     * Getter for Remote Connector ID.
     *
     * @return The full identifier of the connector at the far end of this stream.
     */
    const ::OcaLiteStreamConnectorID& GetRemoteConnectorID() const
    {
        return m_remoteConnectorId;
    }

    /**
     * Getter for Priority.
     *
     * @return The traffic priority of the connection. Values are network implementation dependent.
     */
    ::OcaUint16 GetPriority() const
    {
        return m_priority;
    }

    /**
     * Getter for Secure.
     *
     * @return True iff connection is secure.
     */
    ::OcaBoolean GetSecure() const
    {
        return m_secure;
    }

    /**
     * Getter for StreamParameters.
     *
     * @return The stream parameters (encoding, sampling, etc.).
     */
    const ::OcaLiteStreamParameters& GetStreamParameters() const
    {
        return m_streamParameters;
    }

    /**
     * Set the stream parameters 
     *
     * @param[in] streamParameters  The new stream parameters.
     */
    void SetStreamParameters(const ::OcaLiteStreamParameters& streamParameters)
    {
        m_streamParameters = streamParameters;
    }

    /**
     * Getter for ErrorNumber.
     *
     * @return The index of the most recent error encountered.
     */
    ::OcaUint16 GetErrorNumber() const
    {
        return m_errorNumber;
    }

    /**
     * Getter for Status.
     *
     * @return The stream connection status.
     */
    ::OcaLiteStreamStatus GetStatus() const
    {
        return m_status;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteStream& operator=(const ::OcaLiteStream& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteStream& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteStream& rhs) const;

    // ---- IOcaMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** Public identifier of this stream. */
    ::OcaLiteStreamID                       m_id;
    /** Arbitrary user-settable name for this stream. */
    ::OcaLiteString                         m_label;
    /** Index of this stream. Unique within owner OcaStreamNetwork object. */
    ::OcaLiteStreamIndex                    m_index;
    /** Unicast or Multicast */
    ::OcaLiteStreamType                     m_type;
    /** Object number of OcaStreamConnector object to which this stream is connected. A value of
        zero means the stream is not connected to any connector in this device. */
    ::OcaONo                                m_localConnectorONo;
    /** Full identifier of the connector at the far end of this stream. */
    ::OcaLiteStreamConnectorID              m_remoteConnectorId;
    /** Traffic priority of stream. Values are network implementation dependant. */
    ::OcaUint16                             m_priority;    
    /** True iff connection is secure. */
    ::OcaBoolean                            m_secure;
    /** The stream parameters (encoding, sampling, etc.) */
    ::OcaLiteStreamParameters               m_streamParameters;
    /** The index of the most recent error encountered. */
    ::OcaUint16                             m_errorNumber;
    /** The stream status. */
    ::OcaLiteStreamStatus                   m_status;
};

template <>
void MarshalValue< ::OcaLiteStreamType>(const ::OcaLiteStreamType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteStreamType>(::OcaLiteStreamType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteStreamType>(const ::OcaLiteStreamType& value, const ::IOcaLiteWriter& writer);

#endif // OCALITESTREAM_H
