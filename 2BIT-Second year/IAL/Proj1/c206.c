// Autor riesenia: Marek Marusic,xmarus05
// email:          xmarus05@stud.fit.vutbr.cz
// popis:          riesenie projektu c1 na predmet IAL
// subor:          c206.c

/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2014
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu,
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem,
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu,
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/

  if(L == NULL){
    DLError();
    return;
  }

  L->First  = NULL;
  L->Act    = NULL;
  L->Last   = NULL;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free.
**/

  if(L == NULL){
    DLError();
    return;
  }

  tDLElemPtr Elem = L->First;

  while(Elem != NULL){
    L->First = Elem->rptr;

    free(Elem);

    Elem = L->First;
  }
  L->First = NULL;
  L->Last = NULL;
  L->Act = NULL;

 //solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

  if(L == NULL){
    DLError();
    return;
  }

  tDLElemPtr pElem;
  tDLElemPtr First = L->First;

  pElem = (tDLElemPtr)malloc(sizeof(struct tDLElem));
  if(pElem == NULL)
    DLError();

  pElem->data = val;
  pElem->lptr = NULL;
  pElem->rptr = First; //nasledujuci

  if( First != NULL)
    First->lptr = pElem;
  else
    L->Last = pElem;

  L->First = pElem;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

  if(L == NULL){
    DLError();
    return;
  }

  tDLElemPtr pElem;

  pElem = (tDLElemPtr)malloc(sizeof(struct tDLElem));
  if(pElem == NULL)
    DLError();

  pElem->data = val;

  pElem->lptr = L->Last;
  pElem->rptr = NULL; //nasledujuci

  if( L->Last != NULL)
    L->Last->rptr = pElem;
  else
    L->First = pElem;

  L->Last = pElem;


 //solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/

  L->Act = L->First;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/

  L->Act = L->Last;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

  if(L == NULL || L->First == NULL || L->Last == NULL){
    DLError();
    return;
  }

  *val = L->First->data;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

  if(L == NULL || L->First == NULL || L->Last == NULL){
    DLError();
    return;
  }

  *val = L->Last->data;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/

  if(L == NULL){
    DLError();
    return;
  }

  tDLElemPtr pElem;

  if(L->First != NULL){
    pElem = L->First;

    if(L->First == L->Act)
      L->Act = NULL;

    if(L->First == L->Last){
      L->First = NULL;
      L->Last = NULL;
    }
    else{
      L->First = L->First->rptr;
      L->First->lptr = NULL;
    }
    free(pElem);
  }

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLDeleteLast (tDLList *L) {
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/

  if(L == NULL){
    DLError();
    return;
  }

  tDLElemPtr pElem;

  if(L->Last != NULL){
    pElem = L->Last;

    if(L->Last == L->Act)
      L->Act = NULL;

    if(L->First == L->Last){
      L->First = NULL;
      L->Last = NULL;
    }
    else{
      L->Last = L->Last->lptr;
      L->Last->rptr = NULL;
    }
    free(pElem);
  }

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLPostDelete (tDLList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/

  if(L == NULL){
    DLError();
    return;
  }

  tDLElemPtr pElem;

  if(L->Act != NULL && L->Act->rptr != NULL){
    pElem = L->Act->rptr;

    L->Act->rptr = pElem->rptr;
    if(pElem == L->Last)
      L->Last = L->Act;
    else
      pElem->rptr->lptr = L->Act;
    free(pElem);
  }

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/

  if(L == NULL){
    DLError();
    return;
  }

  tDLElemPtr pElem;

  if(L->Act != NULL && L->Act->lptr != NULL){
    pElem = L->Act->lptr;

    L->Act->lptr = pElem->lptr;
    if(pElem == L->First)
      L->First = L->Act;
    else
      pElem->lptr->rptr = L->Act;
    free(pElem);
  }


// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

  if(L == NULL){
    DLError();
    return;
  }

  if( L->Act == NULL )
    return;

  tDLElemPtr pElem;

  pElem = (tDLElemPtr)malloc(sizeof(struct tDLElem));
  if(pElem == NULL)
    DLError();

  pElem->data = val;


  pElem->lptr = L->Act;
  pElem->rptr = L->Act->rptr; //nasledujuci

  L->Act->rptr = pElem;
  if(L->Act == L->Last)
    L->Last = pElem;
  else
    pElem->rptr->lptr = pElem;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

  if(L == NULL){
    DLError();
    return;
  }

  if( L->Act == NULL )
    return;

  tDLElemPtr pElem;

  pElem = (tDLElemPtr)malloc(sizeof(struct tDLElem));
  if(pElem == NULL)
    DLError();

  pElem->data = val;


  pElem->lptr = L->Act->lptr;
  pElem->rptr = L->Act; //nasledujuci

  L->Act->lptr = pElem;
  if(L->Act == L->First)
    L->First = pElem;
  else
    pElem->lptr->rptr = pElem;

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

  if(L == NULL || L->Act == NULL){
    DLError();
    return;
  }

  *val = L->Act->data;
// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/

  if(L == NULL){
    DLError();
    return;
  }

  if(L->Act != NULL){
    L->Act->data = val;
  }

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/

  if(L == NULL){
    DLError();
    return;
  }

  if(L->Act != NULL){
    L->Act = L->Act->rptr;
  }


// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/

  if(L == NULL){
    DLError();
    return;
  }

  if(L->Act != NULL){
    L->Act = L->Act->lptr;
  }

// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivní, vrací true. V opaèném pøípadì vrací false.
** Funkci implementujte jako jediný pøíkaz.
**/

  return (L->Act != NULL) ? TRUE : FALSE;
// solved = FALSE;                   /* V pøípadì øe¹ení, sma¾te tento øádek! */
}

/* Konec c206.c*/
