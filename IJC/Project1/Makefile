# makefile
# Řešení IJC-DU1, příklad a) b), 20.3.2014
# Autor: Marek Marusic, FIT
# Přeloženo: gcc 4.8.2
#	Makefile pre IJC-DU1 

CC = gcc
CFLAGS = -Wall -O2 -pedantic -std=c99 -g
LDFLAGS = -DUSE_INLINE 
LDLIBS = -lm
DEPS = bit-array.h error.h
OBJ 	= error.o eratosthenes.o 
OBJIN = error.o eratosthenes-inline.o prvocisla-inline.o
OBJPPM = ppm.o steg-decode.o

all: prvocisla-inline prvocisla steg-decode

prvocisla-inline: $(OBJIN) $(DEPS)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJIN) -o prvocisla-inline

prvocisla: $(OBJ) prvocisla.o $(DEPS)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) prvocisla.o -o prvocisla

steg-decode: $(OBJ) $(OBJPPM) $(DEPS) ppm.h
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) $(OBJPPM) -o steg-decode

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

eratosthenes-inline.o: eratosthenes.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c eratosthenes.c -o eratosthenes-inline.o

prvocisla-inline.o: prvocisla.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c prvocisla.c -o prvocisla-inline.o 

clean:
	rm *.o	prvocisla-inline prvocisla steg-decode

.PHONY: clean
