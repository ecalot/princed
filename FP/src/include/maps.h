/*  FreePrince - POP1 remake
    Copyright (C) 2003, 2004 Princed Development Team

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
maps.c: Freeprince : Map handling library
¯¯¯¯¯¯
 Copyright 2003,2004 Princed Development Team
  Created: 20 Jun 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _MAPS_H_
#define _MAPS_H_

#include "map_defs.h"
#include "room.h"
#include "resources.h" /* tMemory */
#include "kid.h" /* tKid */

#define MAP_ENVIRONMENTS {0,0,0,0,1,1,1,0,0,0,1,1,0,0,1,0}

typedef struct {
	tRoomId        links[4*24];
	unsigned char  fore [24*30];
	unsigned char  back [24*30];
	/*
	 * Active door handling here
	 * */
	int time;
	unsigned char start[3];
} tMap;

typedef enum {eLeft=0,eRight=1,eUp=2,eDown=3}tDirection;

/* called from reources */
void* mapLoadLevel(tMemory level);
void  mapDestroy(tData* map);

/* called from the kernel */
tRoom mapGetRoom(tData* map, tRoomId roomId);
void  mapStart(tData* map, tKid* kid, tRoomId* roomId, int level);

/* events */
void  mapPressedTile(tData* map, tTile tile);
void  mapMove(tData* map);

#endif

