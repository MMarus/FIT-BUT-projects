//htab_init.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

htab_t * htab_init(unsigned int size)
{
	//treba skontrolovat size ci je menej ako max int
	htab_t * ptab;
	int i;

	ptab = (htab_t *)malloc(sizeof(htab_t)+ sizeof(htab_listitem*) *size);
	if(ptab == NULL)
		return NULL;
	
	ptab->htab_size = size;
	for(i=0;i<size;i++)
	{
		ptab->data[i] = NULL;
	}
	
	return ptab;
}