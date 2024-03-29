/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMessageGeneral
 *
 */

#ifndef OCALITEMESSAGEGENERAL_H
#define OCALITEMESSAGEGENERAL_H

// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/IOcaLiteMarshal.h>
#include <OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>

// ---- Include local include files ----
#include "OcaLiteHeader.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
#ifndef OCA_BUFFER_SIZE
#ifdef OCA_LITE_CONTROLLER
#define OCA_BUFFER_SIZE (10*1024)
#else
#define OCA_BUFFER_SIZE (4*1024)
#endif
#endif
// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of a general OCA Message.
 */
class  OcaLiteMessageGeneral : public ::IOcaLiteMarshal
{
public:

    /**
     * Getter for MessageType.
     *
     * @return message type.
     */
    ::OcaLiteHeader::OcaLiteMessageType GetMessageType() const
    {
        return m_messageType;
    }

    /**
     * Setter for MessageType.
     *
     * @param[in]   messageType Type of message
     */
    void SetMessageType(::OcaLiteHeader::OcaLiteMessageType messageType)
    {
        assert(messageType < ::OcaLiteHeader::OCA_MSG_MAXIMUM);
        m_messageType = messageType;
    }

protected:
    /**
     * Constructor.
     *
     * @param[in]    messageType Type of message
     */
    explicit OcaLiteMessageGeneral(::OcaLiteHeader::OcaLiteMessageType messageType);

    /** Destructor */
    virtual ~OcaLiteMessageGeneral();

private:
    /** Message type */
    ::OcaLiteHeader::OcaLiteMessageType     m_messageType;

    /** Private copy constructor; no copying of object allowed. */
    OcaLiteMessageGeneral(const ::OcaLiteMessageGeneral&);
    /** Private assignment operator; no assignment of object allowed. */
    ::OcaLiteMessageGeneral& operator=(const ::OcaLiteMessageGeneral&);
};

#endif // OCALITEMESSAGEGENERAL_H
