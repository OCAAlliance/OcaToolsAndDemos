# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Makefile for the OCALite proto
#

NAME_MKE := makefile

TARGETS = macos

TYPE = EXE
NAME = OCALite

USRINC = $(OCA_HOME)/Src; \
         $(OCA_HOME)/Src/common; \
         $(OCA_HOME)/Src/inc; \
         $(OCA_HOME)/Src/common/OCALite; \
         $(OCA_HOME)/Src/platform;


# The order of the libs is important for the GNU linker!
# A library that depends on another library should before the library it depends on.
LIBS = OCALiteLIB \
       HostInterfaceLiteLIB

USRDEFS = OCA_RUN

SRC_INC = ./HostInterface/OCA/OCF/Configuration \
          ./HostInterface/OCA/OCF/Logging \
          ./HostInterface/OCA/OCF/Timer \
          ./HostInterface/OCA/OCF \
          ./HostInterface/OCA/OCP.1/Network \
          ./HostInterface/OCA/OCP.1/ZeroConf \
          ./HostInterface/OCA/OCP.1 

SRCS  = OCALite.cpp \
        Ocp1HostInterfaceOcaLite.cpp \
        OcaLiteOcp1Network.cpp \
        OcaLiteOcp1Socket.cpp \
        OcaLiteOcp1Service.cpp \
        OcaLiteOcfConfiguration.cpp \
        OcaLiteOcfLogger.cpp \
        OcaLiteOcfTimer.cpp 

include $(OCA_HOME)/Src/makeCpp.inc
