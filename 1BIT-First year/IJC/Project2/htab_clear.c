//htab_clear.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

void htab_clear(htab_t *t)
{
	if(t == NULL)
		return;

	htab_listitem * ptr;
	htab_listitem * tmp;

	int i;
	for(i=0; i<t->htab_size; i++)
	{
		
		ptr = t->data[i];

		while(ptr != NULL)
		{
			tmp = ptr->next;
			
			free(ptr->key);
			free(ptr);

			ptr = tmp;
		}
		t->data[i] = ptr;
	}
}