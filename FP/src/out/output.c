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
resources.h: Free Prince : Output Devices Handler
¯¯¯¯¯¯¯¯¯¯¯

 Description: 
 	Platform and device independent functions for handling output
	(text, graphics, sound, music...).

 Copyright 2004 Princed Development Team
  Created: 23 Mar 2004

  Author: vhgh <fhdht.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "resources.h" /* tMemory structure */
#include <stdlib.h> /* malloc */

/* Text Primitives*/
void outputDrawText(const char* text, int x, int y) {}
void outputDrawMessage(const char* text) {}

/* Sound */
void outputPlayWav(tMemory sound) {} /* Starts the reproduction of the sample and returns */
void outputPlayMid(tMemory music) {} /* Starts the music reproduction and returns */

/* Graph */

/* Define a dummy private structure */
typedef struct {
	char* picture;
	unsigned char palette[3*16];
	int h,w;
}SDL_very_cool_structure;


 /* Graph: Primitives for resources module */
void* outputLoadBitmap(const unsigned char* data, int size, const unsigned char* palette, int h,int w,int invert, int firstColorTransparent) {
 /* Returns an abstract object allocated in memory using the data information ti build it
	* invert is 0 when no invertion is needed and non-zero when an inversion is performed
	*/

	/* Dummy function */
	SDL_very_cool_structure* result;
	static char printed[]=" *+@#$%&=|SVHG/OP";
	int i,j;
	
	result=(SDL_very_cool_structure*)malloc(sizeof(SDL_very_cool_structure));
	
	printf("outputLoadBitmap: I'm creating an SDL structure :p\n");
	printf("outputLoadBitmap: invert=%d. transparent=%d. size=%d\n",invert,firstColorTransparent,size);

	/* Notes:
	 * - the image is serialized
	 *    this means that the last bits in the row to complete a full byte are garbage
	 * - there are 2 types of images:
	 *    1 bit/pixel (b/w)
	 *    4 bit/pixel (16 cols)
	 * - for the moment only 4bpp will be supported
	 * - width is in pixels
	 * - width in bytes is (w+1)/2
	 *    we have to add 1 because of the serialization
	 *    division by 2 is because 4bpp are 2 pixel/byte (0.5 byte/pixel)
	 */
	w=(w+1)/2;
	result->picture=malloc(h*w*4+10);
	result->w=w;
	result->h=h;
					
	for (i=0,j=0;i<h*w;i++) { /* transform a 4 bpp array into an ASCII draw 16 bpp*/
		result->picture[j++]=printed[data[i]>>4];
		result->picture[j++]=printed[data[i]>>4];
		result->picture[j++]=printed[data[i]&0x0f];
		result->picture[j++]=printed[data[i]&0x0f];
	}
	
	memcpy(result->palette,palette,3*16); /* copy palette */

	return (void*)result;
}

void outputFreeBitmap(void* image) {}
 /* Frees the abstract object created by the loadBitmap function
	*/

 /* Graph: Primitives for the kernel */
void outputDrawBitmap(void* image,int x, int y) {
 /* Draws an abstract image
	*/

#define myImage ((SDL_very_cool_structure*)image)
				
	int h,w,i,j;		
	w=myImage->w*4;
	h=myImage->h;
	
	/* Draw image */
	for (i=0;i<y;i++) printf("\n");
	for (i=0;i<h;i++) {
		for (j=0;j<x;j++) printf(" ");
		for (j=0;j<w;j++) {
			printf("%c",
				myImage->picture[i*w+j]
			);
		}
		printf("\n");
	}
	
	/* show palette */
	for (i=0;i<16;i++) {
		printf("rgb[%d]=(%d,%d,%d) #%02x%02x%02x\n",
			i+1,
			myImage->palette[3*i],
			myImage->palette[3*i+1],
			myImage->palette[3*i+2],
			(myImage->palette[3*i])<<2,
			(myImage->palette[3*i+1])<<2,
			(myImage->palette[3*i+2])<<2
		);
	}

}

void outputClearScreen() {}
 /* Crears the screen
	*/

/* Initialization */
void outputInit() {}
/* This function must be called before starting using the output functions
 * it will initialize the screen and the output module
 */

void outputStop() {}
/* Finish all output modes, including the screen mode
 */

#endif
