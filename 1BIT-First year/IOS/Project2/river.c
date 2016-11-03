/*
 * Subor:   river.c
 * Datum:   4.5.2014 12:45
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Projek 2 predmet IOS praca so zdielanou pametou a Semaformy
 * Popis:   Modul simuluje nastup na molo, na lod a vylodenie
 */

#include "rivercrossing.h"

void doRiver(int Category,char * CatName,int Time,FILE *fw,synch_t synch,unsigned long *SHM,unsigned long Count)
{
	unsigned long *ActionID = &SHM[2];
	unsigned long *NumOfH = &SHM[3];
	unsigned long *NumOfS = &SHM[4];
	unsigned long ID;
	unsigned long *ID_p;

	if(Category == 0)
		ID_p = &SHM[0];

	if(Category == 1)
		ID_p = &SHM[1];

//vytvorenie
	
	sem_wait(synch.shmWrtID); 		//cakaj na povolenie na pristup k pameti ------WAIT
	(*ID_p)++; 					//pridaj Hack/Serf
	ID = (*ID_p); 			//zapis pocet Hack/Serf do ID
	sem_post(synch.shmWrtID); //uvolni shmW
	
	sem_wait(synch.zapis); /// pri zapise do suboru sa vzdy pristupuje k ActionID ------WAIT
	fprintf(fw,"%lu: %s: %lu: started\n",(*ActionID)++,CatName,ID); //zapis started
	sem_post(synch.zapis); //uvolni subor


	sem_wait(synch.molo); //pristup na molo ------WAIT
	sem_wait(synch.shmWrtNum); //pristup k poctu ludi na mole ------WAIT
	
	if(Category == 0)
		(*NumOfH)++;
	else
		(*NumOfS)++;

	sem_wait(synch.zapis);	// Cakaj na pristup k suboru

	fprintf(fw,"%lu: %s: %lu: waiting for boarding: %lu: %lu\n",(*ActionID)++,CatName,ID,(*NumOfH),(*NumOfS));

	sem_post(synch.zapis);
	sem_post(synch.shmWrtNum);

//spustac nastupovania ak je dostatocny pocet ludi na mole spusti plavby
	if((*NumOfH) >= 2 && (*NumOfS) >= 2 )
	{
		//
		board(Time,fw,Category,CatName,ID,synch,&SHM[2],&SHM[3],&SHM[4],2);	//plavba 2 : 2
	}
	else if((*NumOfH) >= 4 || (*NumOfS) >= 4)
	{	
		board(Time,fw,Category,CatName,ID,synch,&SHM[2],&SHM[3],&SHM[4],4);	//plavba 4 : 0/0:4
	}
	else //ak nieje mozne spustit plavbu cakame kym to bude mozne
	{
		sem_post(synch.molo);
		if(Category == 0)	//podla kategorie cakaj na nastup
		{
			sem_wait(synch.boardH);	//cakaj na povolenie nastupu
		}
		else
		{
			sem_wait(synch.boardS);
		}

		sem_wait(synch.zapis);
		fprintf(fw,"%lu: %s: %lu: boarding: %lu: %lu\n",(*ActionID)++,CatName,ID,(*NumOfH),(*NumOfS));	
		sem_post(synch.zapis);

			sem_post(synch.boarding);//daj znamenie ze clovek nastupil
			sem_wait(synch.trip);	//cakaj kym vsetci nastupia a bude mozne zahajit plavbu

			sem_wait(synch.zapis);
			//vypis aku mam funkciu na lodi
		fprintf(fw,"%lu: %s: %lu: member\n",(*ActionID)++,CatName,ID);	
		sem_post(synch.zapis);

		sem_wait(synch.landing);	//cakaj kym sa ukonci plavba a potom mozeme zapisat ze pristavame
		
		sem_wait(synch.zapis);
		fprintf(fw,"%lu: %s: %lu: landing: %lu: %lu\n",(*ActionID)++,CatName,ID,(*NumOfH),(*NumOfS));	
		sem_post(synch.zapis);			

		sem_post(synch.landed);	//po uspesnom pristani dame znamenie ze vsetci vystupili
		
	}
	
	
	// zistime ci sa prepravili vsetci
	sem_wait(synch.zapis);
	
	if((*ActionID) == Count*10+1)	//ak su skoncene vsetky akcie mozeme spustit end aby vypisal FINSHED
		sem_post(synch.end);

	sem_post(synch.zapis);
	
	//wait for end a zapis FINISHED
	sem_wait(synch.end);

	sem_wait(synch.zapis);
	
	fprintf(fw,"%lu: %s: %lu: finished\n",(*ActionID)++,CatName,ID);

	sem_post(synch.zapis);

	sem_post(synch.end);

//konec vytvorenia
}