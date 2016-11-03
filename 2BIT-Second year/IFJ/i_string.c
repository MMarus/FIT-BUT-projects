/* ************************* i_string.c ************************* */
/* Kodovani:		UTF-8					                    */
/* Predmet:		Formalni jazyky a prekladace (IFJ)	            */
/* Projekt:		Implementace interpretu imperativniho	        */
/*			    jazyka IFJ14				                    */
/* Varianta zadani:	a/2/I					                    */
/* Autori:		Fáber Jakub		xfaber02	                    */
/*			Hladík Daniel		xhladi21	                    */
/*			Marušič Marek		xmarus05	                    */
/*			Surovčík Tomáš		xsurov04	                    */
/* ************************************************************ */

#include "i_string.h"

char *load_string(FILE *stream) {
    char *string = NULL;

    if ((string = (char *) malloc(DEFAULT_MEM_ALLOC)) == NULL) {
        err(INTERNAL_ERR, "Unable to allocate for i_string");
    }

    int c;
    while ((c = fgetc(stream)) != EOF)
        if (!isspace(c))
            break;

    if (c == EOF) {
        free(string);
        err(LEXICAL_ERR, "Empty source file");
    }

    ungetc(c, stream);

    char *new_string = NULL;
    unsigned multipler = 1;
    unsigned counter = 0;

    while ((c = fgetc(stream)) != EOF && !isspace(c)) {
        string[counter] = c;
        ++counter;

        if (counter == DEFAULT_MEM_ALLOC) {
            ++multipler;

            if ((new_string = realloc(string, DEFAULT_MEM_ALLOC * multipler)) == NULL) {
                err(INTERNAL_ERR, "Unable to realloc for i_string");
            }
            else {
                string = new_string;
                counter = 0;
            }
        }
    }
    string[counter] = '\0';

    return string;
}

char *copy_string(char *src, int length) {
    char *dest = NULL;

    if (src == NULL) {
        err(INTERNAL_ERR, "Src source in copy_string() can't be NULL");
        return NULL;
    }

    if ((dest = malloc(length + 1)) == NULL) {
        err(INTERNAL_ERR, "Error allocating for dest in copy_string()");
        return NULL;
    }

    dest = strncpy(dest, src, length + 1);

    return dest;
}

int cmp_string(string_t *s1, string_t *s2) {
    if ((s1 == NULL && s2 != NULL) || (s1 != NULL && s2 == NULL))
        return -1;

    return strcmp(s1->name, s2->name);
}

string_t *make_string(string_t *src) {
    string_t *dst;
    if ((dst = malloc(sizeof(string_t))) == NULL) {
        err(INTERNAL_ERR, "Unable to allocate in make_string()");
        return NULL;
    }

    // length is without '\0'
    if ((dst->name = malloc(src->length + 1)) == NULL) {
        err(INTERNAL_ERR, "Unable to allocate in make_string()");
        return NULL;
    }
    strncpy(dst->name, src->name, src->length + 1);
    dst->length = src->length + 1;

    return dst;
}

void free_string(string_t *src) {
    if (src) {
        free(src->name);
        free(src);
    }
}

void reverse_string(char *str) {
    /* skip null */
    if (str == 0) {
        return;
    }

    /* skip empty string */
    if (*str == 0) {
        return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start) {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;

        /* move */
        ++start;
        --end;
    }
}
