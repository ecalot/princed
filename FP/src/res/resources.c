/*  Princed V3 - Prince of Persia Level Editor for PC Version
    Copyright (C) 2003 Princed Development Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    The authors of this program may be contacted at http://forum.princed.com.ar
*/

/*
extract.c: Princed Resources : DAT Extractor
¯¯¯¯¯¯¯¯¯
 Copyright 2003, 2004 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)
  Version: 1.20 (2004-Mar-07)
  Version: 1.30 (2004-Mar-15)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <string.h>
#include "freeprince.h"

#include "compress.h"

#include "dat.h"
#include "disk.h"

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#define initializePaletteList \
for (id=0;id<MAX_RES_COUNT;id++) {\
	if (r[id]!=NULL) {\
		r[id]->palAux=NULL;\
	}\
}

/***************************************************************\
|                    M A I N   E X T R A C T                    |
\***************************************************************/


/*
	Extracts a dat file
	For parameter documentation, see pr.c
*/

int prVerifyDatType(const char* vFiledat) {
	int                indexNumber;
	long int           id;
	unsigned char*     data;
	unsigned long  int size;
	int                type=RES_TYPE_BINARY;
	unsigned short int numberOfItems;

	/* Initialize abstract variables to read this new DAT file */
	if (!mReadBeginDatFile(&numberOfItems,vFiledat)) return -1;

	/* main loop */
	for (indexNumber=0;(indexNumber<numberOfItems)&&(type==RES_TYPE_BINARY);indexNumber++) {
		id=mReadFileInDatFile(indexNumber,&data,&size);
		if (id<0) return -1; /* Read error */
		if (id==0xFFFF) continue; /* Tammo Jan Bug fix */
		if (id>=MAX_RES_COUNT) return -1; /* A file with an ID out of range will be treated as invalid */
		type=0;//verifyHeader(data,size);
	}

	mReadCloseDatFile();
	return type+10;
}
