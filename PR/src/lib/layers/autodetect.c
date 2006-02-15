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
autodetect.c: Princed Resources : Automatic detection resource types
¯¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include "autodetect.h"

int isA64kPalette(const unsigned char* d, int s) {
	while (s--) if (d[s]>>6) return 0; /* false */
	return 1; /* true */
}

/***************************************************************\
|                       Item Type Detector                      |
\***************************************************************/

int verifyLevelHeader(const unsigned char *array, int size) {
	return (size==12025) || (((size==2306)||(size==2305))&&!(array[1698]&0x0F)&&!(array[1700]&0x0F)&&!(array[1702]&0x0F));
}

int verifyImageHeader(const unsigned char *array, int size) {
	unsigned char imageBitRate;
	if (size<=7) return 0; /* false */
	imageBitRate=(( ((unsigned char)array[6])>>4 ) & 7)+1;
	return (size>7) && (((unsigned char)array[5])<2) && ((imageBitRate==4 || imageBitRate==8));
	/* NOTE:
	 *   imageBitRate==1
	 * works for monochrome images (but is very common and matches more than that)
	 */
}

int verifyPaletteHeader(const unsigned char *array, int size) {
	return (
		((size==101)&&(!array[2])&&(!array[3])&&(array[4]==0x10))
		||
		((size==(256*3+1)||size==(320*3+1))&&isA64kPalette(array+1,size-1))
	);
}

int verifySpeakerHeader(const unsigned char *array, int size) {
	/* format: (checksum)+(0x00)+(even number)+3 bytes per note */
	return
		(size>4)&&(array[1]==0x00)&&(!(array[2]%2))&&(!(size%3))
	;
}

int verifyWaveHeader(const unsigned char *array, int size) {
	/* format: (checksum)+(0x01)+raw wave */
	return
		(size>1)&&(array[1]==0x01)&&((size%3)==2)
	;
}

int verifyMidiHeader(const unsigned char *array, int size) {
	/* format: (checksum)+(0x02)+"MThd"... */
	return
		(size>6) &&
		(array[1]==0x02) &&
		(array[2]=='M') &&
		(array[3]=='T') &&
		(array[4]=='h') &&
		(array[5]=='d')
	;
}

tResourceType verifyHeader(const unsigned char *array, int size) {
	if (verifyLevelHeader  (array,size)) return eResTypeLevel;
	if (verifyMidiHeader   (array,size)) return eResTypeMidi;
	if (verifyImageHeader  (array,size)) return eResTypeImage;
	if (verifyPaletteHeader(array,size)) return eResTypePalette;
	if (verifyWaveHeader   (array,size)) return eResTypeWave;
	if (verifySpeakerHeader(array,size)) return eResTypePcspeaker;
	return eResTypeBinary;
}

