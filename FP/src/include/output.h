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
output.h: Free Prince : Output Devices Handler
��������

 Description: 
 	Platform and device independent functions for handling output
	(text, graphics, sound, music...).

 Copyright 2004 Princed Development Team
  Created: 23 Mar 2004

  Author: Ricardo Markiewicz <rmarkie.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "resources.h"

typedef struct tColor{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}tColor;

typedef struct tPalette{
	int     colors;
	tColor* color;
}tPalette;

/* Text Primitives*/
void outputDrawText(int x, int y, const char *fmt, ...);
void outputDrawMessage(const char* fmt, ...); 

/* Sound */
void outputPlayWav(tMemory sound); /* Starts the reproduction of the sample and returns */
void outputPlayMid(tMemory music); /* Starts the music reproduction and returns */

/* Graph */

 /* Graph: Primitives for resources module */
void* outputLoadBitmap(const unsigned char* data, int size, const tPalette palette, int h,int w,int invert, int firstColorTransparent, int bottom, int left);
	/* Returns an abstract object allocated in memory using the data information ti build it
	* invert is 0 when no invertion is needed and non-zero when an inversion is performed
	*/

void outputFreeBitmap(void* image);
 /* Frees the abstract object created by the loadBitmap function
	*/

 /* Graph: Primitives for the kernel */
void outputDrawBitmap(void* image,int x, int y);
 /* Draws an abstract image
	*/

void outputClearScreen();
void outputUpdateScreen();
 /* Crears the screen
	*/

/* Initialization */
int outputInit();
/* This function must be called before starting using the output functions
 * it will initialize the screen and the output module. Returns a pointer
 * to the initialized screen, or NULL if an error occurs. */

void outputStop();
/* Finish all output modes, including the screen mode
 */

int getHeight();

int getWidth();


#endif

