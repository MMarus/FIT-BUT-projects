/* ************************* exprs.c ************************* */
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

#include "exprs.h"



const static int precTable[15][15] =
        { // +	   // -	   // *	// /  // <   // >    // <=   // >=   // =    // <>   // (  // )   // i // MN // $
                {PT_GREATER, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // +
                {PT_GREATER, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // -
                {PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // *
                {PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // /
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // <
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // >
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // <=
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // >=
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // =
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_LESS, PT_GREATER, PT_LESS, PT_LESS, PT_GREATER}, // <>
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_EQUAL, PT_LESS, PT_LESS, PT_EMPTY}, // (
                {PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_EMPTY, PT_GREATER, PT_EMPTY, PT_EMPTY, PT_GREATER}, // )
                {PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_EMPTY, PT_GREATER, PT_EMPTY, PT_EMPTY, PT_GREATER}, // i
                {PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_GREATER, PT_EMPTY, PT_GREATER, PT_EMPTY, PT_EMPTY, PT_GREATER}, // magicnum
                {PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_LESS, PT_EMPTY, PT_LESS, PT_LESS, PT_EMPTY} // $
        };

int stypeToSatype(int type) {
    if (type == K_INTEGER) return INT;
    else if (type == K_REAL) return DOUBLE;
    else if (type == K_STRING) return STR;
    else return -1;
}

int prevodTokenType(int type) {
    if (type == ADD) return SA_ADD;
    else if (type == SUB) return SA_SUB;
    else if (type == MULT) return SA_MUL;
    else if (type == DIV) return SA_DIV;
    else if (type == LESS) return SA_LESS;
    else if (type == GREATER) return SA_MORE;
    else if (type == LESS_EQ) return SA_LESSEQUAL;
    else if (type == GREATER_EQ) return SA_MOREEQUAL;
    else if (type == EQUAL) return SA_EQUAL;
    else if (type == NOT_EQUAL) return SA_NOTEQUAL;
    else if (type == L_BRACKET) return SA_LBRACK;
    else if (type == R_BRACKET) return SA_RBRACK;
    else if (type == ID) return SA_ID;
    else if (type == INT || type == DOUBLE || type == STR || type == K_TRUE || type == K_FALSE) return SA_MAGICNUM;
    else return SA_TERM;
}

static int expr_rule_1[3] = {SA_E, SA_ADD, SA_E};
static int expr_rule_2[3] = {SA_E, SA_SUB, SA_E};
static int expr_rule_3[3] = {SA_E, SA_MUL, SA_E};
static int expr_rule_4[3] = {SA_E, SA_DIV, SA_E};
static int expr_rule_5[3] = {SA_E, SA_LESS, SA_E};
static int expr_rule_6[3] = {SA_E, SA_MORE, SA_E};
static int expr_rule_7[3] = {SA_E, SA_LESSEQUAL, SA_E};
static int expr_rule_8[3] = {SA_E, SA_MOREEQUAL, SA_E};
static int expr_rule_9[3] = {SA_E, SA_EQUAL, SA_E};
static int expr_rule_10[3] = {SA_E, SA_NOTEQUAL, SA_E};
static int expr_rule_11[3] = {SA_RBRACK, SA_E, SA_LBRACK};
static int expr_rule_12[3] = {SA_ID, 0, 0};
static int expr_rule_13[3] = {SA_MAGICNUM, 0, 0};

int isItSameIntArray(int *pole, int *pole_s) {
    for (int i = 0; i < 3; i++) {
        if (pole[i] != pole_s[i]) return 0;
    }
    return 1;
}

int gimmeRule(int *rule) {
    if (isItSameIntArray(rule, expr_rule_1) == 1) return 1;
    else if (isItSameIntArray(rule, expr_rule_2) == 1) return 2;
    else if (isItSameIntArray(rule, expr_rule_3) == 1) return 3;
    else if (isItSameIntArray(rule, expr_rule_4) == 1) return 4;
    else if (isItSameIntArray(rule, expr_rule_5) == 1) return 5;
    else if (isItSameIntArray(rule, expr_rule_6) == 1) return 6;
    else if (isItSameIntArray(rule, expr_rule_7) == 1) return 7;
    else if (isItSameIntArray(rule, expr_rule_8) == 1) return 8;
    else if (isItSameIntArray(rule, expr_rule_9) == 1) return 9;
    else if (isItSameIntArray(rule, expr_rule_10) == 1) return 10;
    else if (isItSameIntArray(rule, expr_rule_11) == 1) return 11;
    else if (isItSameIntArray(rule, expr_rule_12) == 1) return 12;
    else if (isItSameIntArray(rule, expr_rule_13) == 1) return 13;
    else return 0;
}

int isThereSomeRule(TStack *main_s, TStack *pom_s) {
    stackDelete(pom_s);
    int rule_0[3] = {0, 0, 0};
    token_t *pom_t;
    for (int i = 0; i < 3; i++) {
        if (stackEmpty(main_s)) return -1;
        pom_t = (token_t *) malloc(sizeof(token_t));
        pom_t = stackTopPop(main_s);
        rule_0[i] = pom_t->sa_type;
        stackPush(pom_s, pom_t);
        if ((rule_0[0] == SA_ID) || (rule_0[0] == SA_MAGICNUM)) break;

    }
    if (stackEmpty(main_s)) return -1;
    pom_t = stackTopPop(main_s);
    if (pom_t->sa_type != SA_POM_LESS) return -1;
    popToAll(pom_s, main_s);
    return gimmeRule(rule_0);
}

void sa_core() {
    token_t *pom_t, *l_op, *r_op;
    int konec = 0, lbrack = 0, topTerm, rule, stableTyp, stableTyp2;


    TStack *stack_t, *pomStack_t;
    if ((stack_t = (TStack *) malloc(sizeof(TStack))) == NULL) err(INTERNAL_ERR, "Internal error");
    if ((pomStack_t = (TStack *) malloc(sizeof(TStack))) == NULL) err(INTERNAL_ERR, "Internal error");
    stackInit(pomStack_t);
    stackInit(stack_t);

    Symbol_t tmp, tmp2;

    r_op = (token_t *) malloc(sizeof(token_t));
    l_op = (token_t *) malloc(sizeof(token_t));

    pom_t = (token_t *) malloc(sizeof(token_t));
    pom_t->sa_type = SA_TERM;


    if (stackPush(stack_t, pom_t) != 0) err(INTERNAL_ERR, "Internal error");


    token.sa_type = prevodTokenType(token.type);
    topTerm = stackTop_t(stack_t);

    // samotny algoritmus
    do {
        switch (precTable[topTerm][token.sa_type]) {
            case PT_EQUAL:

                pom_t = (token_t *) malloc(sizeof(token_t));
                pom_t->type = token.type;
                pom_t->sa_type = token.sa_type;
                pom_t->str = malloc(sizeof(string_t));
                pom_t->str = make_string(token.str);
                pom_t->lineNumber = token.lineNumber;

                if (stackPush(stack_t, pom_t) != 0) err(INTERNAL_ERR, "Internal error");
                getNextToken(&token);
                token.sa_type = prevodTokenType(token.type);

                break;

            case PT_LESS:
                popToTerm(stack_t, pomStack_t);

                pom_t = (token_t *) malloc(sizeof(token_t));
                pom_t->sa_type = SA_POM_LESS;

                stackPush(stack_t, pom_t);

                popToAll(pomStack_t, stack_t);

                pom_t = (token_t *) malloc(sizeof(token_t));
                pom_t->type = token.type;
                pom_t->sa_type = token.sa_type;
                pom_t->str = malloc(sizeof(string_t));
                pom_t->str = make_string(token.str);
                pom_t->lineNumber = token.lineNumber;

                stackPush(stack_t, pom_t);

                getNextToken(&token);
                token.sa_type = prevodTokenType(token.type);
                break;

            case PT_GREATER:
                rule = isThereSomeRule(stack_t, pomStack_t);
                switch (rule) {
                    case 1:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);

                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;



                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;
                        // string + string
                        if(stableTyp == STR && stableTyp2 == STR) {
                            pom_t->type = STR;
                            add_instruction(I_CONCATENATE, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == INT && stableTyp2 == INT ) {
                            pom_t->type = INT;
                            add_instruction(I_PLUS, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            add_instruction(I_PLUS, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == INT) {
                            pom_t->type = DOUBLE;
                           // add_instruction(I_INT_TO_DOUBLE, l_op, NULL, l_op);
                            add_instruction(I_PLUS, l_op, r_op, pom_t);

                        }
                        else if (stableTyp == INT && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, r_op, NULL, r_op);
                            add_instruction(I_PLUS, l_op, r_op, pom_t);
                        }
                        else err(SEMANTIC_TYPE, "Type conflict");

                        stackPush(stack_t, pom_t);
                        result = pom_t;

                        break;



                    case 2:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);
                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;

                        if (stableTyp == INT && stableTyp2 == INT ) {
                            pom_t->type = INT;
                            add_instruction(I_MINUS, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            add_instruction(I_MINUS, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == INT) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, l_op, NULL, l_op);
                            add_instruction(I_MINUS, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == INT && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, r_op, NULL, r_op);
                            add_instruction(I_MINUS, l_op, NULL, r_op);
                        }
                        else err(SEMANTIC_TYPE, "Type conflict");

                        stackPush(stack_t, pom_t);
                        result = pom_t;

                        break;

                    case 3:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);
                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;
                        if (stableTyp == INT && stableTyp2 == INT ) {
                            pom_t->type = INT;
                            add_instruction(I_MULTIPLY, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            add_instruction(I_MULTIPLY, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == INT) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, l_op, NULL, l_op);
                            add_instruction(I_MULTIPLY, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == INT && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, r_op, NULL, r_op);
                            add_instruction(I_MULTIPLY, l_op, r_op, pom_t);
                        }
                        else err(SEMANTIC_TYPE, "Type conflict");

                        stackPush(stack_t, pom_t);
                        result = pom_t;
                        break;

                    case 4:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);
                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;
                        if (stableTyp == INT && stableTyp2 == INT ) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, l_op, NULL, l_op);
                            //add_instruction(I_INT_TO_DOUBLE, r_op, NULL, r_op);
                            add_instruction(I_DIVIDE, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            add_instruction(I_DIVIDE, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == DOUBLE && stableTyp2 == INT) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, l_op, NULL, l_op);
                            add_instruction(I_DIVIDE, l_op, r_op, pom_t);
                        }
                        else if (stableTyp == INT && stableTyp2 == DOUBLE) {
                            pom_t->type = DOUBLE;
                            //add_instruction(I_INT_TO_DOUBLE, r_op, NULL, r_op);
                            add_instruction(I_DIVIDE, l_op, r_op, pom_t);
                        }
                        else err(SEMANTIC_TYPE, "Type conflict");

                        result = pom_t;

                        stackPush(stack_t, pom_t);
                        break;

                    case 5:

                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);


                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;


                        if (stableTyp == stableTyp2) add_instruction(I_LT, l_op, r_op, pom_t);
                        else err(SEMANTIC_TYPE, "Type conflict");



                        result = pom_t;
                        stackPush(stack_t, pom_t);
                        break;

                    case 6:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);

                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;


                        if (stableTyp == stableTyp2) add_instruction(I_GT, l_op, r_op, pom_t);
                        else err(SEMANTIC_TYPE, "Type conflict");
                        result = pom_t;


                        stackPush(stack_t, pom_t);
                        break;

                    case 7:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);

                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;


                        if (stableTyp == stableTyp2) add_instruction(I_LTE, l_op, r_op, pom_t);
                        else err(SEMANTIC_TYPE, "Type conflict");


                        stackPush(stack_t, pom_t);
                        result = pom_t;
                        break;

                    case 8:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);

                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;


                        if (stableTyp == stableTyp2) add_instruction(I_GTE, l_op, r_op, pom_t);
                        else err(SEMANTIC_TYPE, "Type conflict");

                        result = pom_t;

                        stackPush(stack_t, pom_t);
                        break;

                    case 9:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);

                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;

                        if (stableTyp == stableTyp2) add_instruction(I_EQ, l_op, r_op, pom_t);
                        else err(SEMANTIC_TYPE, "Type conflict");

                        stackPush(stack_t, pom_t);
                        result = pom_t;

                        break;

                    case 10:
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        l_op = stackTopPop(stack_t);

                        pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;

                        if (r_op->sa_type == SA_E && r_op->type == ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp = stypeToSatype(tmp->type);
                        } else stableTyp = r_op->type;

                        if (l_op->sa_type == SA_E && l_op->type == ID) {
                            if ((tmp2 = find_symbol(l_op->str)) == NULL)
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            stableTyp2 = stypeToSatype(tmp2->type);
                        } else stableTyp2 = l_op->type;


                        if (stableTyp == stableTyp2) add_instruction(I_NEQ, l_op, r_op, pom_t);
                        else err(SEMANTIC_TYPE, "Type conflict");

                        stackPush(stack_t, pom_t);
                        result = pom_t;

                        break;

                        // zavorky
                    case 11:
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        r_op = stackTopPop(stack_t);
                        if (stackPop(stack_t) != 0) err(INTERNAL_ERR, "Internal error");
                        /*pom_t = (token_t *) malloc(sizeof(token_t));
                        pom_t->sa_type = SA_E;
                        pom_t->lineNumber = r_op->lineNumber;*/

                        stackPush(stack_t, r_op);
                        break;

                    case 12:
                    case 13:

                        r_op = stackTopPop(stack_t);
                        //l_op = NULL;
                        pom_t = (token_t *) malloc(sizeof(token_t));

                        if (r_op->sa_type == SA_ID) {
                            if ((tmp = find_symbol(r_op->str)) == NULL) {
                                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");
                            }
                            if(tmp->type == INT) pom_t->tdata.i = tmp->data->i;
                            else if(tmp->type == DOUBLE) pom_t->tdata.d = tmp->data->d;
                            else if(tmp->type == STR) pom_t->tdata.string = make_string(tmp->data->string);
                            else if(tmp->type == K_TRUE) pom_t->tdata.b = true;
                            else if(tmp->type == K_FALSE) pom_t->tdata.b = false;
                        }
                        else {
                            if(r_op->type == INT) pom_t->tdata.i = atoi(r_op->str->name);
                            else if(r_op->type == DOUBLE) pom_t->tdata.d = strtod(r_op->str->name, NULL);
                            else if(r_op->type == K_TRUE) pom_t->tdata.b = true;
                            else if(r_op->type == K_FALSE) pom_t->tdata.b = false;
                            else if(r_op->type == STR) pom_t->tdata.string = make_string(r_op->str);
                        }
                        pom_t->type = r_op->type;
                        pom_t->sa_type = SA_E;
                        pom_t->str = malloc(sizeof(string_t));
                        pom_t->str = make_string(r_op->str);
                        pom_t->lineNumber = r_op->lineNumber;

                        stackPush(stack_t, pom_t);

                        result = pom_t;

                        break;

                    default:
                        err(SYNTAX_ERR, "Invalid expression");

                }


                break;

            case PT_EMPTY:
                if (token.sa_type == SA_RBRACK) lbrack = 1;
                else err(SYNTAX_ERR, "Invalid expression");
                break;


        }  // end switch precTable

        topTerm = stackTop_t(stack_t);

        if (topTerm == SA_TERM) {
            if (token.sa_type == SA_TERM) konec = 1;
            else if (lbrack == 1) konec = 1;
        }
        else konec = 0;


    }
    while (konec == 0);
    //token.type = originToken;

}




























