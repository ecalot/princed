
#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdlib.h>

//Reserves a memory space to allocate an image
//TODO change "unsigned char*" for "unsigned char" in sizeof
#define getMemory(size) ((unsigned char*)malloc(sizeof(unsigned char*)*(size)))

#endif

