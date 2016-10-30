//hash_function.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include "htab.h"

unsigned int hash_function(const char *str, unsigned htab_size) {
	unsigned int h=0;
	const unsigned char *p;
	for(p=(const unsigned char*)str; *p!='\0'; p++)
	    h = 65599*h + *p;
	return h % htab_size;
}