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
pop1_16c.c: Princed Resources : The palette object implementation
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

void addPop1Raw(tPalette* p,unsigned char* data, int dataSize);

/***************************************************************\
|                         Palette Object                        |
\***************************************************************/

static tColor sample_pal4[]={{0,0,0},{4,4,4}}; /*SAMPLE_PAL16;*/

typedef struct {
	tColor c[16];
	unsigned char raw[100];
}tPalettePop1_16c;

void* objectPalettePop1_16Create(tBinary cont, int *error) {
	int i;
	tPalettePop1_16c* pal;

	if (cont.size!=100) {
		*error=PR_RESULT_XML_AND_DAT_FORMAT_DO_NOT_MATCH;
		return NULL;
	}

	pal=(tPalettePop1_16c*)malloc(sizeof(tPalettePop1_16c));

	for (i=0;i<16;i++) {
		pal->c[i].r=cont.data[(i*3)+4]<<2;
		pal->c[i].g=cont.data[(i*3)+5]<<2;
		pal->c[i].b=cont.data[(i*3)+6]<<2;
	}

	memcpy(pal->raw,cont.data,100);

	*error=PR_RESULT_SUCCESS;

	return (void*)pal;
}

int objectPalettePop1_16Write(void* o, const char* file, int optionflag, const char* backupExtension) {
	tPalettePop1_16c* pal=o;
	char aux[260];

	/* Export extra palette information */
	sprintf(aux,"%s.more",file);
	writeData(pal->raw,0,aux,100,optionflag,backupExtension);

	return writePal(file,16,pal->c,optionflag,backupExtension);
}

tColor* objectPalettePop1_16GetColorArray(void* o) {
	tPalettePop1_16c* pal=o;
	return pal->c;
}

tColor* objectPaletteGetColorArrayForColors(int colors) {
	static tColor p1[2];
	static tColor p4[2];
	static tColor p8[2];
	int i;

	switch(colors) {
	case 2:
		/* Palette 1 bit */
		p1[0].r=0;
		p1[0].g=0;
		p1[0].b=0;
		p1[1].r=255;
		p1[1].g=255;
		p1[1].b=255;
		return p1;
	case 16:
		/* Palette 4 bits */
		memcpy(p4, sample_pal4, 16);
		return p4;
	case 256:
		/* Palette 8 bits */
		for (i=0;i<256;i++) {
			p8[i].r=i;
			p8[i].g=i;
			p8[i].b=i;
		}
		return p8;
	default:
		return NULL; /* unsupported bit rate */
	}
}

void* objectPalettePop1_16Read(const char* file,int *result) {
	tPalettePop1_16c* pal=(tPalettePop1_16c*)malloc(sizeof(tPalettePop1_16c));
	tColor* colorArray;
	int colors;
	char aux[260];
	tBinary raw;

	*result=PR_RESULT_ERR_FILE_NOT_OPEN_NOTFOUND;

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

int objectPalettePop1_16Set(void* o,tResource* res) {
	tPalettePop1_16c* pal=o;
	int i;

	res->content.size=100;
	res->content.data=pal->raw;
	for (i=0;i<16;i++) {
		res->content.data[(i*3)+4]=convert24to18(pal->c[i].r);
		res->content.data[(i*3)+5]=convert24to18(pal->c[i].g);
		res->content.data[(i*3)+6]=convert24to18(pal->c[i].b);
	}
/*	res->content.size=100;
	res->content.data=pal->raw;*/
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

