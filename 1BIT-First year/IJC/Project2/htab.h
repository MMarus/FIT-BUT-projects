//htab.h
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "hash_function.h"


typedef struct item htab_listitem;

struct item
{
	char *key;
	int data;
	htab_listitem * next;
};

typedef struct 
{
	int htab_size;
	htab_listitem	* data[];

}htab_t;

void htab_foreach(htab_t *t,void (*f)(htab_listitem*));

void htab_remove(htab_t *t, const char *key);

htab_t * htab_init(unsigned int size);

htab_listitem * htab_lookup(htab_t *t, const char *key);

void htab_clear(htab_t *t);

void htab_statistics(htab_t * t);

void htab_free(htab_t **t);

//void htab_list_print(htab_t *t);