/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMediaSourceConnector
 *
 */

#ifndef OCALITEMEDIASOURCECONNECTOR_H
#define OCALITEMEDIASOURCECONNECTOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteNetworkDataTypes.h"
#include "OcaLiteString.h"
#include "OcaLiteMediaConnection.h"
#include "OcaLiteMap.h"
#include "OcaLiteMediaCoding.h"
#include "OcaLitePortID.h"
#include "OcaLiteWorkerDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Media source (i.e. output) connector.  Connects to an outbound stream. Collected by OcaMediaTransportNetwork.
 */
class OcaLiteMediaSourceConnector : public ::IOcaLiteMarshal
{
public:

    /** Default constructor. */
    OcaLiteMediaSourceConnector();

    /**
     * Constructor.
     *
     * @param[in] IDInternal     The ID Internal.
     * @param[in] IDExternal     Public name of connector.  May be published to the media transport network, depending on the type of network.
     * @param[in] connection     Descriptor of the stream connection to this connector. If there is no stream connected to this controller, (i.e. property Connected = FALSE), the value of this property is undefined.
     * @param[in] coding         Specification of coding used by this connector.
     * @param[in] pinCount       Number of pins in this connector.
     * @param[in] channelPinMap  Map of stream pins (source channels) to OCA ports (input ports) of the owning OcaMediaNetwork object. This defines what source channels are sent to the network. A pin is identified by an OcaUint16 with value 1..MaxPinCount. Not having a certain pin identifier in this map means that the pin is empty (i.e. not carrying a source channel).
     * @param[in] alignmentLevel The alignment level.
     */
    OcaLiteMediaSourceConnector(::OcaLiteMediaConnectorID IDInternal, ::OcaLiteString IDExternal, ::OcaLiteMediaConnection connection,
                                ::OcaLiteMediaCoding coding, ::OcaUint16 pinCount, 
                                ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> channelPinMap, ::OcaDBfs alignmentLevel);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMediaSourceConnector(const ::OcaLiteMediaSourceConnector& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaSourceConnector();

    /**
     * Get the internal ID.
     *
     * @return The internal ID.
     */
    ::OcaLiteMediaConnectorID GetIDInternal() const
    {
        return m_IDInternal;
    }

    /**
     * Set the internal ID.
     *
     * @param[in] idInternal    The internal ID.
     */
    void SetIDInternal(::OcaLiteMediaConnectorID idInternal)
    {
        m_IDInternal = idInternal;
    }

    /**
     * Get the public name of the connector.
     * 
     * @return The public name of the connector.
     */
    ::OcaLiteString GetIDExternal() const
    {
        return m_IDExternal;
    }
    
    /**
     * Set the public name of the connector.
     *
     * @param[in] idExternal    The public name of the connector.
     */
    void SetIDExternal(const ::OcaLiteString& idExternal)
    {
        m_IDExternal = idExternal;
    }

    /** 
     * Get the descriptor of the stream connection to this connector
     * 
     * @retuen The descriptor.
     */
    ::OcaLiteMediaConnection GetConnection() const
    {
        return m_connection;
    }

    /**
     * Set the description of the stream connection.
     *
     * @param[in] connection    The stream connection.
     */
    void SetConnection(const ::OcaLiteMediaConnection& connection)
    {
        m_connection = connection;
    }

    /**
     * Get the coding used by this connector.
     *
     * @return The coding.
     */
    ::OcaLiteMediaCoding GetCoding() const
    {
        return m_coding;
    }

    /** 
     * Set the coding.
     *
     * @param[in] coding    The coding used by this connector.
     */
    void SetCoding(const ::OcaLiteMediaCoding& coding)
    {
        m_coding = coding;
    }

    /**
     * Get the number of pins in this connector
     *
     * @return The number of pins in this connector.
     */
    ::OcaUint16 GetPinCount() const
    {
        return m_pinCount;
    }

    /**
     * Set the pin count for this connector.
     *
     * @param[in] pinCount  The pin count.
     */
    void SetPinCount(::OcaUint16 pinCount)
    {
        m_pinCount = pinCount;
    }

     /**
      * The channel pin map.
      *
      * @return The channel pin map.
      */
    const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& GetChannelPinMap() const
    {
        return m_channelPinMap;
    }

    /**
     * Set the channel pin map.
     *
     * @param[in] channelPinMap The channel pin map
     */
    void SetChannelPinMap(const ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap)
    {
        m_channelPinMap = channelPinMap;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaSourceConnector& operator=(const ::OcaLiteMediaSourceConnector& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteMediaSourceConnector& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaSourceConnector& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
protected:
    /** The internal ID. */
    ::OcaLiteMediaConnectorID                   m_IDInternal;
    /** The advertised name */
    ::OcaLiteString                             m_IDExternal;
    /** The connection parameters */
    ::OcaLiteMediaConnection                    m_connection;
    /** The coding specification */
    ::OcaLiteMediaCoding                        m_coding;
    /** The number of pins */
    ::OcaUint16                                 m_pinCount;
    /** The channel pin map */
    ::OcaLiteMap< ::OcaUint16, ::OcaLitePortID> m_channelPinMap;
    /** The alignment level */
    ::OcaDBfs                                   m_alignmentLevel;
};
#endif //OCALITEMEDIASOURCECONNECTOR_H
