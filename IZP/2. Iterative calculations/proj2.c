/*
 * Subor:   proj2.c
 * Datum:   15.11.2013
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Iteracne vypocty
 * Popis:
*/
#include<stdio.h>     /*praca so vstupom/vystupom*/
#include<stdlib.h>    /*atof*/
#include<string.h>    /*strcmp*/
//#include<math.h>      /*Kontrola*/

#define NAN	(__builtin_nanf (""))   /*definovane Not-A-Number*/
#define PI_2 1.5707963267948966192313  /*Definicia PI/2 koli vypoctu uhlu z kosinovej vety*/
#define eps 0.00000000000001            /*definujem epsilon koli presnosti */
# define INF	(__builtin_inff())

/* Kody chyb*/
enum tecodes
{
  EOK = 0,     /** Bez chyby */
  ECLWRONG,    /** Chybný příkazový řádek. */
  ENotNum,     /** Chybne zadane cisla  */
  ENotTr,      /** Nieje trojuholnik */
};

const char *ECODEMSG[] =
{
  [EOK] = "Vsetko v poriadku.\n",
  [ECLWRONG] = "Chybne parametre prikazoveho riadku!\n",
  [ENotNum] = "zadane parametre nie su platne cisla\n",
  [ENotTr] = "Zadane parametre netvoria trojuholnik.\n",
};

/*-------------------pomocne vypoctove funkcie -------------------------------------------------*/

/*Absolutna hodnota*/
double my_abs(double x)
{
  return (x < 0) ? -x : x;
}

/*Vypocet odmocniny z cisla*/
double my_sqrt(double x)
{
  /*ak je x zaporne vratime NAN*/
  if(x < 0)
    return NAN;
  /*Vratime inf ak je zadane inf*/
  else if(x == INF)
    return INF;
  /*Pre rychlejsi chod programu ak je zadana nula vratime 0*/
  else if(x == 0)
    return 0;
  /*stareX premenna do ktorej vkladame stare x,
  * noveX premenna do kt ukladame nove x
  * dif premenna v ktorej je ulozeny rozdiel medzi starym a novym x
  */
  double stareX = 1,noveX;
  double dif = 1;
  /*Opakuj kym rozdiel neje mensi ako relativna presnost*/
  while(dif>=eps * my_abs(stareX))
  {
    noveX = (x/stareX + stareX)/2;
    dif = my_abs(stareX-noveX);
    stareX = noveX;
  }
  return stareX;
}

/*vypocet dlzky vektoru*/
double lenghtV(double ax, double ay,double bx,double by)
{
  return my_sqrt((bx - ax)*(bx - ax) + (by - ay)*(by - ay));
}

/*Vypocet asinusu*/
double my_asin(double x)
{
  /*Ak je x vacsie ako 1 alebo mensie ako -1 vrtaime NAN*/
  if(x > 1 || x < -1)
  {
    return NAN;
  }
  /*Optimalizacia => ak je x mensie ako -0.9 pouzijeme vzorec*/
  else if(x < -0.9)
  {
    return -PI_2 + my_asin(my_sqrt(1-x*x));
  }
  /*Optimalizacia ak je x vecsie ako 0.9 */
  else if(x > 0.9)
  {
    return PI_2 - my_asin(my_sqrt(1-x*x));
  }
  /*Inak pokracuj v normalnom vypocte asinu*/
  else
  {
      /*sum - premenna do kt ukladame sumu
      * t - premenna do ktorej ukladam vzdy novy vypocitany rozdiel
      * i - i je pouzite pre ciselne hodnoty vzorca
      * moc - je v nej ulozene x^2 koli optimalizacii
      */
      double sum = x;
      double t = x;
      double i = 1.0;
      double moc= x*x;

      /*Opakuj kym prirastok t je mensi ako eps * absolutna hodnota sum - relativna presnost*/
      while(my_abs(t) > (eps * my_abs(sum)))
      {
        /*Vzorec pre vypocet asinus pomocou taylorovej rady*/
        t *= (i*i/(i+1)) * (moc / (i+2));
        sum = sum +t;
        i += 2;
      }
      return sum;
  }
}

/*Vypocet kosinovej vety*/
double VelkostUhla(double a,double b,double c)
{
  /*cosx - vypocitame zo zadanych a,b,c pomocou kosinovej vety
  * uhol - vypocitame pomocou vzorca uhol z cosx pomocou acosinu
  */
  double cosx,uhol;
  cosx = (-(a*a)+b*b+c*c)/(2*b*c);
  uhol = PI_2 - my_asin(cosx);
  return uhol;
}

/*-------------------pomocne vypoctove funkcie ------------------------------------------------------------------*/

/*--------------------hlavne a volacie funkcie -------------------*/
/*Tlac help*/
int PrintHelp(void)
{
  printf("help\n");
  return EOK;
}

/*Urob odmocninu*/
int doSqrt(char *argv[])
{
  /*Ukazatel na posledny znak stringu*/
  char *StrErr;
  /*ziskaj cislo z argumentu pomocou strtod() ak je cislo neplatne vrat ENotNum*/
  double x = strtod(argv[2],&StrErr);
  if( *StrErr != 0)
    return ENotNum;
  /*Vytlac odmocninu*/
  printf("%.10e\n",my_sqrt(x));

  return EOK;
}

/*Urob asin*/
int doAsin(char *argv[])
{
  /*Ukazatel na posledny znak stringu*/
  char *StrErr;
  /*ziskaj cislo z argumentu pomocou strtod() ak je cislo neplatne vrat ENotNum*/
  double x = strtod(argv[2],&StrErr);
  if(*StrErr != 0)
    return ENotNum;

  printf("%.10e\n",my_asin(x));
  //printf("%.10e\n",asin(x));
  return EOK;
}

/*Vypocitaj uhly trojuholnika*/
int doTriangle(char *argv[])
{
  /*Ukazatel na posledny znak stringu*/
  char *StrErr;
  /*ziskaj cislo z argumentu pomocou strtod() ak je cislo neplatne vrat ENotNum*/
  double ax = strtod(argv[2],&StrErr);
  if(*StrErr != 0)
    return ENotNum;
  double ay = strtod(argv[3],&StrErr);
  if(*StrErr != 0)
    return ENotNum;
  double bx = strtod(argv[4],&StrErr);
  if(*StrErr != 0)
    return ENotNum;
  double by = strtod(argv[5],&StrErr);
  if(*StrErr != 0)
    return ENotNum;
  double cx = strtod(argv[6],&StrErr);
  if(*StrErr != 0)
    return ENotNum;
  double cy = strtod(argv[7],&StrErr);
  if(*StrErr != 0)
    return ENotNum;

 /*a,b,c dlzky stran trojuholnika*/
  double a,b,c;
  a = lenghtV(bx,by,cx,cy);
  b = lenghtV(ax,ay,cx,cy);
  c = lenghtV(ax,ay,bx,by);
  /*Ak vsetky body lezia na jednej priamke alebo ak dva body lezia na tych istych suradniciach
  * vypiseme chybovy stav
  */
  if((ax == bx && bx ==cx) || (ay == by && by ==cy) || (ax == bx && ay == by) || (ax == cx && ay == cy) || (cx == bx && cy == by))
    return ENotTr;
  /*Vytlac velkosti uhlov*/
  printf("%.10e\n",VelkostUhla(a,b,c));
  printf("%.10e\n",VelkostUhla(b,a,c));
  printf("%.10e\n",VelkostUhla(c,a,b));

  return EOK;
}

/*Spuctac funkcii podla zadanych argumentov*/
int spustac( int argc, char *argv[])
 {
    if (argc == 2 && strcmp("--help",argv[1])== 0)
      return PrintHelp();
    else if (argc == 3 && strcmp("--sqrt",argv[1])== 0)
      return doSqrt(argv);
    else if (argc == 3 && strcmp("--asin",argv[1])== 0)
      return doAsin(argv);
    else if (argc == 8 && strcmp("--triangle",argv[1])== 0)
      return doTriangle(argv);
    else
      return ECLWRONG;
 }
/*-------------------hlavne a volacie funkcie-------------------*/

/*-------------------Hlavna Funkcia-----------------------------*/
int main( int argc, char *argv[])
{
  /*Premenna pre chybove stavy*/
  int err;

  /*Do err vlozime vratenu chybovu hodnotu z fcie*/
  err = spustac(argc,argv);
  /*Ak vratena hodnota je ina ako EOK vypiseme chybu*/
  if(err != EOK)
    {
      fprintf(stderr,"%s\n",ECODEMSG[err]);
      return 1;
    }
  return 0;
}
