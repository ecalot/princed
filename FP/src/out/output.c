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
output.c: Free Prince : Output Devices Handler
¯¯¯¯¯¯¯¯

 Description: 
 	Platform and device independent functions for handling output
	(text, graphics, sound, music...).

 Copyright 2004 Princed Development Team
  Created: 23 Mar 2004

  Author: Ricardo Markiewicz <rmarkie (en) fi.uba.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include <stdlib.h>    /* malloc */
#include "resources.h" /* tMemory structure */
#include "output.h"

/* Text Primitives*/
void outputDrawText(const char* text, int x, int y) {}
void outputDrawMessage(const char* text) {}

/* Sound */
void outputPlayWav(tMemory sound) {} /* Starts the reproduction of the sample and returns */
void outputPlayMid(tMemory music) {} /* Starts the music reproduction and returns */

/* Graphics */

/* Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!*/
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y*surface->pitch + x*bpp;

	*p = pixel;
}


/* Graphics: Primitives for resources module */
void*
outputLoadBitmap(const unsigned char* data, int size, 
		const unsigned char* palette, int h, int w, int invert, 
		int firstColorTransparent)
{
 /* Returns an abstract object allocated in memory using the data 
  * information ti build it invert is 0 when no invertion is needed and 
  * non-zero when an inversion is performed	*/

	/* Dummy function */
	SDL_Surface* result;
	int i,j;
	SDL_Color colors[256];
																				        
	/* Fill colors with color information */
	for(i=0;i<256;i++) {
		colors[i].r=i;
		colors[i].g=255-i;
		colors[i].b=255-i;
	}
	
	printf("outputLoadBitmap: I'm creating an SDL structure :p\n");
	printf("outputLoadBitmap: invert=%d. transparent=%d. size=%d\n", invert, firstColorTransparent, size);

	result = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0);
	if (!result) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		return NULL;
	}
	SDL_SetPalette(result, SDL_LOGPAL, colors, 0, 256);

	w = (w + 1) / 2;

	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(result)) {
		if (SDL_LockSurface(result) < 0) {
			fprintf(stderr, "Can't lock surface: %s\n", SDL_GetError());
			exit(1);
		}
	}

	for (i = 0; i < result->w; i++) {
		for (j = 0; j < result->h; j++) {
			putpixel(result, i, j, *(data+j+i*size));
		}
	}
	
	if (SDL_MUSTLOCK(result)) {
		SDL_UnlockSurface(result);
	}

	return (void*)result;
}

/* Frees the abstract object created by the loadBitmap function */
void outputFreeBitmap(void* image) {}

/* Graphics: Primitives for the kernel */
void outputDrawBitmap(SDL_Surface *screen, void* image, int x, int y) {
	/* Draws an abstract image */
	SDL_Surface *s = (SDL_Surface *)image;
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = s->w;
	dest.h = s->h;
	if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
	SDL_BlitSurface(s, NULL, screen, &dest);
	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

void outputClearScreen(SDL_Surface *screen)
{
	SDL_FillRect(screen, NULL, 0);
}

void outputUpdateScreen(SDL_Surface *screen) 
{
	SDL_Flip(screen);
}

/* Initialization */
SDL_Surface *outputInit() 
{
	int i;
	SDL_Surface *screen;
	SDL_Color colors[256];
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	atexit(outputStop);
																				        
	/* Fill colors with color information */
	for(i=0;i<256;i++) {
		colors[i].r=255-i;
		colors[i].g=255-i;
		colors[i].b=255-i;
	}
	screen = SDL_SetVideoMode(320, 200, 8, SDL_SWSURFACE|SDL_HWPALETTE);
	if (!screen) return NULL;
	/*SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);*/
	return screen;
}

/* Finish all output modes, including the screen mode */
void outputStop()
{
	SDL_Quit();
}

