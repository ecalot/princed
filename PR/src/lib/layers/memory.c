#include "memory.h"

char* strallocandcopy(const char* text) {
	int size;
	char* aux;
	if (text==NULL) return NULL;
	size=strlen(text)+1;
	aux=getMemory(size);
	if (aux) memcpy(aux,text,size);
	return aux;
}
