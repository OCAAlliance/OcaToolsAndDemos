# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for FreeRTOS configured for OcaLite
# 

TARGETS = stm32

TYPE = LIB
NAME = LwIP_OcaLite

USRINC = $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include; \
         $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/include/ipv4; \
         $(OCA_HOME)/Src/common/SharedLibraries/LwIP/system; \
         $(OCA_HOME)/Src/common/SharedLibraries/LwIP/system/OS; \
         $(OCA_HOME)/Src/common/SharedLibraries/FreeRTOS/Source/include; \
         $(OCA_HOME)/Src/platform/Stm32/FreeRTOS/Stm32CortexM4; \
         $(OCA_HOME)/Src/platform/Stm32/lib/StdPeriphLib/STM32F4xx_HAL_Driver/inc; \
         $(OCA_HOME)/Src/platform/Stm32/lib/StdPeriphLib/CMSIS/inc; \
         $(OCA_HOME)/Src/platform/CMSIS/Include; \
         $(OCA_HOME)/Src/platform/CMSIS/RTOS_1.02; \
         Stm32CortexM4
         
USRDEFS = STM32F437xx

SRC_INC = $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/api \
          $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/core \
          $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/core/ipv4 \
          $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/core/ipv6 \
          $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/core/snmp \
          $(OCA_HOME)/Src/common/SharedLibraries/LwIP/src/netif \
          $(OCA_HOME)/Src/common/SharedLibraries/LwIP/system/OS \
          Stm32CortexM4

SRCS_API =  api_lib.c api_msg.c err.c netbuf.c netdb.c netifapi.c sockets.c tcpip.c
SRCS_CORE = def.c dhcp.c dns.c init.c lwip_timers.c mem.c memp.c netif.c pbuf.c raw.c stats.c sys.c tcp.c tcp_in.c tcp_out.c udp.c 
SRCS_IPV4 = autoip.c icmp.c igmp.c inet.c inet_chksum.c ip.c ip_addr.c ip_frag.c
SRCS_IPV6 = icmp6.c inet6.c ip6.c ip6_addr.c
SRCS_SNMP = asn1_dec.c asn1_enc.c mib2.c mib_structs.c msg_in.c msg_out.c
SRCS_ARP  = etharp.c
SRCS_SYS  = sys_arch.c

SRCS = \
        $(SRCS_API) \
        $(SRCS_CORE) \
        $(SRCS_IPV4) \
        $(SRCS_SNMP) \
        $(SRCS_ARP) \
        $(SRCS_SYS) \
        ethernetif.cpp
        
include $(OCA_HOME)/Src/makeCpp.inc
