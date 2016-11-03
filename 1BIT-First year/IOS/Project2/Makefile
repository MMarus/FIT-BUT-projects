# Subor:   Makefile
# Datum:   4.5.2014 12:45
# Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
# Projekt: Projek 2 predmet IOS praca so zdielanou pametou a Semaformy
# Popis:   Makefile pre projekt 2 z ios


CC=gcc
CFLAGS=-std=gnu99 -Wall -g -Werror -Wextra -pedantic 
LD=-lrt -lpthread
OBJ=rivercrossing.o params.o passengers.o river.o boarding.o
DEPS=rivercrossing.h

all: rivercrossing

rivercrossing: $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) $(LD) $(OBJ) $(DEPS) -o rivercrossing

clean:
	rm *.o	rivercrossing rivercrossing.out

.PHONY: clean
