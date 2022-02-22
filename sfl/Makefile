
# INCLUDEFILE is an environment variable set by the top level Makefile
INCLUDEFILE 	?= ../Makeoptions
include $(INCLUDEFILE)

# optional flags and includes
CXXFLAGS	+= -D_REENTRANT -DCOACH_LANG_TESTER
CXXLIBS		+=

# BUILTPATH is an environment variable set by the top level Makefile
BUILTPATH	?= ../built

BISONFLAGS 	= -dt 
#BISONFLAGS 	+= --no-lines
LEXFLAGS 	= 

OFILES 		= clangparser.o
OFILES 		+= coach_lang.o
OFILES 		+= region.o
OFILES 		+= coach_lang_comp.o
OFILES 		+= garbage.o
OFILES	 	+= SFLShierarchy.o
OFILES 		+= effector.o
OFILES 		+= SFLS.o

NEWOBJS		= $(OFILES:%.o=$(BUILTPATH)/%.o)
DEPENDS		= $(NEWOBJS:%.o=%.d)

CXXINCLUDE 	= -I. -I.. -I../behavior/play

SRCS 		= $(OFILES:%.o=%.C)

GEN_OFILES	+= coach_lang.tab.o lex.yy.o
NEWGENOBJS	= $(GEN_OFILES:%.o=$(BUILTPATH)/%.o)
GEN_SRCS 	= $(GEN_OFILES:%.o=%.c) coach_lang.tab.h

.PHONY: all main clean realclean
all: main

ifeq ($(wildcard $(DEPENDS)),$(DEPENDS))
include $(DEPENDS)
endif

coach_lang.tab.h: coach_lang.y
ifeq ($(VERBOSE), true)
	@echo -n bisoning $@...
	@bison $(BISONFLAGS) $^
	@echo " done"
else
	bison $(BISONFLAGS) $^
endif

coach_lang.tab.c: coach_lang.y
ifeq ($(VERBOSE), true)
	@echo -n bisoning $@...
	@bison $(BISONFLAGS) $^
	@echo " done"
else
	bison $(BISONFLAGS) $^
endif

lex.yy.c: coach_lang.l coach_lang.h
ifeq ($(VERBOSE), true)
	@echo -n flexing $@...
	@flex $(LEXFLAGS) $< 
	@echo " done"
else
	flex $(LEXFLAGS) $< 
endif

main: $(DEPENDS) $(NEWOBJS) $(NEWGENOBJS)

$(BUILTPATH)/%.d: %.C
	@echo -n updating dependencies for $<...
	@$(shell set -e;\
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -MM $(CPPFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/$$(BUILTPATH)\/\1.o $$(BUILTPATH)\/$(@F) : /g' > $@; \
	[ -s $@ ] || rm -f $@)
	@echo " done"

$(BUILTPATH)/lex.yy.o : lex.yy.c
ifeq ($(VERBOSE), true)
	@echo -n building $(@F)...
	@$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
	@echo " done"
else
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
endif

$(BUILTPATH)/%.o : %.c %.h
ifeq ($(VERBOSE), true)
	@echo -n building $(@F)...
	@$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
	@echo " done"
else
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
endif

$(BUILTPATH)/%.o : %.C %.h
ifeq ($(VERBOSE), true)
	@echo -n building $(@F)...
	@$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
	@echo " done"
else
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
endif

clean:
	@echo cleaning up....
	/bin/rm -f $(NEWOBJS) $(NEWGENOBJS) $(DEPENDS)
realclean: clean
	@echo cleaning up everything
	/bin/rm -f $(GEN_SRCS)
