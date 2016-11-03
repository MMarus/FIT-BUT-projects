/* ************************* scanner.h ************************* */
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

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "error.h"
#include "token.h"
#include "i_string.h"
#include "ial.h"
#include "expr_inter.h"


#define DEFAULT_MEM_ALLOC 32



extern const char *TEXTMSGS[];
extern FILE *stream;

char *MemAlloc(char *str);

int isComment(int c);

int isOperator(token_t *token_p, int c, unsigned i);

int getToken(token_t *token_p, FILE *fr);

void test(FILE *fr);

char *StrAppend(char *str, int c, unsigned i);

int getNextToken(token_t *token);

void init_token();
void free_token();

void prepare_token(token_t *t);


#endif
