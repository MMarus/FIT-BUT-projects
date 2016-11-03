# Makefile

CC = gcc
CFLAGS = -I include -std=c99 -Wall -O2 -pedantic -g
OBJ_FILES = main.o scanner.o error.o i_string.o exprs.o parser.o instr.o zasobnik.o interpret.o ial.o
HEADERS = main.h scanner.h token.h error.h i_string.h error.h interpret.h expr_inter.h zasobnik.h ial.h

vpath %.c .
vpath %.h . 

all: compiler

compiler: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@

main.o: 		$(HEADERS)
i_string.o: 	$(HEADERS)
scanner.o: 		$(HEADERS)
ial.o:			$(HEADERS)
error.o: 		$(HEADERS)
instr.o: 		$(HEADERS)
parser.o: 		$(HEADERS)
exprs.o: 		$(HEADERS)
zasobnik.o: 	$(HEADERS)
interpret.o:	$(HEADERS)

.PHONY: clean

clean:
	rm -f *.o compiler i_string
