/*
 * Subor:   passengers.c
 * Datum:   4.5.2014 12:45
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Projek 2 predmet IOS praca so zdielanou pametou a Semaformy
 * Popis:   Modul vyvara postupne zadany pocet Hackerov/serfov
 */

#include "rivercrossing.h"

void  Pasengers(int Category,params_t params,FILE * fw,synch_t synch,unsigned long *SHM)
{
	unsigned long i;
	char * CatName;

	if (Category == 0)
	{
		CatName = "hacker";
	}
	if(Category == 1)
	{
		CatName = "serf";
	}

	for (i = 1; i <= params.Count; i++)
	{
		//Cakaj random 0-time nanosec kym pokracuje pri vytvarani
		
		if(Category == 0)
			sleeping(params.hackerTime);
		else
			sleeping(params.serfTime);
		
		//Vytvor proces
		pid_t pid = fork();
		
		
		if(pid > 0) //child
		{
			doRiver(Category,CatName,params.TripTime,fw,synch,SHM,params.Count);	//sprav prechod cez rieku

			freeSEM;

			exit(0);
		}
		else if(pid == 0)	//child
		{	
			wait(NULL);	//cakaj kym bude prechod hotovy
		}
		else if(pid < 0)//pri chyby skonci cyklus a ukonci proces
		{
			freeSEM;
	  	fprintf(stderr, "FORK CHYBA2");
	  	break;
	  	exit(2);
	  }
	}
	
}