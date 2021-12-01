/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteStreamNetworkDante
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStreamNetworkDante.h"
#include "OcaLiteNetworkSignalChannelDante.h"
#include "DanteLiteHostInterface.h"

// ---- Referenced classes and types ---

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_STREAMNETWORKDANTE_CLASSID};
const ::OcaLiteClassID          OcaLiteStreamNetworkDante::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStreamNetworkDante::OcaLiteStreamNetworkDante(::OcaONo objectNumber,
                                                     ::OcaBoolean lockable,
                                                     const ::OcaLiteString& role,
                                                     const ::OcaLiteNetworkDanteNodeID& idAdvertised)
    : ::OcaLiteStreamNetwork(objectNumber,
                             lockable,
                             role,
                             OCANETWORKLINKTYPE_ETHERNET_WIRED,
                             idAdvertised,
                             OCANETWORKCONTROLPROTOCOL_NONE,
                             OCANETWORKMEDIAPROTOCOL_DANTE),
    m_RxChannelsStartONo(0),
    m_RxChannelsEndONo(0),
    m_TxChannelsStartONo(0),
    m_TxChannelsEndONo(0),
    m_Initialized(false)
{
    // set the Host Interface with object number
    DanteLiteHostInterfaceSetStreamNetworkObject(objectNumber);
}

OcaLiteStreamNetworkDante::~OcaLiteStreamNetworkDante()
{
}

::OcaBoolean OcaLiteStreamNetworkDante::InitSignalChannels(::OcaONo startONo, ::OcaONo& endONo)
{
    bool bResult(false);
    if(m_Initialized)
        return false;
    if (::OcaLiteStreamNetwork::Initialize())
    {
        UINT16 nrRxChannels;
        UINT16 nrTxChannels;
        char name[32];
        bResult = DanteLiteHostInterfaceGetNumberOfChannels(nrRxChannels, nrTxChannels);
        if (bResult)
        {
            m_RxChannelsStartONo = startONo;
            m_RxChannelsEndONo = startONo;
            for (UINT16 channelCounter(0); (channelCounter < nrRxChannels) && bResult; channelCounter++)
            {
                ::OcaLiteList< ::OcaLiteEncoding> encodingList;
                UINT32 encodings(DanteLiteHostInterfaceGetEncodings(channelCounter, true));
                if (encodings & 0x1)
                {
                    encodingList.Add(OCAENCODING_PCM16);
                }
                if (encodings & 0x2)
                {
                    encodingList.Add(OCAENCODING_PCM24);
                }
                if (encodings & 0x4)
                {
                    encodingList.Add(OCAENCODING_PCM32);
                }
                sprintf(name, "SinkCh-%02d", channelCounter + 1);
                ::OcaLiteNetworkSignalChannelDante* rxChannel(new OcaLiteNetworkSignalChannelDante(static_cast< ::OcaONo>(startONo++),
                                                                                                   static_cast< ::OcaBoolean>(false),
                                                                                                   ::OcaLiteString(name),
                                                                                                   ::OcaLiteList< ::OcaLitePort>(),
                                                                                                   ::OcaLiteNetworkSignalChannelDanteID(::OcaLiteString(DanteLiteHostInterfaceGetDanteChannelName(channelCounter, true))),
                                                                                                   GetObjectNumber(),
                                                                                                   ::OCANETWORKMEDIASOURCEORSINK_SINK,
                                                                                                   encodingList,
                                                                                                   static_cast< ::OcaLiteFrequency>(DanteLiteHostInterfaceGetSampleRate(channelCounter, true)),
                                                                                                   static_cast< ::OcaUint16>(channelCounter)));

                if (::OcaLiteBlock::GetRootBlock().AddObject(*rxChannel))
                {
                    if (AddSignalSourceSink(*rxChannel))
                    {
                        const char* subscription(DanteLiteHostInterfaceGetDanteChannelSubscription(channelCounter));
                        if (NULL != subscription)
                        {
                            rxChannel->DanteRemoteChannelIDChanged(::OcaLiteNetworkSignalChannelDanteID(::OcaLiteString(std::string(subscription))));
                        }
                    }
                    else
                    {
                        bResult = false;
                    }
                }
                else
                {
                    bResult = false;
                }
                m_RxChannelsEndONo++;
            }

            m_TxChannelsStartONo = startONo;
            m_TxChannelsEndONo = startONo;
            for (UINT16 channelCounter(0); (channelCounter < nrTxChannels) && bResult; channelCounter++)
            {
                ::OcaLiteList< ::OcaLiteEncoding> encodingList;
                UINT32 encodings(DanteLiteHostInterfaceGetEncodings(channelCounter, false));
                if (encodings & 0x1)
                {
                    encodingList.Add(OCAENCODING_PCM16);
                }
                if (encodings & 0x2)
                {
                    encodingList.Add(OCAENCODING_PCM24);
                }
                if (encodings & 0x4)
                {
                    encodingList.Add(OCAENCODING_PCM32);
                }

                sprintf(name, "SourceCh-%02d", channelCounter + 1);
                ::OcaLiteNetworkSignalChannelDante* txChannel(new OcaLiteNetworkSignalChannelDante(static_cast< ::OcaONo>(startONo++),
                                                                                                   static_cast< ::OcaBoolean>(false),
                                                                                                   ::OcaLiteString(name),
                                                                                                   ::OcaLiteList< ::OcaLitePort>(),
                                                                                                   ::OcaLiteNetworkSignalChannelDanteID(::OcaLiteString(DanteLiteHostInterfaceGetDanteChannelName(channelCounter, false))),
                                                                                                   GetObjectNumber(),
                                                                                                   ::OCANETWORKMEDIASOURCEORSINK_SOURCE,
                                                                                                   encodingList,
                                                                                                   static_cast< ::OcaLiteFrequency>(DanteLiteHostInterfaceGetSampleRate(channelCounter, false)),
                                                                                                   static_cast< ::OcaUint16>(channelCounter)));

                if (::OcaLiteBlock::GetRootBlock().AddObject(*txChannel))
                {
                    if (!AddSignalSourceSink(*txChannel))
                    {
                        bResult = false;
                    }
                }
                else
                {
                    bResult = false;
                }
                m_TxChannelsEndONo++;
            }
        }
    }
    endONo = startONo;

    m_Initialized = true;
    return static_cast< ::OcaBoolean>(bResult);
}

void OcaLiteStreamNetworkDante::Teardown()
{
    OcaLiteStreamNetwork::Teardown();
}

::OcaLiteStatus OcaLiteStreamNetworkDante::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                                   ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response)
{
        ::OcaLiteStatus rc(OCASTATUS_PARAMETER_ERROR);
    if (!IsLocked(sessionID))
    {
        if (methodID.GetDefLevel() == CLASS_ID.GetFieldCount())
        {

            switch (methodID.GetMethodIndex())
            {
            case GET_MAX_RX_LATENCY:
            case GET_RX_LATENCY:
            case SET_RX_LATENCY:
            case CREATE_MULTICAST_SOURCE_FLOW:
            case DELETE_FLOW:
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
            rc = OcaLiteStreamNetwork::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaClassVersionNumber OcaLiteStreamNetworkDante::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteStreamNetwork::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

// Check our receive OcaLiteNetworkSignalChannelDante objects to see if they need updating
::OcaBoolean OcaLiteStreamNetworkDante::CheckforNewSubscription(UINT32 Channel, const char *NewSubscription)
{
    char nullSub[1] = {0};
    if(!m_Initialized)
        return false;
    // If there isn't a routing we'll get a NULL string which will cause an exception
    if(!NewSubscription)
        NewSubscription = nullSub;
    OcaLiteString newSub(NewSubscription);
    OcaLiteString subStr;
    for(::OcaONo Obj(m_RxChannelsStartONo); Obj < m_RxChannelsEndONo; Obj++) {
        // get the object
        OcaLiteNetworkSignalChannelDante *pSigChan = reinterpret_cast<OcaLiteNetworkSignalChannelDante *>(::OcaLiteBlock::GetRootBlock().GetObject(Obj));
        // is it the right channel number
        if(Channel == pSigChan->GetChannelNumber()) {
            // We've got the correct object, check the subscription
            pSigChan->GetSubscriptionName(subStr);
            // check for a subscription change
            if(subStr != newSub) {
                // subscription is different
                pSigChan->DanteRemoteChannelIDChanged(::OcaLiteNetworkSignalChannelDanteID(newSub));
                return true;
            }
        }
    }
    return false;
}

