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
palette.c: Princed Resources : The palette object implementation
ŻŻŻŻŻŻŻŻŻŻ
 Copyright 2006 Princed Development Team
  Created: 09 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2006-Feb-09)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include <stdlib.h>
#include <string.h>
#include "palette.h"

/***************************************************************\
|                         Palette Object                        |
\***************************************************************/

static tColor sample_pal4[]={{0,0,0},{4,4,4}}; /*SAMPLE_PAL16;*/

int setPalette(tPalette* p, int bits, tColor* palette) {
	switch(bits) {
	case 1:
		memcpy(p->p1,palette,2);
		break;
	case 4:
		memcpy(p->p4,palette,16);
		break;
	case 8:
		memcpy(p->p8,palette,256);
		break;
	default:
		return -1; /* unsupported bit rate */
	}
	return 0;
}

int getPalette(tPalette* p, int bits, tColor** apalette) {
	switch(bits) {
	case 1:
		*apalette=&(p->p1);
		break;
	case 4:
		*apalette=&(p->p4);
		break;
	case 8:
		*apalette=&(p->p8);
		break;
	default:
		return -1; /* unsupported bit rate */
	}
	return 0;
}

tPalette createPalette() {
	tPalette r;
	int i;

	//Palette 1 bit
	r.p1[0].r=0;
	r.p1[0].g=0;
	r.p1[0].b=0;
	r.p1[1].r=255;
	r.p1[1].g=255;
	r.p1[1].b=255;

	//Palette 4 bits
	memcpy(r.p4, sample_pal4, 16);

	//Palette 8 bits
	for (i=0;i<256;i++) {
		r.p8[i].r=i;
		r.p8[i].g=i;
		r.p8[i].b=i;
	}
	return r;
}


/* middle layer */

/* reads the information in data and tries to remember it in the palette */
int readPalette(tPalette* p, unsigned char* data, int dataSize) {

}

int applyPalette(tPalette* p,tImage *i) {
	i->pal=*p;
}
