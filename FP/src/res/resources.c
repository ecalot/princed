/*  FreePrince - POP1 remake
    Copyright (C) 2003,2004 Princed Development Team

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
resources.c: Princed Resources : DAT Extractor
¯¯¯¯¯¯¯¯¯¯¯
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
#include <stdlib.h>
#include <string.h>
#include "freeprince.h"
#include "resources.h"
#include "output.h"
#include "maps.h"

#include "compress.h"

#include "dat.h"
#include "disk.h"

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/*
 * Resource creation
 */

/*
 * Functions made to get data from the DAT files using some optimizations
 */

int res_getDataById(int id,int maxItems,tMemory* result) {
	/* This function looks for a data resource in a dat file optimizing the search knowing
	 * that the id's starts in 0
	 */
/* TODO: use static to remember where it is */	
	long int gotId;
	int indexNumber;
	
	/* main loop */
	for (indexNumber=0;indexNumber<maxItems;indexNumber++) {
		gotId=mReadFileInDatFile(
				(indexNumber+id)%maxItems,
				&(result->array),
				(unsigned long *)&(result->size));
		if (gotId==id) break;
	}
	
	/* Return value */
	return (gotId==id); /* 1 if the id was found, 0 if not */
}

static const short res_list[]=RES_LIST;
static const char* res_file[]=RES_FILES;

/**
 * Public functions
 * */

tData* resLoad(long id) {
	/* Initialize abstract variables to read this new DAT file */
	unsigned short int numberOfItems;
	tData* result;
	tMemory raw;
	int mask=res_get_part_mods(id);
	int total=res_get_part_size(id);
	int from=res_get_part_from(id);
	int type=res_get_part_type(id);
printf("type=%d mask=%d total=%d from=%d id=%08x\n",type,mask,total,from,id);
	switch (type) {
		case RES_TYPE_LVL:
			if (total!=2) {
				printf("Fatal Error: resLoad: invalid level define!\n");
				return NULL;
			}
			mask+=res_list[from+1];
			if (!mReadBeginDatFile(&numberOfItems,res_file[res_list[from]])) {
				printf("Fatal Error: resLoad: level file not found!\n");
				return NULL;
			}
			if(!res_getDataById(mask,numberOfItems,&raw)) {
				printf("Fatal Error: resLoad: level not found!\n");
				return NULL;
			}
			mReadCloseDatFile();
			return (tData*)mapLoadLevel(raw); /* transform from raw to a loaded map */
		case RES_TYPE_IMG: {
			tMemory palette;
      tImage image;
      tPalette pal;
 
			result=(tData*)malloc(sizeof(tData));
			result->frames=total-2; /* drop filename and palette */
			result->pFrames=(void**)malloc(result->frames*sizeof(void*));
			if (!mReadBeginDatFile(&numberOfItems,res_file[res_list[from]])) {
				printf("Fatal Error: resLoad: level file not found!\n");
				return NULL;
			}
			if(!res_getDataById(res_list[from+1],numberOfItems,&palette)) {
				printf("Fatal Error: resLoad: palette not found!\n");
				return NULL;
			}
			printf("hola %d\n",palette.size);
			if (palette.size!=100) {
      	printf("Fatal error: resLoad: invalid palette\n");
        return NULL;
      }
      pal.colors=16; /* TODO: detect when it is 2 colors */
 	    pal.color=(tColor*)(palette.array+5);	/*memcpy(image.pal,data.array+5,16*3);*/
			for (total=0;total<result->frames;total++) {
				if(!res_getDataById(res_list[from+2+total],numberOfItems,&raw)) {
					printf("Fatal Error: resLoad: image not found!\n");
					return NULL;
				}

				/* expand raw image into an image structure */
				mExpandGraphic(raw.array,&image,raw.size);

				/* convert image structure into blittable output surfaces */
				result->pFrames[total]=(void*)outputLoadBitmap(
					image.pix,image.widthInBytes*image.height,pal,image.height,image.width,
					(mask&(RES_MODS_LEFT))==(RES_MODS_LEFT),1
        );

				/* free intermediate image data */
				free(image.pix);
			
			}
			mReadCloseDatFile();
			return result;
		}
	}
	return NULL; /* unknown type */
}

void resFree(tData* res) {
/*	if (res->type==eImages) {
		while (res->frames) {
			outputFreeBitmap(res->pFrames[--(res->frames)]);
		}
	}
	printf("ResFree activated\n");
	free(res->pFrames);
	free(res);*/
}

