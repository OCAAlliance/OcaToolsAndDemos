/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMessageNotification
 *
 */

#ifndef OCALITEMESSAGENOTIFICATION_H
#define OCALITEMESSAGENOTIFICATION_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>

// ---- Include local include files ----
#include "OcaLiteMessageGeneral.h"

// ---- Referenced classes and types ----
class OcaLiteBlob;
class OcaLiteEventData;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of an Oca Notification message.
 */
class  OcaLiteMessageNotification : public ::OcaLiteMessageGeneral
{
public:
    /**
     * Getter for TargetONo.
     *
     * @return m_TargetONo
     */
    ::OcaONo GetTargetONo() const
    {
        return m_targetONo;
    }

    /**
     * Getter for MethodID.
     *
     * @return m_MethodID
     */
    const ::OcaLiteMethodID& GetMethodID() const
    {
        return m_methodID;
    }

    /**
     * Getter for the parameters
     *
     * @return m_pParameters
     */
    const ::OcaUint8* GetParameters() const
    {
        return m_pParameters;
    }

    /**
     * Getter for the size of parameters
     *
     * @return m_parametersSize
     */
    ::OcaUint32 GetParametersSize() const
    {
        return m_parametersSize;
    }

    /**
     * Writes the notification parameters to the message
     *
     * @param[in]   targetONo           The target object number.
     * @param[in]   methodID            The method ID.
     * @param[in]   context             The notification context.
     * @param[in]   eventData           The notification event data.
     * @param[in]   writer              The writer to use.
     * @return True if all elements were written successfully, false otherwise.
     */
    bool WriteParameters(::OcaONo targetONo,
                         const ::OcaLiteMethodID& methodID,
                         const ::OcaLiteBlob& context,
                         const ::OcaLiteEventData& eventData,
                         const ::IOcaLiteWriter& writer);

protected:
    /** Constructor */
    explicit OcaLiteMessageNotification();

    /** Destructor */
    virtual ~OcaLiteMessageNotification();

    /**
     * Writes the notification parameters to the message
     *
     * @param[in]   targetONo           The target object number.
     * @param[in]   methodID            The method ID.
     */
    void WriteParameters(::OcaONo targetONo,
                         const ::OcaLiteMethodID& methodID);

private:

    /** Parameters */
    ::OcaUint8*                     m_pParameters;
    ::OcaUint32                     m_parametersSize;

    /** TargetONo */
    ::OcaONo                        m_targetONo;

    /** MethodID */
    ::OcaLiteMethodID               m_methodID;

    /** Private copy constructor; no copying of object allowed. */
    OcaLiteMessageNotification(const ::OcaLiteMessageNotification&);
    /** Private assignment operator; no assignment of object allowed. */
    ::OcaLiteMessageNotification& operator=(const ::OcaLiteMessageNotification&);
};

#endif // OCALITEMESSAGENOTIFICATION_H
