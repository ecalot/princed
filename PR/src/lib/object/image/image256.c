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
image.c: Princed Resources : Image Compression Library
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
#include "disk.h" /* array2short */
#include "dat.h"
#include "object.h" /* paletteGet* */

#include "palette.h" /* getColorArrayByColors */

#include "bmp.h"

/* Compression level explanation:
 *
 * Definitions:
 *  no compression is called RAW
 *  there are 2 algorithms types: RLE and LZG
 *  we can use the modifier: not transposed and transposed (t)
 *  we can use the LZG modifier: higher (checks more extensively the LZG window
 *   without ignoring less probable patterns) (+)
 *
 *  So the possible compression algorithm variants are:
 *   RAW, RLE, RLEt, LZG, LZGt, LZG+, LZGt+
 *
 *  It is known that LZG+ always compresses better or equal than LZG
 *
 * Depending on the compression level, the compressor will compress with
 * all the algorithms specified and keep only the smaller result using
 * the following table
 *
 * Level  Algorithms
 *   1    RAW
 *   2    RAW, RLE
 *   3    RAW, RLE, RLEt
 *   4    RAW, RLE, RLEt, LZG
 *   5    RAW, RLE, RLEt, LZG, LZGt
 *   6    RAW, RLE, RLEt, LZG+, LZGt
 *   7    RAW, RLE, RLEt, LZG+, LZGt+
 *
 * The default level used in PR will be 3.
 *
 * In images with big entropy that generates DAT files bigger than 64kb, using
 * a better compression is a must. The POP1 DAT file format has this limitation
 * and the only way to get through with it is improving the compression.
 *
 * For testing DAT files that are not for distribution compression 3 is highly
 * recommended because is much faster and you perform compressions more often.
 *
 * When you release a DAT file a compression level 7 is the best you can use.
 * You'll have to wait some time to get the importing, but the decompression
 * is as faster as the decompression in other levels. The game supports it and
 * decompresses the files very fast. Another advantage is that it is better to
 * distribute smaller DAT files.
 *
 */

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

int pop2decompress(const unsigned char* input, int inputSize, int verify, unsigned char** output,int* outputSize); 

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
int mExpandGraphic256(const unsigned char* data,tImage *image, int dataSizeInBytes) {
	/*
	 * Reads data and extracts tImage
	 * returns the next image address or -1 in case of error
	 */

	int imageSizeInBytes=0;

	image->height=array2short(data);
	data+=2;
	image->width =array2short(data);
	data+=2;

	if (*(data++)>1) return COMPRESS_RESULT_FATAL; /* Verify format */
	image->type=(unsigned char)(*(data++));
	dataSizeInBytes-=6;
	switch (((image->type>>4)&7)+1) {
	case 8:
		image->widthInBytes=(image->width);
		break;
	case 4: 
		image->widthInBytes=(image->width+1)/2;
		break;
	case 1:
		image->widthInBytes=(image->width+7)/8;
		break;
	default:
		return COMPRESS_RESULT_FATAL;
	}

	/* special format has a special function */
	return pop2decompress(data,dataSizeInBytes-6,image->width,&(image->pix),&imageSizeInBytes);
}

/* Compress an image into binary data */
int mCompressGraphic256(tBinary* input, tBinary* output, int ignoreFirstBytes, int w, int h) {
/*								unsigned char* *data,tImage* image, int* dataSizeInBytes, int ignoreFirstBytes) {*/
	/* Declare variables */
	unsigned char* compressed     [COMPRESS_WORKING_ALGORITHMS];
	int            compressedSize [COMPRESS_WORKING_ALGORITHMS];
	int            algorithm;
	int            i;
	int            imageSizeInBytes;
	int            max_alg=1;

	/* Initialize variables */
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

int pop2decompress(const unsigned char* input, int inputSize, int verify, unsigned char** output,int* outputSize) { 
	/* This function is in an experimental state and hasn't yet been linked to the program */
	unsigned char* tempOutput;
	unsigned char* lineI; /* chunk */
	unsigned char* lineO; /* chunk */
	int            lineSize;
	int aux;

	int tempOutputSize;
	int osCheck;

	*output=malloc(40000);
	lineO=*output;
	*outputSize=0;

	osCheck=array2short(input)-6;
	input+=2;

	/*os=osCheck;*/
	/* First layer: expand the lgz */
	/*tempOutputSize=0;*/
	tempOutputSize=osCheck;
	printf("lzg=%d is=%d osc=%d\n", expandLzg(input,inputSize-2,&tempOutput,&tempOutputSize),inputSize,osCheck);

	/* Second layer expand each rle line */
	lineI=tempOutput;
	do {
		aux=array2short(lineI);
		lineI+=2;
		printf("rle=%d\n", expandRleC(lineI,aux,lineO,&lineSize,1000));
		printf("linesize=%d of %d. size=%d r=%d.\n",lineSize,verify,tempOutputSize,tempOutputSize-aux-2);
		lineO+=lineSize;
		outputSize+=lineSize;
		tempOutputSize-=aux;
		tempOutputSize-=2;
		lineI+=aux;
	} while (lineSize==verify && tempOutputSize>0);
	

/*	printf("rle=%d\n", expandRleC(tempOutput,tempOutputSize,output,outputSize,verify));*/
/*
	printf("lzg=%d\n", os3=expandLzg(input+8+is-8-os3+2,os3-2,&output,&os));
	osCheck=input[7+is-8-os3+2]<<8|input[6+is-8-os3+2];

	printf("rle=%d osCheck=%d\n", expandRleC(output,os,&output2,&os2,verify), osCheck);
	fwrite(output2,os2,1,out);

	fclose(out);

	printf("os=%d oscheck=%d\n",os,osCheck);*/
	return COMPRESS_RESULT_SUCCESS;
}

extern FILE* outputStream;

void* objImage256Create(tBinary cont, tObject palette, int *error) { /* use get like main.c */

	/*
	 * This function will expand the data into an image structure,
	 * then the bitmap structure will be saved to disk
	 *
	 * Note: The old structure is passed by parameters in order to
	 *       keep the right palette.
	 */

	tImage* image;
	int bits;
	image=(tImage*)malloc(sizeof(tImage));

	/* Expand graphic and check results */
	*error=mExpandGraphic256(cont.data,image,cont.size); /* TODO: pass tBinary */
/*	if ((result==COMPRESS_RESULT_WARNING)&&hasFlag(verbose_flag))
		fprintf(outputStream,PR_TEXT_EXPORT_BMP_WARN);*/
	if (*error==COMPRESS_RESULT_FATAL) {
		free(image);
		return NULL;
	}

	image->pal=palette;
	bits=paletteGetBits(image->pal);
	if (bits && bits!=getCarry(image->type)) printf("error, palette mismatch (pal=%d bits=%d)\n",bits,getCarry(image->type));
	image->bits=getCarry(image->type);
	
	return (void*)image;
}

tColor* objPalette_256() {
	static tColor c[256]={{0,0,0},{0,0,0}};
	int i;
	if (!c[2].r)
		for (i=0;i<256;i++) { /* The greyscale */
			c[i].r=i;
			c[i].g=i;
			c[i].b=i;
		}
	return c;
}

int objImage256Write(void* img,const char* file,int optionflag,const char* backupExtension) {
	tImage* i=img;
	int bits;
	int colors;
	tColor* colorArray;
	
	if (i->pal.type!=eResTypeNone) {
		bits=paletteGetBits(i->pal);
		colors=paletteGetColors(i->pal);
		colorArray=paletteGetColorArray(i->pal);
	} else {
		bits=getCarry(i->type);
		colors=1<<bits;
		colorArray=objPalette_256();
	}
	
	/* Write bitmap */
	return mWriteBmp(file,i->pix,i->width,i->height,bits,colors,colorArray,i->widthInBytes,optionflag,backupExtension);
}

void objImage256Free(void* img) {
	if (!img) return;
	/* free bitmap */
	free(((tImage*)img)->pix);
	free(img);
}

void* objImage256Read(const char* file,tObject palette, int *result) {
	int bits;
	tImage* image=(tImage*)malloc(sizeof(tImage));
	tColor* colorArray;
	int colors;

	*result=readBmp(file,&(image->pix),&(image->height),&(image->width),&(image->bits),&colors,&colorArray,&(image->widthInBytes));
	/* check if image was succesfully read loaded */
	if (*result!=PR_RESULT_SUCCESS) {
		free(image);
		return NULL;
	}

	/* check the palette information */
	
	image->pal=palette;
	bits=paletteGetBits(image->pal);
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
	/* TODO: generate image->type in objImageSet */

/*int mFormatImportBmp(tResource *res) { --> objImageSet */
int objImage256Set(void* o,tResource* res) {
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

