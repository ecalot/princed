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
#include "auxiliary.h"

/***************************************************************\
|                       Item Type Detector                      |
\***************************************************************/

int verifyPop1LevelHeader(tBinary c) {
	return (((c.size==2306)||(c.size==2305))&&!(c.data[1697]&0x0F)&&!(c.data[1699]&0x0F)&&!(c.data[1701]&0x0F));
}

int verifyPop2LevelHeader(tBinary c) {
	return (c.size==12025);
}

int verifyImage16Header(tBinary c) {
	unsigned char imageBitRate;
	if (c.size<=7) return 0; /* false */
	imageBitRate=(( (c.data[5])>>4 ) & 7)+1;
	return (c.size>7) && ((c.data[4])==0) && (imageBitRate==4);
	/* NOTE:
	 *   imageBitRate==1
	 * works for monochrome images (but is very common and matches more than that)
	 */
}

int verifyImage256Header(tBinary c) {
	unsigned char imageBitRate;
	if (c.size<=7) return 0; /* false */
	imageBitRate=(( (c.data[5])>>4 ) & 7)+1;
	return (c.size>7) && ((c.data[4])==1) && (imageBitRate==8);
	/* NOTE:
	 *   imageBitRate==1
	 * works for monochrome images (but is very common and matches more than that)
	 */
}

int verifyPaletteHeaderPop1(tBinary c) {
	return (
		((c.size==100)&&(!c.data[1])&&(!c.data[2])&&(c.data[3]==0x10))
	);
}

int verifyPaletteHeaderPop2(tBinary c) {
	return (
		(c.size>=(3*16)) &&
/*		(c.data[0]==0x00) &&
		(c.data[1]==0x00) &&
		(c.data[2]==0x00) &&*/
		(c.size % 3 == 0 ) &&
		isA64kPalette(c)
	);
}

int verifySpeakerHeader(tBinary c) {
	/* format: (checksum)+(0x00)+(even number)+3 bytes per note */
	return
		(c.size>4)&&(c.data[0]==0x00)&&(!(c.data[1]%2))&&(!(c.size%3))
	;
}

int verifyWaveHeader(tBinary c) {
	/* format: (checksum)+(0x01)+raw wave */
	return
		(c.size>1)&&(c.data[0]==0x01)&&((c.size%3)==2) /* TODO: use WAVE_MAGIC */
	;
}

int verifyMidiHeader(tBinary c) {
	/* format: (checksum)+(0x02)+"MThd"... */
	return
		(c.size>6) &&
		(c.data[0]==0x02) && /* magicMidi */
		(c.data[1]=='M') &&
		(c.data[2]=='T') &&
		(c.data[3]=='h') &&
		(c.data[4]=='d')
	;
}

tResourceType verifyHeader(tBinary c) { /* TODO: add the pop version as another parameter to detect types */
	if (verifyPop1LevelHeader (c)) return eResTypeLevelPop1;
	if (verifyPop2LevelHeader (c)) return eResTypeLevelPop2;
	if (verifyMidiHeader      (c)) return eResTypeSoundMidi;
	if (verifyImage16Header   (c)) return eResTypeImage16;
	if (verifyImage256Header  (c)) return eResTypeImage256;
	if (verifyPaletteHeaderPop1 (c)) return eResTypePalettePop1_16;
	if (verifyPaletteHeaderPop2 (c)) return eResTypePalettePop2_NColors;
	if (verifyWaveHeader      (c)) return eResTypeSoundWave;
	if (verifySpeakerHeader   (c)) return eResTypeSoundPcspeaker;
	return eResTypeOtherBinary;
}

