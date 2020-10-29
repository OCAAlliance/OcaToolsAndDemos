# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Makefile for Src
#

TARGETS = winnt stm32 macos

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -C common $(TARGET)
	@$(MAKE) -C Stm32 $(TARGET)
