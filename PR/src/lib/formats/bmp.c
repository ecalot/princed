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
bmp.c: Princed Resources : BMP file support
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)
  Version: 1.10 (2004-Feb-17)

 Note:
  DO NOT remove this copyright notice
*/

#include "bmp.h"
#include "common.h"
#include "compress.h"
#include "dat.h"
#include "disk.h"
#include "memory.h"
#include "palette.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mWriteBmp(const char* file,const unsigned char* data, int w, int h, int bits, int colors, tColor* colorArray, int lineWidth, int optionflag, const char* backupExtension) {

	/* declare variables */
	int a;
	unsigned short int planes=1;
	unsigned long int colours;
	unsigned long int extra=1000;
	unsigned long int filesize;
	unsigned long int headerSize;
	unsigned long int height;
	unsigned long int offset;
	unsigned char color;
	unsigned long int width;
	const unsigned long int zero=0;
	char lineSerialization;
	FILE* bitmap;

	/* open file */
	if (!writeOpen(file,&bitmap,optionflag)) return 0; /* false */

	/* initialize variables */
/*	width=img.width;
	height=img.height;
	bits=getCarry(img.type);
	colours=1<<bits;*/
	headerSize=40;
	offset=54+(colors<<2);
	lineSerialization=(-lineWidth)&3;
	filesize=offset+(lineWidth+lineSerialization)*height;

	/* Write header */
	fwrite     ("BM",2,1   ,bitmap);    /* Magic identifier            */
	fwritelong (&filesize  ,bitmap);    /* File size in bytes          */
	fwritelong (&zero      ,bitmap);    /* reserved                    */
	fwritelong (&offset    ,bitmap);    /* Offset to image data, bytes */
	fwritelong (&headerSize,bitmap);    /* Header size in bytes        */
	fwritelong (&width     ,bitmap);    /* Width of image              */
	fwritelong (&height    ,bitmap);    /* Height of image             */
	fwriteshort(&planes    ,bitmap);    /* Number of colour planes     */
	fwriteshort(&bits      ,bitmap);    /* Bits per pixel              */
	fwritelong (&zero      ,bitmap);    /* Compression type (0=none)   */
	fwritelong (&zero      ,bitmap);    /* Image size in bytes         */
	fwritelong (&extra     ,bitmap);    /* Pixels per meter x          */
	fwritelong (&extra     ,bitmap);    /* Pixels per meter y          */
	fwritelong (&colours   ,bitmap);    /* Number of colours           */
	fwritelong (&zero      ,bitmap);    /* Important colours           */

	/* Write ColorTable */
	for (a=0;a<colors;a++) {
		color=colorArray[a].b;
		fwritechar(&color,bitmap); /* Blue  */
		color=colorArray[a].g;
		fwritechar(&color,bitmap); /* Green */
		color=colorArray[a].r;
		fwritechar(&color,bitmap); /* Red   */
		fwritechar(&zero ,bitmap); /* alpha */
	}

	/* Write data */
	while (h--) {
		fwrite(data+h*lineWidth,lineWidth,1,bitmap);
		fwrite(&zero,lineSerialization,1,bitmap);
	}

	writeCloseOk(bitmap,optionflag,backupExtension);
	return 1; /* true */
}

int mReadBitMap(tImage* image,unsigned char* data, int size) {
	char lineSerialization;
	int ok;
	int serializedWidth;
	int x=0;
	unsigned char carry;
	unsigned long int colours;
	unsigned long int filesize;
	unsigned long int offset;
	unsigned short int bits;
	unsigned short int height;
	unsigned short int width;

	/* Validate if there is header and if it starts in BM */
	ok    = size>50;
	ok=ok&& data[0]=='B' && data[1]=='M';
	/* Read sizes from header */
	width=(unsigned short)array2short(data+18);
	height=(unsigned short)array2short(data+22);
	bits=(unsigned short)(data[28]);

	/* Save sizes into image */
	image->width=width;            /* width in pixels */
	image->height=height;

	/* Calculate serialized widths */
	carry=(bits==1)?0:2;
	image->type=(bits==1)?0:0xB0;
	colours=1<<bits;
	offset=54+(colours<<2);
	width=(width+1)>>1;
	image->widthInBytes=(width+(0x07>>(1+carry)))>>(2-carry); /* this is an optimization of /(8/bits) : image->widthInBytes=imageWidthInPixels*bitsPerPixel/(8 bits/byte) */
	lineSerialization=(-image->widthInBytes)&3;
	serializedWidth=image->widthInBytes+lineSerialization;
	filesize=offset+serializedWidth*height;

	/* Validate image and file size; get memory to allocate the image */
	ok=ok&& (filesize==size);
	ok=ok&& (	(image->pix=malloc(height*image->widthInBytes*2)) != NULL	);
	/* if validations==wrong */
	if (!ok) {
		freeAllocation(image->pix);
		return 0; /* false: this is not a valid BMP file format or memory too low */
	}

	/* Serialize bitmap-->raw array */
	while (height--)
		memcpy(
			image->pix+(x++)*image->widthInBytes,
			data+offset+height*serializedWidth,
			image->widthInBytes
		);

	return 1; /* true */
}
