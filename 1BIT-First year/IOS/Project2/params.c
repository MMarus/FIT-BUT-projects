/*
 * Subor:   params.c
 * Datum:   4.5.2014 12:45
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Projek 2 predmet IOS praca so zdielanou pametou a Semaformy
 * Popis:   Spracovanie parametrov
 */

#include "rivercrossing.h"

//Makro pre tlac chyby plus return E_NOK
#define exitParam \
	fprintf(stderr, "zla zadane parametre \n");\
	params.errNum = E_NOK;\
	return params;\

params_t doParams(int argc, char const *argv[])
{
	//vynolovanie struktury
	params_t params = {
		.errNum = E_OK,
		.Count = 0, 
		.hackerTime = 0, 
		.serfTime = 0, 
		.TripTime = 0, 
	};

	char *strErr;

	if(argc != 5)
	{
		exitParam;
	}

	params.Count = strtoul(argv[1],&strErr,10);
	if(*strErr != 0 || params.Count % 2 != 0)
	{
		exitParam;
	}	

	params.hackerTime = strtoul(argv[2],&strErr,10);
	if(*strErr != 0 || params.hackerTime > 5001)
	{
		exitParam;
	}

	params.serfTime = strtoul(argv[3],&strErr,10);
	if(*strErr != 0 || params.serfTime > 5001)
	{
		exitParam;
	}

	params.TripTime = strtoul(argv[4],&strErr,10);
	if(*strErr != 0 || params.TripTime > 5001)
	{
		exitParam;
	}

	return params;
}