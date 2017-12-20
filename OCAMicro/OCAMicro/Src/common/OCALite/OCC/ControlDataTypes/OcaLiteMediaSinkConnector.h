/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaSinkConnector
 *
 */

#ifndef OCALITEMEDIASINKCONNECTOR_H
#define OCALITEMEDIASINKCONNECTOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteNetworkDataTypes.h"
#include "OcaLiteString.h"
#include "OcaLiteMediaConnection.h"
#include "OcaLiteMultiMap.h"
#include "OcaLiteMediaCoding.h"
#include "OcaLitePortID.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Media sink (i.e. input) connector.  Connects to an inbound stream. Collected by OcaMediaTransportNetwork.
 */
class OcaLiteMediaSinkConnector : public ::IOcaLiteMarshal
{
public:

    /** Default constructor. */
    OcaLiteMediaSinkConnector();

    /**
     * Constructor.
     *
     * @param[in] IDInternal     The ID Internal.
     * @param[in] IDExternal     Public name of connector.  May be published to the media transport network, depending on the type of network.
     * @param[in] connection     Descriptor of the stream connection to this connector. If there is no stream connected to this controller, (i.e. property Connected = FALSE), the value of this property is undefined.
     * @param[in] coding         Specification of coding used by this connector.
     * @param[in] pinCount       Number of pins in this connector.
     * @param[in] channelPinMap  Map of stream pins (sink channels) to OCA ports (input ports) of the owning OcaMediaNetwork object. This defines what sink channels are sent to the network. A pin is identified by an OcaUint16 with value 1..MaxPinCount. Not having a certain pin identifier in this map means that the pin is empty (i.e. not carrying a sink channel).
     */
    OcaLiteMediaSinkConnector(::OcaLiteMediaConnectorID IDInternal, ::OcaLiteString IDExternal, ::OcaLiteMediaConnection connection,
                              ::OcaLiteMediaCoding coding, ::OcaUint16 pinCount, 
                              ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> channelPinMap);

    /**
     * Copy constructor.
     *
     * @param[in]   sink      The sink to copy the object from.
     */
    OcaLiteMediaSinkConnector(const ::OcaLiteMediaSinkConnector& sink);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaSinkConnector();

    /**
     * Get the internal ID.
     *
     * @return The internal ID.
     */
    ::OcaLiteMediaConnectorID GetIDInternal() const
    {
        return m_IDInternal;
    }

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

    void SetPinCount(::OcaUint16 pinCount)
    {
        m_pinCount = pinCount;
    }

     /**
      * The channel pin map.
      *
      * @return The channel pin map.
      */
    const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& GetChannelPinMap() const
    {
        return m_channelPinMap;
    }

    void SetChannelPinMap(const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap)
    {
        m_channelPinMap = channelPinMap;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   sink      The sink to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaSinkConnector& operator=(const ::OcaLiteMediaSinkConnector& sink);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteMediaSinkConnector& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaSinkConnector& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** sink, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
protected:
    /** The internal ID. */
    ::OcaLiteMediaConnectorID                           m_IDInternal;
    /** The advertised name */
    ::OcaLiteString                                     m_IDExternal;
    /** The connection parameters */
    ::OcaLiteMediaConnection                            m_connection;
    /** The coding specification */
    ::OcaLiteMediaCoding                                m_coding;
    /** The number of pins */
    ::OcaUint16                                         m_pinCount;
    /** The channel pin map */
    ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>    m_channelPinMap;
};
#endif //OCALITEMEDIASINKCONNECTOR_H
