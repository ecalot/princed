/****************************************************************\
|     Libreria de manejo de strings, conversiones y parsing      |
\****************************************************************/

#ifndef PARSER_DEFINED
#define PARSER_DEFINED

#include "parser.h"

//Letras a numeros
char num(char n) {
	return ((0x2F<n)&&(n<0x3A))?(n-(0x30)):0;
}

//Interprete de numeros en strings
unsigned long str2int(char r[]) {
	char i     = 0;
	int  valor = 0;
	while(r[i]) {
		valor=(10*valor)+num(r[i++]);
	}
	return valor;
}

//Comparacion de strings
int equals(char s1[],char s2[]) {
	int i=0;
	for (;(s1[i]==s2[i])&&s1[i];i++);
	return !(s1[i]||s2[i]);
}

//Tokenizer simple
//Sintaxis: getToken(string texto,var string token,char caracterSeparador,
//                   var int startPosition, int sizeOfToken)
int getToken(char texto[],char token[],char tokenizer,int* i,int k) {
	int j=0;
	//Copiar el string hasta que se encuentre el token o se termine la linea.
	//En caso de que no entre el texto en el token, lo deja truncado pero avanza i hasta el final
	while (texto[*i]!=tokenizer && ((j<k)?(token[j++]=texto[(*i)++]):(texto[(*i)++])));
	token[j]='\0';
	return (texto[((*i)++)-1]);
}

//Tokenizer simple
//Sintaxis: getNumberToken(string texto,var int token,char caracterSeparador,
//                   var int startPosition, int deprecated)
int getNumberToken(char texto[],unsigned short int *token,char tokenizer,int* i,int k) {
	char a;
	//Copiar el string hasta que se encuentre el token o se termine la linea.
	//En caso de que no entre el texto en el token, lo deja truncado pero avanza i hasta el final
	for (*token=0;(((a=texto[(*i)++])!=tokenizer)&&a);(*token)=(*token)*10+num(a));
	return (a);
}

//Tokenizer + upperCase, muy bueno para interpretar comandos case insensitive
//Sintaxis: getUpperToken(string texto,var string token,char caracterSeparador,
//                   var int startPosition, int sizeOfToken)
int getUpperToken(char texto[],char token[],char tokenizer,int* i,int k) {
	int j=0;
	//Idem, pero si la letra esta entre 'a' y 'z' las pasa a mayusculas
	while ((texto[*i]!=tokenizer) && ((j<k)?(token[j++]=(('a'>texto[(*i)])||(texto[(*i)]>'z'))?(texto[(*i)++]):(texto[(*i)++]&UPPER_CASE)):(texto[(*i)++])));
	token[j]='\0';
	return (texto[((*i)++)-1]);
}

//Tokenizer complejo con soporte de comillas, optimizado para separar palabras
//Sintaxis: getToken(string texto,var string token,
//                   var int startPosition, int sizeOfToken)
int getMaskToken(char texto[],char token[],int* i,int k) {
	int j=0;
	//Copiar el string hasta que se encuentre el token Espacio o se termine la linea.
	//En caso de que no entre el texto en el token, lo deja truncado pero avanza i hasta el final
	char entreComillas=0; //flag que indica si se esta dentro de una cadena entre comillas, alterna entre 0 y 1 dependiendo de si se encuentran las comillas, en caso de valer 1 el while no se detiene
	while ((((entreComillas^=(texto[(*i)]==34)) || (texto[*i]!=32)) && ((j<k)?(token[j++]=texto[(*i)++]):(texto[(*i)++]))));
	//Seteo el caracter nulo al final del token, incremento i y devuelvo el siguiente caracter del texto o 0 en caso de que este sea el nulo.
	token[j]='\0';
	return (texto[((*i)++)-1]);
}

//Funcion privada
char matches1(char* text,char* mask,char ptext, char pmask) {
	/*
		Esta funcion verifica que el texto pertenezca a la mascara,
		no verifica espacios ya que eso debe ser parseado en la funcion
		que la llama.
		En caso de poseer * la funcion se vuelve recursiva.
		 Optimizacion:
		  1) Se contempla el caso del * multiple y se lo toma como simple
		  para evitar la ejecucion de recursividad de O(n cuadrado).
		  2) Se contempla el caso del * al final de la mascara, en dicho caso
		  no ejecuta la recursividad y devuelve verdadero ya que, si se llego a
		  ese punto, el texto matchea.
		En caso de poseer " las ignora.
		En caso de poseer ? contempla cualquier caracter (incluso ? y *).

		Devuelve 1 en caso de que el caracter coincida y 0 en caso contrario.
	*/
	char aux;

	while (text[ptext]||mask[pmask]) {
		if (mask[pmask]=='"') {
			pmask++;
		} else if (mask[pmask]=='?') {
			pmask++;
			if (!text[ptext]) return 0;
			ptext++;
		} else if (mask[pmask]=='*') {
			while (mask[pmask++]=='*');
			pmask--;
			if (!mask[pmask]) return 1; //acelera un poco el proceso para el caso particular de que termine en *
			while ((text[ptext]) && !(aux=matches1(text,mask,ptext++,pmask)));
			return aux;
		} else {
			if (text[ptext]!=mask[pmask]) return 0;
			ptext++;
			pmask++;
		}
	}
	return (text[ptext]==mask[pmask]);
}

//Funcion publica
char matchesIn(char* text,char* mask) {
	int i=0;
	char token[PARSING_MAX_TOKEN_SIZE];
	char valor=0;

	/*
		Esta funcion se encarga de partir los espacios de la mascara y
		llamar a una comparacion por cada parte
	*/
	while (getMaskToken(mask,token,&i,PARSING_MAX_TOKEN_SIZE) && !(valor=(valor || matches1(text,token,0,0))));
	return valor?1:matches1(text,token,0,0);
}

#endif

