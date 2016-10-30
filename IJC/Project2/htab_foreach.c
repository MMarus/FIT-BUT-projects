//htab_foreach.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

void htab_foreach(htab_t *t,void (*f)(htab_listitem*))
{
	if(t == NULL)
		return;

	int i;

	htab_listitem * ptr;

	for(i=0;i<t->htab_size;i++)
	{
		
		ptr = t->data[i];

		while(ptr != NULL)
		{
			f(ptr);
			ptr = ptr->next;
		}
	}
}
/*
void htab_list_print(htab_t *t)
{
	if(t == NULL)
		return;

	int i;

	htab_listitem * ptr;

	for(i=0;i<t->htab_size;i++)
	{
		
		ptr = t->data[i];

		while(ptr != NULL)
		{
			printf("%s -> ",ptr->key);
			ptr = ptr->next;
		}
		printf("\n");
	}
}*/