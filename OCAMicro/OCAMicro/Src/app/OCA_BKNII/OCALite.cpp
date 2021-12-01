/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

// OCALite.cpp : Defines the entry point for the console application.
//


#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteSubscriptionManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteFirmwareManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteMediaClockManager.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCP.1/Ocp1LiteNetwork.h>
#include <OCP.1/Ocp1LiteNetworkNodeID.h>
#include <OCP.1/Ocp1LiteNetworkSystemInterfaceID.h>

#include "OcaLiteStreamNetworkDante.h"
#include "DanteLiteHostInterface.h"

extern void Ocp1LiteServiceRun();

extern "C" {
    // user-defined application call-back for ConMon message monitoring
    // Application should override / replace as required.  See Audinate type
    // conmon_client_handle_monitoring_message_fn for the function definition
    void *GetDeviceStatusMessageCB(void)
    {
        return NULL;
    }

    void *GetDeviceMeteringMessageCB(void)
    {
        return NULL;
    }
}

extern OcaLiteStatus OCALiteOCCFactoryCreate();
OcaLiteStatus OCALiteOCCFactoryDestroy();

int main(int argc, const char* argv[])
{
    std::string manufacturer, name, version;
    char *devNameOCA;
    // set device information
    manufacturer.assign("OCAAlliance");
    name.assign("OCAMicroTestDevice");
    version.assign("1.0.0");
    OcfLiteConfigureSetModelGUID(0x123456, 0x01);
    OcfLiteConfigureSetModelDescription(manufacturer, name, version);
    // Initialize the host interfaces
    bool bSuccess = ::OcfLiteHostInterfaceInitialize();
    bSuccess = bSuccess && ::Ocp1LiteHostInterfaceInitialize();
    bSuccess = bSuccess && ::DanteLiteHostInterfaceInitialize();
    std::string devName = DanteLiteHostInterfaceGetDeviceName();

    devNameOCA = (char *)devName.c_str();
    printf("Trying to connect to <%s>\n", devNameOCA);
    bSuccess = bSuccess && ::DanteLiteHostInterfaceInitializeConMon(devNameOCA);

    static_cast<void>(::OcaLiteBlock::GetRootBlock());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteNetworkManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().SetNrEvents(1/*OCA_NR_EVENTS*/));
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteDeviceManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteFirmwareManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteMediaClockManager::GetInstance().Initialize());

    if (bSuccess)
    {
        Ocp1LiteNetworkSystemInterfaceID interfaceId = ::Ocp1LiteNetworkSystemInterfaceID(static_cast< ::OcaUint32>(0));
        std::vector<std::string> txtRecords;
        txtRecords.push_back("modelGUID=OCAMicroTestDevice01");
        ::OcaLiteString nodeId = ::OcaLiteString(devNameOCA);
        ::Ocp1LiteNetwork* ocp1Network = new ::Ocp1LiteNetwork(static_cast< ::OcaONo>(0x1000), static_cast< ::OcaBoolean>(true), 
                                                               ::OcaLiteString("Ocp1LiteNetwork"), ::Ocp1LiteNetworkNodeID(nodeId),
                                                               interfaceId, txtRecords, ::OcaLiteString("local"), OcaInt16(65000U));
        if (ocp1Network->Initialize())
        {
            OCALiteOCCFactoryCreate();
            if (::OcaLiteBlock::GetRootBlock().AddObject(*ocp1Network))
            {
                OcaLiteStatus rc(ocp1Network->Startup());
                if (OCASTATUS_OK == rc)
                {
                    bSuccess = bSuccess && ::OcaLiteCommandHandler::GetInstance().Initialize();
                    ::OcaLiteDeviceManager::GetInstance().SetErrorAndOperationalState(static_cast< ::OcaBoolean>(!bSuccess), ::OcaLiteDeviceManager::OCA_OPSTATE_OPERATIONAL);
                    ::OcaLiteDeviceManager::GetInstance().SetEnabled(static_cast< ::OcaBoolean>(bSuccess));

                    while (bSuccess)
                    {
                        ::OcaLiteCommandHandler::GetInstance().RunWithTimeout(100);
                        Ocp1LiteServiceRun();
                        DanteLiteHostInterfaceRun();
                    }
                }
            }
            OCALiteOCCFactoryDestroy();
        }
    }
    DanteLiteHostInterfaceStop();
    return 0;
}
