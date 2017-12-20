/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OCALite Proto Application
 *
 */

// ---- Include system wide include files ----
#include <stdio.h>
#include <stdarg.h>
//#include <sys/stat.h>


// common
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteSubscriptionManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteFirmwareManager.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCP.1/Ocp1LiteNetwork.h>
#include <OCP.1/Ocp1LiteNetworkNodeID.h>
#include <OCP.1/Ocp1LiteNetworkSystemInterfaceID.h>
#include <StandardLib/StandardLib.h>

// Local includes
#include "PILEDBitStringActuator.h"
#include "PIRELAYBooleanActuator.h"
#include "PISwitchBitStringSensor.h"
#include "PIencoderInt8Sensor.h"
#include "PISwitchLedBitstringActuator.h"

// platform
#include <common/OcaLite/LogicalDevices/ClockGenerator/ClockGenerator.h>
#include <common/OcaLite/LogicalDevices/Switch/Switch.h>
#include <common/OcaLite/HostInterface/OCA/OCP.1/LwIPAdapterOcaLite.h>
#include <Stm32CortexM3/lib/UtilLib/Rs232Debug.h>
#include <Stm32CortexM3/lib/UtilLib/Watchdog.h>
#include <common/OcaLite/tinymdns/tinymDNSWrapper.h>
#include "stm32f2xx.h"


#include "FreeRTOS.h"
#include "task.h"

static const int8_t UintTask[] = "UintTask";

::PISwitchBitStringSensor*  g_PI_Switches;
::PIencoderInt8Sensor*      g_PI_encoders[2];
::PISwitchLedBitstringActuator* g_PI_SwitchLeds;
// ---- Include local include files ----

// ---- Helper types and constants ----
/** The maximum watchdog time in ms. */
#define WATCHDOG_PERIOD                 (15000)

// ---- Helper functions ----
/** Threading .... */
static void UserInteractionThread(void * argument);

// ---- Local data ----

// ---- External data ----

// ---- Local functions ----

// --------------------------
int OCA_main(void)
{       
    Rs232DebugPrint("\r\n\r\nOCA Micro - the Lite Reference Design!\r\n");

    /* ==== Core and internal peripherals up and running! ==== */
    ::ClockGenerator::GetInstance().Initialize();
    ::ClockGenerator::GetInstance().Configure(0);

    bool bSuccess(false);
    ::Switch::GetInstance().Initialize(true, bSuccess);
    if (!bSuccess)
    {
        ::Rs232DebugPrint(">>> Failed to initialize switch (application processor sync)\r\n");
        while(1) {};
    }
    /* ==== Logical devices up and running! ==== */

    /* Start UserInteraction task */
//    osThreadDef(UserInteraction, UserInteractionThread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE * 12);
//    osThreadCreate(osThread(UserInteraction), NULL);
    (void) xTaskCreate(UserInteractionThread, UintTask, configMINIMAL_STACK_SIZE * 5, NULL, (tskIDLE_PRIORITY + 4), NULL);

    /* Start scheduler */
//    osKernelStart();
 
    /* We should never get here as control is now taken by the scheduler */
    return 0;
}

/**
  * @brief  User interaction task
  * @param  argument not used
  * @retval None
  */
static void UserInteractionThread(void * argument)
{ 

    ::PILEDBitStringActuator *m_Panel_LEDs[8];
    ::PIRELAYBooleanActuator *m_Panel_Relays[2];

    // Initiliaze platform
    bool bSuccess = ::LwIPAdapterOcaLite::GetInstance().Initialize();
    bSuccess = bSuccess && ::tinymDNSWrapper::GetInstance().Initialize();
    if (!bSuccess) { ::Rs232DebugPrint(">>> Failed to initialize platform layer\r\n"); }

    // Initialize the host interfaces
    bSuccess = bSuccess && ::OcfLiteHostInterfaceInitialize();
    bSuccess = bSuccess && ::Ocp1LiteHostInterfaceInitialize();
    if (!bSuccess) { ::Rs232DebugPrint(">>> Failed to initialize OCA interfaces\r\n"); }

    // Initialize Oca Device
    static_cast<void>(::OcaLiteBlock::GetRootBlock());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteNetworkManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().SetNrEvents(1/*OCA_NR_EVENTS*/));
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteDeviceManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteFirmwareManager::GetInstance().Initialize());
    if (!bSuccess) { ::Rs232DebugPrint(">>> Failed to initialize OCA layer\r\n"); }

    if (bSuccess)
    {
        char cPtr[25];
            
        PrintMacAsSerialNumber(cPtr, ::LwIPAdapterOcaLite::GetInstance().GetMacAddress());
        ::OcaLiteString nodeId = ::OcaLiteString("OCA_PI-") + ::OcaLiteString(&cPtr[strlen(cPtr) - 6]);

        Ocp1LiteNetworkSystemInterfaceID interfaceId = ::Ocp1LiteNetworkSystemInterfaceID(static_cast< ::OcaUint32>(0));
        std::vector<std::string> txtRecords;
        txtRecords.push_back("modelGUID=DEADBEEFEATERS");
        ::Ocp1LiteNetwork* ocp1Network = new ::Ocp1LiteNetwork(static_cast< ::OcaONo>(9000), static_cast< ::OcaBoolean>(true), 
                                                               ::OcaLiteString("Ocp1LiteNetwork"), ::Ocp1LiteNetworkNodeID(nodeId),
                                                               interfaceId, txtRecords, ::OcaLiteString("local"), OcaInt16(65000U));
        if (ocp1Network->Initialize())
        {
            if (::OcaLiteBlock::GetRootBlock().AddObject(*ocp1Network))
            {
                OcaLiteStatus rc(ocp1Network->Startup());
                if (OCASTATUS_OK != rc)
                {
                    ::Rs232DebugPrint(">>> Failed to startup network\r\n");
                    bSuccess = false;
                }
                if (true == bSuccess)
                {
                    unsigned char i;
                    for (i = 0; i < 8; i++)
                    {
                        m_Panel_LEDs[i] = new ::PILEDBitStringActuator(static_cast< ::OcaONo>(10000+i),OcaInt16(6), OcaUint16(i+1));
                        ::OcaLiteBlock::GetRootBlock().AddObject(*m_Panel_LEDs[i]);
                    }
                    
                    for (i = 0; i < 2; i++)
                    {
                        m_Panel_Relays[i] = new ::PIRELAYBooleanActuator(static_cast< ::OcaONo>(10100+i), OcaUint16(i+1));
                        ::OcaLiteBlock::GetRootBlock().AddObject(*m_Panel_Relays[i]);
                    }
                    g_PI_Switches = new ::PISwitchBitStringSensor(static_cast< ::OcaONo>(10200), OcaUint16(8));
                    ::OcaLiteBlock::GetRootBlock().AddObject(*g_PI_Switches);

                        
                    for (i = 0; i < 2; i++)
                    {
                        g_PI_encoders[i] = new ::PIencoderInt8Sensor(static_cast< ::OcaONo>(10300+i), -128, 127, i+1);
                        ::OcaLiteBlock::GetRootBlock().AddObject(*g_PI_encoders[i]);
                    }

                    g_PI_SwitchLeds = new ::PISwitchLedBitstringActuator(static_cast< ::OcaONo>(10400), OcaUint16(8));
                    ::OcaLiteBlock::GetRootBlock().AddObject(*g_PI_SwitchLeds);
                }
            }
            else
            {
                ::Rs232DebugPrint("Unable to register Ocp1Network with the rootblock");
                bSuccess = false;
            }
        }
        else
        {
            ::Rs232DebugPrint("Initialization of Ocp1Network failed");
            bSuccess = false;
        }
    }

    // Initialize the device command handler
    bSuccess = bSuccess && ::OcaLiteCommandHandler::GetInstance().Initialize();
    if (!bSuccess) { ::Rs232DebugPrint("Initialization of CommandHandler failed"); }

//    ::OcaLiteDeviceManager::GetInstance().SetErrorAndOperationalState(static_cast< ::OcaBoolean>(!bSuccess), ::OcaLiteDeviceManager::OperationalState::OCA_OPSTATE_OPERATIONAL);
    ::OcaLiteDeviceManager::GetInstance().SetErrorAndOperationalState(static_cast< ::OcaBoolean>(!bSuccess), ::OcaLiteDeviceManager::OCA_OPSTATE_OPERATIONAL);
    ::OcaLiteDeviceManager::GetInstance().SetEnabled(static_cast< ::OcaBoolean>(bSuccess));

    if (bSuccess)
    {
        WatchdogInit(15000);
        StartWatchdog();

//        UINT32 startTime(OcfLiteTimerGetTimerTickCount());
        UINT32 wdTime(OcfLiteTimerGetTimerTickCount());
        unsigned char i;
        while (true)
        {
            /* Kick watchdog each 10 seconds */
            if ((OcfLiteTimerGetTimerTickCount() - wdTime) >= 10000)
            {
                RefreshWatchdog();
                wdTime = OcfLiteTimerGetTimerTickCount();
            }

            /* Run the protocol  */
            ::OcaLiteCommandHandler::GetInstance().RunWithTimeout(100);
            tinymDNSWrapper::GetInstance().Run();

            /* Run the sensors to synchronize the events to the OCA task */
            g_PI_Switches->Run();
            for (i = 0; i < 2; i++)
            {
                g_PI_encoders[i]->Run();
            }
            /* Reschedule */
            vTaskDelay(0);
        }
    }
}


