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
������
 Copyright 2004 Princed Development Team
  Created: 21 Jul 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _ROOM_H_
#define _ROOM_H_

#include "maps.h"

typedef char tRoomId;

typedef struct {
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
	int hasFloor;
	int hasBrokenTile;
	int isWall;
	int hasSword;

} tTile;

void drawScreen(/*tRoom room, */int layer); /* TODO: rename it to roomDraw */

#endif
