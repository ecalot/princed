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
|                                                               |
\***************************************************************/

/*
 * Resource creation
 */

void res_rememberPalette(tMemory data) {}

/* Using the type and the array data this function will return the resources in void* fromat */
void* res_createResource(tMemory data,int type) {
	tMemory* result;
	/* TODO: data->array must be copied or it wont be available after the file is closed */
	result=(tMemory*)malloc(sizeof(tMemory)); /* both sides are void* :)  */
	*result=(tMemory)data;
	printf("res_createResource: Allocating frame[?]=? (type %d)\n",type);
	return (void*)result;
}

/*
 * Functions made to get data from the DAT files using some optimizations
 */

int res_getDataById(int id,int maxItems,tMemory* result) {
	/* This function looks for a data resource in a dat file optimizing the search knowing
	 * that the id's starts in 0
	 */
	
	long int gotId;
	int indexNumber;
	
	/* main loop */
	for (indexNumber=0;indexNumber<maxItems;indexNumber++) {
		gotId=mReadFileInDatFile((indexNumber+id-DATA_START_ITEMS)%maxItems,&(result->array),&(result->size));
		//printf("Debug: res_getData: indexNumber=%d gotId=%d id=%d\n",indexNumber,gotId,id);
		if (gotId==id) break;
	}
	
	/* Return value */
	return (gotId==id); /* 1 if the id was found, 0 if not */
}

int res_getDataByArray(short int* id,int maxItems,void** result,int ids,int type) {
	/* This function looks for a data resource in a dat file optimizing the search knowing
	 * that the id's starts in 0
	 */
	
	long int gotId;
	int indexNumber;
	int i=0;
	int old=-1;
	tMemory data;
	
	/* main loop */
	while((old!=i)&&(i!=ids)) {
		old=i;
		printf("res_getDataByArray: Starting resource cycling\n");
		for (indexNumber=0;(indexNumber<maxItems)&&(i!=ids);indexNumber++) {
			gotId=mReadFileInDatFile(
				indexNumber,
				&(data.array),
				&(data.size)
			);
			if (gotId==id[i]) {
				result[i]=(void*)res_createResource(data,type);
				i++;
			}
		}
	}

	printf("Done!\n");
	
	/* Return value */
	return i;
}

#define res_getIdxFile1 ((char*)(index.array+1))
#define res_getIdxFile2 ((char*)(index.array+15))
#define res_getIdxType (((short int*)(index.array+31))[0])
#define res_getIdxTotal (((short int*)(index.array+29))[0])
#define res_getIdxFrames ((short int*)(index.array+33))

/**
 * Public functions
 * */

tData* resLoad(int id) {
				
	/* Initialize abstract variables to read this new DAT file */
	unsigned short int numberOfItems;
	tData* result;
	int i;
	char file1[25];
	char file2[25];
	short int* frames;
	int nFrames;
	tMemory index;

	/* READ INDEX */
	if (!mReadBeginDatFile(&numberOfItems,"index.dat")) return NULL;
	if (!res_getDataById(id,DATA_END_ITEMS,&index)) {
		printf("Fatal Error: resLoad: index could not be read!\n");
		return NULL;
	}
	printf("eeee\n");
	printf("file1='%s'. file2='%s' type='%d'. frames='%d'.\n",
		res_getIdxFile1,
		res_getIdxFile2,
		res_getIdxType,
		res_getIdxTotal
	);

	nFrames=res_getIdxTotal;
	frames=(short int*)malloc(nFrames*sizeof(short int));
	for (i=0;i<nFrames;i++) {
		frames[i]=res_getIdxFrames[i];
	}

	mReadCloseDatFile();

	/* READ FILE */
	
	for (i=0;i<nFrames;i++) {
		printf("frames[%d]=%d\n",i,frames[i]);
	}
	
	if (!mReadBeginDatFile(&numberOfItems,res_getIdxFile1)) {
		printf("Fatal Error: resLoad: resource file not found!\n");
		free(frames);
		return NULL;
	}

	result=(tData*)malloc(sizeof(tData));
	result->pFrames=(void**)malloc(nFrames*sizeof(void*));
	//result->pFrames[0]=(tMemory*)malloc(nFrames*sizeof(tMemory));
	
	result->frames=nFrames;
	result->type=eImages;//res_getVirtualTypeFromReal(res_getIdxType);

	if (!res_getDataByArray(frames,numberOfItems,result->pFrames,nFrames,res_getIdxType)) {
		printf("Fatal Error: resLoad: resource file invalid!\n");
		free(frames);
		//free(result->pFrames[0]);
		free(result->pFrames);
		free(result);
		return NULL;
	}

	
	//mReadCloseDatFile();
	free(frames);

	return result;
}

