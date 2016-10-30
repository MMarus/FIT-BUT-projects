// ppm.c
// Řešení IJC-DU1, příklad b), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Modul pre nacitanie a vpisanie obrazkov vo formate .ppm 

#include<stdlib.h>	
#include<stdio.h>	//vystup
#include"error.h"	//vypis chyb
#include"ppm.h"	// deklaracia funkcii modulu ppm.c

//nacitanie obrazku to struktury
struct ppm * ppm_read(const char * filename)
{
	FILE *f;

  f = fopen(filename,"rb");
  if (!f) {
      Warning("Nepodarilo sa otvorit subor\n");
			return NULL;
  }

	int ysize ;
	int xsize ;
	int colour;
	char format[3];


	if(fscanf(f,"%2s %d %d %d",format,&xsize,&ysize,&colour) != 4 || format[0] != 'P' || format[1] != '6' || xsize*ysize*3 > MAX_SIZE || colour != 255)
	{
		fclose(f);	
		Warning("Nepodarilo sa nacitat subor zly format\n");
		return NULL;
	}

	struct ppm * pimg; 
		
		
	pimg = (struct ppm *)malloc(sizeof(struct ppm)+ sizeof(char) * xsize * ysize * 3); 
		
	pimg->xsize = xsize;
	pimg->ysize = ysize;

	if(fread(pimg->data,3 * pimg->xsize,pimg->ysize,f) != ysize)
  {
		fclose(f);	
		free(pimg);	
		Warning("Nepodarilo sa nacitat subor\n");
		return NULL;
	}	

	fclose(f);
	return pimg;
}

//zapis struktury do ppm obrazku
int ppm_write(struct ppm *p,const char * filename)
{
	FILE *f;
	
	f = fopen(filename, "wb");
		if (!f) {
			Warning("Nepodarilo sa naciat subor '%s'\n", filename);
			return -1;
		}
	
	fprintf(f,"P6\n");

	fprintf(f,"%d %d\n",p->xsize,p->ysize);

	fprintf(f,"255");
	
	if(fwrite(p->data,3 * p->xsize,p->ysize,f) !=p->ysize)
		return -1;

	fclose(f);

	return 0;
}
