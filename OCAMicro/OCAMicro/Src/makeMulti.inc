# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located at http://ocaalliance.com/EULA as an original contracting party.
#
#  Description        :   Include file for multiple makefiles.
#

################################################################################
# Variables
################################################################################
ifeq ($(IGNORE_MAKE_ERRORS),Y)
  PREFIX := $(PREFIX)
else
  PREFIX := @
endif

################################################################################
# Phony target specification
################################################################################
.PHONY: goal all clean phony \
        uBlazeRelease uBlazeReleaseClean \
        stm32Debug stm32Release \
        stm32DebugClean stm32ReleaseClean

################################################################################
# Targets
################################################################################
goal: stm32Release

all: stm32Release

clean: stm32ReleaseClean

stm32Release:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=stm32Release
  else
	$(PREFIX)echo $@ not configured
  endif
 
stm32ReleaseClean:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=stm32ReleaseClean
  else
	$(PREFIX)echo stm32Release not configured
  endif
uBlazeRelease:
  ifneq (,$(findstring uBlaze,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=uBlazeRelease
  else
	$(PREFIX)echo $@ not configured
  endif

uBlazeReleaseClean:
  ifneq (,$(findstring uBlaze,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=uBlazeReleaseClean
  else
	$(PREFIX)echo uBlazeRelease not configured
  endif

stm32Debug:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=stm32Debug
  else
	$(PREFIX)echo $@ not configured
  endif
 
stm32DebugClean:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=stm32DebugClean
  else
	$(PREFIX)echo stm32Release not configured
  endif
  