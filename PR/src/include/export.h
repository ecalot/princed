
#ifndef _EXTARCT_H_
#define _EXTRACT_H_

#include "resources.h"

char writeData(unsigned char* data, int ignoreChars, char* vFileext, int size);

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
