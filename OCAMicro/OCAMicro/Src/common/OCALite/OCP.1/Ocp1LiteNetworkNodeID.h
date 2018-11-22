/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteNetworkNodeID
 *
 */

#ifndef OCP1LITENETWORKNODEID_H
#define OCP1LITENETWORKNODEID_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteNetworkNodeID.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of OCP.1 network node id.
 * @ingroup OCP1
 */
class Ocp1LiteNetworkNodeID : public ::OcaLiteNetworkNodeID
{
public:
    /**
     * Constructor
     *
     * @param[in]   nodeID      The Node ID
     */
    explicit Ocp1LiteNetworkNodeID(const ::OcaLiteString& nodeID);

   /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    Ocp1LiteNetworkNodeID(const ::Ocp1LiteNetworkNodeID&);

    /** Destructor */
    virtual ~Ocp1LiteNetworkNodeID();

    /**
     * Getter for the node ID.
     *
     * @return m_nodeID
     */
    const ::OcaLiteString& GetNodeID() const;

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::Ocp1LiteNetworkNodeID& operator=(const ::Ocp1LiteNetworkNodeID& source);

    /**
     * Creates an Ocp1LiteNetworkNodeID from the base class.
     *
     * @param[in]   baseClass   The base class to create the instance from.
     * @return The newly created instance, or NULL if parsing failed.
     */
    static ::Ocp1LiteNetworkNodeID* CreateFromBase(const ::OcaLiteNetworkNodeID& baseClass);

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

private:

    /**
     * Unmarshal the base and fill the members
     *
     * @param[in]  baseClass The base class the read the values from.
     * @param[out] nodeId    The node Id.
     *
     * @return True if succeeded, false if not. 
     */
    static bool GetValuesFromBase(const ::OcaLiteNetworkNodeID& baseClass, ::OcaLiteString& nodeId);

    /** Update the base blob with the private members */
    void UpdateBlob();

    /** The node ID. */
    ::OcaLiteString                 m_nodeID;
};

#endif /*  OCP1LITENETWORKNODEID_H */
