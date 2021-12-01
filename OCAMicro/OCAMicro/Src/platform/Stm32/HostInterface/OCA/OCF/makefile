# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Makefile for Ocf Host Interface
# 

TARGETS = stm32

TYPE = LIB
NAME = OcfHostInterfaceOcaLiteLIB

USRDEFS =

USRINC = $(OCA_HOME)/Src/platform/CMSIS/Include; \
         $(OCA_HOME)/Src/platform/CMSIS/RTOS_1.02; \
         $(OCA_HOME)/Src/common/SharedLibraries/FreeRTOS/Source/include; \
         $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include; \
         $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include/ipv4; \
         $(OCA_HOME)/Src/common/SharedLibraries/LwIP/system; \
         $(OCA_HOME)/Src/platform/Stm32/FreeRTOS/Stm32CortexM4; \
         $(OCA_HOME)/Src/platform/common/OcaLite/LwIP/Stm32CortexM4

SRC_INC = ./Configuration \
          ./Logging \
          ./Timer

SRCS = OcaLiteOcfConfiguration.cpp \
       OcaLiteOcfLogger.cpp \
       OcaLiteOcfTimer.cpp

include $(OCA_HOME)/Src/makeCpp.inc