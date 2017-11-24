# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for FreeRTOS configured for OcaLite
# 

TARGETS = stmCortexM4

TYPE = LIB
NAME = FreeRTOS_OcaLite

USRINC = $(OCA_HOME)/Src/common/SharedLibraries/FreeRTOS/Source/include; \
		 Stm32CortexM4
		 
USRDEFS = STM32F437xx __VFP_FP__

SRC_INC = $(OCA_HOME)/Src/common/SharedLibraries/FreeRTOS/Source \
		  Stm32CortexM4

SRCS = croutine.c \
       list.c \
       queue.c \
       tasks.c \
       timers.c \
	   port.c \
	   heap_4.c

include $(OCA_HOME)/Src/makeCpp.inc
