/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaStreamParametersAes67
 *
 */

#ifndef OCALITEMEDIASTREAMPARAMETERSAES67_H
#define OCALITEMEDIASTREAMPARAMETERSAES67_H

// ---- Include system wide include files ----
#include <OCALite/OCC/ControlDataTypes/OcaLiteAES67NetworkAddress.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteWorkerDataTypes.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteNetworkDataTypes.h>
#include <OCALite/OCP.1/Ocp1LiteNetworkAddress.h>

// ---- Include local include files ----
#include "OcaLiteMediaStreamParameters.h"

#ifdef GetUserName
#undef GetUserName
#endif

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Media stream parameters for the AES67 stream network
 */
class OcaLiteMediaStreamParametersAes67 : public ::OcaLiteMediaStreamParameters
{
public:

    /** Default constructor */
    OcaLiteMediaStreamParametersAes67();

    /**
     * Constructor
     *
     * @param[in] sdpVersion            The SDP file version.
     * @param[in] userName              The user name of the device.
     * @param[in] sessionID             The session ID of the stream.
     * @param[in] sessionVersion        The session version of the stream.
     * @param[in] originAddress         The origin address of the stream.
     * @param[in] sessionName           The session name.
     * @param[in] destinationAddress    The destination address of the stream.
     * @param[in] timeToLive            The time to live parameter in the stream.
     * @param[in] mediaLabel            The media label.
     * @param[in] packetTime            The packet time.
     * @param[in] offset                The offset.
     * @param[in] linkOffset            The link offset.
     * @param[in] payloadType           The payload type.
     */
    OcaLiteMediaStreamParametersAes67(::OcaUint16 sdpVersion,
                                      const ::OcaLiteString& userName,
                                      ::OcaUint64 sessionID,
                                      ::OcaUint64 sessionVersion,
                                      const ::Ocp1LiteNetworkAddress& originAddress,
                                      const ::OcaLiteString& sessionName,
                                      const ::Ocp1LiteNetworkAddress& destinationAddress,
                                      ::OcaUint8 timeToLive,
                                      const ::OcaLiteString& mediaLabel,
                                      ::OcaTimeInterval packetTime,
                                      ::OcaUint32 offset,
                                      ::OcaTimeInterval linkOffset,
                                      ::OcaUint8 payloadType);


    /**
     * Get the SDP version
     *
     * @return The m_sdpVersion property.
     */
    ::OcaUint16 GetSdpVersion() const
    {
        return m_sdpVersion;
    }

    /**
     * Get the user name
     *
     * @return The m_userName property.
     */
    const ::OcaLiteString& GetUserName() const
    {
        return m_userName;
    }

    /**
     * Get the session ID
     *
     * @return The m_sessionID property.
     */
    ::OcaUint64 GetSessionID() const
    {
        return m_sessionID;
    }

    /**
     * Get the session version
     *
     * @return The m_sessionVersion property.
     */
    ::OcaUint64 GetSessionVersion() const
    {
        return m_sessionVersion;
    }

    /**
     * Get the origin address
     *
     * @return The m_originAddress property.
     */
    const ::Ocp1LiteNetworkAddress& GetOriginAddress() const
    {
        return m_originAddress;
    }

    /**
     * Get the session name
     *
     * @return The m_sessionName property.
     */
    const ::OcaLiteString& GetSessionName() const
    {
        return m_sessionName;
    }

    /**
     * Get the destination address
     *
     * @return The m_destinationAddress property.
     */
    const ::Ocp1LiteNetworkAddress& GetDestinationAddress() const
    {
        return m_destinationAddress;
    }

    /**
     * Get the time to live.
     *
     * @return The m_timeToLive property.
     */
    ::OcaUint8 GetTimeToLive() const
    {
        return m_timeToLive;
    }

    /**
     * Get the media label
     *
     * @return The m_mediaLabel property.
     */
    const ::OcaLiteString& GetMediaLabel() const
    {
        return m_mediaLabel;
    }

    /**
     * Get the packet time
     *
     * @return The m_packetTime property.
     */
    ::OcaTimeInterval GetPacketTime() const
    {
        return m_packetTime;
    }

    /**
     * Get the offset
     *
     * @return The m_offset property.
     */
    ::OcaUint32 GetOffset() const
    {
        return m_offset;
    }

    /**
     * Get the link offset
     *
     * @return The m_linkOffset property.
     */
    ::OcaTimeInterval GetLinkOffset() const
    {
        return m_linkOffset;
    }

    /**
     * Get the payload type
     *
     * @return The m_payloadType property.
     */
    ::OcaUint8 GetPayloadType() const
    {
        return m_payloadType;
    }

   /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMediaStreamParametersAes67(const ::OcaLiteMediaStreamParametersAes67&);

    /** Destructor */
    virtual ~OcaLiteMediaStreamParametersAes67();

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaStreamParametersAes67& operator=(const ::OcaLiteMediaStreamParametersAes67& source);

    /**
     * Creates an OcaLiteMediaStreamParametersAes67 from the base class.
     *
     * @param[in]   baseClass   The base class to create the instance from.
     * @return The newly created instance, or NULL if parsing failed.
     */
    static ::OcaLiteMediaStreamParametersAes67* CreateFromBase(const ::OcaLiteMediaStreamParameters& baseClass);

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

private:

    /**
     * Unmarshal the base and fill the members
     *
     * @param[in]  baseClass                The base class the read the values from.
     * @param[out] sdpVersion               Output parameter holding the sdp version.
     * @param[out] userName                 Output parameter holding the user name.
     * @param[out] sessionID                Output parameter holding the session identifier.
     * @param[out] sessionVersion           Output parameter holding the session version.
     * @param[out] originAddress            Output parameter holding the origin address.
     * @param[out] sessionName              Output parameter holding the session name.
     * @param[out] destinationAddress       Output parameter holding the destination address.
     * @param[out] timeToLive               Output parameter holding the time to live.
     * @param[out] mediaLabel               Output parameter holding the media label.
     * @param[out] packetTime               Output parameter holding the packet time.
     * @param[out] offset                   Output parameter holding the offset.
     * @param[out] linkOffset               Output parameter holding the link offset.
     * @param[out] payloadType              Output parameter holding the payload type.
     *
     * @return True if succeeded, false if not. 
     */
    static bool GetValuesFromBase(const ::OcaLiteMediaStreamParameters& baseClass,
                                  ::OcaUint16& sdpVersion,
                                  ::OcaLiteString& userName,
                                  ::OcaUint64& sessionID,
                                  ::OcaUint64& sessionVersion,
                                  ::Ocp1LiteNetworkAddress& originAddress,
                                  ::OcaLiteString& sessionName,
                                  ::Ocp1LiteNetworkAddress& destinationAddress,
                                  ::OcaUint8& timeToLive,
                                  ::OcaLiteString& mediaLabel,
                                  ::OcaTimeInterval& packetTime,
                                  ::OcaUint32& offset,
                                  ::OcaTimeInterval& linkOffset,
                                  ::OcaUint8& payloadType);

    /** Update the base blob with the private members */
    void UpdateBlob();
    /** The SDP version */
    ::OcaUint16 m_sdpVersion;
    /** The user name */
    ::OcaLiteString m_userName;
    /** The session ID */
    ::OcaUint64 m_sessionID;
    /** The session version */
    ::OcaUint64 m_sessionVersion;
    /** The origin address */
    ::Ocp1LiteNetworkAddress m_originAddress;
    /** The session name */
    ::OcaLiteString m_sessionName;
    /** The destination address */
    ::Ocp1LiteNetworkAddress m_destinationAddress;
    /** Time to live */
    ::OcaUint8 m_timeToLive;
    /** Media label */
    ::OcaLiteString m_mediaLabel;
    /** Packet time */
    ::OcaTimeInterval m_packetTime;
    /** Offset */
    ::OcaUint32 m_offset;
    /** Link offset */
    ::OcaTimeInterval m_linkOffset;
    /** The payload type */
    ::OcaUint8 m_payloadType;
};

#endif //OCALITEMEDIASTREAMPARAMETERSAES67_H
