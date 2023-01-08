/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
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
#include "OcaLiteWorkerDataTypes.h"

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
     * @param[in] connection     Descriptor of the stream connection to this connector. If there is no stream connected to this controller, 
     *                           (i.e. property Connected = FALSE), the value of this property is undefined.
     * @param[in] coding         Specification of coding used by this connector.
     * @param[in] pinCount       Number of pins in this connector.
     * @param[in] channelPinMap  Map of stream pins (sink channels) to OCA ports (input ports) of the owning OcaMediaNetwork object. 
     *                           This defines what sink channels are sent to the network. A pin is identified by an OcaUint16 with value 1..MaxPinCount. 
     *                           Not having a certain pin identifier in this map means that the pin is empty (i.e. not carrying a sink channel).
     * @param[in] alignmentLevel Alignment level of the interface. Note that the dBFS value is referenced to the
     *                           interface's fullscale value, not to device's internal fullscale value. The value
     *                           must be between the min and max values of the AlignmentLevel property of the network.
     *                           A value of NaN will cause the current value of this network's AlignmentLevel property
     *                           to be used.
     * @param[in] alignmentGain  Alignment gain for the connector. This value will be applied to all signals
     *                           incoming through all pins. The value must be between the min and max values
     *                           of the AlignmentGain property of the network. A value of NaN will cause the
     *                           current value of the network's AlignmentGain property to be used.
     */
    OcaLiteMediaSinkConnector(::OcaLiteMediaConnectorID IDInternal, ::OcaLiteString IDExternal, 
                              ::OcaLiteMediaConnection connection, ::OcaLiteMediaCoding coding, ::OcaUint16 pinCount, 
                              ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID> channelPinMap, ::OcaDBfs alignmentLevel, 
                              ::OcaDB alignmentGain);

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

    /** 
     * Set the ID internal
     * 
     * @param[in] idInternal    The ID internal
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
     * @param[in] idExternal    The public name of the connector
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
     * Set the descriptor
     *
     * @param[in] connection    The connection
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
     * Set the coding
     *
     * @param[in] coding    The media coding.
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
     * Set the pin count
     * 
     * @param[in] pinCount  The new pin count.
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
    const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& GetChannelPinMap() const
    {
        return m_channelPinMap;
    }
    
    /**
     * Set the channel pin map.
     *
     * @param[in] channelPinMap The channel pin map.
     */
    void SetChannelPinMap(const ::OcaLiteMultiMap< ::OcaUint16, ::OcaLitePortID>& channelPinMap)
    {
        m_channelPinMap = channelPinMap;
    }

    /**
     * Get the alignment level
     *
     * @return the alignment level property
     */
    const ::OcaDBfs GetAlignmentLevel() const
    {
        return m_alignmentLevel;
    }

    /**
     * Set the alignment level
     *
     * @param[in] alignmentLevel    The alignment level.
     */
    void SetAlignmentLevel(::OcaDBfs alignmentLevel)
    {
        m_alignmentLevel = alignmentLevel;
    }
    
    /** 
     * Get the alignment gain
     *
     * @return The alignment gain.
     */
    const ::OcaDB GetAlignmentGain() const
    {
        return m_alignmentGain;
    }
    
    /**
     * Set the alignment gain
     *
     * @param[in] alginmentGain     The alignment gain.
     */
    void SetAlginmentGain(::OcaDB alignmentGain)
    {
        alignmentGain = m_alignmentGain;
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
    /** The alignment level */
    ::OcaDBfs                                           m_alignmentLevel;
    /** The alignment gain */
    ::OcaDB                                             m_alignmentGain;
};
#endif //OCALITEMEDIASINKCONNECTOR_H
