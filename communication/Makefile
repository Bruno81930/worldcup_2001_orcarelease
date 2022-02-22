OFILES = message.o communicate.o baseConvert.o

# INCLUDEFILE is an environment variable set by the top level Makefile
INCLUDEFILE 	?= ../Makeoptions
include $(INCLUDEFILE)

# optional flags and includes
CXXFLAGS	+= -pg
CXXLIBS		+=

# BUILTPATH is an environment variable set by the top level Makefile
BUILTPATH	?= ../built

CXXINCLUDE      = -I. -I.. -I../sfl
SRCS            = $(OFILES:%.o=%.C)

NEWOFILES 	= $(OFILES:%.o=$(BUILTPATH)/%.o)
DEPENDS 	= $(NEWOFILES:%.o=%.d)

$(BUILTPATH)/%.o : %.C %.h
ifeq ($(VERBOSE), true)
	@echo -n building $(@F)...
	@$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
	@echo " done"
else
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
endif

.PHONY: all
all: main 

ifeq ($(wildcard $(DEPENDS)), $(DEPENDS))
include $(DEPENDS)
endif

main: $(DEPENDS) $(NEWOFILES)

$(BUILTPATH)/%.d: %.C
	@echo -n updating dependencies for $<...
	@$(shell set -e; $(CXX) $(CXXFLAGS) $(CXXINCLUDE) -MM $(CPPFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/$$(BUILTPATH)\/\1.o $$(BUILTPATH)\/$(@F) : /g' > $@; \
	[ -s $@ ] || rm -f $@)
	@echo " done"
