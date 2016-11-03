/* ************************* exprs.h ************************* */
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

#ifndef _EXPRS_H_
#define _EXPRS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"
#include "i_string.h"
#include "ial.h"
#include "scanner.h"
#include "ial.h"
#include "token.h"
#include "zasobnik.h"
#include "instr.h"
#include "parser.h"

extern token_t token;
extern token_t *result;


// Znaky pro precedencni tabulku - >, <, =, null
enum pt_table_ch {
  PT_EMPTY = 0,
  PT_GREATER,	
  PT_LESS,
  PT_EQUAL,
};

// Terminaly a neterminaly uvnitr SA
enum pt_chars {
  SA_ADD ,	// +
  SA_SUB,	// -
  SA_MUL,	// *
  SA_DIV,	// /
  SA_LESS,	// <
  SA_MORE,	// >
  SA_LESSEQUAL,	// <=
  SA_MOREEQUAL,	// >=
  SA_EQUAL,	// =
  SA_NOTEQUAL,	// <>
  SA_LBRACK,	// (
  SA_RBRACK,	// )
  SA_ID,	// i identifikator
  SA_MAGICNUM,	// pro data type int, real, string, boolean
  SA_TERM,	// $ konecny operator
  SA_E,		// znak E, pro práci se zásobníkem
  SA_POM_LESS	// < znak pro praci pres zasobnik
};

value_u eval_expression();

void initPrecTable (int (*precTable)[15]);
int prevodTokenType (int type);
void sa_core();



#endif