
C=g++ -ggdb
ONLINE=communication.o onlinecoach.o parametervalues.o ../TimeSlice.o ../statsutil.o ../geometry.o analyzestats.o coach_lang.o coach_lang_comp.o region.o clangdefinitions.o ../Foil.o ../../logging.o
OBJECTS=$(ONLINE)
#INCLUDES=-I/usr/include/postgresql -I/usr/local/pgsql/include
#LIBS=-L/usr/lib/postgresql -lpq++ -lg++ -L/usr/local/pgsql/lib -lpq -lpq++ -lg++
#LIBS=-lg++
FLAGS=$(INCLUDES)


.SUFFIXES: .C .o
.C.o :; $(C) -c $(FLAGS) $< -o $@

all: $(OBJECTS)
	$(C) $(LIBS) -static -o onlinecoach $(ONLINE)

clean:
	rm -f $(OBJECTS)

etags:
	etags *.C *.h ../*.C ../*.h


