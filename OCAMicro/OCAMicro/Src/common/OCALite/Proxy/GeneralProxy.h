/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The General Proxy class.
 */
#ifndef GENERAL_PROXY_H
#define GENERAL_PROXY_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteFrameworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteEventSubscriptionDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteBlobFixedLen.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----
class OcaLiteManagerDescriptor;
class OcaLiteBlockMember;
class OcaLiteEvent;
class OcaLiteMethod;
class OcaLiteNetworkAddress;
class OcaLiteDeviceManager;
class OcaLiteClassIdentification;
class OcaLiteObjectIdentification;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * This is a proxy exposing all functionality we need, no class structures etc.
 */
class GeneralProxy
{
public:
    /**
     * Constructor
     *
     * @param[in] sessionId             The session ID for which to create the proxy.
     * @param[in] networkObjectNumber   The network object number of the session ID.
     */
    GeneralProxy(::OcaSessionID sessionId, ::OcaONo networkObjectNumber);

    /**
     * Get the class identification from OcaRoot
     *
     * @param[in] remoteObjectNumber    The object number of the remote object.
     * @param[out] classIdentification  The class identification.
     *
     * @return The resulting status
     */
    ::OcaLiteStatus OcaRoot_GetClassIdentification(::OcaONo remoteObjectNumber, ::OcaLiteClassIdentification& classIdentification);

    /**
     * GetMembersRecursive from an OcaBlock
     *
     * @param[in] remoteObjectNumber        The object number of the remote object.
     * @param[out] members                  The list with members of this block.
     *
     * @return The resulting status.
     */
    ::OcaLiteStatus OcaBlock_GetMembersRecursive(::OcaONo remoteObjectNumber, ::OcaLiteList< ::OcaLiteBlockMember>& members);
    
    /**
     * GetMembers from an OcaBlock
     *
     * @param[in] remoteObjectNumber        The object number of the remote object.
     * @param[out] members                  The list with members of this block.
     *
     * @return The resulting status.
     */
    ::OcaLiteStatus OcaBlock_GetMembers(::OcaONo remoteObjectNumber, ::OcaLiteList< ::OcaLiteObjectIdentification>& members);

    /**
     * GetManager from the OcaDeviceManager
     *
     * @param[out] manager                  The list with manager of the OcaDeviceManager.
     *
     * @return The resulting status.
     */
    ::OcaLiteStatus OcaDeviceManager_GetManagers(::OcaLiteList< ::OcaLiteManagerDescriptor>& managers);

    /**
     * SetResetKey to the OcaDeviceManager.
     *
     * @param[in] resetKet          The reset key
     * @param[in] networkAddress    The network address to listen for device reset messages.
     *
     * @return The resulting status.
     */
    ::OcaLiteStatus OcaDeviceManager_SetResetKey(const ::OcaLiteBlobFixedLen<16>& resetKey, ::OcaLiteNetworkAddress networkAddress);
    
    /**
     * Send a clear reset cause command to the OcaDeviceManger
     *
     * @return The resulting status.
     */
    ::OcaLiteStatus OcaDeviceManager_ClearResetCause();

    /**
     * Get the reset cause from the remote OcaDeviceManager
     *
     * @param[out] resetCause   The reset cause.
     *
     * @return The resulting status.
     */
    ::OcaLiteStatus OcaDeviceManager_GetResetCause(::OcaLiteResetCause& resetCause);
    
    /**
     * Add a subscription to the passed event
     *
     * @param[in] ocaEvent      The event to subscribe on.
     * @param[in] subscriber    The subscriber (E.g. controller method to invoke).
     * @param[in] context       The context (not supported by all devices).
     * @param[in] deliveryMode  The delivery mode (E.g. fast, reliable).
     * @param[in] destInfo      The destination information.
     *
     * @return The resulting status
     */
    ::OcaLiteStatus OcaSubscriptionManager_AddSubscription(const OcaLiteEvent& ocaEvent,
                                                           const OcaLiteMethod& subscriber,
                                                           const OcaLiteBlob& context,
                                                           ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                           const ::OcaLiteNetworkAddress& destInfo);

    /**
     * Remove a subscription from the passed event
     *
     * @param[in] ocaEvent      The event to unsubscribe from.
     * @param[in] subscriber    The subscriber.
     *
     * @return The resulting status
     */
    ::OcaLiteStatus OcaSubscriptionManager_RemoveSubscription(const OcaLiteEvent& ocaEvent,
                                                              const OcaLiteMethod& subscriber);

    /**
     * Send an invalid call to the passed objectnumber / method id. This to verify the existence of the method.
     *
     * @param[in] objectNumber  The object number
     * @param[in] methodId      The method identifier
     *
     * @return The resulting status
     */
    ::OcaLiteStatus OcaCompliancyTestTool_InvalidCall(::OcaONo objectNumber, ::OcaLiteMethodID& methodId);

protected:
private:
    /** The session ID */
    ::OcaSessionID  m_sessionId;
    /** The network object number */
    ::OcaONo        m_networkObjectNumber;
    /** Buffer */
    ::OcaUint8      m_buffer[10*1024];
};

#endif //GENERAL_PROXY_H