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
#include "tiles.h"
#include "maps.h"
#include "walls.h"

static struct {
	tData* torch;
	tData* environment;
	tData* potionAnim;
	tData* potionBase;
} roomGfx;

void roomLoadGfx(long environment) {
	if (roomGfx.environment!=NULL) {
		resFree(roomGfx.environment);
	}
	roomGfx.environment=resLoad(environment);
	/* TODO: make potion base depend on the environment.
	 *       create a typedef tEnvironment and use a switch
	 */
	if (roomGfx.torch==NULL) {
		roomGfx.torch=resLoad(RES_ANIM_TORCH);
		roomGfx.potionAnim=resLoad(RES_ANIM_POTION|RES_MODS_BW|RES_MODS_RED);
		roomGfx.potionBase=resLoad(RES_IMGS_POTION_BASE);
	}
}

void roomFree() {
	if (roomGfx.environment) resFree(roomGfx.environment);
	if (roomGfx.torch) {
		resFree(roomGfx.torch);
		resFree(roomGfx.potionAnim);
		resFree(roomGfx.potionBase);
	}
	roomGfx.torch=(roomGfx.environment=NULL);
}

/* room */
tTile roomGetTile(tRoom* room,int x, int y) {
	tTile   result;
	tTileId fore;
	tRoomId roomId;
	
	fore=room->fore[x+12*y];
	result.back=room->back[x+12*y];
	result.code=fore&0x1F;
	
	switch (result.code) { /* TODO: use the tiles library */
	case TILE_GATE:
	case TILE_EXIT_LEFT:
		roomId=room->id;
		if (y==0)	roomId=room->links[eUp];
		if (x==0) roomId=room->links[eLeft];
		if (y==4) roomId=room->links[eDown];
		if (x==11)roomId=room->links[eRight];
		result.bricks=0;
		result.hasBigPillar=0;
		if (roomId<24)
			result.moreInfo=room->level->screenGates[roomId-1][result.back];
		result.walkable=1;
		result.isExit=(result.code==TILE_EXIT_LEFT)?1:((result.code==TILE_EXIT_RIGHT)?2:0);
		result.block=0;
		break;
	case TILE_BTN_RAISE:
	case TILE_BTN_DROP:
		roomId=room->id;
		if (y==0)	roomId=room->links[eUp]; /*TODO: validate corners */
		if (x==0) roomId=room->links[eLeft];
		if (y==4) roomId=room->links[eDown];
		if (x==11)roomId=room->links[eRight];
		result.bricks=0;
		result.hasBigPillar=0;
		result.walkable=1;
		/* the case that a button is in tile 0 should never happen, but we'll care about it just in case */
		if (roomId<24)
			result.moreInfo=room->level->screenPressables[roomId-1][result.back];
		result.isExit=0;
		result.block=0;
		break;
	case TILE_FLOOR:
	case TILE_TORCH:
	case TILE_SWORD:
	case TILE_CHOPPER:
	case TILE_POTION:
	case TILE_SPIKES:
	case TILE_BP_BOTTOM:
	case TILE_TORCH_DEBRIS:
	case TILE_EXIT_RIGHT:
	case TILE_SKELETON:
	case TILE_LOOSE:
	case TILE_PILLAR:
	case TILE_DEBRIS:
		result.bricks=(result.code==TILE_FLOOR)?result.back:0;
		result.hasBigPillar=(result.code==TILE_BP_BOTTOM);
		result.walkable=1;
		result.isExit=(result.code==TILE_EXIT_LEFT)?1:((result.code==TILE_EXIT_RIGHT)?2:0);
		result.block=0;
		break;
	case TILE_WALL:
		result.bricks=0;
		result.hasBigPillar=0;
		result.walkable=0;
		result.isExit=0;
		result.block=1;
		break;
	case TILE_EMPTY:
	case TILE_TAPESTRY_TOP:
	case TILE_BP_TOP:
	default:
		result.hasBigPillar=(result.code==TILE_BP_TOP)*2;
		result.bricks=result.back;
		result.walkable=0;
		result.isExit=0;
		result.block=0;
		break;
	}
	/*isIn(result,TILES_PILLAR)=isIn(result,TILES_PILLAR);
	isIn(result,TILES_CHOPPER)=isIn(result,TILES_CHOPPER);
	isIn(result,TILES_POTION)=isIn(result,TILES_POTION);
	isIn(result,TILES_SPIKES)=isIn(result,TILES_SPIKES);
	isIn(result,TILES_BROKENTILE)=isIn(result,TILES_BROKENTILE);
	isIn(result,TILES_FLOOR)=isIn(result,TILES_FLOOR);
	isIn(result,TILES_GATEFRAME)=isIn(result,TILES_GATEFRAME);
	isIn(result,TILES_SKELETON)=isIn(result,TILES_SKELETON);
	isIn(result,TILES_SWORD)=isIn(result,TILES_SWORD);
	isIn(result,TILES_TORCH)=isIn(result,TILES_TORCH);
	isIn(result,TILES_DOOR)=isIn(result,TILES_DOOR);
	isIn(result,TILES_PRESSABLE)=isIn(result,TILES_PRESSABLE);
	isIn(result,TILES_RAISE)=isIn(result,TILES_RAISE);
	isIn(result,TILES_WALL)=isIn(result,TILES_WALL);*/
	return result;
}


/*
 * Drawing functions
 */

/* door drawing */
#define drawGateTop(x,y,frame) outputDrawBitmap(roomGfx.environment->pFrames[35-((frame)&7)],x,y)

void drawGate(int x, int y, int frame) {
	/* frames are from 0 to 46, 0 is open; 46 is closed */
	register int i;
	register const int mod=frame&7;
	
	outputDrawBitmap(roomGfx.environment->pFrames[27-mod],x,y+mod);
	for (i=8;i<=frame;i+=8)
		outputDrawBitmap(roomGfx.environment->pFrames[20],x,y+i+mod);
	outputDrawBitmap(roomGfx.environment->pFrames[18],x,y+i+mod+4);
}

void drawExit(int x, int y, int frame) {
	/* Frame defined from 0 (open) to 50 (close) */
	register int i;
	if (frame<47) outputDrawBitmap(roomGfx.environment->pFrames[55],x,y+47);
	outputDrawBitmap(roomGfx.environment->pFrames[50],x,y+51);
	for (i=0;i<=frame;i+=4)
		outputDrawBitmap(roomGfx.environment->pFrames[1],x,y+i+(frame&3));
	outputDrawBitmap(roomGfx.environment->pFrames[2],x,y);
}

typedef enum {layBack=113,layRight=108,layFore=102}tSpikeLayer;
void drawSpike(int x, int y, int frame, tSpikeLayer layer) {
	/* Frame defined from 0 (none) to 5 (near none). 3 is out normal. */
	switch (layer) { /* TODO: use relative offsets in resources */
		case layRight:
			x+=32;
			y-=7;
			break;				
		case layFore:
		case layBack:
			y-=2;
			break;
	}
	outputDrawBitmap(roomGfx.environment->pFrames[(int)layer+((frame>4)?(6-frame):frame)],x,y);
}	

typedef enum {layCBack=1,layCFore=2}tChopperLayer;
void drawChopper(int x, int y, int frame, tChopperLayer layer) {
	/* Frame defined from 0 (normal) to 7 (near normal). 3 is close. */
	if (frame<4) {
		frame++;
	} else {
		frame=7-frame;
	}
	switch (layer) { /* TODO: use relative offsets in resources */
		case layCFore:
			outputDrawBitmap(
				roomGfx.environment->pFrames[97-frame],x,y
			);
			break;
		case layCBack:
			outputDrawBitmap(
				roomGfx.environment->pFrames[92-frame],x,y
			);
			if (frame<3)
				outputDrawBitmap(
					roomGfx.environment->pFrames[100-frame],
					x,
					y-60+outputGetHeight(roomGfx.environment->pFrames[100-frame])
				);
			break;
	}
}

/* main panel block */
void drawBackPanel(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);
	tTile left=roomGetTile(room,x-1,y);
	
	/* Wall/left */
	if (!isIn(tile,TILES_WALL)) {
		if (isIn(left,TILES_WALL)) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[63],
				(x-1)*TILE_W,
				y*TILE_H+2
			);
		}
	}
	/* Gate/left */
	if (isIn(left,TILES_DOOR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[15],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
		drawGate((x-1)*TILE_W,(y-1)*TILE_H+3,((tGate*)left.moreInfo)->frame);
	}
	/* normal/left */
	if (isIn(left,TILES_FLOOR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[10],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* exit_left/left */
	if (left.isExit==2) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[7],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* pillar/left */
	if (isIn(left,TILES_PILLAR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[44],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* pillar_big_up/left */
	if (left.hasBigPillar) {
		if (left.hasBigPillar==1) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[83],
				(x-1)*TILE_W,
				y*TILE_H+2
			);
		} else {
			outputDrawBitmap(
				roomGfx.environment->pFrames[85],
				(x-1)*TILE_W,
				y*TILE_H+3
			);
		}
	}
	/* pressable/left */
	if (isIn(left,TILES_PRESSABLE)) {
		if (isIn(left,TILES_RAISE)) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[10],
				(x-1)*TILE_W,
				y*TILE_H+((((tPressable*)left.moreInfo)->action==eNormal)?1:2)
			);
		} else {
			outputDrawBitmap(
				roomGfx.environment->pFrames[10],
				(x-1)*TILE_W,
				y*TILE_H+((((tPressable*)left.moreInfo)->action==eNormal)?2:3)
			);
		}
	}
	/* debris/left */
	if (isIn(left,TILES_BROKENTILE)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[49],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* spikes/left */
	if (isIn(left,TILES_SPIKES)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[107],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
		drawSpike((x-2)*TILE_W,y*TILE_H,room->level->time%6,layRight);
	}
	/* skeleton/left */
	if (isIn(left,TILES_SKELETON)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[81],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
	}
	/* torch/this */
	if (isIn(tile,TILES_TORCH)) { /* animation */
		outputDrawBitmap(
			roomGfx.torch->pFrames[
				((room->level->time)+2*x+y)%(roomGfx.torch->frames)
			],
			x*TILE_W+11,
			y*TILE_H-39
		);
		outputDrawBitmap( /* base */
			roomGfx.environment->pFrames[56],
			x*TILE_W+3,
			y*TILE_H-24
		);
	}
	/* chopper/this */
	if (isIn(tile,TILES_CHOPPER)) {
		drawChopper((x-1)*TILE_W,y*TILE_H,room->level->time%8,layCBack);
	}
	/* empty_bricks/this */
	if ((0<tile.bricks)&&(tile.bricks<4)&&(tile.code==TILE_EMPTY)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[51+tile.bricks],
			x*TILE_W,
			y*TILE_H-18
		);
	}	
	/* floor_bricks/this */
	if ((0<tile.bricks)&&(tile.bricks<3)&&(tile.code==TILE_FLOOR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[11+tile.bricks],
			x*TILE_W,
			y*TILE_H-18
		);
	}
	/* gate/this */
	if (isIn(tile,TILES_DOOR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[14],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* gate_frame/this */
	if (isIn(tile,TILES_GATEFRAME)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[17],
			(x-1)*TILE_W+24,
			y*TILE_H
		);
	}
	/* normal/this */
	if (isIn(tile,TILES_FLOOR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[9],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* exit_left/this */
	if (tile.isExit) {
		if (tile.isExit==2) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[5],
				(x-1)*TILE_W,
				y*TILE_H
			);
		} else {
			outputDrawBitmap(
				roomGfx.environment->pFrames[9],
				(x-1)*TILE_W,
				y*TILE_H
			);
			drawExit(x*TILE_W+8,(y-1)*TILE_H-1,((tGate*)tile.moreInfo)->frame);
		}
	}
	/* pillar/this */
	if (isIn(tile,TILES_PILLAR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[43],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* big_pillar/this */
	if (tile.hasBigPillar) {
		if (tile.hasBigPillar==1) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[82],
				(x-1)*TILE_W,
				y*TILE_H
			);
		} else {
			outputDrawBitmap(
				roomGfx.environment->pFrames[87],
				(x-1)*TILE_W+8,
				y*TILE_H+3
			);
		}
	}
	/* pressable/this */
	if (isIn(tile,TILES_PRESSABLE)) {
		if (isIn(tile,TILES_RAISE)) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[(((tPressable*)tile.moreInfo)->action==eNormal)?(58-((left.walkable)&&(!isIn(left,TILES_RAISE)))):58],
				(x-1)*TILE_W,
				y*TILE_H+((((tPressable*)tile.moreInfo)->action==eNormal)?0:1)
			);
		} else {
			outputDrawBitmap(
				roomGfx.environment->pFrames[58],
				(x-1)*TILE_W,
				y*TILE_H+((((tPressable*)tile.moreInfo)->action==eNormal)?1:2)
			);
		}
	}
	/* debris/this */
	if (isIn(tile,TILES_BROKENTILE)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[48],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* chopper/this /
	if (isIn(tile,TILES_CHOPPER)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[9],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}*/
	/* spikes/this */
	if (isIn(tile,TILES_SPIKES)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[101],
			(x-1)*TILE_W,
			y*TILE_H
		);
		drawSpike((x-1)*TILE_W,y*TILE_H,room->level->time%6,layFore);
	}
	/* skeleton/this */
	if (isIn(tile,TILES_SKELETON)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[80],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* potion/left */
	if (isIn(left,TILES_POTION)) { /* animation */
		outputDrawBitmap(
			roomGfx.potionAnim->pFrames[
				((room->level->time)+2*x+y)%(roomGfx.potionAnim->frames)
			],
			(x-1)*TILE_W+3-15,
			y*TILE_H-15
		);
		outputDrawBitmap( /* base */
			roomGfx.potionBase->pFrames[0],
			(x-1)*TILE_W-15,
			y*TILE_H-4
		);
	}
}

/* bottom panel block at background */
void drawBackBottomTile(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);
	
	/* normal */
	if (tile.walkable) {
		if (isIn(tile,TILES_PRESSABLE)) {
			outputDrawBitmap( /* TODO: drop has resource 59 for unpressed/reise 47? check game */
				roomGfx.environment->pFrames[59],
				(x-1)*TILE_W,
				y*TILE_H+((((tPressable*)tile.moreInfo)->action==eNormal)?3:4)
			);
		} else {
			outputDrawBitmap(
				roomGfx.environment->pFrames[11],
				(x-1)*TILE_W,
				y*TILE_H+3
			);
		}
	} else {
	/* wall */
		if (isIn(tile,TILES_WALL)) {
			tTile left;
			tTile right;
			void* image;
			left=roomGetTile(room,x-1,y);
			right=roomGetTile(room,x+1,y);
			/* there are 4 cases */
			if (isIn(left,TILES_WALL)&&isIn(right,TILES_WALL)) {
				image=roomGfx.environment->pFrames[65];
			} else if ((!isIn(left,TILES_WALL))&&(isIn(right,TILES_WALL))) {
				image=roomGfx.environment->pFrames[71];
			} else if ((isIn(left,TILES_WALL))&&(!isIn(right,TILES_WALL))) {
				image=roomGfx.environment->pFrames[67];
			} else {
				image=roomGfx.environment->pFrames[69];
			}
			outputDrawBitmap(
				image,
				(x-1)*TILE_W,
				y*TILE_H+3
			);
		} else {
	/* empty */
			tTile dleft=roomGetTile(room,x-1,y+1);
			/* gate_frame/this */
			if (isIn(tile,TILES_GATEFRAME)) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[42],
					(x-1)*TILE_W,
					y*TILE_H+3
				);
			}
			/* gate/left */
			if (isIn(dleft,TILES_DOOR)) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[16],
					(x-1)*TILE_W,
					y*TILE_H+3
				);
			/*	drawGateTop(x*TILE_W,(y-1)*TILE_H+3,((tGate*)tile.moreInfo)->frame);*/
			}
			/* big_pillar/left */
			if (dleft.hasBigPillar==2) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[86],
					(x-1)*TILE_W,
					y*TILE_H+3
				);
			}
			/* pillar/left */
			if (isIn(dleft,TILES_PILLAR)) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[45],
					(x-1)*TILE_W,
					y*TILE_H+3
				);
			}
			/* pillar/left */
			if (isIn(dleft,TILES_WALL)) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[64],
					(x-1)*TILE_W,
					y*TILE_H+3
				);
			}
		}
	}
	/* upper objects */
	if (tile.isExit) {
				outputDrawBitmap(
			roomGfx.environment->pFrames[(tile.isExit==1)?6:8],
			x*TILE_W,
			(y-1)*TILE_H+3
		);
	}
}

/* main panel block */
void drawForePanel(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);

	/* pillar */
	if (isIn(tile,TILES_PILLAR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[46],
			x*TILE_W-24,
			y*TILE_H
		);
	}
	/* big pillar */
	if (tile.hasBigPillar==1) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[84],
			x*TILE_W-24,
			y*TILE_H
		);
	}
	/* wall */
	if (isIn(tile,TILES_WALL)) {
		/*static unsigned short seedArray[]=WALL_PROPERTIES;*/
		register short seed;
		int cases;
		int env=WALL_ENV_DUNGEON;
		tTile left;
		tTile right;
		left=roomGetTile(room,x-1,y);
		right=roomGetTile(room,x+1,y);
		seed=room->id+(x-1)+(y-1)*10-1;
		/* there are 4 cases */
		if (isIn(left,TILES_WALL)&&isIn(right,TILES_WALL)) { 
			/* First step: calculate the seed position and get the element */
			cases=WALL_LOC_WWW;
			outputDrawBitmap(roomGfx.environment->pFrames[66],(x-1)*TILE_W,y*TILE_H);
		} else if ((!isIn(left,TILES_WALL))&&(isIn(right,TILES_WALL))) {
			cases=WALL_LOC_SWW;
			outputDrawBitmap(roomGfx.environment->pFrames[72],(x-1)*TILE_W,y*TILE_H);
		} else if ((isIn(left,TILES_WALL))&&(!isIn(right,TILES_WALL))) {
			cases=WALL_LOC_WWS;
			outputDrawBitmap(roomGfx.environment->pFrames[68],(x-1)*TILE_W,y*TILE_H);
		} else {
			cases=WALL_LOC_SWS;
			outputDrawBitmap(roomGfx.environment->pFrames[70],(x-1)*TILE_W,y*TILE_H);
		}
		/* TODO: use one seed per combination */
			/* the seed generation algorithm */
			/* Upper row */
			if (wallGet(env,cases,WALL_TABLE_LINE1_DARKER,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[75],
				(x-1)*TILE_W,
				y*TILE_H-39
			);
			if (wallGet(env,cases,WALL_TABLE_LINE1_LEFT_DOWN,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[77],
				(x-1)*TILE_W,
				y*TILE_H-39
			);
			if (wallGet(env,cases,WALL_TABLE_LINE1_RIGHT_DOWN,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[79],
				(x-1)*TILE_W+24,
				y*TILE_H-39
			);
			if (wallGet(env,cases,WALL_TABLE_LINE1_RIGHT_UP,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[78],
				(x-1)*TILE_W+24,
				y*TILE_H-49
			);
			if (wallGet(env,cases,WALL_TABLE_LINE1_LEFT_UP,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[76],
				(x-1)*TILE_W,
				y*TILE_H-54
			);
			/* Second row */
			outputDrawBitmap(
				roomGfx.environment->pFrames[74-wallGet(env,cases,WALL_TABLE_LINE2_SEP,seed)],
				(x-1)*TILE_W+7+wallGet(env,cases,WALL_TABLE_LINE2_OFFSET,seed),
				y*TILE_H-18
			);
			if (wallGet(env,cases,WALL_TABLE_LINE2_LEFT_DOWN,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[77],
				(x-1)*TILE_W+7+wallGet(env,cases,WALL_TABLE_LINE2_OFFSET,seed)+5,
				y*TILE_H-39+21
			);
			if (wallGet(env,cases,WALL_TABLE_LINE2_RIGHT_DOWN,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[79],
				(x-1)*TILE_W+24+7+wallGet(env,cases,WALL_TABLE_LINE2_OFFSET,seed)-32+5,
				y*TILE_H-39+21
			);
			if (wallGet(env,cases,WALL_TABLE_LINE2_RIGHT_UP,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[78],
				(x-1)*TILE_W+24+7+wallGet(env,cases,WALL_TABLE_LINE2_OFFSET,seed)-32+5,
				y*TILE_H-49+21
			);
			if (wallGet(env,cases,WALL_TABLE_LINE2_LEFT_UP,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[76],
				(x-1)*TILE_W+7+wallGet(env,cases,WALL_TABLE_LINE2_OFFSET,seed)+5,
				y*TILE_H-54+21
			);
			/* Third row TODO: send to BottomTile */
			outputDrawBitmap(
				roomGfx.environment->pFrames[74-wallGet(env,cases,WALL_TABLE_LINE3_SEP,seed)],
				(x-1)*TILE_W+3+wallGet(env,cases,WALL_TABLE_LINE3_OFFSET,seed),
				y*TILE_H+3
			);
			if (wallGet(env,cases,WALL_TABLE_LINE3_LEFT_UP,seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[76],
				(x-1)*TILE_W+5+wallGet(env,cases,WALL_TABLE_LINE3_OFFSET,seed)+5,
				y*TILE_H-55+21*2
			);

	}
	/* debris/this foreground layer */
	if (isIn(tile,TILES_BROKENTILE)) {
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


