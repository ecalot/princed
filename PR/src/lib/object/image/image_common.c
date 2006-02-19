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
|                   Compression Level Manager                   |
\***************************************************************/

int compressionLevel=3;
int compressionHigher;

void setCompressionLevel(int cl) {
	compressionLevel=cl;
}

/***************************************************************\
|                        Image transpose                        |
\***************************************************************/

/* Determines where the transposed byte must be saved */
int cmp_transpose(int x,int w,int h) {
	return ((x%h)*(w))+(int)(x/h);
}

void cmp_transposeImage(tImage* image,int size) {
	unsigned char* outputaux=getMemory(size);
	int cursor;

	for (cursor=0;cursor<size;cursor++)
		outputaux[cmp_transpose(cursor,image->widthInBytes,image->height)]=image->pix[cursor];
	free(image->pix);
	image->pix=outputaux;
}

void cmp_antiTransposeImage(tImage* image,int size) {
	unsigned char* outputaux=getMemory(size);
	int cursor;

	for (cursor=0;cursor<size;cursor++)
		outputaux[cursor]=image->pix[cmp_transpose(cursor,image->widthInBytes,image->height)];
	free(image->pix);
	image->pix=outputaux;
}

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
int mExpandGraphic(const unsigned char* data,tImage *image, int dataSizeInBytes) {
	/*
	 * Reads data and extracts tImage
	 * returns the next image address or -1 in case of error
	 */

	int imageSizeInBytes=0;
	int result;

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
	if (image->type==0xf3)
		return pop2decompress(data,dataSizeInBytes-6,image->width,&(image->pix),&imageSizeInBytes);

#define checkSize if (imageSizeInBytes!=(image->widthInBytes*image->height))\
	return COMPRESS_RESULT_FATAL
#define checkResult if (result==COMPRESS_RESULT_FATAL)\
	return COMPRESS_RESULT_FATAL

	switch (getAlgor(image->type)) {
		case COMPRESS_RAW: /* No Compression Algorithm */
			if ((image->pix=getMemory(dataSizeInBytes))==NULL) return COMPRESS_RESULT_FATAL;
			memcpy(image->pix,data,dataSizeInBytes);
			imageSizeInBytes=image->widthInBytes*image->height;
			result=COMPRESS_RESULT_SUCCESS;
			break;
		case COMPRESS_RLE_LR: /* RLE Left to Right Compression Algorithm */
			result=expandRle(data,dataSizeInBytes,&(image->pix),&imageSizeInBytes);
			checkSize;
			break;
		case COMPRESS_RLE_UD: /* RLE Up to Down Compression Algorithm */
			result=expandRle(data,dataSizeInBytes,&(image->pix),&imageSizeInBytes);
			checkResult;
			checkSize;
			cmp_transposeImage(image,imageSizeInBytes);
			break;
		case COMPRESS_LZG_LR: /* LZ Groody Left to Right Compression Algorithm */
			result=expandLzg(data,dataSizeInBytes,&(image->pix),&imageSizeInBytes);
			checkSize;
			break;
		case COMPRESS_LZG_UD: /* LZ Groody Up to Down Compression Algorithm */
			result=expandLzg(data,dataSizeInBytes,&(image->pix),&imageSizeInBytes);
			checkResult;
			checkSize;
			cmp_transposeImage(image,imageSizeInBytes);
			break;
		default:
			result=COMPRESS_RESULT_FATAL; /* unknown algorithm */
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
	int            max_alg=1;

	/* Initialize variables */
	imageSizeInBytes=image->widthInBytes*image->height;

	/*
	 * Perform all compressions
	 */

	/* Forward compression */

	/* COMPRESS_RAW
	 * The allocation size is the image size.
	 * The algorithm is hard-coded.
	 * There is no need to code a transposed version because
	 * we have no compression to improve.
	 */
	compressed[COMPRESS_RAW]=getMemory(imageSizeInBytes);
	compressedSize[COMPRESS_RAW]=imageSizeInBytes;
	memcpy(compressed[COMPRESS_RAW],image->pix,imageSizeInBytes);

	/* COMPRESS_RLE_LR
	 * If all the uncompressed data has a big entropy, there
	 * will be a control byte for a block of 127 bytes.
	 * The allocation size has a maximum value of the image
	 * size plus a byte each 127 bytes.
	 * This is accoted by 2*n+50
	 */
	cLevel(2) {
		compressed[COMPRESS_RLE_LR]=getMemory((2*imageSizeInBytes+50));
		compressRle(
			image->pix,imageSizeInBytes,
			compressed[COMPRESS_RLE_LR],&(compressedSize[COMPRESS_RLE_LR])
		);
		max_alg++;
	}
	/* COMPRESS_LZG_LR
	 * If all the uncompressed data has a big entropy, there
	 * will be a maskbyte for a block of 8 bytes.
	 * The allocation size has a maximum value of the image
	 * size plus a byte in 8.
	 * Additionally, this compressor needs 1024 bytes extra
	 * allocated.
	 * This is accoted by 2*n+1050
	 */
	cLevel(4) {
		cLevel(6)
			setHigh;
		else
			unsetHigh;
		compressed[COMPRESS_LZG_LR]=getMemory((2*imageSizeInBytes+1050));
		compressLzg(
			image->pix,imageSizeInBytes,
			compressed[COMPRESS_LZG_LR],&(compressedSize[COMPRESS_LZG_LR])
		);
		max_alg++;
	}

	/* Transposed compression
	 * Transposition is used to test the same compression
	 * algorithms with other input in order to get a better
	 * compression.
	 * The following algorithms will be the same as above, but
	 * using the image matrix transposed.
	 */
	cLevel(3)
		cmp_antiTransposeImage(image,imageSizeInBytes);

	/* COMPRESS_RLE_UD */
	cLevel(3) {
		compressed[COMPRESS_RLE_UD]=getMemory(2*imageSizeInBytes+50);
		compressRle(
			image->pix,imageSizeInBytes,
			compressed[COMPRESS_RLE_UD],&(compressedSize[COMPRESS_RLE_UD])
		);
		max_alg++;
	}

	/* COMPRESS_LZG_UD */
	cLevel(5) {
		cLevel(7)
			setHigh;
		else
			unsetHigh;
		compressed[COMPRESS_LZG_UD]=getMemory(2*imageSizeInBytes+1050);
		compressLzg(
			image->pix,imageSizeInBytes,
			compressed[COMPRESS_LZG_UD],&(compressedSize[COMPRESS_LZG_UD])
		);
		max_alg++;
	}
	/*
	 * Process results
	 */

	/* Select the best compression (find minimum) */
	*dataSizeInBytes=compressedSize[COMPRESS_RAW];
	algorithm=COMPRESS_RAW;
	for (i=COMPRESS_RLE_LR;i<max_alg;i++) {
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
	 * Write header
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
	for (i=COMPRESS_RAW;i<max_alg;i++) free(compressed[i]);
	return 1; /* true */
}

int pop2decompress(const unsigned char* input, int inputSize, int verify, unsigned char** output,int* outputSize) { 
	/* This function is in an experimental state and hasn't yet been linked to the program */
	unsigned char* tempOutput;

	int tempOutputSize;
	int osCheck;

	osCheck=array2short(input)-6;
	input+=2;

	/*os=osCheck;*/
	tempOutputSize=0;
	printf("lzg=%d is=%d osc=%d\n", expandLzg(input,inputSize-2,&tempOutput,&tempOutputSize),inputSize,osCheck);

	printf("rle=%d\n", expandRleC(tempOutput,tempOutputSize,output,outputSize,verify));
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

void* objImageCreate(unsigned char* data, int size, tObject palette, int *error) { /* use get like main.c */

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
	*error=mExpandGraphic(data,image,size);
/*	if ((result==COMPRESS_RESULT_WARNING)&&hasFlag(verbose_flag))
		fprintf(outputStream,PR_TEXT_EXPORT_BMP_WARN);*/
	if (*error==COMPRESS_RESULT_FATAL) {
		free(image);
		return NULL;
	}

	image->pal=palette;
	bits=paletteGetBits(image->pal);
	if (bits!=getCarry(image->type)) printf("error, palette mismatch\n");
	
	return (void*)image;
}

int objImageWrite(void* img,const char* file,int optionflag,const char* backupExtension) {
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
		colorArray=paletteGetColorArrayForColors(colors);
	}
	
	/* Write bitmap */
	return mWriteBmp(file,i->pix,i->width,i->height,bits,colors,colorArray,i->widthInBytes,optionflag,backupExtension);
}

void objImageFree(void* img) {
	if (!img) return;
	/* free bitmap */
	free(((tImage*)img)->pix);
	free(img);
}

int mFormatImportBmp(tResource *res) {
	tImage img;

	if (!mReadBitMap(&img,res->data,res->size)) return 0; /* false */
	free(res->data);
	mCompressGraphic(&(res->data),&img,(int*)&(res->size));
	mWriteFileInDatFile(res);
	free(img.pix);

	return 1; /* true */
}

