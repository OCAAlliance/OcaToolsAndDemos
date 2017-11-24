# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for the OCALite proto
#

TARGETS = stmCortexM4

TYPE = DLL
NAME = OCALiteProto

BSP = OCAStm32F437xxOcaLiteBsp
BSPLD = "$(OCA_HOME)/Src/app/Stm32CortexM4/Board Support Packages/OCAStm32F437xxOcaLite/OCAStm32F437xxOcaLite-rom.ld"

USRINC = $(OCA_HOME)/Src/platform/Stm32CortexM4/lib/StdPeriphLib/STM32F4xx_HAL_Driver/inc; \
		 $(OCA_HOME)/Src/platform/CMSIS/Include; \
		 $(OCA_HOME)/Src/platform/CMSIS/RTOS_1.02; \
		 $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include; \
		 $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include/ipv4; \
		 $(OCA_HOME)/Src/common/SharedLibraries/LwIP/system; \
		 $(OCA_HOME)/Src/platform/common/OcaLite/LwIP/Stm32CortexM4; \
		 $(OCA_HOME)/Src/common/SharedLibraries/FreeRTOS/Source/include; \
		 $(OCA_HOME)/Src/platform/common/OcaLite/FreeRTOS/Stm32CortexM4
         
USRDEFS = STM32F437xx \
		  LWIP_NETIF_LINK_CALLBACK

# The order of the libs is important for the GNU linker!
# A library that depends on another library should before the library it depends on.
LIBS = \
       Ocp1Lite \
       OCFLite \
       OcaLiteManagers \
       OcaLiteAgents \
       OcaLiteWorkers \
       OcaLiteAgents \
       OcaLiteManagers \
       OcaLiteControlDataTypes \
       OcaLiteCore \
	   Ocp1LiteHostInterfaceInterface \
       Ocp1HostInterfaceOcaLite \
	   Ocp1LiteHostInterfaceInterface \
       OcfLiteHostInterfaceInterface \
	   OcfHostInterfaceOcaLite \
	   OcfLiteHostInterfaceInterface \
	   LogicalDevices \
	   pycrc32_small \
       mDNSd_tinyOcaLite \
	   LwIP_OcaLite \
	   CMSIS_OcaLite \
	   FreeRTOS_OcaLite \
	   UtilLib

FWLIBS = StdPeriphLibSTM32F437xx

WINNTLIBS = 

SRC_INC = 

SRCS  = OCALitePrototype.cpp \
		Stm32Interrupts.cpp \
		ReducedStdLib.cpp \
        newlib_stubs.c

include $(OCA_HOME)/Src/makeCpp.inc
