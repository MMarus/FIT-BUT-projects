// ppm.h
// Řešení IJC-DU1, příklad b), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Hlavickovy subor pre modul ppm.c v ktorom je definovana struktura
// na ulozenie ppm obrazka a funkcie pouzite v ppm.c module 

//deklaracia struktury pre obrazok
struct ppm {
        unsigned xsize;
        unsigned ysize;
        char data[];    // RGB bajty, celkem 3*xsize*ysize
};

struct ppm * ppm_read(const char * filename);

int ppm_write(struct ppm *p, const char * filename);

// Maximalna velkost obrazka 
#define MAX_SIZE 1000*1000*3
