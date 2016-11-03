/* ************************* zasobnik.c ************************* */
/* Kodovani:		UTF-8					                    */
/* Predmet:		Formalni jazyky a prekladace (IFJ)	            */
/* Projekt:		Implementace interpretu imperativniho	        */
/*			    jazyka IFJ14				                    */
/* Varianta zadani:	a/2/I					                    */
/* Autori:		Fáber Jakub		xfaber02	                    */
/*			Hladík Daniel		xhladi21	                    */
/*			Marušič Marek		xmarus05	                    */
/*			Surovčík Tomáš		xsurov04	                    */
/* ************************************************************ */


#include "zasobnik.h"
#include "scanner.h"

/*
 * Inicializace zasobniku
 */
void stackInit (TStack *s) {
    s->top = NULL;
}

/*
 * Vrati vrchol zasobniku
 */
void *stackTop (TStack *s) {

    if (!stackEmpty(s)) {
        return s->top->data;
    } else
        return NULL;
}

int stackTop_t (TStack *s) {
    if (stackEmpty(s)) return -1;

    TSItem *tmpTSItem;
    token_t *tmp_t;
    tmp_t = NULL;
    tmpTSItem = s->top;
    tmp_t = (token_t *) malloc (sizeof (token_t));
    tmp_t = tmpTSItem->data;

    while (tmp_t->sa_type == SA_E || tmp_t->sa_type == SA_POM_LESS) {

        tmp_t = NULL;
        tmp_t = (token_t *) malloc (sizeof (token_t));
        tmpTSItem = tmpTSItem->next;
        tmp_t = tmpTSItem->data;

    }
    return tmp_t->sa_type;
}

/*
 * odstraneni vrcholoveho prvku
 */
int stackPop (TStack *s) {

    if (stackEmpty(s)) return E_STACK_EMPTY;

    TSItem *pom = s->top;
    if (s->top->next) {
        s->top = s->top->next;
    }
    else
        s->top = NULL;

    free(pom);

    return STACK_OK;
}
void popToAll (TStack *from, TStack *to)
{
    token_t *pom_t;
//    printf("\n");
    while (!stackEmpty(from))
    {
        pom_t = stackTopPop(from);
        stackPush(to,pom_t);
        pom_t = NULL;
    }
}


void popToTerm (TStack *from, TStack *to)
{
    token_t *pom_t;
    pom_t = NULL;
    //if (!from->top) return NULL;
    TSItem *tmpTSItem;
    token_t *tmp_t;

    tmpTSItem = from->top;
    tmp_t = tmpTSItem->data;
    while (tmp_t->sa_type == SA_E || tmp_t->sa_type == SA_POM_LESS)
    {
        tmpTSItem = tmpTSItem->next;
        tmp_t = tmpTSItem->data;

        pom_t = stackTopPop(from);
        stackPush(to,pom_t);
        pom_t = NULL;
    }

}

/*
 * Vrati a odstrani vrchol
 */
void *stackTopPop (TStack *s) {

    void *pom = stackTop(s);

    return (!pom || !stackPop(s)) ? pom : NULL;
}

/*
 * vlozi prvek do zasobniku
 */
int stackPush (TStack *s, token_t *data) {

    TSItem *NewItem;

    NewItem = (TSItem *) malloc(sizeof (TSItem));
    if (!NewItem) {
        return E_STACK_ALLOC;
    }

    NewItem->data = data;
    NewItem->next = s->top;
    s->top = NewItem;

    return STACK_OK;
}

/*
 * Smaze zasobnik
 */
void stackDelete (TStack *s) {

    while (!stackEmpty(s)) {
        stackPop(s);
    }
}
