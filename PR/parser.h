/****************************************************************\
|     Libreria de manejo de strings, conversiones y parsing      |
\****************************************************************/

#ifndef _PARSER_H_
#define _PARSER_H_

//Constantes del programa referentes al case
#define UPPER_CASE    0xDF
#define INSENSIBLE    0xFF

//Constante que marca el maximo tamagno que puede medir una palabra
// separada por espacios en la funcion matchesIn (token)
#define PARSING_MAX_TOKEN_SIZE 200

//Letras a numeros
char num(char n);

//Interprete de numeros en strings
unsigned long str2int(char r[]);

//Comparacion de strings
int equals(char s1[],char s2[]);

//Tokenizer simple
//Sintaxis: getToken(string texto,var string token,char caracterSeparador,
//                   var int startPosition, int sizeOfToken)
int getToken(char texto[],char token[],char tokenizer,int* i,int k);

//Tokenizer simple
//Sintaxis: getNumberToken(string texto,var int token,char caracterSeparador,
//                   var int startPosition, int deprecated)
int getNumberToken(char texto[],unsigned short int *token,char tokenizer,int* i,int k);

//Tokenizer + upperCase, muy bueno para interpretar comandos case insensitive
//Sintaxis: getUpperToken(string texto,var string token,char caracterSeparador,
//                   var int startPosition, int sizeOfToken)
int getUpperToken(char texto[],char token[],char tokenizer,int* i,int k);

//Tokenizer complejo con soporte de comillas, optimizado para separar palabras
//Sintaxis: getToken(string texto,var string token,
//                   var int startPosition, int sizeOfToken)
int getMaskToken(char texto[],char token[],int* i,int k);

#endif


