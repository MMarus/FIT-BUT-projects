//htab_remove.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

void htab_remove(htab_t *t, const char *key)
{
	if(t == NULL)
		return;
	int i = hash_function(key,t->htab_size);
	htab_listitem * ptr;
	htab_listitem * tmp;

	ptr = t->data[i];
	tmp = ptr;
	
	while( ptr != NULL)
	{
		if(strcmp(key,ptr->key) == 0)
		{
			//printf("%s\n"," uz mame" );
			//najskor prelinkovat predosly s dalsim
			tmp->next = ptr->next;
			if(t->data[i] == ptr)
				t->data[i] = ptr->next;

			free(ptr->key);
			free(ptr);
			
			break;
		}
		tmp = ptr;
		ptr = ptr->next;
	}

}
