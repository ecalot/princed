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
������
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

#define drawAll(x,y,a) outputDrawBitmap(roomGfx.environment->pFrames[a],(x),(y))

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
				drawAll(x,y,tritop-3);
				break;
			case layTribot:
				drawAll(x,y,tribot-3);
				break;
			case layBase:
				drawAll(x,y,base-3);
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
			drawAll(x,y,tritop-3);
			break;
		case layTribot:
			drawAll(x,y,tribot-3);
			break;
		case layBase:
			drawAll(x,y,base-3);
			break;
	}*/
	drawAll(x+TILE_W,y+2,tritop-3);
	drawAll(x,y,tribot-3);
	drawAll(x,y+3,base-3);
}


void drawGate(int x, int y, int frame) {
	/* frames are from 0 to 46, 0 is open; 46 is closed */
	register int i;
	register const int mod=frame&7;
	
	drawAll(x,y+mod,27-mod);
	for (i=8;i<=frame;i+=8)
		drawAll(x,y+i+mod,20);
	drawAll(x,y+i+mod+4,18);
}

void drawExit(int x, int y, int frame) {
	/* Frame defined from 0 (open) to 50 (close) */
	register int i;
	if (frame<47) drawAll(x,y+47,55);
	drawAll(x,y+51,50);
	for (i=0;i<=frame;i+=4)
		drawAll(x,y+i+(frame&3),1);
	drawAll(x,y,2);
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
		drawAll(x,y,(int)layer+((frame>4)?(6-frame):frame));
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
			drawAll(x,y,97-frame);
			break;
		case layCBack:
			drawAll(x,y,92-frame);
			if (frame<3)
				drawAll(x,y-60+outputGetHeight(roomGfx.environment->pFrames[100-frame]),100-frame);
			break;
	}
}

/* End of special drawings */

/* main panel block */
void drawBackPanel(tRoom* room,int x, int y) {
	tTile tile=roomGetTile(room,x,y);
	tTile left=roomGetTile(room,x-1,y);
	drawAllLeft((x-1)*TILE_W,y*TILE_H,left,tile,dummy,dummy);
	drawAllTile((x-1)*TILE_W,y*TILE_H,left,tile,dummy,dummy);
}

/* bottom panel block at background */
void drawBackBottomTile(tRoom* room,int x, int y) {
	tTile left=roomGetTile(room,x-1,y);
	tTile tile=roomGetTile(room,x,y);
	tTile right=roomGetTile(room,x+1,y);
	tTile dleft=roomGetTile(room,x-1,(y==3)?3:(y+1));
	int cases=wallGetCase(left,tile,right);
	drawAllBack((x-1)*TILE_W,y*TILE_H,left,tile,right,dleft); 
}

/* main panel block */
void drawForePanel(tRoom* room,int x, int y) {
	register short seed=room->id+(x-1)+(y-1)*10-1;
	int env=WALL_ENV_DUNGEON;
	tTile left=roomGetTile(room,x-1,y);
	tTile tile=roomGetTile(room,x,y);
	tTile right=roomGetTile(room,x+1,y);
	int cases=wallGetCase(left,tile,right);
	drawAllFore((x-1)*TILE_W,y*TILE_H,left,tile,right,dummy);
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

