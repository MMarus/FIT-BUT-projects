// eratosthenes.c
// Modul pro Řešení IJC-DU1, příklad a)b), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Modul do bitoveho pola ulozi eratosthenovo sito

#include"bit-array.h"	//praca s bitovim polom
#include <math.h>	//kvoli sqrt
#include"error.h"	//chybove vystupy

//Funkcia ktora naplni bitove pole eratosthenovym sitom
void Eratosthenes(BitArray_t  cisla[])
{
	int i,j;

  for (i = 2; i < sqrt(cisla[0]); i++) {
      if (GetBit(cisla,i) == 1) continue;
      for(j = 2*i; j < cisla[0]; j += i)
          SetBit(cisla,j,1);
  }
}

