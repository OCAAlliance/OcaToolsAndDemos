# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description         : Makefile for OCA
#

TARGETS = winnt stmCortexM4

include $(OCA_HOME)/Src/makeMulti.inc

phony:
	@$(MAKE) -f makefile -C OCF $(TARGET)
	@$(MAKE) -f makefile -C OCP.1 $(TARGET)
