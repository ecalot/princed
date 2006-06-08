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
pop2_4bit.c: Princed Resources : The POP2 16 colors palette object implementation
¯¯¯¯¯¯¯¯¯¯
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

#include "pal.h"

void addPop2Raw(tPalette* p,unsigned char* data, int dataSize);

/***************************************************************\
|                         Palette Object                        |
\***************************************************************/

/* TODO: use comon factor or drop */
int setPalette(tPalette* p, int bits, tColor* palette);
int getPalette(const tPalette* p, int bits, const tColor** apalette);
tPalette createPalette();
				
/* middle layer */
#define to8bits_A(a) (((a)<<2)|((a)>>4))
#define to8bits_B(a) (((a)<<2)         )
#define to8bits_C(a) (((a)<<2)+2       )

/* reads the information in data and tries to remember it in the palette */
int readPalette(tPalette* p, unsigned char* data, int dataSize);

typedef struct { 
	tColor c[16];
	unsigned char raw[384];
}tPop2_4bitsPalette;

void* objPalette_pop2_4bitsCreate(tBinary cont, int *error) {
	int i;
	tPop2_4bitsPalette* pal;
	
	if (cont.size!=384) {
		*error=PR_RESULT_XML_AND_DAT_FORMAT_DO_NOT_MATCH;
		return NULL;
	}

	pal=(tPop2_4bitsPalette*)malloc(sizeof(tPop2_4bitsPalette));
	
	for (i=0;i<16;i++) {
		pal->c[i].r=cont.data[(i*3)+4]<<2;
		pal->c[i].g=cont.data[(i*3)+5]<<2;
		pal->c[i].b=cont.data[(i*3)+6]<<2;
	}

	memcpy(pal->raw,cont.data,384);

	*error=PR_RESULT_SUCCESS;
	
	return (void*)pal;
}

int objPalette_pop2_4bitsWrite(void* o, const char* file, int optionflag, const char* backupExtension) {
	tPop2_4bitsPalette* pal=o;
	char aux[260];

	/* Export extra palette information */
	sprintf(aux,"%s.more",file);
	writeData(pal->raw,0,aux,384,optionflag,backupExtension);

	return writePal(file,16,pal->c,optionflag,backupExtension);
}

tColor* objPalette_pop2_4bitsGetColors(void* o) {
	tPop2_4bitsPalette* pal=o;
	return pal->c;
}

tColor* paletteGetColorArrayForColors(int colors); /* TODO: this function should be in a common.c inside palette object */

void* objPop2Palette4bitsRead(const char* file,int *result) {
	tPop2_4bitsPalette* pal=(tPop2_4bitsPalette*)malloc(sizeof(tPop2_4bitsPalette));
	tColor* colorArray;
	int colors;
	char aux[260];
	tBinary raw;

	/* Import extra palette information */
	sprintf(aux,"%s.more",file);
	raw=mLoadFileArray(aux);
	if (raw.size!=100) return NULL; /* TODO; free memory */
	memcpy(pal->raw,raw.data,100);
	free(raw.data);

	*result=readPal(file,&colorArray,&colors);

	if (*result==PR_RESULT_SUCCESS && colors!=16) {
		*result=PR_RESULT_PAL_UNSUPPORTED;
		free(colorArray);
		return NULL;
	}
	memcpy(pal->c,colorArray,sizeof(tColor)*16);
	free(colorArray);
	
	return (void*)pal;
}

#define convert24to18(x) (unsigned char)((x+2)>>2); /* TODO: send to a common palette header */

int objPop2Palette4bitsSet(void* o,tResource* res) {
	tPop2_4bitsPalette* pal=o;
	int i;

	res->content.size=384;
	res->content.data=pal->raw;
	for (i=0;i<16;i++) {
		res->content.data[(i*3)+4]=convert24to18(pal->c[i].r);
		res->content.data[(i*3)+5]=convert24to18(pal->c[i].g);
		res->content.data[(i*3)+6]=convert24to18(pal->c[i].b);
	}
	res->content.size=384;
	res->content.data=pal->raw;
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

