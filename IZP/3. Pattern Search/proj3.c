/*
 * Subor:   proj3.c
 * Datum:   11.12.2013
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Hladanie obrazcov
 * Popis:	Program vyhladá obrazce v bitmape, podla zadanych parametrov
*/

#include<stdio.h>		/*praca so vstupom/vystupom*/
#include<stdlib.h>
#include<string.h>		/*strcmp*/

/* Kody chyb*/
enum tecodes
{
  EOK = 1,     	/**< Bez chyby */
  ECLWRONG,    	/**< Chybný příkazový řádek. */
  EMWRONG,		/**< Chybna matica*/
  ENotFound,	/**< Utvar sa nenasiel*/
};

const char *ECODEMSG[] =
{
  [EOK] = "Vse v pořádku.\n",
  [ECLWRONG] = "Chybné parametry příkazového řádku!\n",
  [ENotFound] = "Obrázok neobsahuje hladany utvar\n",
  [EMWRONG] = "Zla matica\n",
};

/*Struktura pre bitmapu*/
typedef struct {
  int rows;
  int cols;
  char *cells;
} Bitmap;

/*Tlac napovedy*/
int PrintHelp()
{
  printf("Subor: proj3.c\n autor: Marek Marusic,xmarus05@stud.fit.vutbr.cz \n Program sluzi na vyhladavanie obrazcov v bitmape. \n Program sa spusta pomocou argumentov:"
  "--help sluzi na vypis tejto napovedy\n"
  "--test obrazok.txt sluzi na zistenie ci je bitmapa validna\n"
  "--hline obrazok.txt sluzi na vypis suradnic najdlhsej horizontalnej usecky\n"
  "--vline obrazok.txt sluzi na vypis suradnic najdlhsej vertikalnej usecky\n"
  "--square obrazok.txt sluzi na vypis suradnic najvacsieho stvorca\n"
  );
  return EOK;
}

/*-------------------------------------Alokacia a praca so suborom-----------------------------------*/
/*Alokacia pamete pre bitmapu*/
void alloc2d(Bitmap *a,int rows,int cols)
{
	a->cells = (char *)malloc(sizeof(char) * rows * cols);
	if(a->cells != NULL)
	{
		a->rows = rows;
		a->cols = cols;
	}
}

/*Dealokacia pamete bitmapy*/
void free2D(Bitmap *a)
{
	if(a != NULL && a->cells != NULL)
	{
		free(a->cells);
		a->rows = 0;
		a->cols = 0;
	}
}

/*nacitanie bitmapy*/
int readM(char * fname,Bitmap *a)
{
  FILE *f;

  f = fopen(fname,"r");
  if (!f) {
      return EMWRONG;
  }

  int rows,cols;

  if(fscanf(f,"%d %d",&rows,&cols) != 2 || rows <= 1 || cols <= 1)
    return EMWRONG;

  alloc2d(a,rows,cols);

  int i,j,test=0;

  for(i=0;i<rows*cols;i++)
  {
    test += fscanf(f,"%d ",&j);
    if(test-1 != i || j > 1 || j < 0)
      {
        fclose(f);
		free2D(a);
        return EMWRONG;
      }
    a->cells[i] = j;
  }
  if(fgetc(f) != EOF)
    {
		fclose(f);
		free2D(a);
		return EMWRONG;
	}
  fclose(f);
  return EOK;
}

/*-------------------------------------Alokacia a praca so suborom-----------------------------------*/

/*-------------------------------------Mapovacie Funkcie-----------------------------------*/
/*Zistenie hodnoty policka v matici*/
char getcolor(Bitmap *a, int xr, int ys)
{
  if(xr < a->rows && ys < a->cols)
    return a->cells[xr*a->cols+ys];
  else
    return EOF;
}

/*Najdi vertikalnu dlzku usecky zacinajucej v bode [x,y]*/
int findVlength(Bitmap *a,int xr, int ys)
{
  int i,length = 0;
  for(i=xr;getcolor(a,i,ys)==1;i++)
  {
      length++;
  }
  return length;
}

/*Najdi horizontalnu dlzku usecky zacinajucej v bode [x,y]*/
int findHlength(Bitmap *a,int xr, int ys)
{
  int i,length = 0;
  for(i=ys;getcolor(a,xr,i)==1;i++)
  {
      length++;
  }
  return length;
}

/*Najdi najdlzsiu horizontalnu usecku*/
int find_hline(Bitmap *a, int *x1, int *y1, int *x2, int *y2)
{
  int xr,ys,length=0,tmp=0;
  for(xr=0;xr<a->rows;xr++)
  {
    for(ys=0;ys<a->cols;ys++)
    {
      length =findHlength(a,xr,ys);
      if(length > tmp)
      {
        *x1 = xr;
        *y1 = ys;
        *x2 = xr;
        *y2 = ys+length-1;
        tmp = length;
      }
    }
  }
  if(tmp < 2)
    return ENotFound;
  return EOK;
}

/*Najdi najdlzsiu vertikalnu usecku*/
int find_vline(Bitmap *a, int *x1, int *y1, int *x2, int *y2)
{
  int xr,ys,length=0,tmp=0;
  for(ys=0;ys<a->cols;ys++)
  {
    for(xr=0;xr<a->rows;xr++)
    {
      length =findVlength(a,xr,ys);
      if(length > tmp)
      {
        *x1 = xr;
        *y1 = ys;
        *x2 = xr+length-1;
        *y2 = ys;
        tmp = length;
      }
    }
  }
  if(tmp < 2)
    return ENotFound;
  return EOK;
}

/*Najdi najdvecsi stvorec*/
int find_square(Bitmap *a, int *x1, int *y1, int *x2, int *y2)
{
  int xr,ys,i,length,lengthNext,l3,lsquare = 0,tmp = 0;
  //prehadzam maticu vertikalne postupne -
  for(ys=0;ys<a->cols;ys++)
  {
    for(xr=0;xr<a->rows;xr++)
    {
      length = findHlength(a,xr,ys);
      if(length >= 2)
      {
        for(i=xr+1;getcolor(a,i,ys) == 1;i++)
        {
          lengthNext = findHlength(a,i,ys);
          lsquare = i-xr+1;
          if(length >= (lsquare)  && lengthNext >= (lsquare) )
          {

            l3 =  findVlength(a,xr,ys+lsquare-1);
            if(l3 >= lsquare && lsquare > tmp)
            {
              *x1 = xr;
              *y1 = ys;
              *x2 = xr+lsquare-1;
              *y2 = ys+lsquare-1;
              tmp = lsquare;
            }
          }
        }
      }
    }
  }
  if(tmp < 2)
    return ENotFound;
  return EOK;
}
/*-------------------------------------Mapovacie Funkcie-----------------------------------*/

/*-------------------------------------Spracovanie argumentov a volanie mapovacich funkcii-----------------------------------*/

/*vypis testu*/
int doTest(char * fname)
{
  int err=EOK;
  Bitmap a;
  err = readM(fname,&a);
  if(err == EOK)
    {
      printf("Valid\n");
      free2D(&a);
    }
  else
    printf("Invalid\n");
  return EOK;
}

/*Vypis a volanie hladania najdlhsej vertikalnej usecky*/
int doVline(char * fname)
{
  int err=EOK,x1,y1,x2,y2;
  Bitmap a;
  err = readM(fname,&a);
  if(err == EOK)
  {
    err = find_vline(&a,&x1,&y1,&x2,&y2);
    if(err == EOK)
      printf("%d %d %d %d\n",x1,y1,x2,y2);
    free2D(&a);
  }
  return err;
}

/*Vypis a volanie hladania najdlhsej horizontalnej usecky*/
int doHline(char * fname)
{
  int err=EOK,x1,y1,x2,y2;
  Bitmap a;
  err = readM(fname,&a);
  if(err == EOK)
  {
    err = find_hline(&a,&x1,&y1,&x2,&y2);
    if(err == EOK)
      printf("%d %d %d %d\n",x1,y1,x2,y2);
    free2D(&a);
  }
  return err;
}

/*Vypis a volanie hladania najdlhsieho stvorca*/
int doSquare(char * fname)
{
  int err=EOK,x1,y1,x2,y2;
  Bitmap a;
  err = readM(fname,&a);
  if(err == EOK)
  {
    err = find_square(&a,&x1,&y1,&x2,&y2);
    if(err == EOK)
      printf("%d %d %d %d\n",x1,y1,x2,y2);
    free2D(&a);
  }
  return err;
}

/*Spracovanie argumentov a volanie funkcii*/
int doParams(int argc, char * argv[])
{
  if (argc == 2 && strcmp("--help",argv[1])== 0)
    return PrintHelp();
  else if (argc == 3 && strcmp("--test",argv[1])== 0)
    return doTest(argv[2]);
  else if (argc == 3 && strcmp("--hline",argv[1])== 0)
    return doHline(argv[2]);
  else if (argc == 3 && strcmp("--vline",argv[1])== 0)
    return doVline(argv[2]);
  else if (argc == 3 && strcmp("--square",argv[1])== 0)
    return doSquare(argv[2]);
  else
    return ECLWRONG;
}
/*-------------------------------------Spracovanie argumentov a volanie mapovacich funkcii-----------------------------------*/

/*-------------------------------------Hlavna funkcia-----------------------------------*/
int main(int argc, char * argv[])
{
  /*@err premenna s hodnotou chyboveho stavu*/
  int err;
  err = doParams(argc,argv);
  if(err != EOK)
      {
        fprintf(stderr,"%s",ECODEMSG[err]);
        return 1;
      }

  return 0;
}
