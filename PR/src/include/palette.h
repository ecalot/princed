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
pal.h: Princed Resources : JASC PAL files support headers
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PALETTE_H_
#define _PALETTE_H_

#include "image.h" /* tPalette, tImage */

#define SAMPLE_PAL16 {\
	{0x00,0x00,0x00},\
	{0x00,0x00,0xa0},\
	{0x00,0xa7,0x00},\
	{0x00,0xa7,0xa0},\
	{0xa0,0x00,0x00},\
	{0xa0,0x00,0xa0},\
	{0xa0,0x50,0x00},\
	{0xa0,0xa7,0xa0},\
	{0x50,0x50,0x50},\
	{0x50,0x50,0xff},\
	{0x50,0xf8,0x50},\
	{0x50,0xf8,0xff},\
	{0xff,0x50,0x50},\
	{0xff,0x50,0xff},\
	{0xff,0xf8,0x50},\
	{0xff,0xff,0xff}}

int getPalette(const tPalette* p, int bits, const tColor** palette);
int setPalette(tPalette* p, int bits, tColor* palette);
tPalette createPalette();
int readPalette(tPalette* p, unsigned char* data, int dataSize);
int applyPalette(tPalette* p,tImage *i);


void* objPalette_pop1_4bitsCreate(unsigned char* data, int size, int *error);
int objPalette_pop1_4bitsWrite(void* o, const char* file, int optionflag, const char* backupExtension);

#endif
