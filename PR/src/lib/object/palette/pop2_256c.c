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
pop2_256c.c: Princed Resources : The POP2 palette object implementation
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 09 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2006-Feb-09)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "palette.h"
#include "memory.h"
#include "dat.h"
#include "disk.h" /* writeData */
#include "auxiliary.h" /* isA64kPalette */

#include "pal.h"

typedef struct {
	tColor* colorArray;
	int size;
}tGenericPalette;

void* objectPalettePop2_NColorsCreate(tBinary cont, int *error) {
	tGenericPalette* r;
	int i,j;

	*error=PR_RESULT_SUCCESS;

	if (!isA64kPalette(cont)) { /* TODO: use autodetect function to verify */
		*error=-14; /* TODO FIX or assign error code */
		return NULL;
	}

	r=(tGenericPalette*)malloc(sizeof(tGenericPalette));
	r->colorArray=(tColor*)malloc(sizeof(tColor)*((cont.size+2)/3));
	for (i=0,j=0;i<cont.size;i+=3,j++) {
		r->colorArray[j].r=to8bits_B(cont.data[i+0]); /* TODO: use increase per line i++ */
		r->colorArray[j].g=to8bits_B(cont.data[i+1]);
		r->colorArray[j].b=to8bits_B(cont.data[i+2]);
	}
	/*if (j!=256 && j!=320) return NULL; *TODO: add free */
	r->size=j;
printf("created a palette with %d colors. cs=%ld\n",j,cont.size);

	return (void*)r;
}

tColor* objectPalettePop2_NColorsGetColorArray(void* o) {
	tGenericPalette* pal=o;
	return pal->colorArray;
}


int objectPalettePop2_NColorsGetColorCount(void* o) {
	tGenericPalette* pal=o;
	return pal->size;
}

int objectPalettePop2_NColorsWrite(void* o, const char* file, int optionflag, const char* backupExtension) {
	tGenericPalette* pal=o;
	return writePal(file,pal->size,pal->colorArray,optionflag,backupExtension);
}

void* objPop2PaletteNColorsRead(const char* file,int *result) {
	tGenericPalette *r;

	r=(tGenericPalette*)malloc(sizeof(tGenericPalette));

	*result=readPal(file,&r->colorArray,&r->size);

	if (*result==PR_RESULT_SUCCESS && !(r->size==256 || r->size==320)) {
		*result=PR_RESULT_PAL_UNSUPPORTED;
		free(r->colorArray);
		free(r);
		return NULL;
	}

	return (void*)r;
}

int objPop2PaletteNColorsSet(void* o,tResource* res) {
/*	tPop2_ncolorPalette* pal=o;
	int i;

	res->content.size=100;
	res->content.data=pal->raw;
	for (i=0;i<16;i++) {
		res->content.data[(i*3)+4]=convert24to18(pal->c[i].r);
		res->content.data[(i*3)+5]=convert24to18(pal->c[i].g);
		res->content.data[(i*3)+6]=convert24to18(pal->c[i].b);
	}
	res->content.size=100;
	res->content.data=pal->raw;
	mWriteFileInDatFile(res);*/
	return PR_RESULT_SUCCESS;
}
