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
compress.h: Princed Resources : Image compression headers
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _COMPRESS_H_
#define _COMPRESS_H_

typedef struct { /* TODO: move to image.h */
	unsigned char r;
	unsigned char g;
	unsigned char b;
}tColor;

typedef struct {
	tColor p8[256];
	tColor p4[16];
	tColor p1[2];
}tPalette;

#define SIZE_OF_PALETTE 3*16 /* TODO: erase this line */

#define COMPRESS_RAW    0x00
#define COMPRESS_RLE_LR 0x01
#define COMPRESS_RLE_UD 0x02
#define COMPRESS_LZG_LR 0x03
#define COMPRESS_LZG_UD 0x04

#define COMPRESS_WORKING_ALGORITHMS 5

#define COMPRESS_RESULT_FATAL   -2
#define COMPRESS_RESULT_WARNING -1
#define COMPRESS_RESULT_SUCCESS 0

typedef struct { /* TODO: move to image.h */
	int width;
	int widthInBytes;
	int height;
	unsigned char* pix;
	tPalette pal;
	unsigned char type; /* XY where X is F for 8 bits, B for 4 bits and 0 for 1 bit; Y=algorithm */
}tImage;

/* Prototypes */

int mCompressGraphic(unsigned char** a,tImage* i, int* size);
int mExpandGraphic  (const unsigned char* array,tImage *image, int size);

#define cLevel(a) if (compressionLevel>=a)
#define cHigh     if (compressionHigher)
#define setHigh   compressionHigher=1
#define unsetHigh compressionHigher=0

#define getCarry(a) ((((a)>>4)&7)+1)
#define getAlgor(a) a&0x4F /* 0x0F */

/* private declarations */

/***************************************************************\
|                Internal compression prototypes                |
\***************************************************************/

/* compress and sets the bytes */
void compressLzg(const unsigned char* input, int inputSize,
                 unsigned char* output, int *outputSize);
void compressRle(const unsigned char* input, int inputSize,
                 unsigned char* output, int *outputSize);

/* decompress and allocates output */
int expandLzg(const unsigned char* input, int inputSize,
               unsigned char** output, int *outputSize);
int expandRle(const unsigned char* input, int inputSize,
               unsigned char** output, int *outputSize);
int expandRleC(const unsigned char* input, int inputSize,
               unsigned char** output, int *outputSize,int verif);

#endif
