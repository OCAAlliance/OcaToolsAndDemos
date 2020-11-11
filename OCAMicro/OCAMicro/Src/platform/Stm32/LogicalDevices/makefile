# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for LogicalDevices
# 

TARGETS = stm32

TYPE = LIB
NAME = LogicalDevicesLIB

USRINC = $(OCA_HOME)/../Application/inc; \
         $(OCA_HOME)/../Libraries/CMSIS/CM3/CoreSupport; \
         $(OCA_HOME)/../Libraries/STM32F2x7_ETH_Driver/inc; \
         $(OCA_HOME)/../Libraries/STM32F2xx_StdPeriph_Driver/inc; 

USRDEFS = STM32F437xx

SRC_INC = ClockGenerator \
          Switch

SRCS = ClockGenerator.cpp \
       Switch.cpp

include $(OCA_HOME)/Src/makeCpp.inc
