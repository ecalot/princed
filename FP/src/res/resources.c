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
#include "resources.h"

#include "compress.h"

#include "dat.h"
#include "disk.h"

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/
/*
#define initializePaletteList \
for (id=0;id<MAX_RES_COUNT;id++) {\
	if (r[id]!=NULL) {\
		r[id]->palAux=NULL;\
	}\
}
*/
/***************************************************************\
|                    M A I N   E X T R A C T                    |
\***************************************************************/


/*
	Extracts a dat file
	For parameter documentation, see pr.c
*/

int res_getData(int id,int maxItems,unsigned char** data,unsigned long int* size) {
	/* This function looks for a data resource in a dat file optimizing the search knowing
	 * that the id's starts in 0
	 */
	
	long int gotId;
	
	/* main loop */
	for (indexNumber=id;indexNumber+1=id;indexNumber++) {
		if (indexNumber==maxItems) indexNumber=0; /* reset the counter */
		gotId=mReadFileInDatFile(indexNumber,data,&size);
		if (gotId==id) break;
	}
	
	/* Return value */
	return (gotId==id); /* 1 if the id was found, 0 if not */
}



int res_get(const char* vFiledat) {
	int                indexNumber;
	long int           id;
	unsigned char*     data;
	unsigned long  int size;
	int                type=RES_TYPE_BINARY;

return type+10;
}


/**
 * Public interface
 * */

tData* resLoad(int id) {
					
				
	/* Initialize abstract variables to read this new DAT file */
	unsigned short int numberOfItems;
	if (!mReadBeginDatFile(&numberOfItems,"index.dat")) return -1;

	if (!res_getData(id,int maxItems,unsigned char** data,unsigned long int* size)) return -1;
	

	mReadCloseDatFile();
	


}

