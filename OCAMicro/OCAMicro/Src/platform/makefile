# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Makefile for platform
#

TARGETS = winnt stm32 macos

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -C common $(TARGET)
	@$(MAKE) -C Stm32 $(TARGET)
