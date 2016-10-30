//htab_free.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

void htab_free(htab_t **t)
{
	if(*t == NULL)
		return;
	
	htab_clear(*t);

	free(* t );
	
	*t= NULL;
}