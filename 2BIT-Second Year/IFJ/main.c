/* ************************* main.c *************************** */
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

#include "main.h"

error_t ERR = OK;

ftable_t tab;
list_inst_t instruction_list;
FILE *stream;

int main(int argc, const char *argv[])
{
    stream = open_stream(argc, argv);

    ftable_init();
    atexit(ftable_free);

    parse();

    interpret();

    close_stream(stream);
    return 0;
}


FILE *open_stream(int argc, const char *argv[])
{
    FILE *stream;

    if (argc != 2) {
        err(INTERNAL_ERR, "No filename entered");
    }

    if ((stream = fopen(argv[1], "rt")) == NULL) {
        err(INTERNAL_ERR, "Unable to open file");
    }

    return stream;
}

void close_stream(FILE *stream)
{
    if (fclose(stream) == EOF)
        err(INTERNAL_ERR, "Unable to close file");
}