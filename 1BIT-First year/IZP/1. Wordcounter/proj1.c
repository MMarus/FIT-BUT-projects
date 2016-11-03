/*
 * Subor:   proj1.c
 * Datum:   07.11.2013 12:45
 * Autor:   Marek Marusic, xmarus05@stud.fit.vutbr.cz
 * Projekt: Pocitanie slov, projekt c. 1 pre predmet IZP
 * Popis:   Program pocita slova ktore obsahuju zadany znak, resp. zadany znak na zadanom mieste v slove.
 */

// praca so vstupom/vystupom
#include<stdio.h>

// obecne funkcie jazyka C
#include <stdlib.h>

// kvoli funkcii strcmp
#include<string.h>

// typ bool, konstanty true, false
#include <stdbool.h>

//koli obmedzeniu v StrToInt
#include<limits.h>

/*
*Funkcia na vypisanie help. Vytlaci sa pri chybe alebo po zavolani argumentu --help
*/
void PrintHelp(){
  printf("Program Pocitani slov\n"
         "Autor: Marek Marusic (c) 2013\n"
         "Tento program sluzi na vypis poctu slov\n"
         "obsahujucich uzivatelom specifikovany znak.\n"
         "Program sa spusta pomocou \"./proj1 X [N] [-d],\n"
         "kde X reprezentuje hladany znak.\n"
         "Znak \":\" symbolizuje lubovolne cislo 0-9,\n"
         "znak \"^\" symbolizuje lobuvolne velke pismeno A-Z \n"
         "znak \".\" symbolizuje lubovolny znak.\n"
	);
}

/*Zistujeme ci zadana premenna je sucast slova alebo oddelovac.*/
int isWord(char c)
{
  return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || ( c == '-' ) || ( c == '_')) ? 1 : 0;
}

/*Zistujeme ci zadany string sa nachadza v argumentoch.*/
int Search(int argc, char *argv[],char *h)
{
  int i;
  for(i=0;i<argc;i++)
    if(strcmp(h,argv[i])== 0)
      return 1;
  return 0;
}

unsigned int StrToInt(char *pstr)
{
  /*deklaracia premennych
    i - pomocna premenna pre cyklus for
    num - cislo int prevedene zo stringu
  */
  short int i =0;
  unsigned int num = 0;
  /*po jednom rozkladame string na znaky a zistujeme ci je tento znak cislo 0-9*/
  while(pstr[i] != 0)
  {
    /*ak je znak cislo pripocitameho k num a num vynasobime 10*/
    if(pstr[i] >= '0' && pstr[i] <= '9')
    {
        if(num < UINT_MAX/10)
        {
            num = pstr[i]-'0' +num*10;
        }
        else
        {
            return num;
        }
    }
    /*ak zadany znak nieje 0-9 vypiseme chybu*/
    else
    {
      return 0;
    }
    i++;
  }
  /*musime vydelit cislo 10timi pretoze jednotky v cykle sa nasobia tiez 10mi tym sme ziskali 10krat vecsie cislo,
    ale potrebujeme vratit povodne cislo.
  */
  return num;
}
/*Vyhladavame v argumentoch string ktory obsahuje iba cislice - hladame argument [N]*/
unsigned int findN(int argc, char *argv[])
{
  short int i;
  unsigned int n;
  /*Pre kazdy argument zistime ci je cislo, a ak je cislo zistíme aku ma hodnotu*/
  for(i=2;i<argc;i++)
  {
    n = StrToInt(argv[i]);
    if(n>0)
      return n;
  }
  return 0;
}
/*Hladame porovnavany znak(cmp)*/
short int findCmp(char *c,short int *cmp)
{
/*ak argument 1 je dlhsi ako jeden znak vratime 0*/
  if(c[1] !=0 )
    return 0;
  /*zistujem ci zadany argument nieje jeden zo specialnych znakov, ak nie zistujeme ci je zadany argument povoleny znak slova*/
  switch(c[0])
  {
    /*hladaj cisla 0-9*/
	case ':':
      cmp[0] = '0';
      cmp[1] = '9';
      break;
    /*hladaj A-Z*/
	case '^':
      cmp[0] = 'A';
      cmp[1] = 'Z';
      break;
    /*hladaj lubovolny znak*/
	case '.':
      cmp[0] = 0;
      cmp[1] = 255;
      break;
    /*hladaj zadany znak*/
	default:
      if(isWord(c[0]))
      {
        cmp[0] = c[0];
        cmp[1] = c[0];
        break;
      }
      else
        return 0;
  }
  return 1;
}
/*Limit pri debug mode*/
int limit(bool deb,unsigned int length)
{
  return ((deb == 1 && length <= 80) || deb == 0) ? 1 : 0;
}

/*Porovnanie vstupneho textu so zadanymi argumentami*/
int doCompare(int argc, char *argv[])
{
  /*foundC - premenna do ktorej ukladame true ak je najdena zhoda a false ak nie*/
  bool foundC,deb = false;
  /*c- nacctany znak zo vstupu
  * n - pozicia hladaneho znaku v slove
  * cmp hladany znak (cmp[0] od cmp[1] do)
  * deb - zapnuty alebo vypnutý debug mod
  * wCount - pocet najdenych slov spĺnajucich vstupne poziadavky
  * length - dlzka slova, zvysuje sa po kazdom najdenom znaku v slove, nuluje sa po najdenej medzere
	  */
  unsigned int n,length = 1;
  short int c,cmp[2],wCount = 0;
/*do n vlozime int ak je v argumentoch zadane cislo [N]*/
  n = findN(argc,argv);
  /*do deb vlozime 1 ak je najdeny argument -d*/
  deb = Search(argc,argv,"-d");
/*Ak nieje zadany hladany znak v prvom argumente vypiseme chybu*/
  if(!argv[1] || !findCmp(argv[1],cmp))
  {
    fprintf(stderr,"Nepovoleny vyhladavany znak\n");
    return 0;
  }
/*Po jednom nacitavame znaky zo vstupu*/
  while(( c = getchar() ) != EOF)
  {
    /*Ak je znak znak slova a slovo nieje dlhsie ako 80 znakov zistime ci je
	* tento znak rovnaky ako hladany znak
	*/
	if(isWord(c) && limit(deb,length))
    {
      if(c >= cmp[0] && c <= cmp[1] && (length == n || n==0))
      {
        foundC = true;
      }
      /*ak je zapnuty debu vypis znak*/
	    if(deb)
      {
        printf("%c",c);
      }
      length++;
    }
    else
    {
      /*Ak sme nasli hladany znak zvys wCount o jedna a vynuluj foundC*/
      if(foundC == true)
      {
        wCount++;
        foundC = false;
      }
        /*dlzku slova nastavime znova na 1*/
      if(deb)
      {
        if(!isWord(c) && length >=2)
        {
            printf("\n");
            length = 1;
        }
      }
      else{
        length = 1;
      }
    }

  }
  if(foundC == true)
  {
  wCount++;
  //printf("++");
  foundC = false;
  }
  if(deb && length > 1)
  {
    printf("\n");
  }
  length = 1;
  return wCount;
}

int main( int argc, char *argv[] )
{
  // countW premenna s hodnotou najdenych slov splnajucich podmienky
  int countW;

  //Vytlac help ak je zadany argument --help
  if(Search(argc,argv,"--help"))
  {
    PrintHelp();
    return 0;
  }

  //Najdi pocet vhodnych slov
  countW = doCompare(argc,argv);

  //vypis pocet najdenych slov
  printf("%d\n",countW);


  return 0;
}
