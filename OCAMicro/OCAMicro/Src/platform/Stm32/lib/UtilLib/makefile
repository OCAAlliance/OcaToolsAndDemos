# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for LogicalDevices
# 

TARGETS = stm32

TYPE = LIB
NAME = UtilLibLIB

USRINC = $(OCA_HOME)/../Application/inc; \
         $(OCA_HOME)/../Libraries/CMSIS/CM3/CoreSupport; \
         $(OCA_HOME)/../Libraries/STM32F2x7_ETH_Driver/inc; \
         $(OCA_HOME)/../Libraries/STM32F2xx_StdPeriph_Driver/inc; \
         $(OCA_HOME)/../Utilities/Third_Party/FreeRTOSV7.1.1/include; \
         $(OCA_HOME)/../Utilities/Third_Party/FreeRTOSV7.1.1/portable/GCC/ARM_CM3; 

USRDEFS = STM32F437xx

SRCS = Rs232Debug.c \
       Timer.c \
       Watchdog.c

include $(OCA_HOME)/Src/makeCpp.inc
