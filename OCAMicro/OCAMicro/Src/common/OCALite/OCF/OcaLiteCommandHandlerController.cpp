/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The OcaLiteCommandHandlerController.
 *
 */

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteCommandHandlerController.h"
#include "OCF/Messages/OcaLiteHeader.h"
#include "OCP.1/Messages/Ocp1LiteMessageCommand.h"
#include "HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h"

// ---- Helper types and constants ----
/** Default timeout in ms for a command with a response */
#define CMD_RRQ_TIMEOUT (15*1000)

/** The singleton reference to the server. */
::OcaLiteCommandHandlerController* OcaLiteCommandHandlerController::m_pSingleton(NULL);

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----
::OcaLiteCommandHandlerController::OcaLiteCommandHandlerController() :
   OcaLiteCommandHandler::OcaLiteCommandHandler(),
#ifdef _DEBUG
   m_handle(-1),
#else
   m_handle(static_cast< ::OcaUint32>(::OcfLiteTimerGetTimerTickCount())),
#endif
   m_waitingForHandle(0),
   m_responseSize(static_cast< ::OcaUint32>(0)),
   m_pResponseData(NULL),
   m_reponseStatus(OCASTATUS_INVALID_REQUEST),
   m_bReceivedMessage(false)
{
}

::OcaLiteCommandHandlerController::~OcaLiteCommandHandlerController()
{
    if (NULL != m_pResponseData)
    {
        delete[] m_pResponseData;
    }
}

::OcaSessionID OcaLiteCommandHandlerController::Connect(const OcaLiteConnectParameters& connectParameters, ::OcaONo networkONo)
{
   ::OcaSessionID sessionID(OCA_INVALID_SESSIONID);

   ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork(networkONo));
   if ((NULL != pNetwork) &&
      (pNetwork->GetObjectNumber() == networkONo))
   {
      sessionID = pNetwork->Connect(connectParameters);
   }

   return sessionID;
}

bool OcaLiteCommandHandlerController::Disconnect(::OcaSessionID sessionId, ::OcaONo networkONo)
{
   bool bResult(false);

   ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork(networkONo));
   if ((NULL != pNetwork) &&
      (pNetwork->GetObjectNumber() == networkONo))
   {
      bResult = pNetwork->Disconnect(sessionId);
   }

   return bResult;
}

::OcaLiteStatus OcaLiteCommandHandlerController::SendCommandWithResponse(::OcaSessionID sessionId,
                                                                         ::OcaONo networkObjectNumber,
                                                                         ::OcaONo targetObjectNumber,
                                                                         ::OcaLiteMethodID methodId,
                                                                         ::OcaUint32 parametersSize,
                                                                         ::OcaUint8* parameters,
                                                                         ::OcaUint32& responseSize,
                                                                         ::OcaUint8** response)
{
   ::OcaLiteStatus status(OCASTATUS_INVALID_REQUEST);

   if (0 == m_waitingForHandle)
   {
      // Create and write the message command.
      ::Ocp1LiteMessageCommand msg;
      m_waitingForHandle = GenerateNewHandle();
      msg.WriteParameters(m_waitingForHandle, targetObjectNumber, methodId, parametersSize);

      // Set the message type
      msg.SetMessageType(::OcaLiteHeader::OCA_MSG_CMD_RRQ);

      // Copy the paramters into the request
      ::memcpy(msg.GetParameters(), parameters, parametersSize);

      // Get the network
      ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork(networkObjectNumber));
      if ((NULL != pNetwork) &&
         (pNetwork->GetObjectNumber() == networkObjectNumber))
      {
         UINT32 beginTime(::OcfLiteTimerGetTimerTickCount());

         // Send the message
         status = pNetwork->SendOcaMessage(sessionId, msg);
         if (OCASTATUS_OK == status)
         {
             // Wait for the response OR timeout
             while (!m_bReceivedMessage &&
                 (::OcfLiteTimerGetTimerTickCount() < (beginTime + CMD_RRQ_TIMEOUT)))
             {
                 RunWithTimeout(CMD_RRQ_TIMEOUT / 100);
             }

             if (m_bReceivedMessage)
             {
                 // Response received
                 OCA_LOG_TRACE_PARAMS("Message with handle %d took %d", m_handle, ::OcfLiteTimerGetTimerTickCount() - beginTime);

                 responseSize = m_responseSize;
                 *response = m_pResponseData;
                 status = m_reponseStatus;
             }
             else
             {
                 // Timeout
                 OCA_LOG_ERROR_PARAMS("Timeout for request %d", m_handle);
                 status = OCASTATUS_TIMEOUT;
             }
         }
         else
         {
             OCA_LOG_ERROR_PARAMS("Failed to send the command. result = %d", status);
         }
      }

      m_bReceivedMessage = false;
      m_waitingForHandle = 0;
   }
   else
   {
      OCA_LOG_ERROR_PARAMS("Pending CommandWithResponse with handle %d", m_handle);
   }

   return status;
}

::OcaLiteStatus OcaLiteCommandHandlerController::SendCommand(::OcaSessionID sessionId,
                                                             ::OcaONo networkObjectNumber,
                                                             ::OcaONo targetObjectNumber,
                                                             ::OcaLiteMethodID methodId,
                                                             ::OcaUint32 parametersSize,
                                                             ::OcaUint8* parameters)
{
    ::OcaLiteStatus status(OCASTATUS_INVALID_REQUEST);

    if (0 == m_waitingForHandle)
    {
        // Create and write the message command.
        ::Ocp1LiteMessageCommand msg;
        m_waitingForHandle = GenerateNewHandle();
        msg.WriteParameters(m_waitingForHandle, targetObjectNumber, methodId, parametersSize);

        // Set the message type
        msg.SetMessageType(::OcaLiteHeader::OCA_MSG_CMD);

        // Copy the paramters into the request
        ::memcpy(msg.GetParameters(), parameters, parametersSize);

        // Get the network
        ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork(networkObjectNumber));
        if ((NULL != pNetwork) &&
            (pNetwork->GetObjectNumber() == networkObjectNumber))
        {
            // Send the message
            status = pNetwork->SendOcaMessage(sessionId, msg);

            m_waitingForHandle = 0;
        }
        else
        {
            OCA_LOG_ERROR_PARAMS("Pending CommandWithResponse with handle %d", m_handle);
        }
    }

    return status;
}

void OcaLiteCommandHandlerController::HandleResponse(const ::OcaLiteMessageResponse& response)
{
    if (m_waitingForHandle == response.GetHandle())
    {
        if (NULL != m_pResponseData)
        {
            delete[] m_pResponseData;
        }
        m_pResponseData = new OcaUint8[response.GetParametersSize()];

        ::memcpy(m_pResponseData, response.GetParameters(), response.GetParametersSize());
        m_responseSize = response.GetParametersSize();
        m_reponseStatus = response.GetStatus();

    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Received response on handle %d while waiting for handle %d",
            response.GetHandle(), m_waitingForHandle);
    }
    m_bReceivedMessage = true;
}

 ::OcaUint32 OcaLiteCommandHandlerController::GenerateNewHandle()
 {
    ++m_handle;
    while (m_handle == static_cast< ::OcaUint32>(0))
    {
       ++m_handle;
    }
    return m_handle;
 }

::OcaBoolean OcaLiteCommandHandlerController::Initialize()
{
   ::OcaBoolean bResult(::OcaLiteCommandHandler::Initialize());

   return bResult;
}

void OcaLiteCommandHandlerController::Shutdown()
{
   ::OcaLiteCommandHandler::Shutdown();
}

::OcaLiteCommandHandlerController& ::OcaLiteCommandHandlerController::GetInstance()
{
   if (NULL == m_pSingleton)
   {
      m_pSingleton = new ::OcaLiteCommandHandlerController;
      assert(NULL == ::OcaLiteCommandHandler::m_pSingleton);
      ::OcaLiteCommandHandler::m_pSingleton = m_pSingleton;
   }
   return *m_pSingleton;
}
