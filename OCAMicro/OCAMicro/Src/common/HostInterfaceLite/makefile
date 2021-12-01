# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Makefile for the HostInterface
#

TARGETS = winnt stm32 macos

TYPE = LIB
NAME = HostInterfaceLiteLIB

USRINC = $(OCA_HOME)/Src; \
         $(OCA_HOME)/Src/inc;

SRC_INC = $(OCA_HOME)/Src/common/HostInterfaceLite/OCA/OCF \
        $(OCA_HOME)/Src/common/HostInterfaceLite/OCA/OCP.1

SRCS  = OcfLiteHostInterface.cpp \
        Ocp1LiteHostInterface.cpp

include $(OCA_HOME)/Src/makeCpp.inc
