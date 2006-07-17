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
image16.c: Princed Resources : Image Compression Library
¯¯¯¯¯¯¯¯¯
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
#include "common.h"
#include "compress.h"
#include "disk.h" /* array2short */
#include "image.h" /* tImage */
#include "memory.h"

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

void cmp_antiTransposeImage(tBinary* b, int widthInBytes, int height) {
	unsigned char* outputaux=getMemory(b->size);
	int cursor;

	for (cursor=0;cursor<b->size;cursor++)
		outputaux[cursor]=b->data[cmp_transpose(cursor,widthInBytes,height)];
	free(b->data);
	b->data=outputaux;
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
int mExpandGraphic(tBinary input, tImage *image) {
	/*
	 * Reads input and extracts tImage
	 * returns the next image address or -1 in case of error
	 */

	int imageSizeInBytes=0;
	int result;

	image->height=array2short(input.data);
	input.data+=2;
	image->width =array2short(input.data);
	input.data+=2;

	if (*(input.data++)>1) return PR_RESULT_F_COMPRESS_RESULT_FATAL; /* Verify format */
	image->type=(unsigned char)(*(input.data++));
	input.size-=6;
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
		return PR_RESULT_F_COMPRESS_RESULT_FATAL;
	}

#define checkSize if (imageSizeInBytes!=(image->widthInBytes*image->height))\
	return PR_RESULT_F_COMPRESS_RESULT_FATAL
#define checkResult if (result==PR_RESULT_F_COMPRESS_RESULT_FATAL)\
	return PR_RESULT_F_COMPRESS_RESULT_FATAL

	switch (getAlgor(image->type)) {
		case COMPRESS_RAW: /* No Compression Algorithm */
			if ((image->pix=getMemory(input.size))==NULL) return PR_RESULT_F_COMPRESS_RESULT_FATAL;
			memcpy(image->pix,input.data,input.size);
			imageSizeInBytes=image->widthInBytes*image->height;
			if (imageSizeInBytes>input.size) return PR_RESULT_F_COMPRESS_RESULT_FATAL; /* TODO: check if raw images still work */
			result=PR_RESULT_SUCCESS;
			break;
		case COMPRESS_RLE_LR: /* RLE Left to Right Compression Algorithm */
			result=expandRle(input,&(image->pix),&imageSizeInBytes);
			checkSize;
			break;
		case COMPRESS_RLE_UD: /* RLE Up to Down Compression Algorithm */
			result=expandRle(input,&(image->pix),&imageSizeInBytes);
			checkResult;
			checkSize;
			cmp_transposeImage(image,imageSizeInBytes);
			break;
		case COMPRESS_LZG_LR: /* LZ Groody Left to Right Compression Algorithm */
			result=expandLzg(input,&(image->pix),&imageSizeInBytes);
			checkSize;
			break;
		case COMPRESS_LZG_UD: /* LZ Groody Up to Down Compression Algorithm */
			result=expandLzg(input,&(image->pix),&imageSizeInBytes);
			checkResult;
			checkSize;
			cmp_transposeImage(image,imageSizeInBytes);
			break;
		default:
			result=PR_RESULT_F_COMPRESS_RESULT_FATAL; /* unknown algorithm */
			break;
	}
	return result; /* Ok */
}

int mCompressGraphic(tBinary* input, tBinary* output, int ignoreFirstBytes, int w, int h) {

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

	/* Forward compression */

	/* COMPRESS_RAW
	 * The allocation size is the image size.
	 * The algorithm is hard-coded.
	 * There is no need to code a transposed version because
	 * we have no compression to improve.
	 */
	compressed[COMPRESS_RAW]=getMemory(imageSizeInBytes);
	compressedSize[COMPRESS_RAW]=imageSizeInBytes;
	memcpy(compressed[COMPRESS_RAW],input->data,imageSizeInBytes);

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
			input->data,imageSizeInBytes,
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
			input->data,imageSizeInBytes,
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
		cmp_antiTransposeImage(input,w,h);

	/* COMPRESS_RLE_UD */
	cLevel(3) {
		compressed[COMPRESS_RLE_UD]=getMemory(2*imageSizeInBytes+50);
		compressRle(
			input->data,imageSizeInBytes,
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
			input->data,imageSizeInBytes,
			compressed[COMPRESS_LZG_UD],&(compressedSize[COMPRESS_LZG_UD])
		);
		max_alg++;
	}
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

int objectImageGetColorCount(void* img) {
	tImage* i=img;
	return i->colorCount;
}

void applyPalette(tObject image, tObject palette) {
	tImage* i=image.obj;
	i->pal=palette;
}
