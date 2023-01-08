/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The OcaLite member interface
 *
 */

#ifndef IMEMBER_H
#define IMEMBER_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * The interface of a Spi driver.
 * Each SPI Driver should at least implement this interface.
 */
class IMember
{
public:
    /** Member list type */
    typedef std::vector< ::IMember*> MemberList;

    /** member map type */
    typedef std::map<UINT32, MemberList> MemberMap;

    /**
     * Destructor.
     */
    virtual ~IMember() {}

    virtual bool MemberCallback(UINT32 data) = 0;

protected:
    /**
     * Default constructor. Made protected since this is an interface
     * class which should not be directly created.
     */
    IMember() {}

private:
    /**
     * Private copy c'tor to prevent copying.
     */
    IMember(const IMember&);

    /**
     * Private assignment operator to prevent assignment.
     */
    IMember& operator=(const IMember&);

};
#endif //IMEMBER_H
