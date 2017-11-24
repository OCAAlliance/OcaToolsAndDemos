# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for mDNSd configured for OcaLite
# 

TARGETS = stmCortexM4

TYPE = LIB
NAME = mDNSd_tinyOcaLite

USRINC = $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include; \
		 $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include/ipv4; \
		 $(OCA_HOME)/Src/common/SharedLibraries/LwIP/system; \
		 $(OCA_HOME)/Src/common/SharedLibraries/LwIP/system/OS; \
		 $(OCA_HOME)/Src/platform/common/OcaLite/LwIP/Stm32CortexM4; \
		 $(OCA_HOME)/Src/common/SharedLibraries/FreeRTOS/Source/include; \
		 $(OCA_HOME)/Src/platform/common/OcaLite/FreeRTOS/Stm32CortexM4; \
		 $(OCA_HOME)/Src/platform/Stm32CortexM4/lib/StdPeriphLib/STM32F4xx_HAL_Driver/inc; \
         $(OCA_HOME)/Src/platform/Stm32CortexM4/lib/StdPeriphLib/CMSIS/inc; \
		 $(OCA_HOME)/Src/platform/CMSIS/Include; \
		 $(OCA_HOME)/Src/platform/CMSIS/RTOS_1.02;
		 
USRDEFS = STM32F437xx

SRC_INC = $(OCA_HOME)/Src/common/SharedLibraries/tinymDNS

SRCS =  tinymDNSWrapper.cpp mdns.c mdnsd.c
		
include $(OCA_HOME)/Src/makeCpp.inc
