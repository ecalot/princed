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
resources.c: Free Prince : Output Devices Handler
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

#include <stdlib.h> /* malloc */
#include "resources.h" /* tMemory structure */
#include "output.h" /* tMemory structure */

/* Text Primitives*/
void outputDrawText(const char* text, int x, int y) {}
void outputDrawMessage(const char* text) {}

/* Sound */
void outputPlayWav(tMemory sound) {} /* Starts the reproduction of the sample and returns */
void outputPlayMid(tMemory music) {} /* Starts the music reproduction and returns */

/* Graphics */

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
/* Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!*/
{
	int bpp = surface->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *) p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		p[0] = (pixel >> 16) & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = pixel & 0xff;
		} else {
		p[0] = pixel & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *) p = pixel;
		break;
	}
}


/* Graphics: Primitives for resources module */
void* outputLoadBitmap(const unsigned char* data, int size, 
		const unsigned char* palette, int h, int w, int invert, 
		int firstColorTransparent) {
 /* Returns an abstract object allocated in memory using the data 
  * information ti build it invert is 0 when no invertion is needed and 
  * non-zero when an inversion is performed	*/

	/* Dummy function */
	SDL_Surface* result;
	int i,j;
	Uint32 rmask, gmask, bmask, amask;

	printf("outputLoadBitmap: I'm creating an SDL structure :p\n");
	printf("outputLoadBitmap: invert=%d. transparent=%d. size=%d\n",
			invert, firstColorTransparent, size);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	result = SDL_CreateRGBSurface(0, w, h, 8, rmask, gmask, bmask, amask);
	if (!result)
	{
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Notes:
	 * - the image is serialized
	 *    this means that the last bits in the row to complete a full byte
	 *    are garbage
	 * - there are 2 types of images:
	 *    1 bit/pixel (b/w)
	 *    4 bit/pixel (16 cols)
	 * - for the moment only 4bpp will be supported
	 * - width is in pixels
	 * - width in bytes is (w+1)/2
	 *    we have to add 1 because of the serialization
	 *    division by 2 is because 4bpp are 2 pixel/byte (0.5 byte/pixel)
	 */
	w = (w + 1) / 2;

	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(result)) {
		if (SDL_LockSurface(result) < 0) {
			fprintf(stderr, "Can't lock surface: %s\n", SDL_GetError());
			exit(1);
		}
	}

	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			putpixel(result, i, j, 
					SDL_MapRGB(result->format, 0xff, 0xff, 0x00));
		}
	}
	
	if (SDL_MUSTLOCK(result)) {
		SDL_UnlockSurface(result);
	}

/*	memcpy(result->palette, palette, 3 * 16); * copy palette */

	return (void*)result;
}

void outputFreeBitmap(void* image) {}
 /* Frees the abstract object created by the loadBitmap function
	*/

/* Graphics: Primitives for the kernel */
void outputDrawBitmap(SDL_Surface *screen, void* image, int x, int y) {
/* Draws an abstract image */
	SDL_Surface *s = (SDL_Surface *)image;
	/* SDL_Rect destrect = {x, y, s->w, s->h};*/ 
	SDL_BlitSurface(s, NULL, screen, NULL);
}

void outputClearScreen(SDL_Surface *screen) {
}

/* Initialization */
SDL_Surface *outputInit() 
{
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	atexit(outputStop);
	return SDL_SetVideoMode(320, 200, 8, 0);
}

void outputStop()
/* Finish all output modes, including the screen mode
 */
{
	SDL_Quit();
}

