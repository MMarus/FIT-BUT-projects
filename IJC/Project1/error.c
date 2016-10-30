// error.c
// Řešení IJC-DU1, příklad a) b), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Modul ktory vypisuje chybove hlasky a ukoncuje program

#include"error.h" //chybove hlasky

#include<stdio.h> //vystup

#include<stdarg.h>	//koli spracovaniu argumentov funkcii

#include<stdlib.h>	//kvoli funkcii exit

// vypis chyby a ukoncenie programu
void FatalError(const char *fmt, ...)
{
	va_list args;

	va_start( args, fmt );
	fprintf(stderr,"CHYBA: ");
	vfprintf(stderr,fmt,args);
	va_end(args);	

	exit(1);
}

//	vypis chyby
void Warning(const char *fmt, ...)
{
	va_list args;

	va_start( args, fmt );
	fprintf(stderr,"CHYBA: ");
	vfprintf(stderr,fmt,args);
	va_end(args);	
}
