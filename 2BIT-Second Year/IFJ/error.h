/* ************************* error.h ************************* */
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

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
//#include "scanner.h"

// to save from err_masages[100]
#define GET_INTERNAL_ERR 10

#define NO_LINE -1

typedef enum {
    OK,
    LEXICAL_ERR,
    SYNTAX_ERR,
    SEMANTIC_DEFINITION_ERR,
    SEMANTIC_TYPE,
    SEMANTIC_OTHER_ERR,
    RUNTIME_LOAD_ERR,
    RUNTIME_UNITIALIZED_ERR,
    RUNTIME_DIV_ZERO_ERR,
    RUNTIME_OTHER_ERR,
    INTERNAL_ERR = 99,
} error_t;

extern const char *err_masages[];
extern error_t ERR;


/**
* Vypise error a zavola exit(error)
*/
void err(error_t error, const char *msg);

#endif