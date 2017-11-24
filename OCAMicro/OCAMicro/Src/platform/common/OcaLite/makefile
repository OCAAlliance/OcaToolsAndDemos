# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description         : Makefile for OCA Lite hostinterface
#

TARGETS = winnt stmCortexM4

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -C HostInterface $(TARGET)
	@$(MAKE) -C LogicalDevices $(TARGET)
	@$(MAKE) -C CMSIS $(TARGET)
	@$(MAKE) -C FreeRTOS $(TARGET)
	@$(MAKE) -C LwIP $(TARGET)
	@$(MAKE) -C tinymdns $(TARGET)
