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
compress.c: Princed Resources : Image Compression Library
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003, 2004 Princed Development Team
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
#include "memory.h"
#include "pr.h"
#include "disk.h" /* array2short */

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/***************************************************************\
|                Internal compression prototypes                |
\***************************************************************/

void compressLzg(const unsigned char* input, int inputSize, 
                 unsigned char* output, int *outputSize);
void compressRle(const unsigned char* input, int inputSize, 
                 unsigned char* output, int *outputSize);
int expandLzg(const unsigned char* input, int inputSize, 
               unsigned char* output, int *outputSize);
int expandRle(const unsigned char* input, int inputSize, 
               unsigned char* output, int *outputSize);

/***************************************************************\
|                        Image transpose                        |
\***************************************************************/

/* Determines where the transposed byte must be saved */
int transpose(int x,int w,int h) {
	return ((x%h)*(w))+(int)(x/h);
}

void transposeImage(tImage* image,int size) {
	unsigned char* outputaux=getMemory(size);
	int cursor=0;

	while (cursor<size) {outputaux[transpose(cursor,image->widthInBytes,image->height)]=image->pix[cursor];cursor++;}
	free(image->pix);
	image->pix=outputaux;
}

void antiTransposeImage(tImage* image,int size) {
	unsigned char* outputaux=getMemory(size);
	int cursor=0;

	while (cursor<size) {outputaux[cursor]=image->pix[transpose(cursor,image->widthInBytes,image->height)];cursor++;}
	free(image->pix);
	image->pix=outputaux;
}

/***************************************************************\
|               Main compress and expand graphics               |
\***************************************************************/

/*
		Header info:
		 1 byte  - checksum           char checksum
		 2 bytes - height             short int height
		 2 bytes - width              short int width
		 1 byte  - 00                 (char)0
		 1 byte  - compression type   unsigned char compressionType
*/

/* Expands an array into an image */
int mExpandGraphic(const unsigned char* data,tImage *image, int dataSizeInBytes) {
	/*
		Reads data and extracts tImage
		returns the next image address or -1 in case of error
	*/

	int imageSizeInBytes;
	int result;

	data++;
	image->height=array2short(data);
	data+=2;
	image->width =array2short(data);
	data+=2;

	if (*(data++)) return COMPRESS_RESULT_FATAL; /* Verify format */
	image->type=(unsigned char)(*(data++));
	dataSizeInBytes-=7;
	if (image->type&0xB0) {
		image->widthInBytes=(image->width+1)/2;
	} else {
		image->widthInBytes=(image->width+7)/8;
	}
	imageSizeInBytes=image->widthInBytes*image->height;

	switch (getAlgor(image->type)) {
		case COMPRESS_RAW: /* No Compression Algorithm */
			if ((image->pix=getMemory(imageSizeInBytes))==NULL) return COMPRESS_RESULT_FATAL;
			memcpy(image->pix,data,imageSizeInBytes);
			result=COMPRESS_RESULT_SUCCESS;
			break;
		case COMPRESS_RLE_LR: /* RLE Left to Right Compression Algorithm */
			result=expandRle(data,dataSizeInBytes,image,imageSizeInBytes);
			break;
		case COMPRESS_RLE_UD: /* RLE Up to Down Compression Algorithm */
			result=expandRle(data,dataSizeInBytes,image,imageSizeInBytes);
			if (result==COMPRESS_RESULT_FATAL) return COMPRESS_RESULT_FATAL;
			transposeImage(image,imageSizeInBytes);
			break;
		case COMPRESS_LZG_LR: /* LZ Groody Left to Right Compression Algorithm */
			result=expandLzg(data,dataSizeInBytes,image,imageSizeInBytes);
			break;
		case COMPRESS_LZG_UD: /* LZ Groody Up to Down Compression Algorithm */
			result=expandLzg(data,dataSizeInBytes,image,imageSizeInBytes);
			if (result==COMPRESS_RESULT_FATAL) return COMPRESS_RESULT_FATAL;
			transposeImage(image,imageSizeInBytes);
			break;
		default:
			result=COMPRESS_RESULT_FATAL;
			break;
	}
	return result; /* Ok */
}

/* Compress an image into binary data */
int mCompressGraphic(unsigned char* *data,tImage* image, int* dataSizeInBytes) {
	/* Declare variables */
	unsigned char* compressed     [COMPRESS_WORKING_ALGORITHMS];
	int            compressedSize [COMPRESS_WORKING_ALGORITHMS];
	int            algorithm;
	int            i;
	int            imageSizeInBytes;

	/* Initialize variables */
	imageSizeInBytes=image->widthInBytes*image->height;
	for (i=0;i<COMPRESS_WORKING_ALGORITHMS;i++) compressedSize[i]=imageSizeInBytes;

	/*
		Perform all compressions
	*/

	/* COMPRESS_RAW */
	compressed[COMPRESS_RAW]=getMemory(compressedSize[COMPRESS_RAW]);
	memcpy(compressed[COMPRESS_RAW],image->pix,compressedSize[COMPRESS_RAW]);

	/* COMPRESS_RLE_LR */
	compressed[COMPRESS_RLE_LR]=getMemory((10*imageSizeInBytes+50)); /* This will reserve 10*(image size)+50 bytes, to allocate the compressed file */
	compressRle(compressed[COMPRESS_RLE_LR],image,&(compressedSize[COMPRESS_RLE_LR]));

	/* COMPRESS_RLE_UD */
	compressed[COMPRESS_RLE_UD]=getMemory(10*imageSizeInBytes+50); /* This will reserve 10*(image size)+50 bytes, to allocate the compressed file */
	antiTransposeImage(image,imageSizeInBytes);
	compressRle(compressed[COMPRESS_RLE_UD],image,&(compressedSize[COMPRESS_RLE_UD]));

	/*
		Process results
	*/

	/* Select the best compression (find minimum) */
	*dataSizeInBytes=compressedSize[COMPRESS_RAW];
	algorithm=COMPRESS_RAW;
	for (i=COMPRESS_RLE_LR;i<COMPRESS_WORKING_ALGORITHMS;i++) {
		if ((*dataSizeInBytes)>compressedSize[i]) {
			(*dataSizeInBytes)=compressedSize[i];
			algorithm=i;
		}
	}

	/* Copy the best algorithm in the compressed data */
	*data=getMemory(*dataSizeInBytes+6);
	memcpy(*data+6,compressed[algorithm],*dataSizeInBytes);
	(*dataSizeInBytes)+=6;

	/*
		Write header
	*/

	/* (16 bits)height (Intel short int format) */
	(*data)[0]=image->height;
	(*data)[1]=image->height>>8;
	/* (16 bits)width (Intel short int format) */
	(*data)[2]=image->width;
	(*data)[3]=image->width>>8;
	/* (8 bits)00000000+(4 bits)palette type+(4 bits)algorithm */
	(*data)[4]=0;
	(*data)[5]=image->type|algorithm;

	/* Free all compression attempts */
	for (i=COMPRESS_RAW;i<COMPRESS_WORKING_ALGORITHMS;i++) free(compressed[i]);
	return 1;
}

