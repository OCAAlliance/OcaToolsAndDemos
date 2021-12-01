# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
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
        stm32DebugClean stm32ReleaseClean \
        macosRelease macosReleaseClean

################################################################################
# Targets
################################################################################
goal: stm32Release macosRelease

all: stm32Release macosRelease

clean: stm32ReleaseClean macosReleaseClean

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

macosRelease:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=macosRelease
  else
	$(PREFIX)echo $@ not configured $(TARGETS)
  endif

macosReleaseClean:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=macosReleaseClean
  else
	$(PREFIX)echo macosRelease not configured
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

macosDebug:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=macosDebug
  else
	$(PREFIX)echo $@ not configured
  endif
 
macosDebugClean:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) phony TARGET=macosDebugClean
  else
	$(PREFIX)echo macosDebug not configured
  endif
  