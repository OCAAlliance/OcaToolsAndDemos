/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteHeader
 *
 */

#ifndef OCALITEHEADER_H
#define OCALITEHEADER_H

// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/IOcaLiteMarshal.h>
#include <OCC/ControlDataTypes/OcaLiteFrameworkDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of the header of a OCA header.
 */
class  OcaLiteHeader : public ::IOcaLiteMarshal
{
public:
    /** Enumeration of message types. */
    enum OcaLiteMessageType
    {
        /** Command - No response required. */
        OCA_MSG_CMD                 = 0,
        /** Command - Response required. */
        OCA_MSG_CMD_RRQ             = 1,
        /** Notification. */
        OCA_MSG_NTF                 = 2,
        /** Response (to a command). */
        OCA_MSG_RSP                 = 3,
        /** KeepAlive message used for device supervision. */
        OCA_MSG_KEEP_ALIVE          = 4,
        /** Maximum value needed for boundary checking */
        OCA_MSG_MAXIMUM
    };

    /**
     * Getter for MessageType.
     *
     * @return m_MessageType.
     */
    OcaLiteMessageType GetMessageType() const
    {
        return m_messageType;
    }

    /**
     * Getter for MessageCount.
     *
     * @return m_MessageCount.
     */
    ::OcaUint16 GetMessageCount() const
    {
        return m_messageCount;
    }

    /**
     * Writes the header parameters to the header
     *
     * @param[in]    messageType     Type of message
     * @param[in]    messageCount    The message count
     */
    void WriteParameters(::OcaLiteHeader::OcaLiteMessageType messageType,
                         ::OcaUint16 messageCount);

protected:
    /** Constructor */
    OcaLiteHeader();

    /** Destructor */
    virtual ~OcaLiteHeader();

private:
    /** Type of the message. */
    OcaLiteMessageType       m_messageType;

    /** Message count */
    ::OcaUint16          m_messageCount;

    /** Private copy constructor; no copying of object allowed. */
    OcaLiteHeader(const ::OcaLiteHeader&);
    /** Private assignment operator; no assignment of object allowed. */
    ::OcaLiteHeader& operator=(const ::OcaLiteHeader&);
};

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::OcaLiteHeader::OcaLiteMessageType>(const ::OcaLiteHeader::OcaLiteMessageType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteHeader::OcaLiteMessageType>(::OcaLiteHeader::OcaLiteMessageType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteHeader::OcaLiteMessageType>(const ::OcaLiteHeader::OcaLiteMessageType& value, const ::IOcaLiteWriter& writer);

#endif // OCALITEHEADER_H
