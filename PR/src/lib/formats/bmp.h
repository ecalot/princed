
#ifndef _BMP_H_
#define _BMP_H_

#include <stdio.h>
#if DIR_SEPARATOR=='/'
#include "../pg.h"
#include "../resources.h"
#else
#include "pg.h"
#include "resources.h"
#endif

char mFormatCompileBmp(unsigned char* data, FILE* fp, tResource *res);
char mWriteBitMap(tImage img,char* vFile);
char mReadBitMap(tImage* img,char* data, int size); //Note: this will ignore the palette

#endif
