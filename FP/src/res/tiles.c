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
tiles.c: FreePrince : Tile functions
¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 19 Oct 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _TILES_H_
#define _TILES_H_

#include "tiles_conf.h"

int isInGroup(unsigned char tile,unsigned char backtile,short group) {
	static unsigned char tileList[]=TILES_GROUP_LIST;
	unsigned char* i=tileList+group;
	int docontinue;
	tile=tile&0x1F; /* get the last 5 bits and clear the beginning */
	tile++;
	do {
		docontinue=0;
		while ((*i)&&(((*i)&0x7f)!=tile)) i++;
		if ((*i)&0x80) {
			i++;
			if ((*i)==(backtile+1)) return 1;
			docontinue=1;
		}
	} while (docontinue);
	return *i; /* returns non-zero if true and zero if false */
}

#endif

