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
room.c: FreePrince : Room and Tile Object
¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 18 Jul 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include <stdlib.h>
#include "kernel.h"
#include "resources.h"
#include "output.h"
#include "room.h"
#include "maps.h"

static struct {
	tData* torch;
	tData* environment;
} roomGfx;

void roomLoadGfx(long environment) {
	if (roomGfx.environment!=NULL) {
		resFree(roomGfx.environment);
	}
	roomGfx.environment=resLoad(environment);
	if (roomGfx.torch==NULL) {
		roomGfx.torch=resLoad(RES_ANIM_TORCH);
	}
}

void roomFree() {
	if (roomGfx.environment) resFree(roomGfx.environment);
	if (roomGfx.torch) resFree(roomGfx.torch);
	roomGfx.torch=(roomGfx.environment=NULL);
}

/* room */
tTile roomGetTile(tRoom* room,int x, int y) {
	tTile   result;
	tTileId fore;
	tModId  back;

	fore=room->fore[x+12*y];
	back=room->back[x+12*y];
	result.code=fore&0x1F;
	
	switch (result.code) { /* TODO: use arrays and a better algorithm */
	case T_FLOOR:
	case T_TORCH:
	case T_SWORD:
	case T_PILLAR:
	case T_DEBRIS:
		result.hasPillar=(result.code==T_PILLAR);
		result.walkable=1;
		result.block=0;
		result.hasTorch=(result.code==T_TORCH);
		result.hasFloor=1;
		result.hasBrokenTile=(result.code==T_DEBRIS);
		result.isWall=0;
		result.hasSword=(result.code==T_SWORD);
		break;
	case T_WALL:
		result.hasPillar=0;
		result.walkable=0;
		result.block=1;
		result.hasTorch=0;
		result.hasFloor=0;
		result.hasBrokenTile=0;
		result.isWall=1;
		result.hasSword=0;
		break;
	case T_EMPTY:
	default:
		result.hasPillar=0;
		result.walkable=0;
		result.block=0;
		result.hasTorch=0;
		result.hasFloor=0;
		result.hasBrokenTile=0;
		result.isWall=0;
		result.hasSword=0;
		break;
	}
	return result;
}

#define TILE_W 32
#define TILE_H 60

void roomDrawBackground(tRoom* room) {
	int x,y;
	tTile tile;
	tMap* map=room->level;
	
	for (x=0;x<12;x++) {
		for (y=0;y<5;y++) {
			tile=roomGetTile(room,x,y);
			if (tile.hasTorch) {
				outputDrawBitmap(
					roomGfx.torch->pFrames[(map->time+2*x+y)%(roomGfx.torch->frames)],
					x*TILE_W+16,
					y*TILE_H-40
				);
			}
			if (tile.hasFloor) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[10],
					(x-1)*TILE_W,
					y*TILE_H
				);
				outputDrawBitmap(
					roomGfx.environment->pFrames[9],
					(x-1)*TILE_W,
					y*TILE_H+2
				);
			}
			if (tile.hasBrokenTile) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[2],
					(x-1)*TILE_W,
					y*TILE_H
				);
				outputDrawBitmap(
					roomGfx.environment->pFrames[1],
					(x-1)*TILE_W,
					y*TILE_H+2
				);
			}
			if (tile.walkable) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[8],
					x*TILE_W,
					y*TILE_H+1
				);
			}
		}
	}
}

void roomDrawForeground(tRoom* room) {
	int x,y;
	tTile tile;
	tTile tileNext;
	/*tMap* map=room->level;*/
	
	for (x=0;x<12;x++) {
		for (y=0;y<5;y++) {
			tile=roomGetTile(room,x,y);
			if (x!=11) tileNext=roomGetTile(room,x+1,y);
			if (tile.hasPillar) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[25],
					x*TILE_W-20,
					y*TILE_H+1
				);
			}
			if (tile.isWall) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[29],
					(x-1)*TILE_W,
					y*TILE_H+1
				);
				if ((x!=11)&&(!tileNext.isWall)) {
					outputDrawBitmap(
						roomGfx.environment->pFrames[26],
						x*TILE_W,
						y*TILE_H+1
					);
				}
			}
		}
	}
}


