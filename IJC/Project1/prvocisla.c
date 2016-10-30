// prvocisla.c
// Řešení IJC-DU1, příklad a), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Program vypocita 10 poslednych prvocisel
// od 2 do 101 milionov pomocou Eratosthenovho sita

#include<stdio.h>// I/O
#include"bit-array.h" // praca s bitovym polom

#define N 101000000

int main()
{
	BitArray(A,N);	//vytvorenie a vynulovanie pola

	Eratosthenes(A);	//naplnenie e-sita

	int i,j;

	for (i = A[0]-1,j=1;j<=10; i--) {
    if (GetBit(A,i) == 0)
    {
			j++;	
		}
  }

  for (j=1;j<=10;i++)
  {
  	if(GetBit(A,i) == 0)
  	{
  		printf("%d\n", i);
  		j++;
  	}
  }

	return 0;
}

