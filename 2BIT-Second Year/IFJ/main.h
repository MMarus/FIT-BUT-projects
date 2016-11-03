/* ************************* main.h *************************** */
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

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "token.h"
#include "error.h"
#include "i_string.h"
#include "ial.h"
#include "instr.h"
#include "parser.h"
#include "interpret.h"

/**
 * @brief Opens stream from argv[1]
 * @return Valid pointer to the stream
 */
FILE * open_stream(int argc, const char *argv[]);
void close_stream(FILE *stream);

#endif