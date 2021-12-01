# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Makefile for Ocp1 Host Interface platform dependent elements
# 

TARGETS = stm32

TYPE = LIB
NAME = Ocp1HostInterfaceOcaLiteLIB

USRDEFS =

USRINC = $(OCA_HOME)/../Libraries/CMSIS/CM3/CoreSupport; \
         $(OCA_HOME)/../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F2xx; \
         $(OCA_HOME)/../Libraries/STM32F2x7_ETH_Driver/inc; \
         $(OCA_HOME)/../Libraries/STM32F2xx_StdPeriph_Driver/inc; \
         $(OCA_HOME)/../Utilities/Third_Party/FreeRTOSV7.1.1/include; \
         $(OCA_HOME)/../Utilities/Third_Party/FreeRTOSV7.1.1/portable/GCC/ARM_CM3; \
         $(OCA_HOME)/../Utilities/Third_Party/lwip-1.4.1/src/include; \
         $(OCA_HOME)/../Utilities/Third_Party/lwip-1.4.1/src/include/lwip; \
         $(OCA_HOME)/../Utilities/Third_Party/lwip-1.4.1/src/include/ipv4; \
         $(OCA_HOME)/../Utilities/Third_Party/lwip-1.4.1/port/STM32F2x7/FreeRTOS; \
         $(OCA_HOME)/../Utilities/Third_Party/lwip-1.4.1/port/STM32F2x7; \
         $(OCA_HOME)/../Application/inc;

SRC_INC = ./Network \
          ./ZeroConf

SRCS = OcaLiteOcp1Network.cpp \
       LwIPAdapterOcaLite.cpp \
       Ocp1HostInterfaceOcaLite.cpp \
       OcaLiteOcp1Socket.cpp \
       OcaLiteOcp1Service.cpp

include $(OCA_HOME)/Src/makeCpp.inc