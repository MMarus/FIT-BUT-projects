# makefile
# Řešení IJC-DU2, příklad a) b), 20.4.2014
# Autor: Marek Marusic, FIT
#	Makefile pre IJC-DU2
#pouzity prekladac gcc 4.8

CC=gcc
CFLAGS=-Wall -O2 -pedantic -std=c99 -g 
#LDFLAGS = -DUSE_INLINE 
#LDLIBS = -lm

OBJ=fold.o
#OBJIN = error.o eratosthenes-inline.o prvocisla-inline.o
#OBJPPM = ppm.o steg-decode.o
OBJWC=wordcount.o io.o
DEPS=io.h hash_function.h htab.h
LIBFILES=hash_function.o htab_clear.o htab_init.o htab_foreach.o htab_free.o htab_lookup.o htab_remove.o htab_statistics.o

Cxx=g++
CxxFLAGS=-std=c++11 -pedantic -Wall
OBJxx=fold2.o
OBJ2xx=c.o

all: fold fold2 wordcount wordcount-dynamic

fold: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o fold

fold2: $(OBJxx)
	$(Cxx) $(CxxFLAGS) $(OBJxx) -o fold2

wordcount: $(OBJWC) libhtable.a
	$(CC) $(CFLAGS) $(OBJWC) -static libhtable.a -o wordcount

wordcount-dynamic: $(OBJWC) libhtable.so
	$(CC) $(CFLAGS) $(OBJWC) libhtable.so -o wordcount-dynamic

libhtable.a: $(LIBFILES)
	ar crs libhtable.a $(LIBFILES)

libhtable.so: $(LIBFILES)
	gcc $(CFLAGS) -shared -fPIC -o libhtable.so $(LIBFILES)

hash_function.o: hash_function.c hash_function.h
	gcc $(CFLAGS) -fPIC -c hash_function.c

htab_clear.o: htab_clear.c htab.h
	gcc $(CFLAGS) -fPIC -c htab_clear.c -o htab_clear.o

htab_init.o: htab_init.c htab.h
	gcc $(CFLAGS) -fPIC -c htab_init.c -o htab_init.o

htab_lookup.o: htab_lookup.c htab.h
	gcc $(CFLAGS) -fPIC -c htab_lookup.c -o htab_lookup.o

htab_free.o: htab_free.c htab.h
	gcc $(CFLAGS) -fPIC -c htab_free.c -o htab_free.o

htab_foreach.o: htab_foreach.c htab.h
	gcc $(CFLAGS) -fPIC -c htab_foreach.c -o htab_foreach.o

htab_remove.o: htab_remove.c htab.h
	gcc $(CFLAGS) -fPIC -c htab_remove.c -o htab_remove.o

htab_statistics.o: htab_statistics.c htab.h
	gcc $(CFLAGS) -fPIC -c htab_statistics.c -o htab_statistics.o

clean:
	rm *.o	fold fold2 wordcount wordcount-dynamic libhtable.*

.PHONY: clean
