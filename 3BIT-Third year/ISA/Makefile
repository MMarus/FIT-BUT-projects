# Author:      Marek Marusic, xmarus05
# Email:       xmarus05@stud.fit.vutbr.cz
# Purpose:     Project to ISA
# Description: Create client for SIP protokol
# File:        Makefile

CC              = g++
CFLAGS         = -g -std=c++11 -lssl -lcrypto
OBJ_FILES      = main.o file_io.o sip.o
#LIBRARIES       = -static-libstdc++

vpath %.cpp src
vpath %.h src

all: sipklient

sipklient:	$(OBJ_FILES)
		$(CC) $(OBJ_FILES) -o $@ $(CFLAGS)

main.o: main.cpp main.h file_io.h sip.h
	$(CC) -o $@ -c $<  $(CFLAGS)

file_io.o: file_io.cpp file_io.h
	$(CC) -o $@ -c $<  $(CFLAGS)

sip.o: sip.cpp sip.h
	$(CC) -o $@ -c $<  $(CFLAGS)


clean:
	rm -f *.o sipklient

.PHONY: clean
