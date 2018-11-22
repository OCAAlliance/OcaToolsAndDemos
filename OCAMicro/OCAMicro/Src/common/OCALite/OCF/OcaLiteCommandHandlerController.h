/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The OcaLiteCommandHandlerController class.
 */

#ifndef OCALITECOMMANDHANDLERCONTROLLER_H
#define OCALITECOMMANDHANDLERCONTROLLER_H

// ---- Include system wide include files ----
#include <OcaLite/OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>

// ---- Include local include files ----
#include "OcaLiteCommandHandler.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * The command handler controller.
 * Single threaded use only, can be used to setup a connection to one or more devices and semd commands to these devices.
 */
class OcaLiteCommandHandlerController : public ::OcaLiteCommandHandler
{
public:
	/**
     * Establish a connection to a remove device.
	 *
	 * @param[in] connectParameters		Information about the device to connect to.
	 * @param[in] networkONo			The object number of the network to be used to connect to.
	 *
	 * @return The session ID of the established connection.
	 */
	::OcaSessionID Connect(const OcaLiteConnectParameters& connectParameters, ::OcaONo networkONo);
	
	/**
	 * Esecute a disconnect. The actual disconnect will only happen in the next run.
	 *
	 * @param[in] sessionId		The session ID of the session to disconnect.
	 * @param[in] networkONo	The network object number.
	 */
	bool Disconnect(::OcaSessionID sessionId, ::OcaONo networkONo);

    /**
     * Send a command and wait for the response. Only a single command can be send at any give time.
     *
     * @param[in] sessionId             The session ID to use for the commands
     * @param[in] networkObjectNumber   The object number of the network to use.
     * @param[in] targetObjectNumber    The object number of the target device to invoke
     * @param[in] methodId              The method Id of the method of the target device to invoke
     * @param[in] parametersSize        The size (number of bytes) of the parameters to pass with the method.
     * @param[in] parameters            Pointer to an array of the length of parametersSize which contains the parameters.
     * @param[out] responseSize         The length of the response parameters. Only valid of response is OCASTATUS_OK.
     * @param[out] response             Pointer to an array containing the response with the length of responseSize. Valid till the next call to the OcaLiteCommandHandlerController.
     *
     * @return The result of the operation.
     */
	::OcaLiteStatus SendCommandWithResponse(::OcaSessionID sessionId,
											::OcaONo networkObjectNumber,
											::OcaONo targetObjectNumber,
											::OcaLiteMethodID methodId,
											::OcaUint32 parametersSize,
											::OcaUint8* parameters,
											::OcaUint32& responseSize,
											::OcaUint8** response);

    /**
     * Send a command WITHOUT response.
     *
     * @param[in] sessionId             The session ID to use for the commands
     * @param[in] networkObjectNumber   The object number of the network to use.
     * @param[in] targetObjectNumber    The object number of the target device to invoke
     * @param[in] methodId              The method Id of the method of the target device to invoke
     * @param[in] parametersSize        The size (number of bytes) of the parameters to pass with the method.
     * @param[in] parameters            Pointer to an array of the length of parametersSize which contains the parameters.
     *
     * @ return The result of the operation. This only tell something about the sending, nothing about the execution on the remote side.
     */
    ::OcaLiteStatus SendCommand(::OcaSessionID sessionId,
                                ::OcaONo networkObjectNumber,
                                ::OcaONo targetObjectNumber,
                                ::OcaLiteMethodID methodId,
                                ::OcaUint32 parametersSize,
                                ::OcaUint8* parameters);

	virtual ::OcaBoolean Initialize();

	virtual void Shutdown();

	/**
	 * Get the singleton instance.
	 *
	 * @return a reference to the singleton instance.
	 */
	static ::OcaLiteCommandHandlerController& GetInstance();

protected:

	/** Constructor */
	::OcaLiteCommandHandlerController();

    /** Destructor */
    ~OcaLiteCommandHandlerController();

	virtual void HandleResponse(const ::OcaLiteMessageResponse& response);

    /**
     * Generate a new handle to use.
     * 
     * @return A new handle
     */
	::OcaUint32 GenerateNewHandle();

private:

	/** Singleton */
	static ::OcaLiteCommandHandlerController* m_pSingleton;
	/* Parameter to track the last handle send so we can increment the value for uniqueness */
	::OcaUint32 m_handle;
	/** None zero if we are waiting for a response */
	::OcaUint32 m_waitingForHandle;
	/** Members to hold the response data. */
	::OcaUint32 m_responseSize;
	::OcaUint8* m_pResponseData;
    ::OcaLiteStatus m_reponseStatus;
	/** Boolen to indicate whether we received the response */
	bool m_bReceivedMessage;

	/** private copy constructor, no copying of object allowed */
	OcaLiteCommandHandlerController(const ::OcaLiteCommandHandlerController&);
	/** private assignment operator, no assignment of object allowed */
	::OcaLiteCommandHandlerController& operator=(const ::OcaLiteCommandHandlerController&);

};

#endif //OCALITECOMMANDHANDLERCONTROLLER_H