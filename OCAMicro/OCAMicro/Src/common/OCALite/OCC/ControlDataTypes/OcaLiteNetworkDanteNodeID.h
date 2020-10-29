/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteNetworkDanteNodeID
 *
 */

#ifndef OCALITENETWORKDANTENODEID_H
#define OCALITENETWORKDANTENODEID_H

// ---- Include system wide include files ----

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of OCP.1 network dante id.
 * @ingroup OCP1
 */
class OcaLiteNetworkDanteNodeID : public ::OcaLiteNetworkNodeID
{
public:
    /**
     * Constructor
     *
     * @param[in]   danteId      The dante ID
     */
    explicit OcaLiteNetworkDanteNodeID(const ::OcaLiteString& danteId);

   /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteNetworkDanteNodeID(const ::OcaLiteNetworkDanteNodeID&);

    /** Destructor */
    virtual ~OcaLiteNetworkDanteNodeID();

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
    ::OcaLiteNetworkDanteNodeID& operator=(const ::OcaLiteNetworkDanteNodeID& source);

    /**
     * Creates an OcaLiteNetworkDanteNodeID from the base class.
     *
     * @param[in]   baseClass   The base class to create the instance from.
     * @return The newly created instance, or NULL if parsing failed.
     */
    static ::OcaLiteNetworkDanteNodeID* CreateFromBase(const ::OcaLiteNetworkNodeID& baseClass);

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
    static bool GetValuesFromBase(const ::OcaLiteNetworkNodeID& baseClass, ::OcaLiteString& danteId);

    /** Update the base blob with the private members */
    void UpdateBlob();

    /** The dante ID. */
    ::OcaLiteString                 m_danteId;
};

#endif //OCALITENETWORKDANTENODEID_H
