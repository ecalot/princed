/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#ifndef _COMPILE_H_
#define _COMPILE_H_

#include <stdio.h>
#include "resources.h"

void mAddFileToDatFile(FILE* fp, char* data, int size);
int compile(char* vFiledat, char* vDirExt, tResource* r[], char opt);

#endif
