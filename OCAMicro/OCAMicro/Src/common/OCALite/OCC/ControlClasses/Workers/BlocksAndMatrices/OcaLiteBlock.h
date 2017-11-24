/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBlock
 *
 */
#ifndef OCALITEBLOCK_H
#define OCALITEBLOCK_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteObjectIdentification.h>

// ---- Include local include files ----
#include "../OcaLiteWorker.h"

// ---- Referenced classes and types ----
class OcaLiteBlob;
class OcaLiteBlock;
class OcaLiteBlockMember;
class OcaLiteString;

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 * @ingroup BlockMatrixWorkers
 */
#define OCA_BLOCK_CLASSID       OCA_WORKER_CLASSID,static_cast< ::OcaUint16>(3)

/**
 * The object number of the OCA root block
 * @ingroup BlockMatrixWorkers
 */
#define OCA_ROOT_BLOCK_ONO      static_cast< ::OcaONo>(100)

/**
 * The block type of the OCA root block
 * @ingroup BlockMatrixWorkers
 */
#define OCA_ROOT_BLOCK_TYPE     static_cast< ::OcaONo>(1)

// ---- Class Definition ----
/**
 * A block is an object with three aspects:
 * - It can contain other blocks.
 * - It can contain workers.
 * - It can contain agents.
 * - It has a signal flow topology.
 * @ingroup BlockMatrixWorkers
 */
class OcaLiteBlock : public ::OcaLiteWorker
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetType() */
        GET_TYPE                                = 1,
        /** ConstructMember() */
        CONSTRUCT_MEMBER                        = 2,
        /** ConstructMemberUsingFactory() */
        CONSTRUCT_MEMBER_USING_FACTORY          = 3,
        /** DeleteMember() */
        DELETE_MEMBER                           = 4,
        /** GetMembers() */
        GET_MEMBERS                             = 5,
        /** GetMembersRecursive() */
        GET_MEMBERS_RECURSIVE                   = 6,
        /** AddSignalPath() */
        ADD_SIGNAL_PATH                         = 7,
        /** DeleteSignalPath() */
        DELETE_SIGNAL_PATH                      = 8,
        /** GetSignalPaths() */
        GET_SIGNAL_PATHS                        = 9,
        /** GetSignalPathsRecursive() */
        GET_SIGNAL_PATHS_RECURSIVE              = 10,
        /** GetMostRecentParamSetIdentifier() */
        GET_MOST_RECENT_PARAMSET_IDENTIFIER     = 11,
        /** ApplyParamSet() */
        APPLY_PARAMSET                          = 12,
        /** GetCurrentParamSetData() */
        GET_CURRENT_PARAMSET_DATA               = 13,
        /** StoreCurrentParamSetData() */
        STORE_CURRENT_PARAMSET_DATA             = 14
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** Readonly block type. For statically-defined blocks, this value is a Uint32 with a value
            corresponding to the unique configuration of this block. For dynamically-defined blocks, this
            value is the object number of the block's factory. For the root block, the value of this property is 1. */
        OCA_PROP_TYPE                               = 1,
        /** List of members in the block. */
        OCA_PROP_MEMBERS                            = 2,
        /** List of signal paths in the block. */
        OCA_PROP_SIGNAL_PATHS                       = 3,
        /** Library volume identifier of the paramset most recently applied to this block. */
        OCA_PROP_MOST_RECENT_PARAM_SET_IDENTIFIER   = 4
    };    

    /**
     * Destructor.
     */
    virtual ~OcaLiteBlock();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    /**
     * Returns instance of the root block
     */
    static ::OcaLiteBlock& GetRootBlock();

    /**
     * Destroys the root block
     */
    static void FreeRootBlock();

    // ---- Interface methods  ----

    /**
     * Gets the list of block members. Does not recurse inner blocks. Each inner block is included in
     * the returned list as a single object -- its contents are not enumerated
     *
     * @param[out]  members            The list of block members
     * @return Indicates whether the list was successfully retrieved.
     */
    ::OcaLiteStatus GetMembers(::OcaLiteList< ::OcaLiteObjectIdentification>& members) const;

    /**
     * Gets the list of block members. Recurses inner blocks. Each inner block is included in the
     * returned list as a single object, and its contents are enumerated.
     *
     * @param[out]  members            The list of block members
     * @return Indicates whether the list was successfully retrieved.
     */
    ::OcaLiteStatus GetMembersRecursive(::OcaLiteList< ::OcaLiteBlockMember>& members) const;

    // ---- Miscelaneous methods  ----
    /**
     * Adds an object to the member object list.
     * Only for internal use.
     *
     * @param[in] rObject   Reference to the instance of the object.
     *
     * @return Indicates whether the object was added successfully; if adding
     *         the object failed the object number was already in use.
     */
    ::OcaBoolean AddObject(::OcaLiteRoot& rObject);

    /**
     * Deletes the object with the given object number.
     * The object number is released and can be re-used later on.
     * Only for internal use.
     *
     * @param[in] oNo   The object number of the object that must be deleted.
     */
    void RemoveObject(::OcaONo oNo);

    /**
     * Gets the object pointer of the object that has the specified object number.
     * It recurses all the block members to find the object. Only for internal use.
     *
     * @param[in] oNo   The object number of the object.
     * @return Pointer to the object. If no object with that number exists
     *         a null pointer is returned.
     */
    ::OcaLiteRoot* GetObject(::OcaONo oNo);

    virtual void SessionLost(::OcaSessionID sessionID);

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                The role of this instance.
     * @param[in]   ports               The OCA input and output ports.
     * @param[in]   type                Block type. For statically-defined blocks, the block type is a Uint32 with a value
     *                                  corresponding to the unique configuration of this block. For dynamically-defined blocks, the block
     *                                  type is the object number of the block's factory. For the root block, the value of this property is 1.
     */
    OcaLiteBlock(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaONo type);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    virtual ::OcaLiteStatus SetEnabledValue(::OcaBoolean bEnabled)
    {
        return OCASTATUS_OK;
    }

private:
    /** The root block */
    static ::OcaLiteBlock*                  m_pRootBlock;

    /** The map of object numbers to object pointers */
    typedef std::map< ::OcaONo, ::OcaLiteRoot*> OcaMemberList;
    OcaMemberList                           m_members;

	/** List of members of type OcaBlock */
    typedef std::vector< ::OcaLiteBlock*>       OcaLiteBlockList;
    OcaLiteBlockList                            m_blocks;
};

#endif // OCALITEBLOCK_H
