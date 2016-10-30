// error.c
// Řešení IJC-DU1, příklad a) b), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Hlavickovy subor v ktorom su definovane funkcie pre vypis chybovych
// hlasok a na ukoncenie programu

// deklaracia fce na vypis chyb a ukoncenie programu
void FatalError(const char *fmt, ...);

// deklaracia fce na vypis chyb
void Warning(const char *fmt, ...);
