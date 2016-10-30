//hash_function.h
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

unsigned int hash_function(const char *str, unsigned htab_size);