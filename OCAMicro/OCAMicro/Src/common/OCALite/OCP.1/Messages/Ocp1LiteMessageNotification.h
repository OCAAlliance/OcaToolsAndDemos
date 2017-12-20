/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteMessageNotification
 *
 */

#ifndef OCP1LITEMESSAGENOTIFICATION_H
#define OCP1LITEMESSAGENOTIFICATION_H

// ---- Include system wide include files ----
#include <OCF/Messages/OcaLiteMessageNotification.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

class Ocp1LiteMessageNotification;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of an OCP.1 Message Notification
 */
class Ocp1LiteMessageNotification : public ::OcaLiteMessageNotification
{
public:

    /** Constructor */
    explicit Ocp1LiteMessageNotification();

    /** Destructor */
    virtual ~Ocp1LiteMessageNotification();

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:

    /** Private copy constructor; no copying of object allowed. */
    Ocp1LiteMessageNotification(const ::Ocp1LiteMessageNotification&);
    /** Private assignment operator; no assignment of object allowed. */
    ::Ocp1LiteMessageNotification& operator=(const ::Ocp1LiteMessageNotification&);
};

#endif // OCP1LITEMESSAGENOTIFICATION_H
