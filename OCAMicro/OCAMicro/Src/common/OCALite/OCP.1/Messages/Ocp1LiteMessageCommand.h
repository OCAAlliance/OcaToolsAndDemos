/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Ocp1LiteMessageCommand
 *
 */

#ifndef OCP1LITEMESSAGECOMMAND_H
#define OCP1LITEMESSAGECOMMAND_H

// ---- Include system wide include files ----
#include <OCF/Messages/OcaLiteMessageCommand.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

class Ocp1LiteMessageCommand;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of an OCP.1 Command message.
 */
class Ocp1LiteMessageCommand : public ::OcaLiteMessageCommand
{
public:
    /** Constructor */
    explicit Ocp1LiteMessageCommand();

    /** Destructor */
    virtual ~Ocp1LiteMessageCommand();

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** Private copy constructor; no copying of object allowed. */
    Ocp1LiteMessageCommand(const ::Ocp1LiteMessageCommand&);
    /** Private assignment operator; no assignment of object allowed. */
    ::Ocp1LiteMessageCommand& operator=(const ::Ocp1LiteMessageCommand&);
};

#endif // OCP1LITEMESSAGECOMMAND_H
