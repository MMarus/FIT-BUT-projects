/* ************************* zasobnik.h ************************* */
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

#ifndef ZASOBNIK_H
#define ZASOBNIK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "exprs.h"

#define STACK_OK 0
#define E_STACK_ALLOC -1
#define E_STACK_EMPTY -2
#define stackEmpty(S) ((S)->top == NULL)

extern token_t token;

// struktura SSItem pro polozku v zasobniku
typedef struct SSItem { 
    token_t *data;
    struct SSItem *next;
} TSItem; 
// typ TSItem -> polozka v zasobniku

typedef struct {
    TSItem *top;
} TStack; 
//typ TStack pro zasobnik

void stackInit (TStack *s); 
void *stackTop (TStack *s);
int stackTop_t (TStack *s);
int stackPop (TStack *s);
void *stackTopPop (TStack *s);
int stackPush (TStack *s, token_t *data);
void stackDelete (TStack *s);
void popToTerm (TStack *from, TStack *to);
void popToAll (TStack *from, TStack *to);

#endif
