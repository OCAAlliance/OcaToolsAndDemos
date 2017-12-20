/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteMessageHeader
 *
 */

#ifndef OCP1LITEMESSAGEHEADER_H
#define OCP1LITEMESSAGEHEADER_H

// ---- Include system wide include files ----
#include <OCF/Messages/OcaLiteHeader.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

class Ocp1LiteMessageHeader;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of the header of an OCP.1 Message Header.
 */
class Ocp1LiteMessageHeader : public ::OcaLiteHeader
{
public:
    /** Constructor */
    Ocp1LiteMessageHeader();

    /** Destructor */
    virtual ~Ocp1LiteMessageHeader();

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

    /**
     * Getter for ProtocolVersion.
     *
     * @return m_ProtocolVersion.
     */
    ::OcaUint16 GetProtocolVersion() const
    {
        return m_protocolVersion;
    }

    /**
     * Getter for MessageSize.
     *
     * @return m_MessageSize.
     */
    ::OcaUint32 GetMessageSize() const
    {
        return static_cast< ::OcaUint32>(m_messageSize);
    }

    /**
     * Writes the header parameters to the header
     *
     * @param[in]    messageType     Type of message
     * @param[in]    messageCount    The message count
     * @param[in]    messageSize     The total message size of all messages (including the header)
     */
    void WriteOcp1Parameters(::OcaLiteHeader::OcaLiteMessageType messageType,
                             ::OcaUint16 messageCount,
                             ::OcaUint32 messageSize);

private:
    /** The latest protocol version */
    static ::OcaUint16   m_latestProtocolVersion;

    /** Versionnumber of OCP.1 */
    ::OcaUint16          m_protocolVersion;

    /** Size of the message (in bytes). */
    ::OcaUint32          m_messageSize;

    /** Private copy constructor; no copying of object allowed. */
    Ocp1LiteMessageHeader(const ::Ocp1LiteMessageHeader&);
    /** Private assignment operator; no assignment of object allowed. */
    ::Ocp1LiteMessageHeader& operator=(const ::Ocp1LiteMessageHeader&);
};

#endif // OCP1LITEMESSAGEHEADER_H
