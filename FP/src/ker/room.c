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
	case T_POTION:
	case T_SPIKES:
	case T_BTN_RAISE:
	case T_SKELETON:
	case T_LOOSE:
	case T_PILLAR:
	case T_DEBRIS:
		result.hasPillar=(result.code==T_PILLAR);
		result.walkable=1;
		result.block=0;
		result.isPressable=(result.code==T_BTN_RAISE);
		result.hasSkeleton=(result.code==T_SKELETON);
		result.hasSpikes=(result.code==T_SPIKES);
		result.hasTorch=(result.code==T_TORCH);
		result.hasFloor=((result.code==T_FLOOR)|(result.code==T_TORCH)|(result.code==T_LOOSE)|(result.code==T_POTION));
		result.hasBrokenTile=(result.code==T_DEBRIS);
		result.isWall=0;
		result.hasSword=(result.code==T_SWORD);
		break;
	case T_WALL:
		result.hasPillar=0;
		result.walkable=0;
		result.isPressable=0;
		result.hasSkeleton=0;
		result.hasSpikes=0;
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
		result.isPressable=0;
		result.hasSkeleton=0;
		result.hasSpikes=0;
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
#define TILE_H 63

/*
 * Drawing functions
 */

/* main panel block */
void drawBackPanel(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);
	tTile left=roomGetTile(room,x-1,y);
	
	/* Wall/left */
	if (!tile.isWall) {
		if (left.isWall) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[63],
				(x-1)*TILE_W,
				y*TILE_H+2
			);
		}
	}
	/* normal/left */
	if (left.hasFloor) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[10],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* pillar/left */
	if (left.hasPillar) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[44],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* pressable/left */
	if (left.isPressable) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[10],
			(x-1)*TILE_W,
			y*TILE_H+1
		);
	}
	/* debris/left */
	if (left.hasBrokenTile) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[49],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* spikes/left */
	if (left.hasSpikes) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[107],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* skeleton/left */
	if (left.hasSkeleton) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[81],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* torch/this */
	if (tile.hasTorch) { /* animation */
		outputDrawBitmap(
			roomGfx.torch->pFrames[
				(((tMap*)(room->level))->time+2*x+y)%(roomGfx.torch->frames)
			],
			x*TILE_W+16,
			y*TILE_H-38
		);
		outputDrawBitmap( /* base */
			roomGfx.environment->pFrames[56],
			x*TILE_W+8,
			y*TILE_H-23
		);
	}
	/* normal/this */
	if (tile.hasFloor) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[9],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* pillar/this */
	if (tile.hasPillar) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[43],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* pressable/this */
	if (tile.isPressable) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[((left.walkable)&&(!left.isPressable))?57:58],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* debris/this */
	if (tile.hasBrokenTile) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[48],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* spikes/this */
	if (tile.hasSpikes) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[101],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* skeleton/this */
	if (tile.hasSkeleton) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[80],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
}

/* bottom panel block at background */
void drawBackBottomTile(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);
	
	/* normal */
	if (tile.walkable) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[11],
			(x-1)*TILE_W,
			y*TILE_H+3
		);
	}
	/* wall */
	if (tile.isWall) {
		tTile left;
		tTile right;
		void* image;
		left=roomGetTile(room,x-1,y);
		right=roomGetTile(room,x+1,y);
		/* there are 4 cases */
		if (left.isWall&&right.isWall) {
			/* TODO: code the seed generation algorithm */
			image=roomGfx.environment->pFrames[65];
		} else if ((!left.isWall)&&(right.isWall)) {
			image=roomGfx.environment->pFrames[71];
		} else if ((left.isWall)&&(!right.isWall)) {
			image=roomGfx.environment->pFrames[67];
		} else {
			image=roomGfx.environment->pFrames[69];
		}
		outputDrawBitmap(
			image,
			(x-1)*TILE_W,
			y*TILE_H+3
		);
	}
}

/* main panel block */
void drawForePanel(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);

	/* pillar */
	if (tile.hasPillar) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[46],
			x*TILE_W-24,
			y*TILE_H
		);
	}
	/* wall */
	if (tile.isWall) {
		tTile left;
		tTile right;
		void* image;
		left=roomGetTile(room,x-1,y);
		right=roomGetTile(room,x+1,y);
		/* there are 4 cases */
		if (left.isWall&&right.isWall) {
			/* TODO: code the seed generation algorithm */
			image=roomGfx.environment->pFrames[66];
		} else if ((!left.isWall)&&(right.isWall)) {
			image=roomGfx.environment->pFrames[72];
		} else if ((left.isWall)&&(!right.isWall)) {
			image=roomGfx.environment->pFrames[68];
		} else {
			image=roomGfx.environment->pFrames[70];
		}
		outputDrawBitmap(
			image,
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* debris/this foreground layer */
	if (tile.hasBrokenTile) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[51],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
}	

/*
 * Drawing interfaces
 */

void roomDrawBackground(tRoom* room) {
	int x,y;
	
	for (x=1;x<11;x++) {
		drawBackBottomTile(room,x,0);
		for (y=1;y<4;y++) {
			drawBackPanel(room,x,y);
			drawBackBottomTile(room,x,y);
		}
	}
}

void roomDrawForeground(tRoom* room) {
	int x,y;
	
	for (x=1;x<11;x++) {
		for (y=1;y<4;y++) {
			drawForePanel(room,x,y);
			/* bottom tile-floor block TODO: separate in two functions */
		}
	}
}


