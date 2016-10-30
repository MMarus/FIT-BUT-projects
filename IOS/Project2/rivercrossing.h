/*
 * Subor:   rivercrossing.h
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Projek 2 predmet IOS praca so zdielanou pametou a Semaformy
 * Popis:   Hlavickovy subor pre program rivercrossing
 */

#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
// fork() a wait()
#include <unistd.h>
#include <sys/wait.h>

// Semafory a zdielana pamat
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define E_OK 0	 //- err status ok
#define E_NOK 1  //- err status not ok

typedef struct param
{
    int errNum;
    unsigned long Count;
    unsigned long hackerTime;
    unsigned long serfTime;
    unsigned long TripTime;
} params_t;


typedef struct synch
{
    sem_t *zapis;         //pristup k suboru a pristup k ACTION
    sem_t *molo;  //pozadavek na zapis
    sem_t *shmWrtID;
    sem_t *shmWrtNum;
    sem_t *boarding;
    sem_t *boardH;
    sem_t *boardS;
    sem_t *trip;
    sem_t *landing;
    sem_t *landed;
    sem_t *end;
} synch_t;

#define freeSEM \
sem_close(synch.zapis);\
sem_close(synch.molo);\
sem_close(synch.shmWrtID);\
sem_close(synch.shmWrtNum);\
sem_close(synch.boardH);\
sem_close(synch.boardS);\
sem_close(synch.boarding);\
sem_close(synch.trip);\
sem_close(synch.landing);\
sem_close(synch.landed);\
sem_close(synch.end);\
fclose(fw);\

params_t doParams(int argc, char const *argv[]);

void  Pasengers(int Category,params_t params,FILE * fw,synch_t synch,unsigned long *SHM);

void doRiver(int Category,char * CatName,int Time,FILE *fw,synch_t synch,unsigned long *SHM,unsigned long Count);

void board(int Time,FILE *fw,int Category,char * CatName,unsigned long ID,synch_t synch,unsigned long *ActionID,unsigned long *NumOfH,unsigned long *NumOfS,int Type);

void sleeping(int Time);

void beginBording(int Type,int Category,unsigned long *NumOfH,unsigned long *NumOfS,synch_t synch);