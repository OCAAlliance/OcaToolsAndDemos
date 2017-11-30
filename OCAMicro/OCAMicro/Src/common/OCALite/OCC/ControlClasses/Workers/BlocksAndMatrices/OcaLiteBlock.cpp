/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteBlock
 *
 */

// ---- Include system wide include files ----
#include <limits>
#include <OCC/ControlClasses/Agents/OcaLiteAgent.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Workers/OcaLiteWorker.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCC/ControlDataTypes/OcaLiteBlockMember.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteBlock.h"

// ---- Helper types and constants ----

static const ::OcaUint16                classID[] = {OCA_BLOCK_CLASSID};
const ::OcaLiteClassID                  OcaLiteBlock::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);
::OcaLiteBlock*                         OcaLiteBlock::m_pRootBlock(NULL);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBlock::OcaLiteBlock(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaONo type)
    : ::OcaLiteWorker(objectNumber, lockable, role, ports),
      m_blocks(),
      m_members()
{
    assert(((OCA_ROOT_BLOCK_TYPE == type) && (OCA_ROOT_BLOCK_ONO == objectNumber)) ||
           ((OCA_ROOT_BLOCK_TYPE != type) && (OCA_MINIMUM_DEVICE_OBJECT_ONO <= objectNumber)));
}

OcaLiteBlock::~OcaLiteBlock()
{
    // Clear the owners of all members
    assert(m_members.empty());
    assert(m_blocks.empty());
}

::OcaLiteBlock& OcaLiteBlock::GetRootBlock()
{
    if (NULL == OcaLiteBlock::m_pRootBlock)
    {
        const ::OcaLiteList< ::OcaLitePort> ports;
        OcaLiteBlock::m_pRootBlock = new OcaLiteBlock(OCA_ROOT_BLOCK_ONO, static_cast< ::OcaBoolean>(true), ::OcaLiteString("RootBlock"), ports, OCA_ROOT_BLOCK_TYPE);
    }

    return *OcaLiteBlock::m_pRootBlock;
}

void OcaLiteBlock::FreeRootBlock()
{
    if (OcaLiteBlock::m_pRootBlock != NULL)
    {
        delete m_pRootBlock;
        m_pRootBlock = NULL;
    }
}

::OcaLiteStatus OcaLiteBlock::GetMembers(::OcaLiteList< ::OcaLiteObjectIdentification>& members) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    members.Clear();
    members.Reserve(static_cast< ::OcaUint16>(m_members.size()));

    OcaMemberList::const_iterator iter(m_members.begin());
    while (m_members.end() != iter)
    {
        ::OcaLiteClassIdentification classIdentification;
        assert(NULL != iter->second);
        assert(GetObjectNumber() != iter->second->GetObjectNumber());

        if (OCASTATUS_OK == iter->second->GetClassIdentification(classIdentification))
        {
            assert(iter->first == iter->second->GetObjectNumber());

            members.Add(::OcaLiteObjectIdentification(iter->second->GetObjectNumber(),
                                                      classIdentification));
        }
        else
        {
            rc = OCASTATUS_DEVICE_ERROR;
            break;
        }

        ++iter;
    }
    return rc;
}

::OcaLiteStatus OcaLiteBlock::GetMembersRecursive(::OcaLiteList< ::OcaLiteBlockMember>& members) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    members.Reserve(members.GetCount() + static_cast< ::OcaUint16>(m_members.size()));

    OcaMemberList::const_iterator iter(m_members.begin());
    while (m_members.end() != iter)
    {
        ::OcaLiteClassIdentification classIdentification;
        assert(NULL != iter->second);
        assert(GetObjectNumber() != iter->second->GetObjectNumber());

        if (OCASTATUS_OK == iter->second->GetClassIdentification(classIdentification))
        {
            assert(iter->first == iter->second->GetObjectNumber());

            members.Add(::OcaLiteBlockMember(::OcaLiteObjectIdentification(iter->second->GetObjectNumber(),
                                                                           classIdentification),
                                             GetObjectNumber()));
        }
        else
        {
            rc = OCASTATUS_DEVICE_ERROR;
            break;
        }

        ++iter;
    }
    for (OcaLiteBlockList::const_iterator blockIter(m_blocks.begin());
         (blockIter != m_blocks.end()) && (OCASTATUS_OK == rc);
         ++blockIter)
    {
        if ((*blockIter)->GetObjectNumber() != GetObjectNumber())
        {
            rc = (*blockIter)->GetMembersRecursive(members);
        }
    }

	
	return rc;
}

::OcaBoolean OcaLiteBlock::AddObject(::OcaLiteRoot& rObject)
{
    // Managers should be added to the device manager
    assert(NULL == dynamic_cast< ::OcaLiteManager*>(&rObject));

    bool result(false);

    ::OcaONo newObjectNumber(rObject.GetObjectNumber());

    // The root block should not be added
    if (OCA_ROOT_BLOCK_ONO != newObjectNumber)
    {
        ::OcaLiteWorker* pWorker(dynamic_cast< ::OcaLiteWorker*>(&rObject));
        ::OcaLiteBlock* pBlock(dynamic_cast< ::OcaLiteBlock*>(pWorker));

		assert(m_members.end() == m_members.find(newObjectNumber));
        static_cast<void>(m_members.insert(OcaMemberList::value_type(newObjectNumber, &rObject)));

        if (NULL != pBlock)
        {
            assert(m_blocks.end() == std::find(m_blocks.begin(), m_blocks.end(), pBlock));
            m_blocks.push_back(pBlock);
        }

		// We are OCALite, don't expect dynamic objects in this implementation.
        assert(::OcaLiteDeviceManager::GetInstance().GetOperationalState() == ::OcaLiteDeviceManager::OCA_OPSTATE_INITIALIZING);

        result = true;
    }

    return static_cast< ::OcaBoolean>(result);
}

void OcaLiteBlock::RemoveObject(::OcaONo oNo)
{
    OcaMemberList::const_iterator iter(m_members.find(oNo));
    if (m_members.end() != iter)
    {
        assert(NULL != iter->second);
        assert(iter->first == iter->second->GetObjectNumber());

        // Clear the owner property of the class
        ::OcaLiteWorker* pWorker(dynamic_cast< ::OcaLiteWorker*>(iter->second));

        OcaLiteBlockList::iterator blockIter(std::find(m_blocks.begin(), m_blocks.end(), pWorker));
        if (m_blocks.end() != blockIter)
        {
            static_cast<void>(m_blocks.erase(blockIter));
        }

		static_cast<void>(m_members.erase(oNo));  //lint !e792 Void cast not needed for certain STL implementations

        // We are OCALite, don't expect dynamic objects in this implementation.
        assert(::OcaLiteDeviceManager::GetInstance().GetOperationalState() == ::OcaLiteDeviceManager::OCA_OPSTATE_SHUTTING_DOWN);
    }
}

::OcaLiteRoot* OcaLiteBlock::GetObject(::OcaONo oNo)
{
    ::OcaLiteRoot* pObject(NULL);

    if (OCA_INVALID_ONO != oNo)
    {
        // If the root block is requested, return the root block
        if (OCA_ROOT_BLOCK_ONO == oNo)
        {
            pObject = m_pRootBlock;
        }
        else
        {
            OcaMemberList::const_iterator iter(m_members.find(oNo));
            if (m_members.end() != iter)
            {
                assert(NULL != iter->second);
                pObject = iter->second;
            }
            else
            {
                for (OcaLiteBlockList::iterator blockIter(m_blocks.begin());
                     (blockIter != m_blocks.end()) && (NULL == pObject);
                     ++blockIter)
                {
                    pObject = (*blockIter)->GetObject(oNo);
                }
            }
        }
    }

    return pObject;
}

::OcaLiteRoot* OcaLiteBlock::GetOCAObject(::OcaONo oNo)
{
    return GetObject(oNo);
}

void OcaLiteBlock::SessionLost(::OcaSessionID sessionID)
{
    for (OcaMemberList::iterator iter(m_members.begin()); m_members.end() != iter; ++iter)
    {
        assert(NULL != iter->second);

        // Note that the members may not perform an action on the containing block inside the SessionLost, else
        // it can lead to a recursive lock!
        iter->second->SessionLost(sessionID);
    }
    OcaLiteWorker::SessionLost(sessionID);
}

::OcaLiteStatus OcaLiteBlock::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                      ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_ERROR);
    if (!IsLocked(sessionID))
    {
        if (methodID.GetDefLevel() == CLASS_ID.GetFieldCount())
        {
            ::OcaUint8* responseBuffer(NULL);
            const ::OcaUint8* pCmdParameters(parameters);
            ::OcaUint32 bytesLeft(parametersSize);

            switch (methodID.GetMethodIndex())
            {
            case GET_MEMBERS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteObjectIdentification> members;
                        rc = GetMembers(members);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     members.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                members.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_MEMBERS_RECURSIVE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteBlockMember> members;
                        rc = GetMembersRecursive(members);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     members.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                members.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case GET_TYPE:
            case GET_SIGNAL_PATHS:
            case GET_SIGNAL_PATHS_RECURSIVE:
            case CONSTRUCT_MEMBER:
            case CONSTRUCT_MEMBER_USING_FACTORY:
            case DELETE_MEMBER:
            case ADD_SIGNAL_PATH:
            case DELETE_SIGNAL_PATH:
            case GET_MOST_RECENT_PARAMSET_IDENTIFIER:
            case APPLY_PARAMSET:
            case GET_CURRENT_PARAMSET_DATA:
            case STORE_CURRENT_PARAMSET_DATA:
            case GET_GLOBAL_TYPE:
            case GET_ONO_MAP:
            case FIND_OBJECTS_BY_ROLE:
            case FIND_OBJECTS_BY_ROLE_RECURSIVE:
            case FIND_OBJECTS_BY_LABEL_RECURSIVE:
            case FIND_OBJECTS_BY_PATH:
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteWorker::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteBlock::GetClassVersion() const
{
    return (OcaLiteWorker::GetClassVersion() + CLASS_VERSION_INCREMENT);
}
