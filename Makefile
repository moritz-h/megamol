#
# Makefile
# TriSoup (MegaMol)
#
# Copyright (C) 2008-2010 by VISUS (Universitaet Stuttgart).
# Alle Rechte vorbehalten.
#

include common.mk
include ExtLibs.mk

# Target name
# TODO: Change the name "Template" to the name of your plugin
TargetName := TriSoup
# subdirectories below $(InputRootDir)
InputRootDir := $(InputDir)
InputDirs := .
IncludeDir := $(IncludeDir) $(vislibpath)/base/include $(vislibpath)/math/include \
	$(vislibpath)/sys/include $(vislibpath)/graphics/include $(mmcorepath)


# Additional compiler flags
CompilerFlags := $(CompilerFlags) -fPIC
ExcludeFromBuild += ./dllmain.cpp


# Libraries
LIBS := $(LIBS) m pthread pam pam_misc dl ncurses uuid GL


# Additional linker flags
LinkerFlags := $(LinkerFlags) -shared -Wl,-Bsymbolic


# Collect Files
# WARNING: $(InputDirs) MUST be relative paths!
CPP_SRCS := $(filter-out $(ExcludeFromBuild), $(foreach InputDir, $(InputDirs), $(wildcard $(InputDir)/*.cpp)))
CPP_DEPS := $(addprefix $(IntDir)/$(DebugDir)/, $(patsubst %.cpp, %.d, $(CPP_SRCS)))\
	$(addprefix $(IntDir)/$(ReleaseDir)/, $(patsubst %.cpp, %.d, $(CPP_SRCS)))
CPP_D_OBJS := $(addprefix $(IntDir)/$(DebugDir)/, $(patsubst %.cpp, %.o, $(CPP_SRCS)))
CPP_R_OBJS := $(addprefix $(IntDir)/$(ReleaseDir)/, $(patsubst %.cpp, %.o, $(CPP_SRCS)))

CPPFLAGS := $(CompilerFlags) $(addprefix -I, $(IncludeDir)) $(addprefix -isystem, $(SystemIncludeDir))
LDFLAGS := $(LinkerFlags) -L$(vislibpath)/lib -L$(expatpath)/lib


all: $(TargetName)d $(TargetName)


# Rules for plugins in $(SolOutputDir):
$(TargetName)d: $(IntDir)/$(DebugDir)/$(TargetName)$(BITS)d.lin$(BITS).mmplg
	@mkdir -p $(outbin)
	@mkdir -p $(outshaders)
	cp $< $(outbin)/$(TargetName)$(BITS)d.lin$(BITS).mmplg
	cd Shaders && find . -wholename '*.svn' -prune -o -exec cp --parents \{\} $(outshaders) \;

$(TargetName): $(IntDir)/$(ReleaseDir)/$(TargetName)$(BITS).lin$(BITS).mmplg
	@mkdir -p $(outbin)
	@mkdir -p $(outshaders)
	cp $< $(outbin)/$(TargetName)$(BITS).lin$(BITS).mmplg
	cd Shaders && find . -wholename '*.svn' -prune -o -exec cp --parents \{\} $(outshaders) \;


# Rules for intermediate plugins:
$(IntDir)/$(DebugDir)/$(TargetName)$(BITS)d.lin$(BITS).mmplg: Makefile $(addprefix $(IntDir)/$(DebugDir)/, $(patsubst %.cpp, %.o, $(CPP_SRCS)))
	@echo -e '\E[1;32;40m'"LNK "'\E[0;32;40m'"$(IntDir)/$(DebugDir)/$(TargetName)$(BITS)d.lin$(BITS).mmplg: "
	@tput sgr0
	$(Q)$(LINK) $(LDFLAGS) $(CPP_D_OBJS) $(addprefix -l,$(LIBS)) \
	-lvislibgraphics$(BITS)d -lvislibsys$(BITS)d -lvislibmath$(BITS)d -lvislibbase$(BITS)d \
	-o $(IntDir)/$(DebugDir)/$(TargetName)$(BITS)d.lin$(BITS).mmplg

$(IntDir)/$(ReleaseDir)/$(TargetName)$(BITS).lin$(BITS).mmplg: Makefile $(addprefix $(IntDir)/$(ReleaseDir)/, $(patsubst %.cpp, %.o, $(CPP_SRCS)))
	@echo -e '\E[1;32;40m'"LNK "'\E[0;32;40m'"$(IntDir)/$(ReleaseDir)/$(TargetName)$(BITS).lin$(BITS).mmplg: "
	@tput sgr0
	$(Q)$(LINK) $(LDFLAGS) $(CPP_R_OBJS) $(addprefix -l,$(LIBS)) \
	-lvislibgraphics$(BITS) -lvislibsys$(BITS) -lvislibmath$(BITS) -lvislibbase$(BITS) \
	-o $(IntDir)/$(ReleaseDir)/$(TargetName)$(BITS).lin$(BITS).mmplg


# Rules for dependencies:
$(IntDir)/$(DebugDir)/%.d: $(InputDir)/%.cpp Makefile
	@mkdir -p $(dir $@)
	@echo -e '\E[1;32;40m'"DEP "'\E[0;32;40m'"$@: "
	@tput sgr0
	@echo -n $(dir $@) > $@
	$(Q)$(CPP) -MM $(CPPFLAGS) $(DebugCompilerFlags) $< >> $@

$(IntDir)/$(ReleaseDir)/%.d: $(InputDir)/%.cpp Makefile
	@mkdir -p $(dir $@)
	@echo -e '\E[1;32;40m'"DEP "'\E[0;32;40m'"$@: "
	@tput sgr0
	@echo -n $(dir $@) > $@
	$(Q)$(CPP) -MM $(CPPFLAGS) $(ReleaseCompilerFlags) $< >> $@


ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), sweep)
-include $(CPP_DEPS)
endif
endif


# Rules for object files:
$(IntDir)/$(DebugDir)/%.o:
	@mkdir -p $(dir $@)
	@echo -e '\E[1;32;40m'"CPP "'\E[0;32;40m'"$@: "
	@tput sgr0
	$(Q)$(CPP) -c $(CPPFLAGS) $(DebugCompilerFlags) -o $@ $<

$(IntDir)/$(ReleaseDir)/%.o:
	@mkdir -p $(dir $@)
	@echo -e '\E[1;32;40m'"CPP "'\E[0;32;40m'"$@: "
	@tput sgr0
	$(Q)$(CPP) -c $(CPPFLAGS) $(ReleaseCompilerFlags) -o $@ $<


# Cleanup rules:
clean: sweep
	rm -f $(outbin)/$(TargetName)$(BITS)d.lin$(BITS).mmplg \
	$(outbin)/$(TargetName)$(BITS).lin$(BITS).mmplg


sweep:
	rm -f $(CPP_DEPS)
	rm -rf $(IntDir)/*


rebuild: clean all


.PHONY: clean sweep rebuild tags
