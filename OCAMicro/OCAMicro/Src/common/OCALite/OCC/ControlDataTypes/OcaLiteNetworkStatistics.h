/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteNetworkStatistics
 *
 */

#ifndef OCALITENETWORKSTATISTICS_H
#define OCALITENETWORKSTATISTICS_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteNetworkDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * The statistics of the network.
 */
class OcaLiteNetworkStatistics : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteNetworkStatistics();

    /**
     * Constructor.
     *
     * @param[in]   txPacketErrors  The number of transmitter packet errors.
     * @param[in]   rxPacketErrors  The number of receiver packet errors.
     */
    OcaLiteNetworkStatistics(::OcaUint32 txPacketErrors, ::OcaUint32 rxPacketErrors);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteNetworkStatistics(const ::OcaLiteNetworkStatistics& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteNetworkStatistics();

    /**
     * Getter for TxPacketErrors.
     *
     * @return The number of transmitter packet errors.
     */
    ::OcaUint32 GetTxPacketErrors() const
    {
        return m_txPacketErrors;
    }

    /**
     * Getter for RxPacketErrors.
     *
     * @return The number of receiver packet errors.
     */
    ::OcaUint32 GetRxPacketErrors() const
    {
        return m_rxPacketErrors;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteNetworkStatistics& operator=(const ::OcaLiteNetworkStatistics& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteNetworkStatistics& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteNetworkStatistics& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The number of transmitter packet errors. */
    ::OcaUint32                 m_txPacketErrors;

    /** The number of receiver packet errors. */
    ::OcaUint32                 m_rxPacketErrors;
};

#endif // OCALITENETWORKSTATISTICS_H
