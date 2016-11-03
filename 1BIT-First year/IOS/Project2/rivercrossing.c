/*
 * Subor:   rivercrossing.c
 * Datum:   4.5.2014 12:45
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Projek 2 predmet IOS praca so zdielanou pametou a Semaformy
 * Popis:   Program pomocou semaforov a zdielanou pametou vypisuje postup prepravy osob cez rieku
 */

#include "rivercrossing.h"

#define pocetSHM 5	//pocet premennych v shared memory

int main(int argc, char const *argv[])
{
	params_t params; //struktura s parametramy
	
	params = doParams(argc,argv);

	if(params.errNum != 0) //ukonci program ak su zle parametre
		return 1;

	pid_t  pid; //uchovava cislo procesu
	FILE * fw = fopen("rivercrossing.out","w");

	setbuf(fw, NULL);	//vypne buffer

	const char *shmname = "/xmarus05";
	unsigned long * sharedMem;

	int fd = shm_open(shmname, O_RDWR|O_CREAT, 0600);	//vytvorenie zdielanej pamete
  if (fd < 0) // treba vsetko free atd
  {	
		shm_unlink(shmname);
		close(fd);
  	fclose(fw);
  	exit(EXIT_FAILURE);
  }	
 

  if (ftruncate(fd, sizeof(unsigned long)*pocetSHM))
  {
  	shm_unlink(shmname);
		close(fd);
  	fclose(fw);
  	exit(EXIT_FAILURE);
  }	

  sharedMem = mmap(NULL, sizeof(unsigned long)*pocetSHM, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (sharedMem == MAP_FAILED)
  {
  	munmap(NULL, pocetSHM*sizeof(unsigned long));
  	shm_unlink(shmname);
		close(fd);
  	fclose(fw);
  	exit(EXIT_FAILURE);
  }	

  sharedMem[0] = 0; // Hacker COUNT
  sharedMem[1] = 0; // Serf COUNT
 	sharedMem[2] = 1; // ACTION / ROW NUMBER
 	sharedMem[3] = 0; // NUMBER OF HACKERS Na MOLE
 	sharedMem[4] = 0; // NUMBER OF SERFS Na MOLE

	synch_t synch;
	synch.zapis = sem_open("/xMzapis", O_CREAT | O_EXCL, 0644, 1);	//semafor signalizujuci pracu so suborom
	synch.molo = sem_open("/xMmolo", O_CREAT | O_EXCL, 0644, 1);	//semafor signalizujuci molo
	synch.shmWrtID = sem_open("/xMshmWrtID", O_CREAT | O_EXCL, 0644, 1);	//semafor signalizujuci pracu s cislom akcie
	synch.shmWrtNum = sem_open("/xMshmWrtNum", O_CREAT | O_EXCL, 0644, 1);	//praca s ID Hack/serf
	synch.boarding = sem_open("/xMboarding", O_CREAT | O_EXCL, 0644, 0);	// Nastup na lod
	synch.boardH = sem_open("/xMboardH", O_CREAT | O_EXCL, 0644, 0);	//Nastup hackerov
	synch.boardS = sem_open("/xMboardS", O_CREAT | O_EXCL, 0644, 0);	//Nastup serfov
	synch.trip = sem_open("/xMtrip", O_CREAT | O_EXCL, 0644, 0);	//Signalizuje plavbu
	synch.landing = sem_open("/xMlanding", O_CREAT | O_EXCL, 0644, 0); //Signalizuje pristavanie
	synch.landed = sem_open("/xMlanded", O_CREAT | O_EXCL, 0644, 0);	//Signalizuje pristanie
	synch.end = sem_open("/xMend", O_CREAT | O_EXCL, 0644, 0);	//Signalizuje koniec prechodu cez rieku
	//synch.tripEnded = sem_open("/tripEnded", O_CREAT | O_EXCL, 0644, 0);	//Signalizuje ukoncenie plavby

	pid = fork();	//rozdeluje main proces na dva
	if(pid == 0)
	{
		pid = fork();
		if (pid == 0) //ChildProcess - vytvori hackerov
		{

			Pasengers(0,params,fw,synch,sharedMem);

			freeSEM;
			exit(0);
		}
		else if( pid > 0) //ParentProcess - vytvori serfs
	  {		
	  	
	  	Pasengers(1,params,fw,synch,sharedMem);

	  	freeSEM;
	  	
	  	wait(NULL);
	  }
	  else if( pid < 0)	//fork hodil chybu ukoncit program
	  {
	  	freeSEM;
	  	munmap(NULL, pocetSHM*sizeof(unsigned long));
	  	shm_unlink(shmname);
			close(fd);
	  	fprintf(stderr, "FORK CHYBA1");
	  	exit(2);
	  }
	  exit(0);
	}
	else if(pid > 0)	//cakat na dokoncenie child
		wait(NULL);
	else if(pid < 0) //ak nastala chyba uvolnit vsetky zdroje
	{
			freeSEM;
	  	munmap(NULL, pocetSHM*sizeof(unsigned long));
	  	shm_unlink(shmname);
			close(fd);
	  	fprintf(stderr, "FORK CHYBA");
	  	exit(2);
	}	
	
	freeSEM;

 	sem_unlink("/xMzapis");
	sem_unlink("/xMmolo");
	sem_unlink("/xMshmWrtID");
	sem_unlink("/xMshmWrtNum");
	sem_unlink("/xMboarding");
	sem_unlink("/xMboardH");
	sem_unlink("/xMboardS");
	sem_unlink("/xMtrip");
	sem_unlink("/xMlanding");
	sem_unlink("/xMlanded");
	sem_unlink("/xMend");
	//sem_unlink("/tripEnded");

	
	munmap(NULL, pocetSHM*sizeof(unsigned long));
  shm_unlink(shmname);
	close(fd);

  return 0;
}
