/* ************************* ial.h ************************* */
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

#ifndef IAL_H
#define IAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ial.h"
#include "i_string.h"
#include "error.h"
#include "scanner.h"
#include "token.h"
#include "expr_inter.h"

char *copy(char *string, int start, int end);

int find(char *string, char *search);

int length(char *string);

char *sort(char *string);


///////////////////////////// SYMBOL TABLE ////////////////////////////////

typedef struct {
    int ids_len;
    int types_len;
    int allocated_ids;
    int allocated_types;
    char *ids;
    char *types;
} params_cmp_t;

typedef struct symbol_s {
    string_t *str;
    int type;
    bool defined;
    value_u *data;

    struct symbol_s *left;
    struct symbol_s *right;
} *Symbol_t, symbol_t;

// function node in separate ftable
typedef struct {
    bool declared;
    bool defined;
    void *address;
    string_t *str; // ID
    params_cmp_t params_cmp_string; // Params
    symbol_t return_value; // Ret val

    Symbol_t env_root; // Link to root node of local vars
} function_env_t;

// function item in ftable
typedef struct ftable_item_s {
    function_env_t item;
    struct ftable_item_s *next;
} ftable_item_t;

// linked list of functions
typedef struct ftable_list_s {
    ftable_item_t *first;
    ftable_item_t *active;
    ftable_item_t *last;
} ftable_t;

extern ftable_t tab;

//////////////////////////////////////////////////////////////////////

// activates global environment
void activate_global_env();

// if function found, sets it as active, if not found returns NULL
function_env_t *activate_function_env(string_t *f);

// finds function in function function table and returns pointer to function table
ftable_item_t *find_function(string_t *function_name);

// finds symbol in current active environment, if not found tries to find it global symbol table
// returns NULL if not found
Symbol_t find_symbol(string_t *symbol_name);

// adds to symbol table according to the current active environment
Symbol_t add_to_stable(token_t *t);

// finds symbol && puts value to it according to the current active environment
// returns NULL if not found
int update_to_stable(string_t *id, symbol_t *src);

//////////////////////////////////////////////////////////////////////


Symbol_t find_symbol_wrapper(char *symbol_name);

// checks if all functions are defined
function_env_t *all_defined();

// appends function f to the function table and initializes it
function_env_t *function_append(string_t *f);

// only removes function from function table
// returns pointer to function environment, has to be free separately
function_env_t *function_delete(function_env_t *f);

// returns 0 if functions are identical
int compare_functions(function_env_t *f_tab, function_env_t *f_copy, token_t *token);

// adds function names to special string
void add_id_param_cmp(function_env_t *f, token_t *t);

// adds function parameters to special string
void add_type_param_cmp(function_env_t *f, token_t *t);


//////////////////////////////////////////////////////////////////////

// initializes function table, has to be called only once
void ftable_init();

// frees function table and all its contents
void ftable_free();

// frees function environment (str, symbol table, ...)
void function_free(function_env_t *f);

// initializes string for comparing parameters
void init_param_cmp(function_env_t *f);

value_u *init_value_t();

symbol_t *init_symbol_t();

//////////////////////////////////////////////////////////////////////

void tree_init(Symbol_t *r);
void tree_free(Symbol_t r);
Symbol_t add_to_tree(Symbol_t *r, token_t *t);
Symbol_t find_in_tree(Symbol_t r, string_t *str);

// prints all identifiers in alphabetical order in a symbol table
// r is pointer to the root node
void dbg_print_tree(Symbol_t r);

int convert_to_k_type(int type);

#endif