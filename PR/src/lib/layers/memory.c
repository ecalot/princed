#include "memory.h"

//Reserves a memory space to allocate an image
unsigned char* getMemory(int size) {
	return (unsigned char*)malloc(sizeof(unsigned char*)*size);
}
