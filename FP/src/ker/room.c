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
#include "object.h" /* DIR_LEFT DIR_RIGHT */
#include "tiles.h"
#include "maps.h"
#include "walls.h"

static struct {
	tData* torch;
	tData* environment;
	tData* potionAnimRed;
	tData* potionAnimGreen;
	tData* potionAnimBlue;
	tData* potionBase;
	tData* sword;
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
		roomGfx.potionAnimRed=resLoad(RES_ANIM_POTION|RES_MODS_BW|RES_MODS_RED);
		roomGfx.potionAnimGreen=resLoad(RES_ANIM_POTION|RES_MODS_BW|RES_MODS_GREEN);
		roomGfx.potionAnimBlue=resLoad(RES_ANIM_POTION|RES_MODS_BW|RES_MODS_BLUE);
		roomGfx.potionBase=resLoad(RES_IMGS_POTION_BASE);
		roomGfx.sword=resLoad(RES_IMGS_SWORD);
	}
}

void roomFree() {
	if (roomGfx.environment) resFree(roomGfx.environment);
	if (roomGfx.torch) {
		resFree(roomGfx.torch);
		resFree(roomGfx.potionAnimRed);
		resFree(roomGfx.potionAnimGreen);
		resFree(roomGfx.potionAnimBlue);
		resFree(roomGfx.potionBase);
		resFree(roomGfx.sword);
	}
	roomGfx.torch=(roomGfx.environment=NULL);
}

/* room */
tTile roomGetTile(tRoom* room,int x, int y) {
	tTile   result;
	tRoomId roomId;
	
	result.back=room->back[x+12*y];
	result.code=room->fore[x+12*y]&0x1F;
	result.moreInfo=NULL;
	
	/* TODO: use a tile group: special, with GATES, PRESSABLE, SPIKES,
	 * CHOPPER.
	 */
	roomId=room->id;

	if ((!roomId)||(roomId>24)) {
		fprintf(stderr,"Assert: kid outside the map\n");					
		exit(1);
	}
						
	if (isIn(result,TILES_GATES)) { 
		if (y==0)	roomId=room->links[eUp];
		if (x==0) roomId=room->links[eLeft];
		if (y==4) roomId=room->links[eDown];
		if (x==11)roomId=room->links[eRight];
		result.moreInfo=room->level->screenGates[roomId-1][result.back];
	} else if (isIn(result,TILES_PRESSABLE)) {
		if (y==0)	roomId=room->links[eUp]; /*TODO: validate corners */
		if (x==0) roomId=room->links[eLeft];
		if (y==4) roomId=room->links[eDown];
		if (x==11)roomId=room->links[eRight];
		/* the case that a button is in tile 0 should never happen, but we'll care about it just in case */
		result.moreInfo=room->level->screenPressables[roomId-1][result.back];
	} else if (isIn(result,TILES_CHOPPER_SPIKE)) {
		if (y==0)	roomId=room->links[eUp]; /*TODO: validate corners */
		if (x==0) roomId=room->links[eLeft];
		if (y==4) roomId=room->links[eDown];
		if (x==11)roomId=room->links[eRight];
		result.moreInfo=room->level->screenDangers[roomId-1][result.back];
	}
	return result;
}

/* Wall functions */
int wallGetCase(tTile left, tTile tile, tTile right) {
	if (!isIn(tile,TILES_WALL)) { 
		return WALL_LOC_NONE;
	} else if (isIn(left,TILES_WALL)&&isIn(right,TILES_WALL)) { 
		return WALL_LOC_WWW;
	} else if ((!isIn(left,TILES_WALL))&&(isIn(right,TILES_WALL))) {
		return WALL_LOC_SWW;
	} else if ((isIn(left,TILES_WALL))&&(!isIn(right,TILES_WALL))) {
		return WALL_LOC_WWS;
	} else {
		return WALL_LOC_SWS;
	}
}
#define wallCase(a) (cases==(a))

/*
 * Drawing functions
 */

#define e(a,x,y) outputDrawBitmap(roomGfx.environment->pFrames[a],(x),(y))

/*#define isIn(a,TILES_UNPRESSED)  (((tPressable*)a.moreInfo)->action==eNormal)*/
#define chopperGetFrame(a) (((tDanger*)a.moreInfo)->frame)
#define gateGetFrame(a)    (((tGate*)a.moreInfo)->frame)
#define spikeGetFrame(a)   (((tDanger*)a.moreInfo)->frame)
#define looseGetFrame(a)   (((tDanger*)a.moreInfo)->frame)
#define wallGetInfo(a)     (wallGet(env,cases,(a),seed))
#define touchLoose(a)      (((tDanger*)a.moreInfo)->action=eLosMoving)

/* Special drawings */
#define drawGateTop(x,y,frame) outputDrawBitmap(roomGfx.environment->pFrames[35-((frame)&7)],x,y)
#define drawTorchFire(x,y) outputDrawBitmap(roomGfx.torch->pFrames[((room->level->time)+2*x+y)%(roomGfx.torch->frames)],x,y)
#define drawSword(x,y) outputDrawBitmap(roomGfx.sword->pFrames[(room->level->time%20)==0],x,y)
#define drawPotionSmallBase(x,y) outputDrawBitmap(roomGfx.potionBase->pFrames[0],x,y)
#define drawPotionBigBase(x,y) outputDrawBitmap(roomGfx.potionBase->pFrames[1],x,y)
#define drawPotionRedBubbles(x,y,isBig) outputDrawBitmap(roomGfx.potionAnimRed->pFrames[((room->level->time)+2*x+y)%(roomGfx.potionAnimRed->frames)],x,y-5*(isBig))
#define drawPotionGreenBubbles(x,y,isBig) outputDrawBitmap(roomGfx.potionAnimGreen->pFrames[((room->level->time)+2*x+y)%(roomGfx.potionAnimGreen->frames)],x,y-5*(isBig))
#define drawPotionBlueBubbles(x,y,isBig) outputDrawBitmap(roomGfx.potionAnimBlue->pFrames[((room->level->time)+2*x+y)%(roomGfx.potionAnimBlue->frames)],x,y-5*(isBig))

typedef enum {layTritop,layTribot,layBase}tLooseLayer;
void drawLoose(int x, int y, int frame,tLooseLayer layer) {
	static char looseAnimation[]={1,0,2,2,0,0,0,2,2,2,2};
	int base,tritop,tribot;
	if (frame<11) {
		switch(looseAnimation[frame]) {
			case 1:
				base=43;
				tritop=41;
				tribot=39;
				break;
			case 2:
				base=44;
				tritop=42;
				tribot=40;
				break;
			case 0:
			default:
				base=14;
				tritop=13;
				tribot=12;
				break;
		}
		switch(layer) {
			case layTritop:
				e(tritop-3,x,y);
				break;
			case layTribot:
				e(tribot-3,x,y);
				break;
			case layBase:
				e(base-3,x,y);
				break;
		}
	}
}

void drawUnlinkedLoose(int x, int y/*, int frame,tLooseLayer layer*/) {
	register int base,tritop,tribot;
	base=44;
	tritop=42;
	tribot=40;
/*	switch(layer) {
		case layTritop:
			e(tritop-3,x,y);
			break;
		case layTribot:
			e(tribot-3,x,y);
			break;
		case layBase:
			e(base-3,x,y);
			break;
	}*/
			e(tritop-3,x+TILE_W,y+2);
			e(tribot-3,x,y);
			e(base-3,x,y+3);
}


void drawGate(int x, int y, int frame) {
	/* frames are from 0 to 46, 0 is open; 46 is closed */
	register int i;
	register const int mod=frame&7;
	
	e(27-mod,x,y+mod);
	for (i=8;i<=frame;i+=8)
		e(20,x,y+i+mod);
	e(18,x,y+i+mod+4);
}

void drawExit(int x, int y, int frame) {
	/* Frame defined from 0 (open) to 50 (close) */
	register int i;
	if (frame<47) e(55,x,y+47);
	e(50,x,y+51);
	for (i=0;i<=frame;i+=4)
		e(1,x,y+i+(frame&3));
	e(2,x,y);
}

typedef enum {layFore=113,layRight=108,layBack=102}tSpikeLayer;
void drawSpike(int x, int y, int frame, tSpikeLayer layer) {
	if (frame) {
		frame--;
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
		e((int)layer+((frame>4)?(6-frame):frame),x,y);
	}
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
			e(97-frame,x,y);
			break;
		case layCBack:
			e(92-frame,x,y);
			if (frame<3)
				e(100-frame,x,y-60+outputGetHeight(roomGfx.environment->pFrames[100-frame]));
			break;
	}
}

/* End of special drawings */


/* main panel block */
void drawBackPanel(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);
	tTile left=roomGetTile(room,x-1,y);
	/* LEFT (left,tile) */
	
	/* Wall/left */
	if ((!isIn(tile,TILES_WALL))&&(isIn(left,TILES_WALL))) 
		e(63,(x-1)*TILE_W+0,y*TILE_H+2);
	/* Gate/left */
	if (isIn(left,TILES_DOOR)) {
		e(15,(x-1)*TILE_W+0,y*TILE_H+2);
		drawGate((x-1)*TILE_W+0,(y-1)*TILE_H+3,gateGetFrame(left));
	}
	/* normal/left */
	if (isIn(left,TILES_FLOOR)) 
		e(10,(x-1)*TILE_W+0,y*TILE_H+2);
	/* loose moving/left */
	if (isIn(left,TILES_LOOSEMOVING)) 
		drawLoose((x-1)*TILE_W+0,y*TILE_H+2,looseGetFrame(left),layTritop);
	/* exit_left/left */
	if (isIn(left,TILE_EXIT_RIGHT)) 
		e(7,(x-1)*TILE_W+0,y*TILE_H+2);
	/* pillar/left */
	if (isIn(left,TILES_PILLAR)) 
		e(44,(x-1)*TILE_W+0,y*TILE_H+2);
	/* pillar_big_up/left */
	if (isIn(left,TILE_BP_BOTTOM)) 
		e(83,(x-1)*TILE_W+0,y*TILE_H+2);
	if (isIn(left,TILE_BP_TOP)) 
		e(85,(x-1)*TILE_W+0,y*TILE_H+3);
	/* pressable/left */
	if (isIn(left,TILES_RAISE_PRESSED)) 
		e(10,(x-1)*TILE_W+0,y*TILE_H+2);
	if (isIn(left,TILES_RAISE_UNPRESSED)) 
		e(10,(x-1)*TILE_W+0,y*TILE_H+1);
	if (isIn(left,TILES_DROP_PRESSED)) 
		e(10,(x-1)*TILE_W+0,y*TILE_H+3);
	if (isIn(left,TILES_DROP_UNPRESSED)) 
		e(10,(x-1)*TILE_W+0,y*TILE_H+2);
	/* debris/left */
	if (isIn(left,TILES_BROKENTILE)) 
		e(49,(x-1)*TILE_W+0,y*TILE_H+2);
	/* spikes/left */
	if (isIn(left,TILES_SPIKES)) {
		e(107,(x-1)*TILE_W+0,y*TILE_H+2);
		drawSpike((x-2)*TILE_W+0,y*TILE_H,spikeGetFrame(left),layRight);
	}
	/* skeleton/left */
	if (isIn(left,TILES_SKELETON)) 
		e(81,(x-1)*TILE_W+0,y*TILE_H+2);
	/* torch/this */
	if (isIn(tile,TILES_TORCH)) { /* animation */
		drawTorchFire(x*TILE_W+11,y*TILE_H-39); 
		/* base */
		e(56,x*TILE_W+3,y*TILE_H-24);
	}
	/* chopper/this */
	if (isIn(tile,TILE_CHOPPER)) 
		drawChopper((x-1)*TILE_W+0,y*TILE_H,chopperGetFrame(tile),layCBack);
	/* empty_bricks/this */
	if (isIn(tile,TILES_BRICKE1)) 
		e(52,x*TILE_W+0,y*TILE_H-18);
	if (isIn(tile,TILES_BRICKE2)) 
		e(53,x*TILE_W+0,y*TILE_H-18);
	if (isIn(tile,TILES_WINDOW)) 
		e(54,x*TILE_W+0,y*TILE_H-18);
	/* floor_bricks/this */
	if (isIn(tile,TILES_BRICKF1)) 
		e(12,x*TILE_W+0,y*TILE_H-18);
	if (isIn(tile,TILES_BRICKF2)) 
		e(13,x*TILE_W+0,y*TILE_H-18);
	/* gate/this */
	if (isIn(tile,TILES_DOOR)) 
		e(14,(x-1)*TILE_W+0,y*TILE_H+0);
	/* gate_frame/this */
	if (isIn(tile,TILES_GATEFRAME)) 
		e(17,(x-1)*TILE_W+24,y*TILE_H+0);
	/* normal/this */
	if (isIn(tile,TILES_FLOOR)) 
		e(9,(x-1)*TILE_W+0,y*TILE_H+0);
	/* loose moving/this */
	if (isIn(tile,TILES_LOOSEMOVING)) 
		drawLoose((x-1)*TILE_W+0,y*TILE_H+0,looseGetFrame(tile),layTribot);
	/* exit_left/this */
	if (isIn(tile,TILE_EXIT_RIGHT)) 
		e(5,(x-1)*TILE_W+0,y*TILE_H+0);
	if (isIn(tile,TILE_EXIT_LEFT)) {
		e(9,(x-1)*TILE_W+0,y*TILE_H+0);
		drawExit(x*TILE_W+8,(y-1)*TILE_H-1,gateGetFrame(tile));
	}
	/* pillar/this */
	if (isIn(tile,TILES_PILLAR)) 
		e(43,(x-1)*TILE_W+0,y*TILE_H+0);
	/* big_pillar/this */
	if (isIn(tile,TILE_BP_BOTTOM)) 
		e(82,(x-1)*TILE_W+0,y*TILE_H+0);
	if (isIn(tile,TILE_BP_TOP)) 
		e(87,(x-1)*TILE_W+8,y*TILE_H+3);
	/* pressable/this */
	/* TODO: use boolean algebra to simplify this */
	if (isIn(tile,TILES_RAISE_UNPRESSED)&&isIn(left,TILES_WALKABLE)&&(!isIn(left,TILES_RAISE)))
		e(57,(x-1)*TILE_W+0,y*TILE_H);
	if ((isIn(tile,TILES_RAISE)&&((!isIn(tile,TILES_UNPRESSED))||(!isIn(left,TILES_WALKABLE))||isIn(left,TILES_RAISE)))&&(isIn(tile,TILES_UNPRESSED))) 
		e(58,(x-1)*TILE_W+0,y*TILE_H);
	if (((isIn(tile,TILES_RAISE)&&((!isIn(tile,TILES_UNPRESSED))||(!isIn(left,TILES_WALKABLE))||isIn(left,TILES_RAISE)))&&(!isIn(tile,TILES_UNPRESSED)))||isIn(tile,TILES_DROP_UNPRESSED))
		e(58,(x-1)*TILE_W+0,y*TILE_H+1);
						
	if (isIn(tile,TILES_DROP_PRESSED)) 
		e(58,(x-1)*TILE_W+0,y*TILE_H+2);
	/* debris/this */
	if (isIn(tile,TILES_BROKENTILE)) 
		e(48,(x-1)*TILE_W+0,y*TILE_H+0);
	/* spikes/this */
	if (isIn(tile,TILES_SPIKES)) {
		e(101,(x-1)*TILE_W+0,y*TILE_H+0);
		drawSpike((x-1)*TILE_W+0,y*TILE_H,spikeGetFrame(tile),layBack);
	}
	/* skeleton/this */
	if (isIn(tile,TILES_SKELETON)) 
		e(80,(x-1)*TILE_W+0,y*TILE_H+0);
	/* sword/left */
	if (isIn(left,TILES_SWORD)) 
		drawSword((x-1)*TILE_W-15,y*TILE_H-4);
	/* potion base/left */
	if (isIn(left,TILES_SMALLPOTION))
		drawPotionSmallBase((x-1)*TILE_W-15,y*TILE_H-4);
	if ((isIn(left,TILES_BIGPOTION))) 
		drawPotionBigBase((x-1)*TILE_W-15,y*TILE_H-4);
	/* potion animation/left */
	if (isIn(left,TILES_REDPOTION))
		drawPotionRedBubbles((x-1)*TILE_W+3-15,y*TILE_H-15,isIn(left,TILES_BIGPOTION));
	if (isIn(left,TILES_GREENPOTION))
		drawPotionGreenBubbles((x-1)*TILE_W+3-15,y*TILE_H-15,isIn(left,TILES_BIGPOTION));
	if (isIn(left,TILES_BLUEPOTION))
		drawPotionBlueBubbles((x-1)*TILE_W+3-15,y*TILE_H-15,isIn(left,TILES_BIGPOTION));
}

/* bottom panel block at background */
void drawBackBottomTile(tRoom* room,int x, int y) {
	tTile left=roomGetTile(room,x-1,y);
	tTile tile=roomGetTile(room,x,y);
	tTile right=roomGetTile(room,x+1,y);
	tTile dleft=roomGetTile(room,x-1,(y==3)?3:(y+1));
	int cases=wallGetCase(left,tile,right);

	/* TODO: fix this conditions to make miniterms */
	/* loose moving */
	if (isIn(tile,TILES_LOOSEMOVING)) 
		drawLoose((x-1)*TILE_W+0,y*TILE_H+3,looseGetFrame(tile),layBase);
	/* normal */
	if (isIn(tile,TILES_PRESSABLE)) 
		/* TODO: drop has resource 59 for unpressed/raise 47? checkgame */
		e(59,(x-1)*TILE_W+0,y*TILE_H+(isIn(tile,TILES_UNPRESSED)?3:4));
	if (isIn(tile,TILES_WALKABLE)&&(!isIn(tile,TILES_LOOSEMOVING))&&(!isIn(tile,TILES_PRESSABLE)))
		e(11,(x-1)*TILE_W+0,y*TILE_H+3);
	/* wall */
			/* there are 4 cases */
			if (wallCase(WALL_LOC_WWW))
				e(65,(x-1)*TILE_W+0,y*TILE_H+3);
			if (wallCase(WALL_LOC_SWW))
				e(71,(x-1)*TILE_W+0,y*TILE_H+3);
			if (wallCase(WALL_LOC_WWS))
				e(67,(x-1)*TILE_W+0,y*TILE_H+3);
			if (wallCase(WALL_LOC_SWS))
				e(69,(x-1)*TILE_W+0,y*TILE_H+3);
		if (isIn(tile,TILES_SPACE)) {
	/* empty */
			/* tile, dleft*/
			
			/* gate_frame/this */
			if (isIn(tile,TILES_GATEFRAME)) 
				e(42,(x-1)*TILE_W+0,y*TILE_H+3);
			/* gate/left */
			if (isIn(dleft,TILES_DOOR)) 
				e(16,(x-1)*TILE_W+0,y*TILE_H+3);
			/*	drawGateTop(x*TILE_W+0,(y-1)*TILE_H+3,gateGetFrame(tile));*/
			/* big_pillar/left */
			if (isIn(dleft,TILE_BP_TOP)) 
				e(86,(x-1)*TILE_W+0,y*TILE_H+3);
			/* pillar/left */
			if (isIn(dleft,TILES_PILLAR)) 
				e(45,(x-1)*TILE_W+0,y*TILE_H+3);
			/* wall/left */
			if (isIn(dleft,TILES_WALL)) 
				e(64,(x-1)*TILE_W+0,y*TILE_H+3);
		}
	/* upper objects */
	if (isIn(tile,TILE_EXIT_LEFT)) 
		e(6,x*TILE_W+0,(y-1)*TILE_H+3);
	if (isIn(tile,TILE_EXIT_RIGHT)) 
		e(8,x*TILE_W+0,(y-1)*TILE_H+3);
}

/* main panel block */
void drawForePanel(tRoom* room,int x, int y) {
	register short seed=room->id+(x-1)+(y-1)*10-1;
	int env=WALL_ENV_DUNGEON;
	tTile left=roomGetTile(room,x-1,y);
	tTile tile=roomGetTile(room,x,y);
	tTile right=roomGetTile(room,x+1,y);
	int cases=wallGetCase(left,tile,right);

	/* pillar */
	if (isIn(tile,TILES_PILLAR)) 
		e(46,x*TILE_W-24,y*TILE_H+0);
	/* big pillar */
	if (isIn(tile,TILE_BP_BOTTOM)) 
		e(84,x*TILE_W-24,y*TILE_H+0);
	/* wall */
	/* there are 4 cases */
	if (wallCase(WALL_LOC_WWW))
		e(66,(x-1)*TILE_W+0,y*TILE_H+0);
	if (wallCase(WALL_LOC_SWW))
		e(72,(x-1)*TILE_W+0,y*TILE_H+0);
	if (wallCase(WALL_LOC_WWS))
		e(68,(x-1)*TILE_W+0,y*TILE_H+0);
	if (wallCase(WALL_LOC_SWS))
		e(70,(x-1)*TILE_W+0,y*TILE_H+0);
		
	/* the seed generation algorithm */
	/* Upper row */
	if (wallGetInfo(WALL_TABLE_LINE1_DARKER))
		e(75,(x-1)*TILE_W+0,y*TILE_H-39);
	if (wallGetInfo(WALL_TABLE_LINE1_LEFT_DOWN))
		e(77,(x-1)*TILE_W+0,y*TILE_H-39);
	if (wallGetInfo(WALL_TABLE_LINE1_RIGHT_DOWN))
		e(79,(x-1)*TILE_W+24,y*TILE_H-39);
	if (wallGetInfo(WALL_TABLE_LINE1_RIGHT_UP))
		e(78,(x-1)*TILE_W+24,y*TILE_H-49);
	if (wallGetInfo(WALL_TABLE_LINE1_LEFT_UP))
		e(76,(x-1)*TILE_W+0,y*TILE_H-54);
	/* Second row */
	if (isIn(tile,TILES_WALL))
		e(74-wallGetInfo(WALL_TABLE_LINE2_SEP),(x-1)*TILE_W+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET),y*TILE_H-18);
	if (wallGetInfo(WALL_TABLE_LINE2_LEFT_DOWN))
		e(77,(x-1)*TILE_W+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)+5,y*TILE_H-39+21);
	if (wallGetInfo(WALL_TABLE_LINE2_RIGHT_DOWN))
		e(79,(x-1)*TILE_W+24+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)-32+5,y*TILE_H-39+21);
	if (wallGetInfo(WALL_TABLE_LINE2_RIGHT_UP))
		e(78,(x-1)*TILE_W+24+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)-32+5,y*TILE_H-49+21);
	if (wallGetInfo(WALL_TABLE_LINE2_LEFT_UP))
		e(76,(x-1)*TILE_W+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)+5,y*TILE_H-54+21);
	/* Third row TODO: send to BottomTile */
	if (isIn(tile,TILES_WALL))
		e(74-wallGetInfo(WALL_TABLE_LINE3_SEP),(x-1)*TILE_W+3+wallGetInfo(WALL_TABLE_LINE3_OFFSET),y*TILE_H+3);
	if (wallGetInfo(WALL_TABLE_LINE3_LEFT_UP))
		e(76,(x-1)*TILE_W+5+wallGetInfo(WALL_TABLE_LINE3_OFFSET)+5,y*TILE_H-55+21*2);

	/* debris/this foreground layer */
	if (isIn(tile,TILES_BROKENTILE)) 
		e(51,(x-1)*TILE_W+0,y*TILE_H+0);
	/* spikes/this foreground layer */
	if (isIn(tile,TILES_SPIKES)) 
		drawSpike((x-1)*TILE_W+0,y*TILE_H+0,spikeGetFrame(tile),layFore);
}	

/*
 * Drawing interfaces
 */

void roomDrawBackground(tRoom* room) {
	int x,y;
	tFlying* loose=room->level->flyingObjects;
	
	for (x=1;x<11;x++) {
	drawBackBottomTile(room,x,0);
		for (y=1;y<4;y++) {
			drawBackPanel(room,x,y);
			drawBackBottomTile(room,x,y);
		}
	}
	while (loose) { /* for each flying object check if it is in this room */
		if (loose->screen==room->id) /* if it is draw it */
			drawUnlinkedLoose(loose->x,loose->y);
		loose=loose->next;
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

/* kernel functions */

int roomPress(tRoom* room, tObject* obj) {
	/* Catching the press events done by an object to a room
	 * returns 0 if the room didn't change, 1 if it did
	 */

	int x=(obj->location/TILE_W)+1;
	int y=obj->floor+1;
	tTile tile=roomGetTile(room,x,y);
	tTile aux;
	int refresh=0;
	
	enum {border,nearborder,nearbordernotlooking,middle} where;
	int i;

	/* buttons */
	if (isIn(tile,TILES_PRESSABLE)) {
		tEvent* event;
		((tPressable*)tile.moreInfo)->action=eJustPressed;
		/* drop or raise button */
		event=((tPressable*)tile.moreInfo)->event;
#ifdef DEBUGROOM
		fprintf(stderr,"mapPressedTile: throw event from button %d event:%p\n",tile.back,(void*)event);
#endif
		do {
			event->gate->action=isIn(tile,TILES_RAISE)?eOpening:eClosingFast;
		} while	((event++)->triggerNext);
	}

	/* Loose tiles */
#ifdef DEBUGROOM
	printf("s=%d x=%d y=%d\n",s,x,y);
#endif
  if (isIn(tile,TILES_LOOSENORMAL)) 
		touchLoose(tile);
	
	/* spikes */
	/* there are 7 possibilities to be */
#define WHERE_NEAR 6
#define WHERE_IN 3
	where=border;i=1;
	if (((obj->location%TILE_W)<(TILE_W-WHERE_IN))&&obj->direction==DIR_LEFT){
		where=nearborder;i=1;}
	if (((obj->location%TILE_W)<(TILE_W-WHERE_IN))&&obj->direction==DIR_RIGHT){
		where=nearbordernotlooking;i=1;}
	if ((obj->location%TILE_W)<(TILE_W-WHERE_NEAR)){
		where=middle;}
	if (((obj->location%TILE_W)<WHERE_NEAR)&&obj->direction==DIR_LEFT){
		where=nearborder;i=-1;}
	if (((obj->location%TILE_W)<WHERE_NEAR)&&obj->direction==DIR_RIGHT){
		where=nearbordernotlooking;i=-1;}
	if ((obj->location%TILE_W)<WHERE_IN){
		where=border;i=-1;}

	if (where!=middle) { /* TODO: the first case is different.
	                      * try to make it as similar as possible */
		if (isIn(aux=roomGetTile(room,x+i,y),TILES_SPIKES)) {
			/* spikes left in this floor */
			tDanger* danger=aux.moreInfo;
			danger->action=eSpiUp;
		} else if ((y<3)&&isIn(roomGetTile(room,x+i,y),TILE_EMPTY)&&isIn(aux=roomGetTile(room,x+i,y+1),TILES_SPIKES)) {
			/* spikes left in the lower floor, there is
			 * a space so you can fall down */
			tDanger* danger=aux.moreInfo;
			danger->action=eSpiUp;
		} else if ((y<2)&&isIn(roomGetTile(room,x+i,y),TILE_EMPTY)&&isIn(roomGetTile(room,x+i,y+1),TILE_EMPTY)&&isIn(aux=roomGetTile(room,x+i,y+2),TILES_SPIKES)) {
			/* spikes left in the 2 level lower floor, there are
			 * spaces so you can fall down */
			tDanger* danger=aux.moreInfo;
			danger->action=eSpiUp;
		}
	}	

	return refresh;

}

void roomKidChangedFloor(tRoom* room, tObject* kid) {
	/* Choppers */
	tTile aux;
	int i,j;

#ifdef ROOMDEBUG
	printf("kid had changed the floor. Guards and choppers may be allerted.\n");
#endif
	for (j=1;j<4;j++) {
		for (i=1;i<10;i++) {
			if (isIn((aux=roomGetTile(room,i,j)),TILE_CHOPPER)) {
				tDanger* chopper=aux.moreInfo;
				chopper->action=((j-1)==kid->floor)?eChoActive:eChoInactive;
			}
		}
	}
}

