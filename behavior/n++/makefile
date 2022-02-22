# Directories, where object files and header files are exported to
LIB_DEST = ../lib
INCLUDE_DEST = ../include

CC = g++
CFLAGS = -O -Wall -static
#CFLAGS = -g -Wall 

AR              = ar
ARFLAGS 	= rc
RANLIB          = ranlib

OBJS = n++.o PatternSet.o aux.o
N++_LIB = libn++.a

default: $(OBJS) 

install: $(OBJS)
	cp $(OBJS) $(LIB_DEST)
	cp $(OBJS:.o=.h) $(INCLUDE_DEST)
	cp $(N++_LIB) $(LIB_DEST)

$(N++_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(N++_LIB) $(OBJS)
	$(RANLIB) $(N++_LIB)

copy2dos:
	mcopy -t n++.cc a:npp.cc
	mcopy -t n++.h a:npp.h
	mcopy -t makefile a:
	mcopy -t functions.c a:
	mcopy -t aux.cc aux.h a:
	mcopy -t PatternSet.cc PatternSet.h a:


.cc.o:
	 $(CC) $(CFLAGS) -c $<

depend:
	makedepend -- $(CFLAGS) -- $(OBJS:.o=.cc)

# DO NOT DELETE THIS LINE -- make depend depends on it.

n++.o: n++.h /usr/include/stdio.h /usr/include/features.h
n++.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
n++.o: /usr/include/bits/types.h /usr/include/libio.h
n++.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
n++.o: /usr/include/math.h /usr/include/bits/huge_val.h
n++.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
n++.o: /usr/include/string.h /usr/include/stdlib.h /usr/include/sys/types.h
n++.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
n++.o: /usr/include/sys/select.h /usr/include/bits/select.h
n++.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
n++.o: /usr/include/alloca.h functions.c
PatternSet.o: PatternSet.h /usr/include/stdio.h /usr/include/features.h
PatternSet.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
PatternSet.o: /usr/include/bits/types.h /usr/include/libio.h
PatternSet.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
PatternSet.o: /usr/include/string.h /usr/include/stdlib.h
PatternSet.o: /usr/include/sys/types.h /usr/include/time.h
PatternSet.o: /usr/include/endian.h /usr/include/bits/endian.h
PatternSet.o: /usr/include/sys/select.h /usr/include/bits/select.h
PatternSet.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
PatternSet.o: /usr/include/alloca.h
aux.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
aux.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
aux.o: /usr/include/libio.h /usr/include/_G_config.h
aux.o: /usr/include/bits/stdio_lim.h n++.h /usr/include/math.h
aux.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
aux.o: /usr/include/bits/mathcalls.h /usr/include/string.h
aux.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
aux.o: /usr/include/endian.h /usr/include/bits/endian.h
aux.o: /usr/include/sys/select.h /usr/include/bits/select.h
aux.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
aux.o: /usr/include/alloca.h PatternSet.h aux.h
