# file to be included in all Makefiles
INCLUDEFILE	= $(shell pwd)/Makeoptions
export INCLUDEFILE

# optional flags and includes
CXXFLAGS	+= -ggdb
CXXFLAGS        += -DRELEASE_VERSION
CXXLIBS		+=

include $(INCLUDEFILE)

# list of all sub makefiles
SUBDIRS		= sfl behavior communication 
SUBDIRS		+= behavior/play

EXEC_ORCA	= orcaclient

CXXINCLUDE 	= -I. -I.. -I../../.. -Ibehavior/n++ -Ibehavior/train -Isfl

# set built path for all makefiles
CUR-DIR		:= $(shell pwd)
BUILTPATH	= $(CUR-DIR)/built
export BUILTPATH


OBJECTS		=  netif.o client.o parse.o utils.o geometry.o
OBJECTS		+= MemOption.o MemPlayer.o MemPosition.o MemAction.o \
		   MemFormation.o Memory.o
OBJECTS		+= behave.o kick.o dribble.o test.o

NEWOBJS		= $(OBJECTS:%.o=$(BUILTPATH)/%.o)
DEPENDS		= $(NEWOBJS:%.o=%.d)

BUILDER		=  $(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<

$(BUILTPATH)/%.o : %.C %.h
ifeq ($(VERBOSE), true)
	@echo -n building $(@F)... 
	@$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
	@echo " done"
else
	$(CXX) -c $(CXXFLAGS) $(CXXINCLUDE) -o $@ $<
endif

.PHONY: main all bind subdirs $(SUBDIRS) clean tgz server_tgz
main:
	$(shell if ! [ -d $(BUILTPATH) ]; then \
	mkdir $(BUILTPATH);\
	fi;)
	@$(MAKE) all
	@$(MAKE) bind

ifeq ($(wildcard $(DEPENDS)),$(DEPENDS))
include $(DEPENDS)
endif                                                                                                         

all: $(DEPENDS) $(NEWOBJS) subdirs quicksleep 

$(BUILTPATH)/%.d: %.C
	@echo -n updating dependencies for $<...
	$(shell set -e; $(CXX) $(CXXFLAGS) $(CXXINCLUDE) -MM $(CPPFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/$$(BUILTPATH)\/\1.o $$(BUILTPATH)\/$(@F) : /g' > $@; \
	[ -s $@ ] || rm -f $@)
	@echo " done"


bind: $(EXEC_ORCA)

$(EXEC_ORCA): $(wildcard $(BUILTPATH)/*.o)
	@echo -n creating client...
	@$(CXX) -o $@ $^ $(CXXLIBS) -static
	@echo " done"

quicksleep: quicksleep.C
	gcc -o quicksleep quicksleep.C

subdirs: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean :
	@echo -n cleaning up ...
	$(shell rm -f $(EXEC_ORCA) \
	$(wildcard $(BUILTPATH)/*.o) \
	$(wildcard $(BUILTPATH)/*.d) \
	ballbyters.tgz sserver.tgz \
	quicksleep pureclient core)
	@echo " done"

tgz: main
	@echo creating ballbyters.tgz ...
	tar -hczf ballbyters.tgz \
		orcaclient quicksleep \
		server.conf formation.conf client.conf \
		automatic_start \
		orca.conf pppoints
	@echo " done"

server_tgz:
	@echo creating server.tgz ...
	cd coach/online; make; cd ../..
	cd server; cp ../coach/online/onlinecoach .; 
	cd server; tar -hczf ../sserver.tgz \
		onlinecoach player.conf server.conf soccerserver
	@echo " done"

