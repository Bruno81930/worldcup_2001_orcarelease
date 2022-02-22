C=g++ -DDEBUG# -ggdb
READLOG=readLog.o TimeSlice.o geometry.o statsutil.o Foil.o ../logging.o# dbAccess.o
ONLINE=online/onlinecoach.o online/parametervalues.o online/communication.o TimeSlice.o statsutil.o geometry.o online/analyzestats.o online/coach_lang.o online/coach_lang_comp.o online/region.o online/clangdefinitions.o Foil.o ../logging.o
DBTEST=dbTest.o dbAccess.o
VIEWER=Viewer.o netif.o viewermain.o ../logging.o
FOIL=Foil.o foiltest.o TimeSlice.o geometry.o statsutil.o ../logging.o
OBJECTS=$(READLOG) $(VIEWER) $(ONLINE) $(FOIL) # $(DBTEST)
#INCLUDES=-I/usr/include/postgresql -I/usr/local/pgsql/include 
#LIBS=-L/usr/lib/postgresql -lpq++ -lg++ -L/usr/local/pgsql/lib -lpq -lpq++ -lg++
LIBS=-lg++
FLAGS=$(INCLUDES)
DATABASE=robocup
#DBHOST=tresserhorn
BINARIES=readLog viewer online/onlinecoach # dbTest

.SUFFIXES: .C .o
.C.o :; $(C) -c $(FLAGS) $< -o $@

all: $(OBJECTS)
	g++ $(LIBS) -o readLog $(READLOG)
#	g++ $(LIBS) -o dbTest $(DBTEST)
	g++ $(LIBS) -o viewer $(VIEWER) TimeSlice.o geometry.o statsutil.o
	g++ $(LIBS) -o online/onlinecoach $(ONLINE)
	g++ $(LIBS) -o foiltest $(FOIL)

clean:
	rm -f $(OBJECTS) $(BINARIES)

#db:
#	psql -h $(DBHOST) $(DATABASE) <database-1.0.sql

#clean-db:
#	psql -h $(DBHOST) $(DATABASE) <clear-database.sql

