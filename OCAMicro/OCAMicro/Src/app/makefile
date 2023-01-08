# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Makefile for applications
#

TARGETS = stm32 macos

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -C Stm32CortexM3 $(TARGET)
	@$(MAKE) -C OCA_PI -f makefileLIB $(TARGET)
	@$(MAKE) -C OCA_PI -f makefileBIN $(TARGET)
	@$(MAKE) -C OCALite $(TARGET)
	@$(MAKE) -C OCALiteController $(TARGET)

