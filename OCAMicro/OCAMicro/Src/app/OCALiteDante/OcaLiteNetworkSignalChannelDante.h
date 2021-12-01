/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteNetworkSignalChannel
 *
 */

#ifndef OCALITENETWORKSIGNALCHANNELDANTE_H
#define OCALITENETWORKSIGNALCHANNELDANTE_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Workers/Networking/OcaLiteNetworkSignalChannel.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_NETWORK_SIGNAL_CHANNEL_DANTE_CLASSID  OCA_NETWORK_SIGNAL_CHANNEL_CLASSID,OCA_CLASS_ID_PROPRIETARY_CLASS_FIELD_MASK | static_cast< ::OcaUint16>(0x01)

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of a signal channel Dante
 */
class OcaLiteNetworkSignalChannelDante : public ::OcaLiteNetworkSignalChannel
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetEncodings() */
        GET_ENCODINGS               = 1,
        /** GetChannelLabel() */
        GET_CHANNEL_LABEL           = 2,
        /** SetChannelLabel() */
        SET_CHANNEL_LABEL           = 3,
        /** GetSampleRate() */
        GET_SAMPLERATE              = 4,
        /** GetSampleRate() */
        GET_CHANNEL_NUMBER          = 5
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Supported encodings of the Dante channel */
        OCA_PROP_ENCODINGS                  = 1,
        /** Label of the Dante channel. */
        OCA_PROP_CHANNEL_LABEL              = 2,
        /** Sample rate of the Dante channel. */
        OCA_PROP_SAMPLERATE                 = 3,
        /** Dante channel number. */
        OCA_PROP_CHANNEL_NUMBER             = 4
    };

    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   ports                   The OCA input and output ports.
     * @param[in]   idAdvertised            Default signal channel ID.
     * @param[in]   network                 Default Object number of stream network object (OcaStreamNetwork
     *                                      or one of its subclasses) to which this signal channel belongs.
     * @param[in]   sourceOrSink            Default I/O direction for this port. Sink = from this
     *                                      network into device; source = from device into network.
     * @param[in]   encodings               List of encodings supported by the channel.
     * @param[in]   sampleRate              Sample rate of the channel.
     * @param[in]   channelNr               Dante channel number.
     */
    OcaLiteNetworkSignalChannelDante(::OcaONo objectNumber,
                                     ::OcaBoolean lockable,
                                     const ::OcaLiteString& role,
                                     const ::OcaLiteList< ::OcaLitePort>& ports,
                                     const ::OcaLiteNetworkSignalChannelDanteID& idAdvertised,
                                     ::OcaONo network,
                                     ::OcaLiteNetworkMediaSourceOrSink sourceOrSink,
                                     const ::OcaLiteList< ::OcaLiteEncoding>& encodings,
                                     ::OcaLiteFrequency sampleRate,
                                     ::OcaUint16 channelNr);

    /**
     * Destructor.
     */
    virtual ~OcaLiteNetworkSignalChannelDante();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Implement interface ----
    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);
    // Get channel number
    virtual ::OcaUint16 GetChannelNumber() { return m_channelNr; }

    // Get Sample rate of the channel
    virtual ::OcaLiteFrequency GetChannelSampleRate() { return m_sampleRate; }

    // Get Encoding of the channel
    virtual ::OcaLiteList< ::OcaLiteEncoding> GetChannelEncoding() { return m_encodings; }

    // Get current subscription ID string
    virtual ::OcaBoolean GetSubscriptionName(OcaLiteString &Name);

    void DanteRemoteChannelIDChanged(::OcaLiteNetworkSignalChannelID remoteChannelID);


protected:
    
    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    virtual ::OcaLiteStatus SetRemoteChannelIDValue(::OcaLiteNetworkSignalChannelID signalChannelID);

private:

    /** List of encodings */
    ::OcaLiteList< ::OcaLiteEncoding>                       m_encodings;

    /** Sample rate of the channel */
    ::OcaLiteFrequency                                      m_sampleRate;

    /** Dante channel number. */
    ::OcaUint16                                             m_channelNr;

    /** private copy constructor, no copying of object allowed */
    OcaLiteNetworkSignalChannelDante(const ::OcaLiteNetworkSignalChannelDante&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteNetworkSignalChannelDante& operator=(const ::OcaLiteNetworkSignalChannelDante&);

    ::OcaLiteNetworkSignalChannelDanteID    m_DanteSubscription;
    ::OcaLiteString                         m_SubscriptionStr;
};

#endif //OCALITENETWORKSIGNALCHANNELDANTE_H
