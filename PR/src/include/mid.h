
#ifndef _MID_H_
#define _MID_H_

#include <stdio.h>
#include "resources.h"

char mFormatCompileMid(unsigned char* data, FILE* fp, tResource *res);
char mFormatExtractMid(unsigned char* data, char *vFileext,unsigned long int size);

#endif
