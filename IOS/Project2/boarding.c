/*
 * Subor:   boarding.c
 * Datum:   4.5.2014 12:45
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Projek 2 predmet IOS praca so zdielanou pametou a Semaformy
 * Popis:   Modul pre prepravu, a obsluhu lode
 */

#include "rivercrossing.h"

void board(int Time,FILE *fw,int Category,char * CatName,unsigned long ID,synch_t synch,unsigned long *ActionID,unsigned long *NumOfH,unsigned long *NumOfS,int Type)
{
	//sem_wait(synch.molo);	//cakaj na molo
	sem_wait(synch.shmWrtNum);	//cakaj na pristup k poctu H/S

	beginBording(Type,Category,NumOfH,NumOfS,synch);	//spusti semafory podla kategorie a podla druhu nalodenia
	
	sem_wait(synch.zapis);
	//vypis nalodenie
 	fprintf(fw,"%lu: %s: %lu: boarding: %lu: %lu\n",(*ActionID)++,CatName,ID,(*NumOfH),(*NumOfS));
 	
 	sem_post(synch.zapis);
 	
	sem_wait(synch.boarding);	//cakaj kym sa nalodia 3 ludia
	sem_wait(synch.boarding);
	sem_wait(synch.boarding);

	sem_post(synch.trip);	//mozeme spustit plavbu
	sem_post(synch.trip);
	sem_post(synch.trip);

	sem_wait(synch.zapis);
 	
 	fprintf(fw,"%lu: %s: %lu: captain\n",(*ActionID)++,CatName,ID);//vypiseme kapitana
	 	
 	sem_post(synch.zapis);

	sleeping(Time);	//simulacia plavby 

	sem_post(synch.landing);	//spustime 3 semafory aby sa moholo vykonat pristanie
	sem_post(synch.landing);
	sem_post(synch.landing);

	sem_wait(synch.zapis);
 	//zapiseme ze pristal aj kapitan
 	fprintf(fw,"%lu: %s: %lu: landing: %lu: %lu\n",(*ActionID)++,CatName,ID,(*NumOfH),(*NumOfS));
	 	
 	sem_post(synch.zapis);

 	sem_wait(synch.landed);
 	sem_wait(synch.landed);
 	sem_wait(synch.landed);	

 	sem_post(synch.shmWrtNum);	//UVOLNIme pristup k ID H/S
	sem_post(synch.molo);	//uvolnime molo
 	
 	/// LANDING

}

void sleeping(int Time)	//simulacia spanku
{
 	int random = rand() % Time+1;
		
	usleep(random * 1000);
	//printf("sleep for %d\n",random);

}

//spusti smeafory podla toho kto sa ma nalodit
void beginBording(int Type,int Category,unsigned long *NumOfH,unsigned long *NumOfS,synch_t synch)
{
	if(Type == 2)
	{
		(*NumOfH) -= 2;
		(*NumOfS) -= 2;

		sem_post(synch.boardH);
		sem_post(synch.boardS);
		
		if (Category == 0)
			sem_post(synch.boardS);
		else
			sem_post(synch.boardH);
	}
	if(Type == 4)
	{
		if (Category == 0)
		{
			(*NumOfH) -= 4; 
			sem_post(synch.boardH);
			sem_post(synch.boardH);
			sem_post(synch.boardH);
		}	
		else
		{
			(*NumOfS) -= 4; 
			sem_post(synch.boardS);
			sem_post(synch.boardS);
			sem_post(synch.boardS);
		}	
	}
}
