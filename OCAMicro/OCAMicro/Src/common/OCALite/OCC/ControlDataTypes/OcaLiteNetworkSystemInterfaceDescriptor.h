/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteNetworkSystemInterfaceDescriptor
 *
 */

#ifndef OCALITENETWORKSYSTEMINTERFACEDESCRIPTOR_H
#define OCALITENETWORKSYSTEMINTERFACEDESCRIPTOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBlob.h"
#include "OcaLiteNetworkAddress.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Operating system handle for the interface the network uses to do I/O.
 */
class OcaLiteNetworkSystemInterfaceDescriptor : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteNetworkSystemInterfaceDescriptor();

    /**
     * Constructor.
     *
     * @param[in]   systemInterfaceHandle   Operating system handle for the interface the
     *                                      network uses to do I/O.
     * @param[in]   myNetworkAddress        The network address that corresponds to this
     *                                      system interface.
     */
    OcaLiteNetworkSystemInterfaceDescriptor(const ::OcaLiteBlob& systemInterfaceHandle,
                                            const ::OcaLiteNetworkAddress& myNetworkAddress);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteNetworkSystemInterfaceDescriptor(const ::OcaLiteNetworkSystemInterfaceDescriptor& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteNetworkSystemInterfaceDescriptor();

    /**
     * Getter for SystemInterfaceHandle.
     *
     * @return The operating system handle for the interface the network uses to do I/O.
     */
    const ::OcaLiteBlob& GetSystemInterfaceHandle() const
    {
        return m_systemInterfaceHandle;
    }

    /**
     * Getter for MyNetworkAddress.
     *
     * @return The network address that corresponds to this system interface.
     */
    const ::OcaLiteNetworkAddress& GetMyNetworkAddress() const
    {
        return m_myNetworkAddress;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteNetworkSystemInterfaceDescriptor& operator=(const ::OcaLiteNetworkSystemInterfaceDescriptor& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteNetworkSystemInterfaceDescriptor& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteNetworkSystemInterfaceDescriptor& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

protected:
    /** Set system interface handle */
    void SetSystemInterfaceHandle(const ::OcaLiteBlob& systemInterfaceHandle)
    {
        m_systemInterfaceHandle = systemInterfaceHandle;
    }
    /** Set network address */
    void SetNetworkAddress(const ::OcaLiteNetworkAddress& myNetworkAddress)
    {
        m_myNetworkAddress = myNetworkAddress;
    }

private:
    /** The operating system handle for the interface. */
    ::OcaLiteBlob                   m_systemInterfaceHandle;

    /** The network address that corresponds to this system interface. */
    ::OcaLiteNetworkAddress         m_myNetworkAddress;
};

#endif // OCALITENETWORKSYSTEMINTERFACEDESCRIPTOR_H
