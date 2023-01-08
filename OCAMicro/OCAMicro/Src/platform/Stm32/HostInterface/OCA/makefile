# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description         : Makefile for OCA hostinterface
#

TARGETS = stm32

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -f makefile -C OCF $(TARGET)
	@$(MAKE) -f makefile -C OCP.1 $(TARGET)
