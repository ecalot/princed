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
room.h: FreePrince : Draw Screen
¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 21 Jul 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _ROOM_H_
#define _ROOM_H_

typedef char tRoomId;
typedef unsigned char tTileId;
typedef unsigned char tModId;

typedef struct {
	/*tMap*/ void*          level;
	tRoomId        id;
	tRoomId        links[4];
	tRoomId        corners[4];
	unsigned char  fore [60];	
	unsigned char  back [60];
	/* TODO: add guards */
} tRoom;

typedef struct {
	/* Physical attributes */
	int walkable;
	int block;
	
	/* Graphical arributes */
	int hasTorch;
	int hasSkeleton;
	int hasSpikes;
	int hasPillar;
	int hasFloor;
	int hasBrokenTile;
	int isWall;
	int hasSword;

	char code;
} tTile;

tTile roomGetTile(tRoom* room,int x, int y);
void roomDrawBackground(tRoom* room);
void roomDrawForeground(tRoom* room);
void roomLoadGfx(long environment); /* Only to initialize environment by mapStart */

#define w1_darker(a)     ((a)&1)
#define w1_left_down(a)  ((a>>1)&1)
#define w1_right_down(a) ((a>>2)&1)
#define w1_right_up(a)   ((a>>3)&1)
#define w2_left_down(a)  ((a>>4)&1)
#define w2_left_up(a)    ((a>>5)&1)
#define w2_right_down(a) ((a>>6)&1)
#define w2_right_up(a)   ((a>>7)&1)

#define w2_offset(a)     ((a>>8)&1)
#define w2_sep(a)        ((a>>9)&1)
#define w3_offset(a)     ((a>>10)&1)
#define w3_sep(a)        ((a>>13)&1)

#define WALL_PROPERTIES {37632, 28483, 27648, 9224, 46081, 37760, 19200, 9280, 9248, 46888, 28418, 19328, 9216, 46081, 37632, 28416, 19201, 46080, 36864, 0, 0, 0, 128, 0, 64, 1, 4, 0, 128, 0, 8, 1, 4, 16, 65, 2, 8, 0, 128, 0, 0, 0, 0, 0, 2, 0, 16, 0, 0, 1, 0, 0, 0}

#endif

