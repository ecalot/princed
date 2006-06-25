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
image256.c: Princed Resources : Image Compression Library
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003, 2004, 2005, 2006 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)
  Version: 2.00 (2004-Mar-07)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <string.h>
#include "compress.h"
#include "common.h"
#include "image.h"
#include "memory.h"
#include "disk.h"    /* array2short */
#include "dat.h"
#include "object.h"  /* paletteGet* */

#include "palette.h" /* getColorArrayByColors */

#include "bitmap.h"

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

int pop2decompress(tBinary input, int verify, unsigned char** output,int* outputSize);

/***************************************************************\
|               Main compress and expand graphics               |
\***************************************************************/

/*
 * 	Header info:
 * 	 1 byte  - checksum           char checksum
 * 	 2 bytes - height             short int height
 * 	 2 bytes - width              short int width
 * 	 1 byte  - 00                 (char)0
 * 	 1 byte  - compression type   unsigned char compressionType
 */

/* Expands an array into an image */
int mExpandGraphic256(tBinary input, tImage *image) {
	/*
	 * Reads tBinary input and extracts tImage image
	 * returns an error code or 0 if success
	 */

	int imageSizeInBytes=0;

	image->height=array2short(input.data);
	image->width =array2short(input.data+2);

	if (*(input.data+4)!=1) return PR_RESULT_COMPRESS_RESULT_FATAL; /* Verify format */
	/* if (*(input.data+5)!=0xb4) return PR_RESULT_COMPRESS_RESULT_FATAL; * Verify format */

	image->type=(unsigned char)(*(input.data+5));
	image->widthInBytes=(image->width);
	imageSizeInBytes=image->widthInBytes*image->height;

	/* special format has a special function */
	return pop2decompress(binaryCrop(input,6,0),image->width,&(image->pix),&imageSizeInBytes); /* TODO: use tBinary */
}

/* Compress an image into binary data */
int mCompressGraphic256(tBinary* input, tBinary* output, int ignoreFirstBytes, int w, int h) {

	/* Declare variables */
	unsigned char* compressed     [COMPRESS_WORKING_ALGORITHMS];
	int            compressedSize [COMPRESS_WORKING_ALGORITHMS];
	int            algorithm;
	int            i;
	int            imageSizeInBytes;
	int            max_alg=1;

	/* Initialise variables */
	imageSizeInBytes=input->size; /*=image->widthInBytes*image->height;*/

	/*
	 * Perform all compressions
	 */

	/* COMPRESS_LZG_UD */
	/*	cLevel(7)
			setHigh;
		else
			unsetHigh;*/
		compressed[COMPRESS_LZG_UD]=getMemory(2*imageSizeInBytes+1050);
		compressLzg(
			input->data,imageSizeInBytes,
			compressed[COMPRESS_LZG_UD],&(compressedSize[COMPRESS_LZG_UD])
		);
		max_alg++;
	/*
	 * Process results
	 */

	/* Select the best compression (find minimum) */
	output->size=compressedSize[COMPRESS_RAW];
	algorithm=COMPRESS_RAW;
	for (i=COMPRESS_RLE_LR;i<max_alg;i++) {
		if (output->size>compressedSize[i]) {
			output->size=compressedSize[i];
			algorithm=i;
		}
	}

	/* Copy the best algorithm in the compressed data */
	output->data=getMemory(output->size+ignoreFirstBytes);
	memcpy(output->data+ignoreFirstBytes,compressed[algorithm],output->size);
	output->size+=ignoreFirstBytes;

	/* Free all compression attempts */
	for (i=COMPRESS_RAW;i<max_alg;i++) free(compressed[i]);
	return algorithm;
}

/* TODO: send to its own compression file */
int pop2decompress(tBinary input, int verify, unsigned char** output,int* outputSize) {
	unsigned char* tempOutput;
	tBinary        lineI; /* chunk */
	unsigned char* lineO; /* chunk */
	int            lineSize;
	int            aux,aux2,remaining;
	int            tempOutputSize;
	int            osCheck;

	*output=malloc(*outputSize);
	lineO=*output;
	for (aux=0;aux<*outputSize;aux++) (*output)[aux]=0; /* initialise the array (TODO: only for debug, in fixed images it won't be necessary) */
	*outputSize=0;

	osCheck=array2short(input.data)-6;
	input=binaryCrop(input,2,0);

	/* First layer: expand the LGZ */
	tempOutputSize=osCheck+6;

	remaining=expandLzg(input,&tempOutput,&tempOutputSize);
	/*printf("Call:\n return=%d function input.data size=%d\n internal output size=%d result output size=%d\n",
		remaining,input.size,osCheck,tempOutputSize);*/
	/*if ((osCheck+6)!=tempOutputSize)
		printf(" Special case: more is coming\n");*/

	/* Second layer expand each rle line */
	lineI.data=tempOutput;
	/*printf("RLE loop layer:\n");*/
	do {
		lineI.size=array2short(lineI.data);
		lineI.data+=2;
		if (lineI.size>tempOutputSize) {
			/*printf(" error: lineI.size=%d tempOutputSize=%d\n",lineI.size,tempOutputSize);*/
			return PR_RESULT_COMPRESS_RESULT_WARNING;
		}
		aux2=expandRleV(lineI,lineO,&lineSize);
		/*if (aux2) printf(" error: rle=%d linesize=%d of %d. size=%d r=%d.\n",aux2, lineSize,verify,tempOutputSize,tempOutputSize-lineI.size-2);*/
		lineO+=lineSize;
		*outputSize+=lineSize;
		tempOutputSize-=lineI.size;
		tempOutputSize-=2;
		lineI.data+=lineI.size;
	} while (lineSize==verify && tempOutputSize>0);
	/*printf(" return: linesize=%d verify=%d tempOutputSize=%d\n", lineSize, verify, tempOutputSize);*/
	if (remaining) {
		/*const unsigned char* start=input.data+(input.size+2)-remaining;*/
		tBinary tail;

		tail.data=input.data+(input.size+2-remaining);
		tail.size=remaining;

		/*printf("Remaining tailing data: size=%d first=%02x %02x\n", remaining,start[0],start[1]);*/
		tempOutputSize=0;
		expandLzg(tail,&tempOutput,&tempOutputSize); /* TODO: check error output */

		lineI.data=tempOutput;

		while (lineSize==verify && tempOutputSize>0) {
			lineI.size=array2short(lineI.data);
			lineI.data+=2;
			if (lineI.size>tempOutputSize) {
				/*printf(" error: lineI.size=%d tempOutputSize=%d\n",lineI.size,tempOutputSize);*/
				return PR_RESULT_COMPRESS_RESULT_WARNING;
			}
			aux2=expandRleV(lineI,lineO,&lineSize);
			/*if (aux2) printf(" error: rle=%d linesize=%d of %d. size=%d r=%d.\n",aux2, lineSize,verify,tempOutputSize,tempOutputSize-lineI.size-2);*/
			lineO+=lineSize;
			*outputSize+=lineSize;
			tempOutputSize-=lineI.size;
			tempOutputSize-=2;
			lineI.data+=lineI.size;
		}
/*
		lineI.size=array2short(lineI.data);
		lineI.data+=2;
		if (lineI.size>tempOutputSize) printf(" error: lineI.size=%d tempOutputSize=%d\n",lineI.size,tempOutputSize);
		aux2= expandRleC(lineI.data,tempOutputSize,lineO,&lineSize,1000);
		if (aux2) printf(" error: rle=%d linesize=%d of %d. size=%d r=%d.\n",aux2, lineSize,verify,tempOutputSize,tempOutputSize-lineI.size-2);
*/
	}

/*
	printf("rle=%d\n", expandRleC(tempOutput,tempOutputSize,output,outputSize,verify));

	printf("lzg=%d\n", os3=expandLzg(input.data+8+is-8-os3+2,os3-2,&output,&os));
	osCheck=input.data[7+is-8-os3+2]<<8|input.data[6+is-8-os3+2];

	printf("rle=%d osCheck=%d\n", expandRleC(output,os,&output2,&os2,verify), osCheck);
	fwrite(output2,os2,1,out);

	fclose(out);

	printf("os=%d oscheck=%d\n",os,osCheck);
*/

	return PR_RESULT_SUCCESS;
}

void* objectImage256Create(tBinary cont, int *error) { /* use get like main.c */

	/*
	 * This function will expand the data into an image structure,
	 * then the bitmap structure will be saved to disk
	 *
	 * Note: The old structure is passed by parameters in order to
	 *       keep the right palette.
	 */

	tImage* image;
	int i;
	int max=0;

	/*int bits;*/
	image=(tImage*)malloc(sizeof(tImage));

	/* Expand graphic and check results */
	*error=mExpandGraphic256(cont,image);

	if (*error==PR_RESULT_COMPRESS_RESULT_FATAL) {
		free(image);
		return NULL;
	}

	for (i=0;i<image->height*image->widthInBytes;i++) {
		if (image->pix[i]>max) max=image->pix[i];
	}
	printf("max pixel in this image is %d\n",max);
	image->colorCount=max;
	image->bits=getCarry(image->type);

	return (void*)image;
}

tColor* objPalette_256() {
	static tColor c[256]={{0,0,0},{0,0,0}};
	int i;
	if (!c[1].r)
		for (i=0;i<256;i++) { /* The grayscale */
			c[i].r=i;
			c[i].g=i;
			c[i].b=i;
		}
	return c;
}

int objectImage256Write(void* img,const char* file,int optionflag,const char* backupExtension) {
	tImage* i=img;
	int bits;
	int colors;
	tColor* colorArray;

	if (i->pal.type!=eResTypeNone) {
		bits=objectPaletteGetBitRate(i->pal);
		colors=i->colorCount;
		colorArray=objectPaletteGetColorArray(i->pal);
	} else {
		bits=8;
		colors=256;
		colorArray=objPalette_256();
	}

	/* Write bitmap */
	return mWriteBmp(file,i->pix,i->width,i->height,bits,colors,colorArray,i->widthInBytes,optionflag,backupExtension);
}

void objectImage256Free(void* img) {
	if (!img) return;
	/* free bitmap */
	free(((tImage*)img)->pix);
	free(img);
}

void* objectImage256Read(const char* file,tObject palette, int *result) {
	int bits;
	tImage* image=(tImage*)malloc(sizeof(tImage));
	tColor* colorArray;
	int colors;

	*result=readBmp(file,&(image->pix),&(image->height),&(image->width),&(image->bits),&colors,&colorArray,&(image->widthInBytes));
	/* check if image was successfully read loaded */
	if (*result!=PR_RESULT_SUCCESS) {
		free(image);
		return NULL;
	}

	/* check the palette information */

	image->pal=palette;
	bits=objectPaletteGetBitRate(image->pal);
	if (bits && bits!=image->bits) { /* bits=0 means all palettes allowed or ignore palette check */
		*result=PR_RESULT_ERR_BMP_BITRATE_DIFFERS;
		free(image->pix);
		free(colorArray);
		free(image);
		return NULL;
	}

	/* TODO: palette content checks */

	free(colorArray);
	return (void*)image;
}

/* TODO: generate image->type in objectImageSet */

/*int mFormatImportBmp(tResource *res) { --> objectImageSet */
int objectImage256Set(void* o,tResource* res) {
	tImage* img=o;
	tBinary decompressed,compressed;

	decompressed.data=img->pix;
	decompressed.size=img->widthInBytes*img->height;

	mCompressGraphic256(&decompressed,&compressed,6,img->widthInBytes,img->height);

	/*
	 * Write header
	 */

	/* (16 bits)height (Intel short int format) */
	compressed.data[0]=img->height; /* TODO: use shorttoarray */
	compressed.data[1]=img->height>>8;
	/* (16 bits)width (Intel short int format) */
	compressed.data[2]=img->width;
	compressed.data[3]=img->width>>8;
	/* (8 bits)00000000+(4 bits)palette type+(4 bits)algorithm */
	compressed.data[4]=0;
	compressed.data[5]=0xf3;

	res->content=compressed;
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}
