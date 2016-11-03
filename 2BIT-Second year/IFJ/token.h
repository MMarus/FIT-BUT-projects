/* ************************* token.h ************************* */
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

#ifndef TOKEN_H
#define TOKEN_H

//Typ lexemu v tokene
enum {
  START,
  BASE_START,
  SIGN,
  COMMENT,
  ID,
  INT,
  BASE,
  DOUBLE,
  EXP,
  SIGN_EXP,
  STR,
  END_STR,
  ESCAPE,
  ESCAPE_END,
  LESS,
  LESS_EQ,
  GREATER,
  GREATER_EQ,
  EQUAL,
  NOT_EQUAL,
  ASSIGN,
  MOD,
  ADD,
  SUB,
  MULT,
  DIV,
  L_BRACKET,
  R_BRACKET,
  L_QUAD_BRACKET,
  R_QUAD_BRACKET,
  OR,
  AND,
  OPERATOR,
  END_OF_FILE,

  COMMA,
  COLON,
  SEMICOLON,
  DOT,

  //ERRORS
  ERR_OK,
  ERR_LEX,
  ERR_LEX_OPERATOR_AT_START,
  ERR_LEX_OPERATOR,
  ERR_LEX_STRING_EXCEED_LINE,
  ERR_LEX_STR_NOT_ESCAPED,
  ERR_LEX_INT_NOT_DIGIT,
  ERR_LEX_ID_NOT_ALLOWED_CHARS,
  ERR_LEX_ESCAPE_NOT_NUM,
  ERR_LEX_ESCAPE_NOT_ENDED,
  ERR_LEX_DOUBLE_NOT_ALLOWED_CHARS,
  ERR_LEX_EXP,
  ERR_LEX_SIGN_EXP,
  ERR_LEX_NOT_OR,
  ERR_LEX_NOT_AND,


  //KEYWORDS
  K_BEGIN,
  K_BOOLEAN,
  K_DO,
  K_ELSE,
  K_END,
  //K_END_F,
  //K_END_P,
  K_FALSE,
  K_FIND,
  K_FORWARD,
  K_FUNCTION,
  K_IF,
  K_INTEGER,
  K_REDALN,
  K_REAL,
  K_REPEAT,
  K_SORT,
  K_STRING,
  K_THEN,
  K_TRUE,
  K_UNTIL,
  K_VAR,
  K_WHILE,
  K_WRITE,
  K_AND,
  K_OR,
  K_XOR,
  K_LENGTH,
  K_COPY,
  K_NOT,

  ERR_UNEXPECTED_EOF,
  ERR_ILLEGAL_CHAR_CONST,
  L_TERM,
  L_E,
  L_POM_LESS,
}LexA_Status;

#endif
