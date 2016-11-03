//wordcount.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include <stdio.h>
#include "io.h"
#include "hash_function.h"
#include "htab.h"


#define SIZE 12289
// SIZE zvolene podla http://planetmath.org/GoodHashTablePrimes

void it_print(htab_listitem * ptr)
{
	printf("%s\t%u\n",ptr->key,ptr->data);
}

int main()
{
	//FILE *f;

	char s[128];
	int max = 127;
	int strLen = 0;
	int WL = 0;


	htab_t *t;

	/*f = fopen("test","r");
	if(!f)
	{
		fprintf(stderr,"Cant open file\n");
		return 1;
	}*/

	t = htab_init(SIZE);
	if( t == NULL)
  {
    fprintf(stderr, "ERROR: Chyba pri alokacii pameti pre tabulku.\n");
    //fclose(f);
    return 1;
  }

	while( (strLen = fgetw(s,max,stdin)) != EOF)
	{

		if(strLen > max && WL !=1)
		{
			fprintf(stderr,"%s\n","ERROR: word too long" );
			WL = 1;
			strLen = max;
		} 
		
		//printf("key= %s len= %d\n",s,strLen );
		
		if(htab_lookup(t,s) == NULL)
		{
			fprintf(stderr, "ERROR: chyba pri alokacii pameti");
			htab_free(&t);
			return 1;
		}
			
	}
	
	htab_foreach(t,it_print);

	htab_free(&t);
	//fclose(f);

	return 0;
}