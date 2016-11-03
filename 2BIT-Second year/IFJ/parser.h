/* ************************* parser.h ************************* */
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

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "ial.h"
#include "exprs.h"

#define DEFINED 1
#define DECLARED 0

extern token_t token;


//////////////////////////////////////////////////////////////////////

// processes statement
void statement();

// calls semantics analysis && processes assigment x := ... => eats ; || END
void assignment();

// calls semantics analysis && processes if ... begin => eats begin
void if_begin();

// calls semantics analysis && processes while ... begin => eats begin
void while_begin();

// calls semantics analysis && processes readln(...) => eats )
void readln_fun();

// calls semantics analysis && processes write(...) => eats )
void write_fun();

// calls semantics analysis on parameters => eats )
void function_call();

void eval(); // DELETE

//////////////////////////////////////////////////////////////////////

// initial point of syntax and semantics analysis
// processes main body
void parse();

// processes function declaration/definition
void fun_decl();

// processes function body
void fun_body();

// magical recursive function which controls syntax of begin ... end and its subbodies
// DO NOT EVER TOUCH IT
int subbody(int begin_end, bool more, bool can_begin);

// puts all vars to current scope environment's symbol table
void var_decl(bool def_needed);

void parse_function_params(function_env_t *f, bool semicolon, bool in_table);
void parse_function_ret(function_env_t *f);
int parse_function_forward(function_env_t *f);

Symbol_t compare_functions_calls(ftable_item_t *f, int *type_index, int *id_index, int val_type);

int get_type(char id);

Symbol_t get_uval(function_env_t *f, int *index);

typedef struct {
    char *src;
    int token_type;
} pair_t;

token_t *init_token2();

void sort_fun(Symbol_t out);

void length_fun(Symbol_t in);

void find_fun(Symbol_t in);

void copy_fun(Symbol_t in);

#endif

