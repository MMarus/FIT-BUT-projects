/* ************************* scanner.c ************************* */
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

#include "scanner.h"
#include "parser.h"
#include "i_string.h"

unsigned current_size = 0;
token_t token;

void init_token()
{
    token.type = 0;
    token.str = malloc(sizeof(string_t));
    token.str->name = MemAlloc(token.str->name);
    token.lineNumber = 1;
    token.col = 1;
}

void free_token()
{
    free_string(token.str);
}

void prepare_token(token_t *t)
{
    t->str = malloc(sizeof(string_t));
}

int getNextToken(token_t *token)
{
    int r = getToken(token, stream);

#ifdef DEBUG
    printf("getNextToken() | Line: %3d | Type: %-13s(%2d) | Name: %s(%d)\n", token->lineNumber, TEXTMSGS[token->type], token->type, token->str->name, token->str->length);
#endif

    return r;
}

char *MemAlloc(char *str)
{
    if (current_size == 0) {
        str = malloc(DEFAULT_MEM_ALLOC * sizeof(char));

        if (str == NULL) {
            fprintf(stderr, "i_string.c - failed - malloc\n");
            return str;
        }
        current_size = current_size + DEFAULT_MEM_ALLOC;
    }
    else {
        if (str == NULL) {
            fprintf(stderr, "i_string.c - failed - realloc, must alloc first \n");
            return str;
        }

        current_size = current_size + DEFAULT_MEM_ALLOC;
        str = (char *) realloc(str, current_size);
        if (str == NULL) {
            fprintf(stderr, "i_string.c - failed - realloc\n");
        }
    }

    return str;
}

void strToLower(char *str)
{
    int c;
    int lowerDifference = 'a' - 'A';
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        c = str[i];

        if (c > 64 && c < 91)//je velke pismeno
            c = c + lowerDifference;
        str[i] = c;
    }
}

int IsKeyWord(char *str)
{
    strToLower(str);
    int i;
    for (i = K_BEGIN; i <= K_NOT; i++) {
        if (strcmp(str, TEXTMSGS[i]) == 0)
            return i;
    }

    return ID;
}

int convertBase(char * str, int base){
  long int num;
  char *endptr;
  int i = 0;


  num = strtol(str,&endptr,base);
//  printf("num %d\n",num);

  str[i] = '\0';
  if( *endptr)
    return 1;
    //TODO zavolat LEX ERROR lebo
    //printf("%c\n",*endptr);

  if( num < 0 )
    str[i++] = '-';

  while( num > 0){
    str[i++] =  (num % 10) + '0';
    str[i] = '\0';
    num = num / 10;
  }

//  printf("str = %s\n", str);
  reverse_string(str);
  return 0;
}


char *StrAppend(char *str, int c, unsigned i)
{
    //current_size of allocated str - [0] - '\0' = -2
    if (i >= current_size - 2) {
        str = MemAlloc(str);
        if (str == NULL) {
            return str;
        }
    }

    str[i] = c;
    i++;
    str[i] = '\0';

    return str;
}


int isOperator(token_t *token_p, int c, unsigned i)
{
    int status = ERR_LEX;
    switch (c) {
        case ',':     //pretoze 1,1 -> lex_err
 //       case '%':
        case '/':
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case ';':
        case '=':
        case ':':
        case '>':
        case '<':
        case '.':
            status = ERR_OK;
    }
    return status;
}


int getToken(token_t *token_p, FILE *fr)
{

    int c;
    int base = 10;  //Default base for INT number is 10
    token_p->str->length = 0;
    token_p->str->name[0] = '\0';
    int status = 0;
    static int Program_Ended = 0;

    while ((c = fgetc(fr)) != EOF) {
        //Pridaj cislo riadku
        if (c == '\n') {
            token_p->lineNumber++;
            //vynuluj stlpec
            token_p->col = 0;
        }
        //Pridaj cislo stlpca
        token_p->col++;

        switch (status) {
            //ROZDELIT VSETKY OPERATORY A ZNAKY FUNKCIONALNE ATD
            case START:
//LEX PRE VIACERO ZNAKOV NARAZ
                if (isspace(c)) {
                    continue;
                }
                else if (isdigit(c)) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    token_p->type = INT;
                    status = INT;
                    break;
                }
                else if (isalpha(c) || c == '_') {
                    token_p->type = ID;
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = ID;
                    break;
                }

                switch (c) {
                    case '{'://Pokracovat ako string
                        status = COMMENT;
                        //token_p->str->name = StrAppend(token_p->str->name,c,token_p->str->length);
                        break;

                    case '\''://Pokracovat ako string
                        status = STR;
                        token_p->type = STR;
                        //token_p->str->name = StrAppend(token_p->str->name,c,token_p->str->length);
                        break;

//LEX ERRORS PRAVDEPODOBNE
//MOZEM ICH DAT DO JEDNEHO IFU
                    case '!':
                    case '^':
                    case '?':
                    case '"':
                    case '#':
                    case '`':
                    case '@':
                    case '}':
                    case '~':
                    case '|':
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_OPERATOR_AT_START]);
                        return token_p->str->length;
                        break;
//LEX ERRORS PRAVDEPODOBNE

//LEX OPERATORY SAMOSTATNE

                    case '.':
                        token_p->type = DOT;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;

                    //BASE CONVERSION
                    case '%':
                    case '&':
                    case '$':
                        //token_p->type = MOD;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = BASE_START;//START;
//                        return token_p->str->length;
                        break;

                    case '=':
                        token_p->type = EQUAL;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;


                    case ',':
                        token_p->type = COMMA;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;
                    case '/':
                        token_p->type = DIV;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;

                    case '(':
                        token_p->type = L_BRACKET;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;

                    case ')':
                        token_p->type = R_BRACKET;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;

                    case '+':
                        token_p->type = ADD;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;

                    case '-':
                        token_p->type = SUB;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;

                    case '*':
                        token_p->type = MULT;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;

                    case ';':
                        token_p->type = SEMICOLON;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;


//LEX OPERATORY SAMOSTATNE

//LEX OPERATORY SAMOSTATNE AJ SPOJENE
                    case ':':
                        token_p->type = COLON;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = COLON;
                        break;

                    case '>':
                        token_p->type = GREATER;
                        status = GREATER;
                        break;

                    case '<':

                        token_p->type = LESS;
                        status = LESS;
                        break;
//LEX OPERATORY SAMOSTATNE AJ SPOJENE

//LEX OPERATORY SPOJENE
/*                    case '|':
                        token_p->type = OR;
                        status = OR;
                        break;
*/

                    case '[':
                        token_p->type = L_QUAD_BRACKET;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;
                    case ']':
                        token_p->type = R_QUAD_BRACKET;
                        token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                        token_p->str->length++;
                        status = START;
                        return token_p->str->length;
                        break;
//LEX OPERATORY SPOJENE
                }
                break;

            case BASE_START:
                if(isdigit(c)){
                  switch (token_p->str->name[0]) {
                    case '%':
                        base = 2;
                      break;
                    case '&':
                        base = 8;
                      break;
                    case '$':
                        base = 16;
                      break;
                    default:
                        printf("errorr scanner.c 370");
                      break;
                  }
                  token_p->type = INT;
                  token_p->str->length--;
                  token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                  token_p->str->length++;
                  status = BASE;
                }
                else{
                    // TODO mozno syntax err
                    err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_INT_NOT_DIGIT]);
                    ungetc(c,fr);
                    token_p->col--;
                    return token_p->str->length;
                }
              break;

            case BASE:
                if (isspace(c)) {
                    if (  (convertBase(token_p->str->name,base) ) == 1 )
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_INT_NOT_DIGIT]);
                    status = START;
                    return token_p->str->length;
                }
                else if (c == '{') {
                    if (  (convertBase(token_p->str->name,base) ) == 1 )
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_INT_NOT_DIGIT]);
                    ungetc(c, fr);
token_p->col--;
                    return token_p->str->length;
                }
                else if(isdigit(c)){
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                }
                else {
                    if ( (status = isOperator(token_p, c, token_p->str->length)) != ERR_LEX) {
                        ungetc(c, fr);
token_p->col--;
                        if (  (convertBase(token_p->str->name,base) ) == 1 )
                            err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_INT_NOT_DIGIT]);
                        return token_p->str->length;
                    }
                    else {
                        ungetc(c, fr);
token_p->col--;
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_INT_NOT_DIGIT]);
                        return token_p->str->length;
                    }
                }
                break;

/*
            case AND:
                if (c == '&') {
                    token_p->type = AND;
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = START;
                    return token_p->str->length;
                }
                else
                    err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_NOT_AND], token_p->lineNumber);
                return token_p->str->length;
                break;
*/



                //POKRACOVANIE KOMENTU
            case COMMENT:
                if (c == '}')
                    status = START;
                continue;
                break;
                //END OF POKRACOVANIE KOMENTU

                //POKRACOVANIE STRINGU
            case STR:
                if (c == '\n') {
                    err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_STRING_EXCEED_LINE]);
                    return token_p->str->length;
                }
                else if (c == '\'') {
                    //konec stringu
                    //token_p->str->name = StrAppend(token_p->str->name,c,token_p->str->length);
                    //token_p->str->length++;
//                    printf("hellooo\n");
                    token_p->type = STR;
                    status = END_STR;
                }

                else if (c > 31) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                }
                else {
                    err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_STR_NOT_ESCAPED]);
                    return token_p->str->length;
                }
                break;

                //END OF POkRACOVANIE STRINGU

                //POKRACUJ V INT
            case INT:
                //RETURN INT
                if (isspace(c)) {
                    status = START;
                    return token_p->str->length;
                }
                else if (c == '{') {
                    status = COMMENT;
                    return token_p->str->length;
                }
                    //POKRACUJ v INT
                else if (isdigit(c)) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                }
                    //POKRACUJ v DOUBLE
                else if (c == '.') {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    token_p->type = DOUBLE;
                    status = DOUBLE;
                }
                    //POKRACUJ v EXP
                else if (c == 'e' || c == 'E') {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    token_p->type = DOUBLE;
                    status = EXP;
                }
                    //VRAT INT A unget(c)
                else {
                    if ( (status = isOperator(token_p, c, token_p->str->length)) != ERR_LEX) {
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        return token_p->str->length;
                    }
                    else {
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_INT_NOT_DIGIT]);
                        return token_p->str->length;
                    }
                }
                //VRAT ERR_LEX A unget(c)

                break;
                //KONEC INT

                //ZACIATOOK ID
            case ID:
                //RETURN ID
                if (isspace(c)) {
                    status = START;
                    //printf("%c",c);
                    token_p->type = IsKeyWord(token_p->str->name);
                    if (token_p->type == K_END) {
                        status = K_END;
                        break;
                    }
                    return token_p->str->length;
                }
                else if (c == '{') {
                    status = COMMENT;
                    return token_p->str->length;//MOZE BYT KOMENT V STREDE PREMENNEJ ? ALEBO AZ ZA NOU
                }
                    //POKRACUJ V ID
                else if (isalnum(c) || c == '_') {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                }
                    //VRAT INT A unget(c)
                else {
                    if ((status = isOperator(token_p, c, token_p->str->length)) != ERR_LEX) {
                        token_p->type = IsKeyWord(token_p->str->name);
                        if (token_p->type == K_END && c == '.')
                            Program_Ended = 1;
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        return token_p->str->length;
                    }
                    else {
                        status = START;
                        token_p->type = IsKeyWord(token_p->str->name);
                        ungetc(c, fr);

                        token_p->col--;
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_ID_NOT_ALLOWED_CHARS]);
                        return token_p->str->length;
                    }
                }
                break;
                //KONEC ID

                //ZACIATOOK ESCAPE
            case END_STR:
                if (c == '#') {
                    //escape charakter
                    //NACITAJ INT CHARU A PTM HO HOD NASPET DO SUBORU
                    //NEVIEM CI BUDE FUNGOVAT!!!!!!!!

                    if (fscanf(fr, "%d", &c) != 1) {
                        err(LEXICAL_ERR, TEXTMSGS[ERR_ILLEGAL_CHAR_CONST]);
                    }
                    ungetc(c, fr);

                    token_p->col--;
                    status = ESCAPE;
                }
                else if (c == '\'') {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = STR;
                }
                else {
                    status = START;
                    ungetc(c, fr);

                    token_p->col--;
                    return token_p->str->length;
                }
                break;

            case ESCAPE:
                if (c <= 255 && c > 0 ) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = ESCAPE_END;
                }
                else {
                    status = START;
                    err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_ESCAPE_NOT_NUM]);
                    return token_p->str->length;
                }
                break;

            case ESCAPE_END:
                if (c == '\'') {
                    status = STR;
                }
                else {
                    status = START;
                    err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_ESCAPE_NOT_ENDED]);
                    return token_p->str->length;
                }
                break;

            case DOUBLE:
                if (isspace(c)) {
//                  printf("%c\n", token_p->str->name[token_p->str->length-1]);
                    if (token_p->str->name[token_p->str->length - 1] == '.')
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS]);
                    status = START;
                    return token_p->str->length;
                }
                else if (c == '{') {
                    status = COMMENT;
                    return token_p->str->length;//MOZE BYT KOMENT V STREDE PREMENNEJ ? ALEBO AZ ZA NOU
                }
                if (isdigit(c)) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                }

                else if (c == 'e' || c == 'E') {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    //token_p->type = EXP;
                    status = EXP;
                }
                else {
                    if ((status = isOperator(token_p, c, token_p->str->length)) != ERR_LEX) {
                        if (token_p->str->name[token_p->str->length - 1] == '.')
                            err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS]);
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        return token_p->str->length;
                    }
                    else {
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS]);
                        return token_p->str->length;
                    }
                }
                break;
            case EXP:
                if (isspace(c)) {
                    if (token_p->str->name[token_p->str->length - 1] == 'E' || token_p->str->name[token_p->str->length - 1] == 'e')
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS]);
                    status = START;
                    return token_p->str->length;
                }
                else if (c == '{') {
                    status = COMMENT;
                    return token_p->str->length;//MOZE BYT KOMENT V STREDE PREMENNEJ ? ALEBO AZ ZA NOU
                }
                else if (isdigit(c)) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                }
                else if ( (c == '+' || c == '-') && (token_p->str->name[token_p->str->length - 1] == 'E' || token_p->str->name[token_p->str->length - 1] == 'e') ) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = SIGN_EXP;
                }
                else {
                    if ((status = isOperator(token_p, c, token_p->str->length)) != ERR_LEX) {
                        if (token_p->str->name[token_p->str->length - 1] == 'E' || token_p->str->name[token_p->str->length - 1] == 'e')
                            err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS]);
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        return token_p->str->length;
                    }
                    else {
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS]);
                        return token_p->str->length;
                    }
                }

                break;

            case SIGN_EXP:
                if (isspace(c)) {
                    status = START;
                    return token_p->str->length;
                }
                else if (c == '{') {
                    status = COMMENT;
                    return token_p->str->length;//MOZE BYT KOMENT V STREDE PREMENNEJ ? ALEBO AZ ZA NOU
                }
                else if (isdigit(c)) {
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                }
                else {
                    if ((status = isOperator(token_p, c, token_p->str->length)) != ERR_LEX) {
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        return token_p->str->length;
                    }
                    else {
                        ungetc(c, fr);

                        token_p->col--;
                        status = START;
                        err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS]);
                        return token_p->str->length;
                    }
                }
                break;

            case COLON:
                if (c == '=') {
                    token_p->type = ASSIGN;
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = START;
                    return token_p->str->length;
                }
                else {
                    ungetc(c, fr);

                    token_p->col--;
                    return token_p->str->length;
                }
                break;

            case GREATER:
                if (c == '=') {
                    token_p->type = GREATER_EQ;
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = START;
                    return token_p->str->length;
                }
                else
                    ungetc(c, fr);

                token_p->col--;
                return token_p->str->length;
                break;

            case LESS:
                if (c == '=') {
                    token_p->type = LESS_EQ;
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = START;
                    return token_p->str->length;
                }
                else if (c == '>') {
                    token_p->type = NOT_EQUAL;
                    token_p->str->name = StrAppend(token_p->str->name, c, token_p->str->length);
                    token_p->str->length++;
                    status = START;
                    return token_p->str->length;
                }
                else {
                    ungetc(c, fr);

                    token_p->col--;
                    return token_p->str->length;
                }
                break;

            case K_END:
                if (c == '.') {
                    Program_Ended = 1;
                }
                else {
                    ungetc(c, fr);
                    token_p->col--;
                    status = START;
                    return token_p->str->length;
                }
                break;
        }
    }


    if (c == EOF) {
        if (status == STR)
            err(LEXICAL_ERR, TEXTMSGS[ERR_LEX_STRING_EXCEED_LINE]);
        else if (status == COMMENT)
            err(LEXICAL_ERR, TEXTMSGS[ERR_UNEXPECTED_EOF]);
        else if (Program_Ended != 1)
            err(LEXICAL_ERR, TEXTMSGS[ERR_UNEXPECTED_EOF]);
        else if (status != START) {
            ungetc(c, fr);
            token_p->col--;
            return token_p->str->length;
        }
        token_p->type = END_OF_FILE;
        return token_p->str->length;
    }
    return token_p->str->length;
}


void test(FILE *fr)
{
    int c;
    while ((c = fgetc(fr)) != EOF) {
        if (isalpha(c) || '_')
            printf("%c", c);
        else if (isspace(c)) {
            printf("\n");
        }
    }
}

const char *TEXTMSGS[] =
        {
                [START] = "START",
                [ADD] = "ADD",
                [ID] = "ID",
                [INT] = "INT",
                [DOUBLE] = "DOUBLE",
                [EXP] = "EXP",
                [SIGN_EXP] = "SIGN_EXP",
                [STR] = "STR",
                [ESCAPE] = "ESCAPE",
                [END_STR] = "END_STR",
                [ESCAPE_END] = "ESCAPE_END",
                [LESS] = "LESS",
                [LESS_EQ] = "LESS_EQ",
                [GREATER] = "GREATER",
                [GREATER_EQ] = "GREATER_EQ",
                [EQUAL] = "EQUAL",
                [NOT_EQUAL] = "NOT_EQUAL",
                [ASSIGN] = "ASSIGN",
                [MULT] = "MULT",
                [L_BRACKET] = "L_BRACKET",
                [SUB] = "SUB",
                [COMMA] = "COMMA",
                [DIV] = "DIV",
                [R_BRACKET] = "R_BRACKET",
                [L_QUAD_BRACKET] = "L_QUAD_BRACKET",
                [R_QUAD_BRACKET] = "R_QUAD_BRACKET",
                [OR] = "OR",
                [DOT] = "DOT",
                [AND] = "AND",
                [OPERATOR] = "OPERATOR",
                [END_OF_FILE] = "END_OF_FILE",
                [COLON] = "COLON",
                [SEMICOLON] = "SEMICOLON",
                [ERR_OK] = "ERR_OK",
                [ERR_LEX] = "ERR_LEX",
                [ERR_UNEXPECTED_EOF] = "Unexpected EOF",
                [ERR_ILLEGAL_CHAR_CONST] = "Illegal char constatn",
                [ERR_LEX_OPERATOR_AT_START] = "ERR_LEX_OPERATOR_AT_START",
                [ERR_LEX_OPERATOR] = "ERR_LEX_OPERATOR",
                [ERR_LEX_STRING_EXCEED_LINE] = "ERR_LEX_STRING_EXCEED_LINE",
                [ERR_LEX_STR_NOT_ESCAPED] = "ERR_LEX_STR_NOT_ESCAPED",
                [ERR_LEX_INT_NOT_DIGIT] = "ERR_LEX_INT_NOT_DIGIT",
                [ERR_LEX_ID_NOT_ALLOWED_CHARS] = "ERR_LEX_ID_NOT_ALLOWED_CHARS",
                [ERR_LEX_ESCAPE_NOT_NUM] = "ERR_LEX_ESCAPE_NOT_NUM",
                [ERR_LEX_ESCAPE_NOT_ENDED] = "ERR_LEX_ESCAPE_NOT_ENDED",
                [ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS] = "ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS",
                [ERR_LEX_EXP] = "ERR_LEX_EXP",
                [ERR_LEX_SIGN_EXP] = "ERR_LEX_SIGN_EXP",
                [ERR_LEX_NOT_OR] = "ERR_LEX_NOT_OR",
                [ERR_LEX_NOT_AND] = "ERR_LEX_NOT_AND",
                [K_BEGIN]       = "begin",
                [K_BOOLEAN]     = "boolean",
                [K_DO]          = "do",
                [K_ELSE]        = "else",
                [K_END]         = "end",
                [K_FALSE]       = "false",
                [K_FIND]        = "find",
                [K_FORWARD]     = "forward",
                [K_FUNCTION]    = "function",
                [K_IF]          = "if",
                [K_INTEGER]     = "integer",
                [K_REDALN]      = "readln",
                [K_REAL]        = "real",
                [K_SORT]        = "sort",
                [K_STRING]      = "string",
                [K_THEN]        = "then",
                [K_TRUE]        = "true",
                [K_VAR]         = "var",
                [K_WHILE]       = "while",
                [K_WRITE]       = "write",
                [K_REPEAT]      = "repeat",
                [K_UNTIL]       = "until",
                [K_AND]       = "and",
                [K_OR]       = "or",
                [K_XOR]       = "xor",
                [K_NOT]       = "not",

                [K_COPY]       = "copy",
                [K_LENGTH]       = "length",
        };
