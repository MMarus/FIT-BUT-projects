//htab_lookup.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

htab_listitem * htab_lookup(htab_t *t, const char *key)
{
	if(t == NULL)
		return NULL;
	
	int i = hash_function(key,t->htab_size);

	htab_listitem * ptr;
	htab_listitem * tmp;

	ptr = t->data[i];
	tmp = ptr;
	
	while( ptr != NULL)
	{
		//printf("/%s %s/",ptr->key,key );
		if(strcmp(key,ptr->key) == 0)
		{
			//printf("%s\n"," uz mame" );
			ptr->data++; 
			return ptr;
		}
		tmp = ptr;
		ptr = ptr->next;
	}
	
	ptr = tmp;

	int len = strlen(key);

	htab_listitem * Item;

	Item = malloc(sizeof(htab_listitem));
	if(Item == NULL)
		return NULL;

	Item->key = malloc(sizeof(char) * (len+1));
	if(Item->key == NULL)
		return NULL;

	Item->key = strcpy(Item->key,key);
	
	Item->data = 1;
	Item->next = NULL;	

	if(t->data[i] == NULL)
	{
		ptr = Item; 
		t->data[i] = ptr;
	}
	else
	{
		ptr->next = Item;
		ptr = Item; 
	}

	return ptr;
}