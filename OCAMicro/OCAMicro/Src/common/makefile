# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Makefile for common
#

TARGETS = winnt macos stm32

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -C HostInterfaceLite $(TARGET)
	@$(MAKE) -C OCALite $(TARGET)
	@$(MAKE) -C SharedLibraries $(TARGET)
