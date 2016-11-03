/* ************************* error.c ************************* */
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

#include "error.h"
#include "expr_inter.h"
#include "zasobnik.h"

// variables can't go to .h
// modify this bullshit
const char *err_masages[] = {
    [LEXICAL_ERR] = "LEXICAL_ERR (1)",
    [SYNTAX_ERR] = "SYNTAX_ERR (2)",
    [SEMANTIC_DEFINITION_ERR] = "SEMANTIC_DEFINITION_ERR (3)",
    [SEMANTIC_TYPE] = "SEMANTIC_TYPE (4)",
    [SEMANTIC_OTHER_ERR] = "SEMANTIC_OTHER_ERR (5)",
    [RUNTIME_LOAD_ERR] = "RUNTIME_LOAD_ERR (6)",
    [RUNTIME_UNITIALIZED_ERR] = "RUNTIME_UNITIALIZED_ERR (7)",
    [RUNTIME_DIV_ZERO_ERR] = "RUNTIME_DIV_ZERO_ERR (8)",
    [RUNTIME_OTHER_ERR] = "RUNTIME_OTHER_ERR (9)",
    [GET_INTERNAL_ERR] = "INTERNAL_ERR (99)"
};

extern token_t token;

void err(error_t error, const char *msg)
{
    const char *p_err;

    if (msg)
        fprintf(stderr, "interpret-ifj14.src:%d:%d: %s\n", token.lineNumber, token.col, msg);

    exit(error);
}

// mayble warning() ???

