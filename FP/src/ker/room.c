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
	tRoomId roomId;
	
	fore=room->fore[x+12*y];
	result.back=room->back[x+12*y];
	result.code=fore&0x1F;
	
	switch (result.code) { /* TODO: use arrays and a better algorithm */
	case T_GATE:
	case T_EXIT_LEFT:
		roomId=room->id;
		if (y==0)	roomId=room->links[eUp];
		if (x==0) roomId=room->links[eLeft];
		if (y==4) roomId=room->links[eDown];
		if (x==11)roomId=room->links[eRight];
		result.hasGateFrame=(result.code==T_GATE);
		result.bricks=0;
		result.hasPillar=0;
		result.hasBigPillar=0;
		result.isGate=(result.code==T_GATE);
		if (roomId<24)
			result.moreInfo=room->level->screenGates[roomId-1][result.back];
		result.walkable=1;
		result.hasChopper=0;
		result.isExit=(result.code==T_EXIT_LEFT)?1:((result.code==T_EXIT_RIGHT)?2:0);
		result.block=0;
		result.isPressable=0;
		result.hasSkeleton=0;
		result.isRaise=0;
		result.hasSpikes=0;
		result.hasTorch=0;
		result.hasFloor=0;
		result.hasBrokenTile=0;
		result.isWall=0;
		result.hasSword=0;
		break;
	case T_BTN_RAISE:
	case T_BTN_DROP:
		roomId=room->id;
		if (y==0)	roomId=room->links[eUp]; /*TODO: validate corners */
		if (x==0) roomId=room->links[eLeft];
		if (y==4) roomId=room->links[eDown];
		if (x==11)roomId=room->links[eRight];
		result.hasGateFrame=0;
		result.bricks=0;
		result.hasPillar=0;
		result.hasBigPillar=0;
		result.isGate=0;
		result.walkable=1;
		/* the case that a button is in tile 0 should never happen, but we'll care about it just in case */
		if (roomId<24)
			result.moreInfo=room->level->screenPressables[roomId-1][result.back];
		result.hasChopper=0;
		result.isExit=0;
		result.block=0;
		result.isRaise=(result.code==T_BTN_RAISE);
		result.isPressable=1;
		result.hasSkeleton=0;
		result.hasSpikes=0;
		result.hasTorch=0;
		result.hasFloor=0;/*(result.code==T_BTN_DROP);*/
		result.hasBrokenTile=0;
		result.isWall=0;
		result.hasSword=0;
		break;
	case T_FLOOR:
	case T_TORCH:
	case T_SWORD:
	case T_CHOPPER:
	case T_POTION:
	case T_SPIKES:
	case T_BP_BOTTOM:
	case T_TORCH_DEBRIS:
	case T_EXIT_RIGHT:
	case T_SKELETON:
	case T_LOOSE:
	case T_PILLAR:
	case T_DEBRIS:
		result.hasGateFrame=0;
		result.bricks=(result.code==T_FLOOR)?result.back:0;
		result.hasPillar=(result.code==T_PILLAR);
		result.hasBigPillar=(result.code==T_BP_BOTTOM);
		result.isGate=0;
		result.walkable=1;
		result.hasChopper=(result.code==T_CHOPPER);
		result.isExit=(result.code==T_EXIT_LEFT)?1:((result.code==T_EXIT_RIGHT)?2:0);
		result.block=0;
		result.isRaise=0;
		result.isPressable=0;
		result.hasSkeleton=(result.code==T_SKELETON);
		result.hasSpikes=(result.code==T_SPIKES);
		result.hasTorch=(result.code==T_TORCH)|(result.code==T_TORCH_DEBRIS);
		result.hasFloor=((result.code==T_FLOOR)|(result.code==T_TORCH)|(result.code==T_LOOSE)|(result.code==T_POTION)|(result.code==T_SWORD)|(result.code==T_CHOPPER));
		result.hasBrokenTile=(result.code==T_DEBRIS)|(result.code==T_TORCH_DEBRIS);
		result.isWall=0;
		result.hasSword=(result.code==T_SWORD);
		break;
	case T_WALL:
		result.hasGateFrame=0;
		result.bricks=0;
		result.hasPillar=0;
		result.isRaise=0;
		result.hasBigPillar=0;
		result.walkable=0;
		result.hasChopper=0;
		result.isExit=0;
		result.isGate=0;
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
	case T_TAPESTRY_TOP:
	case T_BP_TOP:
	default:
		result.hasGateFrame=(result.code==T_TAPESTRY_TOP);
		result.hasBigPillar=(result.code==T_BP_TOP)*2;
		result.bricks=result.back;
		result.hasPillar=0;
		result.walkable=0;
		result.isExit=0;
		result.isGate=0;
		result.hasChopper=0;
		result.isRaise=0;
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
	if (!tile.isWall) {
		if (left.isWall) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[63],
				(x-1)*TILE_W,
				y*TILE_H+2
			);
		}
	}
	/* Gate/left */
	if (left.isGate) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[15],
			(x-1)*TILE_W,
			y*TILE_H+2
		);
		drawGate((x-1)*TILE_W,(y-1)*TILE_H+3,((tGate*)left.moreInfo)->frame);
	}
	/* normal/left */
	if (left.hasFloor) {
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
	if (left.hasPillar) {
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
	if (left.isPressable) {
		if (left.isRaise) {
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
		drawSpike((x-2)*TILE_W,y*TILE_H,room->level->time%6,layRight);
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
	if (tile.hasChopper) {
		drawChopper((x-1)*TILE_W,y*TILE_H,room->level->time%8,layCBack);
	}
	/* empty_bricks/this */
	if ((0<tile.bricks)&&(tile.bricks<4)&&(tile.code==T_EMPTY)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[51+tile.bricks],
			x*TILE_W,
			y*TILE_H-18
		);
	}	
	/* floor_bricks/this */
	if ((0<tile.bricks)&&(tile.bricks<3)&&(tile.code==T_FLOOR)) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[11+tile.bricks],
			x*TILE_W,
			y*TILE_H-18
		);
	}
	/* gate/this */
	if (tile.isGate) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[14],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* gate_frame/this */
	if (tile.hasGateFrame) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[17],
			(x-1)*TILE_W+24,
			y*TILE_H
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
	if (tile.hasPillar) {
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
	if (tile.isPressable) {
		if (tile.isRaise) {
			outputDrawBitmap(
				roomGfx.environment->pFrames[(((tPressable*)tile.moreInfo)->action==eNormal)?(58-((left.walkable)&&(!left.isRaise))):58],
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
	if (tile.hasBrokenTile) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[48],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}
	/* chopper/this /
	if (tile.hasChopper) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[9],
			(x-1)*TILE_W,
			y*TILE_H
		);
	}*/
	/* spikes/this */
	if (tile.hasSpikes) {
		outputDrawBitmap(
			roomGfx.environment->pFrames[101],
			(x-1)*TILE_W,
			y*TILE_H
		);
		drawSpike((x-1)*TILE_W,y*TILE_H,room->level->time%6,layFore);
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
		if (tile.isPressable) {
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
		if (tile.isWall) {
			tTile left;
			tTile right;
			void* image;
			left=roomGetTile(room,x-1,y);
			right=roomGetTile(room,x+1,y);
			/* there are 4 cases */
			if (left.isWall&&right.isWall) {
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
		} else {
	/* empty */
			tTile dleft=roomGetTile(room,x-1,y+1);
			/* gate_frame/this */
			if (tile.hasGateFrame) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[42],
					(x-1)*TILE_W,
					y*TILE_H+3
				);
			}
			/* gate/left */
			if (dleft.isGate) {
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
			if (dleft.hasPillar) {
				outputDrawBitmap(
					roomGfx.environment->pFrames[45],
					(x-1)*TILE_W,
					y*TILE_H+3
				);
			}
			/* pillar/left */
			if (dleft.isWall) {
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
	if (tile.hasPillar) {
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
	if (tile.isWall) {
		static unsigned short seedArray[]=WALL_PROPERTIES;
		unsigned short seed;
		tTile left;
		tTile right;
		left=roomGetTile(room,x-1,y);
		right=roomGetTile(room,x+1,y);
		/* there are 4 cases */
		if (left.isWall&&right.isWall) { 
			/* First step: calculate the seed position and get the element */	
			seed=seedArray[room->id+(x-1)+(y-1)*10-1];
			outputDrawBitmap(roomGfx.environment->pFrames[66],(x-1)*TILE_W,y*TILE_H);
		} else if ((!left.isWall)&&(right.isWall)) {
			outputDrawBitmap(roomGfx.environment->pFrames[72],(x-1)*TILE_W,y*TILE_H);
			seed=seedArray[room->id+(x-1)+(y-1)*10-1];
		} else if ((left.isWall)&&(!right.isWall)) {
			outputDrawBitmap(roomGfx.environment->pFrames[68],(x-1)*TILE_W,y*TILE_H);
			seed=seedArray[room->id+(x-1)+(y-1)*10-1];
		} else {
			outputDrawBitmap(roomGfx.environment->pFrames[70],(x-1)*TILE_W,y*TILE_H);
			seed=seedArray[room->id+(x-1)+(y-1)*10-1];
		}
		/* TODO: use one seed per combination */
			/* the seed generation algorithm */
			/* Upper row */
			if (w1_darker(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[75],
				(x-1)*TILE_W,
				y*TILE_H-39
			);
			if (w1_left_down(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[77],
				(x-1)*TILE_W,
				y*TILE_H-39
			);
			if (w1_right_down(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[79],
				(x-1)*TILE_W+24,
				y*TILE_H-39
			);
			if (w1_right_up(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[78],
				(x-1)*TILE_W+24,
				y*TILE_H-49
			);
			if ((room->id+(x-1)+(y-1)*10)==37) outputDrawBitmap(
				roomGfx.environment->pFrames[76],
				(x-1)*TILE_W,
				y*TILE_H-54
			);
			/* Second row */
			outputDrawBitmap(
				roomGfx.environment->pFrames[74-w2_sep(seed)],
				(x-1)*TILE_W+7+w2_offset(seed),
				y*TILE_H-18
			);
			if (w2_left_down(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[77],
				(x-1)*TILE_W+7+w2_offset(seed)+5,
				y*TILE_H-39+21
			);
			if (w2_right_down(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[79],
				(x-1)*TILE_W+24+7+w2_offset(seed)-32+5,
				y*TILE_H-39+21
			);
			if (w2_right_up(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[78],
				(x-1)*TILE_W+24+7+w2_offset(seed)-32+5,
				y*TILE_H-49+21
			);
			if (w2_left_up(seed)) outputDrawBitmap(
				roomGfx.environment->pFrames[76],
				(x-1)*TILE_W+7+w2_offset(seed)+5,
				y*TILE_H-54+21
			);
			/* Third row TODO: send to BottomTile */
			outputDrawBitmap(
				roomGfx.environment->pFrames[74-w3_sep(seed)],
				(x-1)*TILE_W+3+w3_offset(seed),
				y*TILE_H+3
			);
			if ((room->id+(x-1)+(y-1)*10)==16) outputDrawBitmap(
				roomGfx.environment->pFrames[76],
				(x-1)*TILE_W+5+w3_offset(seed)+5,
				y*TILE_H-55+21*2
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


