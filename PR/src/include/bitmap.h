
#ifndef _BMP_H_
#define _BMP_H_

#include "resources.h"
#include "compress.h"

char mFormatExtractBmp(unsigned char* data, const char *vFileext,unsigned long int size,tImage image);
char mFormatCompileBmp(unsigned char* data, FILE* fp, tResource *res);
char mWriteBitMap(tImage img,const char* vFile);
char mReadBitMap(tImage* img,unsigned char* data, int size); //Note: this will ignore the palette

#endif
