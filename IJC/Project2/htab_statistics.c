//htab_statistics.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

void htab_statistics(htab_t * t)
{
	if(t == NULL)
		return;

	int i;
	unsigned all = 0;
	unsigned int count = 0;
	unsigned int min = INT_MAX;
	unsigned int max = 0;
	unsigned int average = 0;

	htab_listitem * ptr;

	for(i=0 ; i < t->htab_size; i++)
	{

		ptr = t->data[i];
		count = 0;

		while(ptr != NULL)
		{
			count++;
			ptr = ptr->next;
		}

		if(min > count)
			min = count;
		if (max < count)
			max = count;
		all += count;
	}
	average = all/t->htab_size;
	printf("max = %d\nmin = %d\npriem = %d\n",max,min,average );
}