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
#include "compress.h"
#include "common.h"
#include "image.h"
#include "memory.h"
#include "disk.h" /* array2short */
#include "dat.h"
#include "object.h" /* paletteGet* */

#include "palette.h" /* getColorArrayByColors */

#include "bitmap.h"

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

tColor* objPalette_16() {
	static tColor c[16]=SAMPLE_PAL16;
	return c;
}

void* objectImage16Create(tBinary cont, int *error) { /* use get like main.c */

	/*
	 * This function will expand the data into an image structure,
	 * then the bitmap structure will be saved to disk
	 *
	 * Note: The old structure is passed by parameters in order to
	 *       keep the right palette.
	 */

	tImage* image;
	/*int bits;*/
	image=(tImage*)malloc(sizeof(tImage));

	/* Expand graphic and check results */
	*error=mExpandGraphic(cont,image); /* TODO: pass tBinary */
/*	if ((result==COMPRESS_RESULT_WARNING)&&hasFlag(verbose_flag))
		fprintf(outputStream,PR_TEXT_EXPORT_BMP_WARN);*/
	if (*error==PR_RESULT_COMPRESS_RESULT_FATAL) {
		free(image);
		return NULL;
	}
/*
	image->pal=palette;
	bits=objectPaletteGetBitRate(image->pal);
	if (bits && bits!=getCarry(image->type)) printf("error, palette mismatch (pal=%d bits=%d)\n",bits,getCarry(image->type));
	image->bits=getCarry(image->type);*/

	image->colorCount=2;

	return (void*)image;
}

int objectImage16Write(void* img,const char* file,int optionflag,const char* backupExtension) {
	tImage* i=img;
	tColor* colorArray;

	colorArray=(i->pal.type!=eResTypeNone)?objectPaletteGetColorArray(i->pal):objPalette_16();

	/* Write bitmap */
	return mWriteBmp(file,i->pix,i->width,i->height,4,16,colorArray,i->widthInBytes,optionflag,backupExtension);
}

void objectImageFree(void* img) {
	if (!img) return;
	/* free bitmap */
	free(((tImage*)img)->pix);
	free(img);
}

void* objectImage16Read(const char* file,tObject palette, int *result) {
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
int objectImage16Set(void* o,tResource* res) {
	tImage* img=o;
	tBinary decompressed,compressed;
	int algorithm;

	decompressed.data=img->pix;
	decompressed.size=img->widthInBytes*img->height;

	algorithm=mCompressGraphic(&decompressed,&compressed,6,img->widthInBytes,img->height);

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
	compressed.data[5]=0xb0|algorithm;

	res->content=compressed;
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

