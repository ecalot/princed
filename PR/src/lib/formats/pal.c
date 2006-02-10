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
pal.c: Princed Resources : JASC PAL files support
�����
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

/* Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pal.h"
#include "disk.h"
#include "dat.h"

/***************************************************************\
|                 JASC Palette handling functions               |
\***************************************************************/

/*static const char* enter="\r\n";*/

/* Public functions */
int mFormatExportPal(const tPalette* p, int bits, char *vFileext, int optionflag, const char* backupExtension) {
/*	unsigned char* pal=malloc(256*4+50);*/
	unsigned char* aux=malloc(MAX_FILENAME_SIZE);
	const tColor* palette;
	int i;
	FILE* fd;

	/* Export extra palette information */
	/*sprintf((char*)aux,"%s.more",vFileext);
	writeData(data,1,(char*)aux,size,optionflag,backupExtension); TODO fix that */

	/* open file */
	if (!writeOpen(vFileext,&fd,optionflag)) return 0; /* false */

	/* Convert palette from tPalette format to JASC format */
	getPalette(p, bits, &palette);

	fprintf(fd,"JASC-PAL\r\n%04d\r\n%d\r\n",100,1<<bits);
	for (i=0;i<(1<<bits);i++) {
		fprintf(fd,"%d %d %d\r\n",
			palette[i].r,
			palette[i].g,
			palette[i].b
		);
	}
	/*for (i=0;pal[i];i++);
	size=i-1;*/

	/* save JASC palette */
/*	i=writeData(pal,0,vFileext,size,optionflag,backupExtension);*/
	writeCloseOk(fd,optionflag,backupExtension);

/*	free(pal);*/
	free(aux);
	return i;
}

int mFormatImportPal(tPalette* p,int* bits, const char* vFile) {
	/* declare variables */
	unsigned char* pals;
	unsigned char* pals1;
	unsigned char  pals2[400]/*PAL_SAMPLE*/;
	unsigned char  palh [30];
	unsigned char* pal;
	unsigned char* pal2;
/*	char* data2;*/
	char aux[MAX_FILENAME_SIZE];
/*	int r;
	int g;
	int b;*/
	int i=0;
/*	int k=16;*/
	int sample1;

	/* check size */
	/*if ((res->size)<130) return 0; * false */

	/* TODO: fix, pal 256 support. verify JASC pal header */
	/*while (palh[i]==(res->data)[i++]);*/
	if (i!=sizeof(palh)) return 0; /* false: palette differs with headers */

	/* Read sample */
	sprintf(aux,"%s.more",vFile);
	sample1=mLoadFileArray(aux,&pals1);
	if (sample1<0) return sample1;

	if (sample1==100) {
		pals=pals1;
	} else {
		pals=pals2;
	}

	/* Allocate palette */
	pal=malloc(100);

	/* set palette with sample */
	memcpy(pal,pals,100);
	pal2=pal+4;
	if (sample1) free(pals1);

	/* set current values */
	/*data2=strtok((char*)(res->data)+sizeof(palh)-1,enter);
	while (k--) {
		if (!sscanf(data2,"%d %d %d",&r,&g,&b)) return 0; * false *
		* Those lines mean a loss of data (palette colours are saved in the nearest multiple of 4) *
		*(pal2++)=(unsigned char)((r+2)>>2);
		*(pal2++)=(unsigned char)((g+2)>>2);
		*(pal2++)=(unsigned char)((b+2)>>2);
		data2=strtok(NULL,enter);
	}
*/
	/* save and free palette */
/*	res->size=100;
	free(res->data);
	res->data=pal;
	mWriteFileInDatFile(res);
*/
	return 1; /* true */
}

