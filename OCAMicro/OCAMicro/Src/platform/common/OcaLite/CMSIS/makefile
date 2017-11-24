# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
# 
#  Description        :   Makefile for CMSIS configured for OcaLite
# 

TARGETS = stmCortexM4

TYPE = LIB
NAME = CMSIS_OcaLite

USRINC = $(OCA_HOME)/Src/common/SharedLibraries/FreeRTOS/Source/include; \
		 $(OCA_HOME)/Src/platform/CMSIS/Include; \
		 $(OCA_HOME)/Src/platform/common/OcaLite/FreeRTOS/Stm32CortexM4

USRDEFS = 

SRC_INC = $(OCA_HOME)/Src/platform/CMSIS/RTOS_1.02/FreeRTOS

SRCS = cmsis_os.c
       
include $(OCA_HOME)/Src/makeCpp.inc