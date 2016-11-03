// steg-decode.c
// Řešení IJC-DU1, příklad b), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Program pomocou eratosthenovho sita zisti prvociselne hdonty, ktore
// sa puziju pri hladani spravy zakodovanej v obrazku vo formate .ppm 

#include<stdio.h>	//vstup vystup
#include"ppm.h"	//spracovanie ppm obrazkov
#include"error.h"	//vypis chyb
#include"bit-array.h"	//praca s bitovym polom
#include<stdlib.h>	//
#include<ctype.h>	//isprint

int main(int argc, char * argv[])
{
		if(argc == 2)
		{
			struct	ppm * Name ;
			
			Name = ppm_read(argv[1]);
			if(Name == NULL)
				return 1;
			
			BitArray(A,MAX_SIZE);
			
			Eratosthenes(A);
			
			int i,j=0;
			char c = 0;
		
			for(i=2;i<Name->xsize * Name->ysize *3;i++)
			{
				if (GetBit(A,i) == 0)
				{
					
					if(Name->data[i+1]%2)	
					{
						c |= 1 << j;
					}
					j++;
				}
				if(j == 8)
				{
					if(c == '\0')
						break;
					if(isprint(c))
					{
						putchar(c);
					}
					else
					{	
						putchar('\n');
						free(Name);
						FatalError("Sprava obsahuje netisknutelne znaky\n");
					}
					c = 0;
					j = 0;
				}
			}

			free(Name);

			return 0;
		}
		else 
		return 1;
}
