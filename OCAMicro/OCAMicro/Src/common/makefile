# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for applications
#

TARGETS = winnt macos stm32

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -C HostInterfaceLite $(TARGET)
	@$(MAKE) -C OCALite $(TARGET)
	@$(MAKE) -C SharedLibraries $(TARGET)
