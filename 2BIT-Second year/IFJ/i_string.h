/* ************************* i_string.h ************************* */
/* Kodovani:		UTF-8					                    */
/* Predmet:		Formalni jazyky a prekladace (IFJ)	            */
/* Projekt:		Implementace interpretu imperativniho	        */
/*			    jazyka IFJ14				                    */
/* Varianta zadani:	a/2/I					                    */
/* Autori:		Fáber Jakub		xfaber02	                    */
/*			Hladík Daniel		xhladi21	                    */
/*			Marušič Marek		xmarus05	                    */
/*			Surovčík Tomáš		xsurov04	                    */
/* ************************************************************ */

#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "error.h"

#define DEFAULT_MEM_ALLOC 32

typedef struct {
    int length;
    char *name;
} string_t;

// Calling functions saves to Symbol table
char *load_identifier(FILE *stream);
char *load_string(FILE *stream);
int get_escaped(FILE *stream);
char * copy_string(char * src, int length);


int cmp_string(string_t *s1, string_t *s2);
string_t *make_string(string_t *src);
void free_string(string_t *src);
void reverse_string(char * str);

char *load_string(FILE *stream);

// int stringInit(string_t *s);
// void stringFree(string_t *s);

// void stringClear(string_t *s);
// int stringAddChar(string_t *s1, char c);
// int stringCopyString(string_t *s1, string_t *s2);
// int stringCmpString(string_t *s1, string_t *s2);
// int stringCmpConstStr(string_t *s1, char *s2);

#endif
