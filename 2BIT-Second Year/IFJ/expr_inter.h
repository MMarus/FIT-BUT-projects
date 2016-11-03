/* ************************* expr_inter.h ************************* */
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

#ifndef EXPR_INTER_H
#define EXPR_INTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i_string.h"
#include "error.h"

typedef union {
    bool b;
    int i;
    double d;
    string_t *string;
} value_u;

typedef struct {
    int type;
    int lineNumber;
    int col;
    string_t *str;
    int sa_type;
    value_u tdata;
} token_t;

#endif