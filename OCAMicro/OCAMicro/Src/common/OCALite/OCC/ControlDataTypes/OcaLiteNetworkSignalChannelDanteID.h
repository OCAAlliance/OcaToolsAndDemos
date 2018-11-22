/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteNetworkSignalChannelDanteID
 *
 */

#ifndef OCALITENETWORKSIGNALCHANNELDANTEID_H
#define OCALITENETWORKSIGNALCHANNELDANTEID_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteNetworkSignalChannelID.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of OCP.1 network dante id.
 * @ingroup OCP1
 */
class OcaLiteNetworkSignalChannelDanteID : public ::OcaLiteNetworkSignalChannelID
{
public:
    /**
     * Constructor
     *
     * @param[in]   danteId      The dante ID
     */
    explicit OcaLiteNetworkSignalChannelDanteID(const ::OcaLiteString& danteId);

   /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteNetworkSignalChannelDanteID(const ::OcaLiteNetworkSignalChannelDanteID&);

    /** Destructor */
    virtual ~OcaLiteNetworkSignalChannelDanteID();

    /**
     * Getter for the dante ID.
     *
     * @return m_danteId
     */
    const ::OcaLiteString& GetdanteId() const;

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteNetworkSignalChannelDanteID& operator=(const ::OcaLiteNetworkSignalChannelDanteID& source);

    /**
     * Creates an OcaLiteNetworkSignalChannelDanteID from the base class.
     *
     * @param[in]   baseClass   The base class to create the instance from.
     * @return The newly created instance, or NULL if parsing failed.
     */
    static ::OcaLiteNetworkSignalChannelDanteID* CreateFromBase(const ::OcaLiteNetworkSignalChannelID& baseClass);

    static ::OcaBoolean getDanteIDFromBase(const ::OcaLiteNetworkSignalChannelID& baseClass, OcaLiteString &danteID);

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

private:

    /**
     * Unmarshal the base and fill the members
     *
     * @param[in]  baseClass The base class the read the values from.
     * @param[out] danteId   The dante Id.
     *
     * @return True if succeeded, false if not. 
     */
    static bool GetValuesFromBase(const ::OcaLiteNetworkSignalChannelID& baseClass, ::OcaLiteString& danteId);

    /** Update the base blob with the private members */
    void UpdateBlob();

    /** The dante ID. */
    ::OcaLiteString                 m_danteId;
};

#endif //OCALITENETWORKSIGNALCHANNELDANTEID_H
