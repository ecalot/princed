
#ifndef _EXTARCT_H_
#define _EXTRACT_H_

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "formats/wav.h"
#include "pg.h"
#include "parser.h"
#include "resources.h"

/***************************************************************\
|                     M A I N   E X T R A C T                   |
\***************************************************************/

/*
	Tasks:
		1 - extract
		2 - update resources
		3 - 1&2
		4 - use raw file if extraction
		8 - rebuild resources list
*/

int extract(char* vFiledat,char* vDirExt, tResource* r[], char task);

#endif
