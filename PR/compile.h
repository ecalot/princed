/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#ifndef _COMPILE_H_
#define _COMPILE_H_

#include <stdio.h>
#include <stdlib.h>
#include "formats/wav.h"
#include "pg.h"
#include "resources.h"
#include "memory.h"
#include "parser.h"

int compile(char* vFiledat, char* vDirExt, tResource* r[], char opt);

#endif
