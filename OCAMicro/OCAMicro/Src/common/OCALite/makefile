# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for the OCALite proto
#

TARGETS = stm32 macos

TYPE = LIB
NAME = OCALiteLIB

USRINC = $(OCA_HOME)/Src; \
         $(OCA_HOME)/Src/common; \
         $(OCA_HOME)/Src/inc; \
         $(OCA_HOME)/Src/common/OCALite;

#timers.c is in FreeRTOS and lwip, we need lwip, so first src_inc lwip
SRC_INC = $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Agents \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Managers \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Networks \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Workers \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Workers/Actuators \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Workers/BlocksAndMatrices \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Workers/Sensors \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses/Workers/Networking \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlClasses \
        $(OCA_HOME)/Src/common/OCALite/OCC/ControlDataTypes \
        $(OCA_HOME)/Src/common/OCALite/OCF \
        $(OCA_HOME)/Src/common/OCALite/OCF/Messages \
        $(OCA_HOME)/Src/common/OCALite/OCP.1 \
        $(OCA_HOME)/Src/common/OCALite/OCP.1/Messages \
        $(OCA_HOME)/Src/common/OCALite/Proxy

SRCS  = OcaLiteAgent.cpp \
        OcaLiteMediaClock.cpp \
        OcaLiteMediaClock3.cpp \
        OcaLiteNetwork.cpp \
        OcaLiteStreamConnector.cpp \
        OcaLiteStreamNetwork.cpp \
        OcaLiteTimeSource.cpp \
        \
        OcaLiteDeviceManager.cpp \
        OcaLiteFirmwareManager.cpp \
        OcaLiteManager.cpp \
        OcaLiteMediaClockManager.cpp \
        OcaLiteNetworkManager.cpp \
        OcaLiteSubscriptionManager.cpp \
        \
        OcaLiteApplicationNetwork.cpp \
        OcaLiteMediaTransportNetwork.cpp \
        OcaLiteMediaTransportNetworkAES67.cpp \
        \
        OcaLiteWorker.cpp \
        \
        OcaLiteActuator.cpp \
        OcaLiteBasicActuator.cpp \
        OcaLiteBitstringActuator.cpp \
        OcaLiteBooleanActuator.cpp \
        OcaLiteFloat32Actuator.cpp \
        OcaLiteGain.cpp \
        OcaLiteIdentificationActuator.cpp \
        OcaLiteInt32Actuator.cpp \
        OcaLiteStringActuator.cpp \
        OcaLiteSwitch.cpp \
        \
        OcaLiteBlock.cpp \
        \
        OcaLiteNetworkSignalChannel.cpp \
        \
        OcaLiteBasicSensor.cpp \
        OcaLiteBitstringSensor.cpp \
        OcaLiteBooleanSensor.cpp \
        OcaLiteFloat32Sensor.cpp \
        OcaLiteInt8Sensor.cpp \
        OcaLiteInt16Sensor.cpp \
        OcaLiteInt32Sensor.cpp \
        OcaLiteInt64Sensor.cpp \
        OcaLiteLevelSensor.cpp \
        OcaLiteSensor.cpp \
        OcaLiteStringSensor.cpp \
        OcaLiteTemperatureSensor.cpp \
        OcaLiteUint8Sensor.cpp \
        OcaLiteUint16Sensor.cpp \
        OcaLiteUint32Sensor.cpp \
        OcaLiteUint64Sensor.cpp \
        \
        OcaLiteRoot.cpp \
        \
        OcaLiteAES67NetworkAddress.cpp \
        OcaLiteApplicationNetworkDataTypes.cpp \
        OcaLiteBitstring.cpp \
        OcaLiteBlob.cpp \
        OcaLiteBlobDataType.cpp \
        OcaLiteBlobFixedLen.cpp \
        OcaLiteBlockMember.cpp \
        OcaLiteClassID.cpp \
        OcaLiteClassIdentification.cpp \
        OcaLiteConnectParameters.cpp \
        OcaLiteEvent.cpp \
        OcaLiteEventData.cpp \
        OcaLiteEventID.cpp \
        OcaLiteEventSubscriptionDataTypes.cpp \
        OcaLiteFrameworkDataTypes.cpp \
        OcaLiteList.cpp \
        OcaLiteManagementDataTypes.cpp \
        OcaLiteManagerDescriptor.cpp \
        OcaLiteMap.cpp \
        OcaLiteMediaClockRate.cpp \
        OcaLiteMediaCoding.cpp \
        OcaLiteMediaConnection.cpp \
        OcaLiteMediaConnectorStatus.cpp \
        OcaLiteMediaConnectorStatusChangedEventData.cpp \
        OcaLiteMediaSinkConnector.cpp \
        OcaLiteMediaSinkConnectorChangedEventData.cpp \
        OcaLiteMediaSourceConnector.cpp \
        OcaLiteMediaSourceConnectorChangedEventData.cpp \
        OcaLiteMediaStreamParameters.cpp \
        OcaLiteMediaStreamParametersAes67.cpp \
        OcaLiteMethod.cpp \
        OcaLiteMethodID.cpp \
        OcaLiteModelDescription.cpp \
        OcaLiteModelGUID.cpp \
        OcaLiteMultiMap.cpp \
        OcaLiteNetworkAddress.cpp \
        OcaLiteNetworkDataTypes.cpp \
        OcaLiteNetworkStatistics.cpp \
        OcaLiteNetworkSystemInterfaceDescriptor.cpp \
        OcaLiteNetworkSystemInterfaceID.cpp \
        OcaLiteObjectIdentification.cpp \
        OcaLitePort.cpp \
        OcaLitePortID.cpp \
        OcaLitePropertyChangedEventData.cpp \
        OcaLitePropertyID.cpp \
        OcaLiteStream.cpp \
        OcaLiteStreamConnectorID.cpp \
        OcaLiteStreamID.cpp \
        OcaLiteStreamParameters.cpp \
        OcaLiteString.cpp \
        OcaLiteStringInABlob.cpp \
        OcaLiteTemplateHelpers.cpp \
        OcaLiteVersion.cpp \
        OcaLiteWorkerDataTypes.cpp \
        \
        OcaLiteCommandHandler.cpp \
        OcaLiteCommandHandlerController.cpp \
        \
        OcaLiteHeader.cpp \
        OcaLiteMessageCommand.cpp \
        OcaLiteMessageGeneral.cpp \
        OcaLiteMessageKeepAlive.cpp \
        OcaLiteMessageNotification.cpp \
        OcaLiteMessageResponse.cpp \
        \
        Ocp1LiteConnectParameters.cpp \
        Ocp1LiteNetwork.cpp \
        Ocp1LiteUdpNetwork.cpp \
        Ocp1LiteNetworkAddress.cpp \
        Ocp1LiteNetworkSystemInterfaceID.cpp \
        Ocp1LiteReader.cpp \
        Ocp1LiteSocketConnection.cpp \
        Ocp1LiteUdpSocketConnection.cpp \
        Ocp1LiteWriter.cpp \
        \
        Ocp1LiteMessageCommand.cpp \
        Ocp1LiteMessageHeader.cpp \
        Ocp1LiteMessageKeepAlive.cpp \
        Ocp1LiteMessageNotification.cpp \
        Ocp1LiteMessageResponse.cpp \
        \
        GeneralProxy.cpp

include $(OCA_HOME)/Src/makeCpp.inc
