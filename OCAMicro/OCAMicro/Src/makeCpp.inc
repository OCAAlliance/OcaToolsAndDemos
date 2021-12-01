# By downloading or using this file, the user agrees to be bound by the terms of the license 
# agreement located in the LICENSE file in the root of this project
# as an original contracting party.
#
#  Description        :   Include file for C / CPP makefiles.
#

################################################################################
# Environment variable checking
################################################################################
#
# If one of the variables below is not set,
# calling make will result in a "missing seperator" error.
#
ifeq ($(OCA_HOME),)
  variable OCA_HOME is not set
endif

################################################################################
# Variables
################################################################################
ifeq ($(NAME_MKE),)
  NAME_MKE := makefile
endif
ifeq ($(IGNORE_MAKE_ERRORS),Y)
  PREFIX := -@
else
  ifneq ($(DISPLAY_MAKE_CMDS),Y)
    PREFIX := @
  endif
endif
#
# Only set variables when the makefile is called with an actual target
# (not a phony one).
#
ifneq ($(PLATFORM),)
  ifneq ($(RELEASE),)

    # Project home
    PRJ_HOME := $(OCA_HOME)

    # Src home
    SRC_HOME := $(PRJ_HOME)/Src

    # Include directories
    ifneq ($(USRINC),)
    INCDIRS := $(subst ; ,;,$(subst \,/,.;\
        $(SRC_HOME)/inc;\
        $(SRC_HOME)/lib/inc;\
        $(SRC_HOME)/common;\
        $(SRC_HOME)/common/OCALite;\
        $(SRC_HOME)/common/HostInterfaceLite;\
        $(SRC_HOME)/platform;\
        $(SRC_HOME)/platform/common;\
        $(USRINC)))
    else
    INCDIRS := $(subst ; ,;,$(subst \,/,.;\
        $(SRC_HOME)/inc;\
        $(SRC_HOME)/lib/inc;\
        $(SRC_HOME)/common;\
        $(SRC_HOME)/common/OCALite;\
        $(SRC_HOME)/common/HostInterfaceLite;\
        $(SRC_HOME)/platform;\
        $(SRC_HOME)/platform/common))
	endif

    # Output directories
    OBJDIR := Obj
    OBJINT := intermediate
    OUTDIR := $(subst \,/,$(PRJ_HOME))/$(OBJDIR)/$(PLATFORM)/$(RELEASE)
    INTDIR := $(OUTDIR)/$(OBJINT)/$(NAME)

    # Tools
    ifeq ($(OS),Windows_NT)
        SD := gcc -M
        SE := sed
        RM := erase /F
        MD := mkdir
        RN := ren _PATH__OLDNAME_ _NEWNAME_
        RD := rd /s/q
        RC :=
    else
        SD := gcc -M
        SE := sed
        RM := rm -f
        MD := mkdir -p
        RN := mv _PATH__OLDNAME_ _PATH__NEWNAME_
        RD := rm -rf
        RC :=
    endif

    # Commands
    ifeq ($(OS),Windows_NT)
        IFEXIST    := if exist _FILE_ _COMMAND_
        IFNOTEXIST := if not exist _FILE_ _COMMAND_
    else
        IFEXIST    := if [ -e "_FILE_" ]; then _COMMAND_; fi
        IFNOTEXIST := if [ ! -e "_FILE_" ]; then _COMMAND_; fi
    endif

    # Other OS dependent variables
    # Work around to make sure only a single backslash is set
    ifeq ($(OS),Windows_NT)
        PATH_SEP := $(subst \\,\,\\)
    else
        PATH_SEP := $(subst \\,/,/)
    endif

    ifeq ($(PLATFORM),macosApp)

### macosRelease ##################################################################

      ifeq ($(RELEASE),Release)

        # Determine the extensions for the target platform
        OBJ_EXT := o
        DEP_EXT := dep
        EXE_EXT :=
        LIB_EXT := a
        DLL_EXT := so

        # Lists
        OBJS := $(patsubst %.cpp,%.$(OBJ_EXT),$(SRCS))
        OBJS := $(patsubst %.c,%.$(OBJ_EXT),$(OBJS))
        DEPS := $(patsubst %.$(OBJ_EXT),%.$(DEP_EXT),$(OBJS))
        LIBS := $(LIBS) $(FWLIBS)
        LIBS := $(addsuffix .$(LIB_EXT), $(LIBS))
        BSP := $(addsuffix .$(LIB_EXT), $(BSP))

        SDFLAGS := -g -c \
                   -Wall -x none -I"$(subst ;," -I",$(INCDIRS))" \
                   -D"NDEBUG" -DTARGET_APPLE \
                   $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
                   $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)")
        ifeq ($(OS),Windows_NT)
            CCFLAGS = $(SDFLAGS) -o $(INTDIR)\$@
        else
            CCFLAGS = $(SDFLAGS) -o $(INTDIR)/$@
        endif
        CXXFLAGS := -std=gnu++0x
        ifeq ($(TYPE),DLL)
          OUT_EXT := $(EXE_EXT)
          BIN_EXT := bin
          LDFLAGS = -g -Os -Xlinker -Map=$@.map --stats -Wl,--gc-sections -fno-keep-static-consts -T $(BSPLD) -o$@
        endif
        ifeq ($(TYPE),EXE)
          LD := g++
          OUT_EXT := $(EXE_EXT)
          LDFLAGS = -o $@
        endif
        ifeq ($(TYPE),LIB)
          LD := ar
          OUT_EXT := $(LIB_EXT)
          ifeq ($(OS),Windows_NT)
              ASFLAGS = -g -x assembler-with-cpp -c $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUN DEFS),-U"$(USRUNDEF)") -o $(INTDIR)\$@
          else
              ASFLAGS = -g -x assembler-with-cpp -c $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)") -o $(INTDIR)/$@
          endif
          LDFLAGS = crus $@
        endif
      endif

      ifeq ($(RELEASE),Debug)

        # Tools

        # Determine the extensions for the target platform
        OBJ_EXT := o
        DEP_EXT := dep
        EXE_EXT :=
        LIB_EXT := a
        DLL_EXT := so

        # Lists
        OBJS := $(patsubst %.cpp,%.$(OBJ_EXT),$(SRCS) $(STM32SRCS))
        OBJS := $(patsubst %.c,%.$(OBJ_EXT),$(OBJS))
        DEPS := $(patsubst %.$(OBJ_EXT),%.$(DEP_EXT),$(OBJS))
        LIBS := $(LIBS) $(FWLIBS)
        LIBS := $(addsuffix .$(LIB_EXT), $(LIBS))
        BSP := $(addsuffix .$(LIB_EXT), $(BSP))

        SDFLAGS := -g -c \
                   -Wall -x none -I"$(subst ;," -I",$(INCDIRS))" \
                   -D"NDEBUG" -DTARGET_APPLE \
                   $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
                   $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)")
        ifeq ($(OS),Windows_NT)
            CCFLAGS = $(SDFLAGS) -o $(INTDIR)\$@
        else
            CCFLAGS = $(SDFLAGS) -o $(INTDIR)/$@
        endif
        CXXFLAGS := -std=gnu++0x
        ifeq ($(TYPE),DLL)
          OUT_EXT := $(EXE_EXT)
          BIN_EXT := bin
          LDFLAGS = -g -O0 -Xlinker -Map=$@.map --stats -Wl,--gc-sections -fno-keep-static-consts -T $(BSPLD) -o$@
        endif
        ifeq ($(TYPE),EXE)
          LD := clang++
          OUT_EXT := $(EXE_EXT)
          LDFLAGS = -print-search-dirs -L $(OUTDIR) -o $@
        endif
        ifeq ($(TYPE),LIB)
          LD := ar
          OUT_EXT := $(LIB_EXT)
          ifeq ($(OS),Windows_NT)
              ASFLAGS = -g -x assembler-with-cpp -c $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)") -o $(INTDIR)\$@
          else
              ASFLAGS = -g -x assembler-with-cpp -c $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)") -o $(INTDIR)/$@
          endif
          LDFLAGS = crus $@
        endif
      endif

    endif #ifeq ($(PLATFORM),macosApp)

    ifeq ($(PLATFORM),stm32App)

### stm32Release ##################################################################

      ifeq ($(RELEASE),Release)

        # Tools
        AS := arm-none-eabi-gcc
        CC := arm-none-eabi-g++
        SD := arm-none-eabi-gcc -M
        B2B := arm-none-eabi-objcopy

        # Determine the extensions for the target platform
        OBJ_EXT := o
        DEP_EXT := dep
        EXE_EXT := out
        LIB_EXT := a

        # Lists
        OBJS := $(patsubst %.cpp,%.$(OBJ_EXT),$(SRCS) $(STM32SRCS))
        OBJS := $(patsubst %.c,%.$(OBJ_EXT),$(OBJS))
        OBJS := $(patsubst %.S,%.$(OBJ_EXT),$(OBJS))
        OBJS := $(patsubst %.lzma,%.$(OBJ_EXT),$(OBJS))
        DEPS := $(patsubst %.$(OBJ_EXT),%.$(DEP_EXT),$(OBJS))
        LIBS := $(LIBS) $(FWLIBS)
        LIBS := $(addsuffix .$(LIB_EXT), $(LIBS))
        BSP := $(addsuffix .$(LIB_EXT), $(BSP))

        SDFLAGS := -g -Os -nostdlib -fno-strict-aliasing -fno-exceptions -ffunction-sections -fdata-sections -c \
                   -Wall -x none -I"$(subst ;," -I",$(INCDIRS))" \
                   -D"NDEBUG" -DTARGET_STM32 \
                   $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
                   $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)")
        ifeq ($(OS),Windows_NT)
            CCFLAGS = $(SDFLAGS) -mcpu=cortex-m3 -mthumb -o $(INTDIR)\$@
        else
            CCFLAGS = $(SDFLAGS) -mcpu=cortex-m3 -mthumb -o $(INTDIR)/$@
        endif
        CXXFLAGS := -std=gnu++0x
        ifeq ($(TYPE),DLL)
          LD := arm-none-eabi-g++
          OUT_EXT := $(EXE_EXT)
          BIN_EXT := bin
          LDFLAGS = -g -Os -Xlinker -Map=$@.map -mcpu=cortex-m3 -mthumb --stats \
					 -Wl,--gc-sections -fno-keep-static-consts -T $(BSPLD) -o$@
          B2BFLAGS = -O binary
        endif
        ifeq ($(TYPE),EXE)
          LD := arm-none-eabi-g++
          OUT_EXT := $(EXE_EXT)
          BIN_EXT := bin
          LDFLAGS = -g -Xlinker -Map=$@.map -mcpu=cortex-m3 -mthumb --stats \
					 -Wl,--gc-sections -fno-keep-static-consts -T $(BSPLD) -o$@
          B2BFLAGS = -O binary
        endif
        ifeq ($(TYPE),LIB)
          LD := arm-none-eabi-ar
          OUT_EXT := $(LIB_EXT)
          ifeq ($(OS),Windows_NT)
              ASFLAGS = -g -x assembler-with-cpp -c -mcpu=cortex-m3 -mthumb $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)") -o $(INTDIR)\$@
          else
              ASFLAGS = -g -x assembler-with-cpp -c -mcpu=cortex-m3 -mthumb $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)") -o $(INTDIR)/$@
          endif
          LDFLAGS = crus $@
        endif
      endif

      ifeq ($(RELEASE),Debug)

        # Tools
        AS := arm-none-eabi-gcc
        CC := arm-none-eabi-g++
        SD := arm-none-eabi-gcc -M
        B2B := arm-none-eabi-objcopy

        # Determine the extensions for the target platform
        OBJ_EXT := o
        DEP_EXT := dep
        EXE_EXT := out
        LIB_EXT := a

        # Lists
        OBJS := $(patsubst %.cpp,%.$(OBJ_EXT),$(SRCS) $(STM32SRCS))
        OBJS := $(patsubst %.c,%.$(OBJ_EXT),$(OBJS))
        OBJS := $(patsubst %.S,%.$(OBJ_EXT),$(OBJS))
        OBJS := $(patsubst %.lzma,%.$(OBJ_EXT),$(OBJS))
        DEPS := $(patsubst %.$(OBJ_EXT),%.$(DEP_EXT),$(OBJS))
        LIBS := $(LIBS) $(FWLIBS)
        LIBS := $(addsuffix .$(LIB_EXT), $(LIBS))
        BSP := $(addsuffix .$(LIB_EXT), $(BSP))

        SDFLAGS := -g -O0 -nostdlib -fno-strict-aliasing -fno-exceptions -ffunction-sections -fdata-sections -c \
                   -Wall -x none -I"$(subst ;," -I",$(INCDIRS))" \
                   -D"NDEBUG" -DTARGET_STM32 \
                   $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
                   $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)")
        ifeq ($(OS),Windows_NT)
            CCFLAGS = $(SDFLAGS) -mcpu=cortex-m3 -mthumb -o $(INTDIR)\$@
        else
            CCFLAGS = $(SDFLAGS) -mcpu=cortex-m3 -mthumb -o $(INTDIR)/$@
        endif
        CXXFLAGS := -std=gnu++0x
        ifeq ($(TYPE),DLL)
          LD := arm-none-eabi-g++
          OUT_EXT := $(EXE_EXT)
          BIN_EXT := bin
          LDFLAGS = -g -O0 -Xlinker -Map=$@.map -mcpu=cortex-m3 -mthumb --stats \
					 -Wl,--gc-sections -fno-keep-static-consts -T $(BSPLD) -o$@
          B2BFLAGS = -O binary
        endif
        ifeq ($(TYPE),EXE)
          LD := arm-none-eabi-g++
          OUT_EXT := $(EXE_EXT)
          BIN_EXT := bin
          LDFLAGS = -Xlinker -Map=$@.map -mcpu=cortex-m3 -mthumb --stats \
					 -Wl,--gc-sections -fno-keep-static-consts -T $(BSPLD) -o$@
          B2BFLAGS = -O binary
        endif
        ifeq ($(TYPE),LIB)
          LD := arm-none-eabi-ar
          OUT_EXT := $(LIB_EXT)
          ifeq ($(OS),Windows_NT)
              ASFLAGS = -g -x assembler-with-cpp -c -mcpu=cortex-m3 -mthumb $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)") -o $(INTDIR)\$@
          else
              ASFLAGS = -g -x assembler-with-cpp -c -mcpu=cortex-m3 -mthumb $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
					 $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)") -o $(INTDIR)/$@
          endif
          LDFLAGS = crus $@
        endif
      endif

    endif #ifeq ($(PLATFORM),stm32App)

    ifeq ($(PLATFORM),uBlazeApp)

### uBlaze #################################################################

      ifeq ($(RELEASE),Release)

        PATH_SEP := $(subst \\,/,/)
        LSTFILE := $(subst \,/,$(INTDIR)/obj-list.lst)

        # Tools
        AS := /opt/microblaze-uclinux-tools/bin/microblaze-uclinux-gcc
        CC := /opt/microblaze-uclinux-tools/bin/microblaze-uclinux-g++
        SD := /opt/microblaze-uclinux-tools/bin/microblaze-uclinux-gcc -M

        # Determine the extensions for the target platform
        OBJ_EXT := o
        DEP_EXT := dep
        EXE_EXT :=
        LIB_EXT := a
        DLL_EXT := so

        # Lists
        OBJS := $(patsubst %.cpp,%.$(OBJ_EXT),$(SRCS) $(UBLAZESRCS))
        OBJS := $(patsubst %.c,%.$(OBJ_EXT),$(OBJS))
        DEPS := $(patsubst %.$(OBJ_EXT),%.$(DEP_EXT),$(OBJS))
        LIBS := $(LIBS) $(UBLAZELIBS) $(EXTERNAL_LIBS)
        LIBS := $(addsuffix .$(LIB_EXT), $(LIBS))
        LIBS := $(addprefix $(OUTDIR)/, $(LIBS))
        UBLAZESYSTEMLIBS := $(addprefix -l, $(UBLAZESYSTEMLIBS))

        SDFLAGS := -s -Os -nostdlib -fno-strict-aliasing -fno-exceptions -ffunction-sections -fdata-sections -c -Wall -x none \
                   -I"$(subst ;," -I",$(INCDIRS))" -I$(DANTE_PATH)/dev/include/audinate \
                   -I$(DANTE_PATH)/dev/include/audinate/dante  -I$(DANTE_PATH)/dev/include/audinate/conmon \
                   -I$(DANTE_PATH)/dev/include/  -I$(DANTE_PATH)/dev/include/linux-2.4.x \
				   -DTARGET_LINUX -DCAP6 -D"CPPUNIT_TUI_VERSION" \
                   $(foreach USRDEF,$(USRDEFS),-D"$(USRDEF)") \
                   $(foreach USRUNDEF,$(USRUNDEFS),-U"$(USRUNDEF)")
        CCFLAGS = $(SDFLAGS) -s -msoft-float -mxl-barrel-shift -fno-keep-static-consts -mxl-pattern-compare \
                   -mno-xl-soft-mul -mxl-soft-div -D NDEBUG -Wall -o $(INTDIR)/$@

        ifeq ($(TYPE),EXE)
          OUT_EXT := $(EXE_EXT)
          LD := /opt/microblaze-uclinux-tools/bin/microblaze-uclinux-g++
          LDFLAGS = -elf2flt="-s 61440" -L$(DANTE_PATH)/dev/lib -L$(OUTDIR) -o$@
        endif
        ifeq ($(TYPE),LIB)
          OUT_EXT := $(LIB_EXT)
          LD := /opt/microblaze-uclinux-tools/bin/microblaze-uclinux-ar
          LDFLAGS = crus $@
        endif
        ifeq ($(TYPE),DLL)
          OUT_EXT := $(DLL_EXT)
          LD := /opt/microblaze-uclinux-tools/bin/microblaze-uclinux-ld
          LDFLAGS = -o$@
        endif
      endif

    endif #ifeq ($(PLATFORM),uBlazeApp)
    # This must be done in a variable section (using $(shell))
    # since the creation of $(LSTFILE) is done in a
    # variable section as well (and variable expansion always
    # takes precedence over commands in a rule).
    #
    $(shell $(subst _FILE_,$(LSTFILE),$(subst _COMMAND_,$(RM) $(LSTFILE),$(IFEXIST))))
    
  endif #ifneq ($(RELEASE),)
endif #ifneq ($(PLATFORM),)

### Generic ####################################################################

	ifneq ($(OUT_EXT),)
    OUTFILE := $(subst /,$(PATH_SEP),$(OUTDIR)/$(NAME).$(OUT_EXT))
	else
    OUTFILE := $(subst /,$(PATH_SEP),$(OUTDIR)/$(NAME))
	endif
	ifneq ($(BIN_EXT),)
    BINFILE := $(subst /,$(PATH_SEP),$(OUTDIR)/$(NAME).$(BIN_EXT))
    vpath %.$(BIN_EXT) $(OUTDIR)
	endif
	ifneq ($(RPM_EXT),)
    RPMFILE := $(subst /,$(PATH_SEP),$(OUTDIR)/$(NAME).$(RPM_EXT))
    SPECFILE := $(NAME).$(SPEC_EXT)
	endif

    vpath %.$(OBJ_EXT) $(INTDIR)
    vpath %.$(LIB_EXT) $(OUTDIR)
    vpath %.c          $(SRC_INC)
    vpath %.cpp        $(SRC_INC)
    vpath %.asm        $(SRC_INC)
    vpath %.S          $(SRC_INC)

    LSTFILE := $(subst /,$(PATH_SEP),$(INTDIR)/obj-list.lst)

    #
    # Remove the listfile of the current component,
    # since we need to append to it later on.
    #

################################################################################
# Phony target specification
################################################################################
.PHONY: goal all clean phonyclean $(NAME)\
        stm32Release stm32ReleaseClean \
        stm32Debug stm32DebugClean \
        macosRelease macosReleaseClean \
        macosDebug macosDebugClean

################################################################################
# Targets
################################################################################
goal: stm32Release stm32Debug macosRelease macosDebug

all: stm32Release stm32Debug macosRelease macosDebug

clean: stm32ReleaseClean macosReleaseClean

stm32Release:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) $(NAME) --no-print-directory PLATFORM=stm32App RELEASE=Release
  else
	$(PREFIX)echo $@ not configured for $(NAME)
  endif

macosRelease:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) $(NAME) --no-print-directory PLATFORM=macosApp RELEASE=Release
  else
	$(PREFIX)echo $@ not configured for $(NAME)
  endif

uBlazeRelease:
  ifneq (,$(findstring uBlaze,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) $(NAME) --no-print-directory PLATFORM=uBlazeApp RELEASE=Release
  else
	$(PREFIX)echo $@ not configured for $(NAME)
  endif

stm32ReleaseClean:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) phonyclean --no-print-directory PLATFORM=stm32App RELEASE=Release CLEAN=Yes
  else
	$(PREFIX)echo stm32Release not configured for $(NAME)
  endif

macosReleaseClean:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) phonyclean --no-print-directory PLATFORM=macosApp RELEASE=Release CLEAN=Yes
  else
	$(PREFIX)echo macosRelease not configured for $(NAME)
  endif

uBlazeReleaseClean:
  ifneq (,$(findstring uBlaze,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) phonyclean --no-print-directory PLATFORM=uBlazeApp RELEASE=Release CLEAN=Yes
  else
	$(PREFIX)echo uBlazeRelease not configured for $(NAME)
  endif

stm32Debug:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) $(NAME) --no-print-directory PLATFORM=stm32App RELEASE=Debug
  else
	$(PREFIX)echo $@ not configured for $(NAME)
  endif

stm32DebugClean:
  ifneq (,$(findstring stm32,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) phonyclean --no-print-directory PLATFORM=stm32App RELEASE=Debug CLEAN=Yes
  else
	$(PREFIX)echo stm32Release not configured for $(NAME)
  endif

macosDebug:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) $(NAME) --no-print-directory PLATFORM=macosApp RELEASE=Debug
  else
	$(PREFIX)echo $@ not configured for $(NAME)
  endif

macosDebugClean:
  ifneq (,$(findstring macos,$(TARGETS)))
	$(PREFIX)$(MAKE) -f $(NAME_MKE) phonyclean --no-print-directory PLATFORM=macosApp RELEASE=Debug CLEAN=Yes
  else
	$(PREFIX)echo macosDebug not configured for $(NAME)
  endif

### Dependency files ###########################################################

$(INTDIR)/%.$(DEP_EXT) : %.c
	$(PREFIX)echo $(@F)
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$@),$(subst _COMMAND_,$(RM) $(subst /,$(PATH_SEP),$@),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$@.tmp),$(subst _COMMAND_,$(RM) $(subst /,$(PATH_SEP),$@.tmp),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(OUTDIR)/$(OBJINT)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(OUTDIR)/$(OBJINT)),$(IFNOTEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(INTDIR)),$(IFNOTEXIST)))
  #
  # Do not use (OBJ_EXT) in the SED command, since all dependencies are generated with gcc.
  # Use .o instead
  #
	$(PREFIX)$(SD) $(SDFLAGS) $< | $(SE) "s/$*.o:/$*.$(OBJ_EXT) $(subst /,\/,$@):/" > $(subst /,$(PATH_SEP),$@.tmp)
	$(PREFIX)$(subst _PATH_,$(subst /,$(PATH_SEP),$(INTDIR)/),$(subst _OLDNAME_,$(@F).tmp,$(subst _NEWNAME_,$(@F),$(RN))))

$(INTDIR)/%.$(DEP_EXT) : %.cpp
	$(PREFIX)echo $(@F)
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$@),$(subst _COMMAND_,$(RM) $(subst /,$(PATH_SEP),$@),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$@.tmp),$(subst _COMMAND_,$(RM) $(subst /,$(PATH_SEP),$@.tmp),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(OUTDIR)/$(OBJINT)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(OUTDIR)/$(OBJINT)),$(IFNOTEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(INTDIR)),$(IFNOTEXIST)))
  #
  # Do not use (OBJ_EXT) in the SED command, since all dependencies are generated with gcc.
  # Use .o instead
  #
	$(PREFIX)$(SD) $(SDFLAGS) $< | $(SE) "s/$*.o:/$*.$(OBJ_EXT) $(subst /,\/,$@):/" > $(subst /,$(PATH_SEP),$@.tmp)
	$(PREFIX)$(subst _PATH_,$(subst /,$(PATH_SEP),$(INTDIR)/),$(subst _OLDNAME_,$(@F).tmp,$(subst _NEWNAME_,$(@F),$(RN))))

### Object files ###############################################################

%.$(OBJ_EXT) : %.asm
	$(PREFIX)echo $(@F)
	$(PREFIX)$(AS) $(ASFLAGS) $<

%.$(OBJ_EXT) : %.S
	$(PREFIX)echo $(@F)
	$(PREFIX)$(AS) $(ASFLAGS) $<

%.$(RES_EXT) : %.rc
	$(PREFIX)echo $(@F)
	$(PREFIX)"$(RC)" $(RCFLAGS) -fo "$(INTDIR)/$(@F)" $<

%.$(OBJ_EXT) : %.c $(TOOL_DEPS)
  ifneq ($(CC),cl.exe)
	$(PREFIX)echo $(@F)
  endif
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)/$@),$(subst _COMMAND_,$(RM) $(subst /,$(PATH_SEP),$(INTDIR)/$@),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(INTDIR)),$(IFNOTEXIST)))
	$(PREFIX)$(CC) $(CCFLAGS) $<

%.$(OBJ_EXT) : %.cpp $(TOOL_DEPS)
  ifneq ($(CC),cl.exe)
	$(PREFIX)echo $(@F)
  endif
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)/$@),$(subst _COMMAND_,$(RM) $(subst /,$(PATH_SEP),$(INTDIR)/$@),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(INTDIR)),$(IFNOTEXIST)))
	$(PREFIX)$(CC) $(CCFLAGS) $(CXXFLAGS) $<


### Output file ################################################################

$(NAME): $(OUTFILE) $(BINFILE) $(RPMFILE)

$(OUTFILE): $(INTDIR)/$(NAME).x $(OBJS) $(LIBS) $(NAME_MKE)
	$(PREFIX)echo $@
	$(PREFIX)$(subst _FILE_,$@,$(subst _COMMAND_,$(RM) $@,$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(OUTDIR)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(OUTDIR)),$(IFNOTEXIST)))
    ifneq (,$(findstring $(OBJINT),$(OBJS)))
	$(foreach obj,$(OBJS),$(shell echo $(obj) >> $(LSTFILE)))
    else
      ifneq ($(TYPE),LIB)
	$(shell echo $(OUTDIR)/$(BSP) >> $(LSTFILE))
      endif
	$(foreach obj,$(OBJS),$(shell echo $(INTDIR)/$(obj) >> $(LSTFILE)))
	$(foreach lib,$(LIBS),$(shell echo $(OUTDIR)/$(lib) >> $(LSTFILE)))
    endif  
    ifeq ($(PLATFORM),uBlazeApp)
	$(PREFIX)$(subst \,/,$(LD)) $(subst \,/,$(LDFLAGS)) $(foreach obj,$(OBJS),$(INTDIR)/$(obj)) $(LIBS) $(UBLAZESYSTEMLIBS)
    else
      ifeq ($(PLATFORM),macosApp)
	$(PREFIX)$(subst \,/,$(LD)) $(subst \,/,$(LDFLAGS)) $(foreach obj,$(OBJS),$(INTDIR)/$(obj)) $(foreach obj,$(LIBS),$(OUTDIR)/$(obj))  
      else
	$(PREFIX)$(subst \,/,$(LD)) $(subst \,/,$(LDFLAGS)) @$(LSTFILE)
      endif
    endif
	$(PREFIX)$(subst _FILE_,$(LSTFILE),$(subst _COMMAND_,$(RM) $(LSTFILE),$(IFEXIST)))

$(BINFILE): $(OUTFILE)
	$(PREFIX)echo $@
	$(PREFIX)$(B2B) $(B2BFLAGS) $(OUTFILE) $(BINFILE)

$(INTDIR)/$(NAME).x:
  ifneq ($(RELEASE),FxCop)
    ifneq ($(RELEASE),Doc)
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)),$(subst _COMMAND_,$(MD) $(subst /,$(PATH_SEP),$(INTDIR)),$(IFNOTEXIST)))
	$(PREFIX)echo Checkpoint file for component $(NAME) > $(subst /,$(PATH_SEP),$@)
    endif
  endif

### Clean ######################################################################

phonyclean:
  ifneq ($(PLATFORM),)
    ifneq ($(RELEASE),)
	$(PREFIX)$(subst _FILE_,$(subst /,$(PATH_SEP),$(INTDIR)),$(subst _COMMAND_,$(RD) $(subst /,$(PATH_SEP),$(INTDIR)),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(patsubst %.$(OUT_EXT),%.pdb,$(OUTFILE)),$(subst _COMMAND_,$(RM) $(patsubst %.$(OUT_EXT),%.pdb,$(OUTFILE)),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(patsubst %.$(OUT_EXT),%.map,$(OUTFILE)),$(subst _COMMAND_,$(RM) $(patsubst %.$(OUT_EXT),%.map,$(OUTFILE)),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(patsubst %.$(OUT_EXT),%.dat,$(OUTFILE)),$(subst _COMMAND_,$(RM) $(patsubst %.$(OUT_EXT),%.dat,$(OUTFILE)),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(patsubst %.$(OUT_EXT),%.exp,$(OUTFILE)),$(subst _COMMAND_,$(RM) $(patsubst %.$(OUT_EXT),%.exp,$(OUTFILE)),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(patsubst %.$(OUT_EXT),%.lib,$(OUTFILE)),$(subst _COMMAND_,$(RM) $(patsubst %.$(OUT_EXT),%.lib,$(OUTFILE)),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(patsubst %.$(OUT_EXT),%.err,$(OUTFILE)),$(subst _COMMAND_,$(RM) $(patsubst %.$(OUT_EXT),%.err,$(OUTFILE)),$(IFEXIST)))
	$(PREFIX)$(subst _FILE_,$(OUTFILE),$(subst _COMMAND_,$(RM) $(OUTFILE),$(IFEXIST)))
    else
	$(PREFIX)echo Target $@ should not be used from command line
    endif
  else
	$(PREFIX)echo Target $@ should not be used from command line
  endif

################################################################################
# Include dependency files
################################################################################
ifneq ($(PLATFORM),)
  ifneq ($(RELEASE),)
    #
    # If cleaning, we must not include dependency files.
    # Otherwise they will be created before cleaning.
    #
    ifeq ($(CLEAN),)
      ifeq (,$(findstring $(OBJINT),$(OBJS)))
        #
        # Use a - in front of the include, otherwise make will complain
        # when it is ran the first time (when there are no .dep files).
        #
        -include $(patsubst %.$(DEP_EXT),$(INTDIR)/%.$(DEP_EXT),$(DEPS))
      endif
    endif
  endif
endif
