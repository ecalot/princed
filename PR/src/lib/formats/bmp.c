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
�����
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)
  Version: 1.10 (2004-Feb-17)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pr.h"
#include "bmp.h"
#include "disk.h"
#include "memory.h"
#include "dat.h"

extern FILE* outputStream;

int mFormatExportBmp(const unsigned char* data, const char *vFileext,unsigned long int size,tImage image,int optionflag, const char* backupExtension) {
	/*
		This function will expand the data into an image structure,
		then the bitmap structure will be saved to disk

		Note: The old structure is passed by paremeters in order to
		      keep the right palette.
	*/
	int result;
fld("ZA1");

	/* Expand graphic and check results */
	result=mExpandGraphic(data,&image,size);
printf("0) %d+%d*%d,%d\n",image.pix,image.height,image.widthInBytes,image.widthInBytes);
	if ((result==COMPRESS_RESULT_WARNING)&&hasFlag(verbose_flag))
		fprintf(outputStream,PR_TEXT_EXPORT_BMP_WARN);
	if (result==COMPRESS_RESULT_FATAL) return 0;
fld("ZA2");

	/* Write bitmap */
	mWriteBitMap(image,vFileext,optionflag,backupExtension);
fld("ZA3");

	/* free bitmap */
	free(image.pix);
	return 1;
}


int mFormatImportBmp(unsigned char* data, tResource *res) {
	int size;
	tImage img;
	unsigned char* aux;

	if (!mReadBitMap(&img,data,res->size)) return 0;
	mCompressGraphic(&aux,&img,&size);
	mWriteFileInDatFile(aux,size);
	free(img.pix);
	free(aux);
	res->size=(unsigned short)size;

	return 1;
}

int mWriteBitMap(tImage img,const char* vFile,int optionflag,const char* backupExtension) {

	/* declare variables */
	int a;
	int x;
	unsigned char* b;
	unsigned char* g;
	unsigned char* r;
	unsigned short int bits;
	unsigned short int planes=1;
	unsigned long int colours;
	unsigned long int extra=1000;
	unsigned long int filesize;
	unsigned long int headerSize;
	unsigned long int height;
	unsigned long int offset;
	unsigned long int width;
	const unsigned long int zero=0;
	char lineSerialization;
	FILE* bitmap;
fld("alpha");
	/* open file */
	if (!writeOpen(vFile,&bitmap,optionflag)) return 0;
fld("beta");
printf("a) %d+%d*%d,%d\n",img.pix,img.height,img.widthInBytes,img.widthInBytes);

	/* initialize variables */
	width=img.width;
	height=img.height;
	bits=1<<getCarry(img.type);
	colours=1<<bits;
	headerSize=40;
	offset=54+(colours<<2);
	r=(g=(b=(unsigned char*)&extra)+1)+1;
	lineSerialization=(-img.widthInBytes)&3;
	filesize=offset+(img.widthInBytes+lineSerialization)*height;
printf("b) %d+%d*%d,%d\n",img.pix,img.height,img.widthInBytes,img.widthInBytes);
fld("gamma");

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
fld("delta");

	/* Write ColorTable */
	if (colours==2) {
		/* Black & White */
		fwrite(FORMATS_BMP_PALETTE_BW,8,1,bitmap);    /* 24-bit palette: #000000, #FFFFFF */
	} else {
		/* Colours */
		for (a=0,x=0;a<colours;a++) {
			*r=(unsigned char)(img.pal[x++]<<2);  /* Red   */
			*g=(unsigned char)(img.pal[x++]<<2);  /* Green */
			*b=(unsigned char)(img.pal[x++]<<2);  /* Blue  */
			fwritelong (&extra,bitmap);    /* 24-bit Color value */
		}
	}
fld("epsilon");

	/* Write data */
	while (img.height--) {
fld("e1");
printf("c) %d+%d*%d,%d\n",img.pix,img.height,img.widthInBytes,img.widthInBytes);
		fwrite(img.pix+img.height*img.widthInBytes,img.widthInBytes,1,bitmap);
fld("e2");
		fwrite(&zero,lineSerialization,1,bitmap);
	}
fld("zeta");

	writeCloseOk(bitmap,optionflag,backupExtension);
fld("etha");
	return 1;
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
	ok=ok&& (	(image->pix=getMemory(height*image->widthInBytes*2)) != NULL	);
	/* if validations==wrong */
	if (!ok) {
		freeAllocation(image->pix);
		return 0; /* this is not a valid bmp file format or memory too low */
	}

	/* Serialize bitmap-->raw array */
	while (height--) memcpy(image->pix+(x++)*image->widthInBytes,data+offset+height*serializedWidth,image->widthInBytes);

	return 1;
}
