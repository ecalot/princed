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

#define TILE_W 32
#define TILE_H 62

#include "types.h"

tTile roomGetTile(tRoom* room,int x, int y);
void roomDrawBackground(tRoom* room);
void roomDrawForeground(tRoom* room);
void roomLoadGfx(long environment); /* Only to initialize environment by mapStart */
int roomPress(tRoom* room, tObject* obj);
void roomKidChangedFloor(tRoom* room, tObject* kid);

#define drawAllLeft(x,y,left,tile) \
	/* Gate/left */\
	if (isIn(left,TILES_DOOR)) {\
		e((x)+0,(y)+2,15);\
		drawGate((x)+0,(y)-TILE_H+3,gateGetFrame(left));\
	}\
	/* normal/left */\
	if (isIn(left,TILES_FLOOR)) \
		e((x)+0,(y)+2,10);\
	/* loose moving/left */\
	if (isIn(left,TILES_LOOSEMOVING)) \
		drawLoose((x)+0,(y)+2,looseGetFrame(left),layTritop);\
	/* exit_left/left */\
	if (isIn(left,TILE_EXIT_RIGHT)) \
		e((x)+0,(y)+2,7);\
	/* pillar/left */\
	if (isIn(left,TILES_PILLAR)) \
		e((x)+0,(y)+2,44);\
	/* pillar_big_up/left */\
	if (isIn(left,TILE_BP_BOTTOM)) \
		e((x)+0,(y)+2,83);\
	if (isIn(left,TILE_BP_TOP)) \
		e((x)+0,(y)+3,85);\
	/* pressable/left */\
	if (isIn(left,TILES_RAISE_PRESSED)) \
		e((x)+0,(y)+2,10);\
	if (isIn(left,TILES_RAISE_UNPRESSED)) \
		e((x)+0,(y)+1,10);\
	if (isIn(left,TILES_DROP_PRESSED)) \
		e((x)+0,(y)+3,10);\
	if (isIn(left,TILES_DROP_UNPRESSED)) \
		e((x)+0,(y)+2,10);\
	/* debris/left */\
	if (isIn(left,TILES_BROKENTILE)) \
		e((x)+0,(y)+2,49);\
	/* spikes/left */\
	if (isIn(left,TILES_SPIKES)) {\
		e((x)+0,(y)+2,107);\
		drawSpike((x)-TILE_W+0,(y),spikeGetFrame(left),layRight);\
	}\
	/* skeleton/left */\
	if (isIn(left,TILES_SKELETON)) \
		e((x)+0,(y)+2,81);\
	/* sword/left */\
	if (isIn(left,TILES_SWORD)) \
		drawSword((x)-15,(y)-4);\
	/* potion base/left */\
	if (isIn(left,TILES_SMALLPOTION))\
		drawPotionSmallBase((x)-15,(y)-4);\
	if ((isIn(left,TILES_BIGPOTION))) \
		drawPotionBigBase((x)-15,(y)-4);\
	/* potion animation/left */\
	if (isIn(left,TILES_REDPOTION))\
		drawPotionRedBubbles((x)+3-15,(y)-15,isIn(left,TILES_BIGPOTION));\
	if (isIn(left,TILES_GREENPOTION))\
		drawPotionGreenBubbles((x)+3-15,(y)-15,isIn(left,TILES_BIGPOTION));\
	if (isIn(left,TILES_BLUEPOTION))\
		drawPotionBlueBubbles((x)+3-15,(y)-15,isIn(left,TILES_BIGPOTION));

#define drawAllTile(x,y,left,tile) \
	/* Wall/left */\
	if ((!isIn(tile,TILES_WALL))&&(isIn(left,TILES_WALL))) \
		e((x)+0,(y)+2,63);\
	/* torch/this */\
	if (isIn(tile,TILES_TORCH)) { /* animation */\
		drawTorchFire((x)+11+TILE_W,(y)-39); \
		/* base */\
		e((x)+3+TILE_W,(y)-24,56);\
	}\
	/* chopper/this */\
	if (isIn(tile,TILE_CHOPPER)) \
		drawChopper((x)+0,(y),chopperGetFrame(tile),layCBack);\
	/* empty_bricks/this */\
	if (isIn(tile,TILES_BRICKE1)) \
		e((x)+TILE_W+0,(y)-18,52);\
	if (isIn(tile,TILES_BRICKE2)) \
		e((x)+TILE_W+0,(y)-18,53);\
	if (isIn(tile,TILES_WINDOW)) \
		e((x)+TILE_W+0,(y)-18,54);\
	/* floor_bricks/this */\
	if (isIn(tile,TILES_BRICKF1)) \
		e((x)+TILE_W+0,(y)-18,12);\
	if (isIn(tile,TILES_BRICKF2)) \
		e((x)+TILE_W+0,(y)-18,13);\
	/* gate/this */\
	if (isIn(tile,TILES_DOOR)) \
		e((x)+0,(y)+0,14);\
	/* gate_frame/this */\
	if (isIn(tile,TILES_GATEFRAME)) \
		e((x)+24,(y)+0,17);\
	/* normal/this */\
	if (isIn(tile,TILES_FLOOR)) \
		e((x)+0,(y)+0,9);\
	/* loose moving/this */\
	if (isIn(tile,TILES_LOOSEMOVING)) \
		drawLoose((x)+0,(y)+0,looseGetFrame(tile),layTribot);\
	/* exit_left/this */\
	if (isIn(tile,TILE_EXIT_RIGHT)) \
		e((x)+0,(y)+0,5);\
	if (isIn(tile,TILE_EXIT_LEFT)) {\
		e((x)+0,(y)+0,9);\
		drawExit((x)+TILE_W+8,(y)-TILE_H-1,gateGetFrame(tile));\
	}\
	/* pillar/this */\
	if (isIn(tile,TILES_PILLAR)) \
		e((x)+0,(y)+0,43);\
	/* big_pillar/this */\
	if (isIn(tile,TILE_BP_BOTTOM)) \
		e((x)+0,(y)+0,82);\
	if (isIn(tile,TILE_BP_TOP)) \
		e((x)+8,(y)+3,87);\
	/* pressable/this */\
	/* TODO: use boolean algebra to simplify this */\
	if (isIn(tile,TILES_RAISE_UNPRESSED)&&isIn(left,TILES_WALKABLE)&&(!isIn(left,TILES_RAISE)))\
		e((x)+0,(y),57);\
	if ((isIn(tile,TILES_RAISE)&&((!isIn(tile,TILES_UNPRESSED))||(!isIn(left,TILES_WALKABLE))||isIn(left,TILES_RAISE)))&&(isIn(tile,TILES_UNPRESSED))) \
		e((x)+0,(y),58);\
	if (((isIn(tile,TILES_RAISE)&&((!isIn(tile,TILES_UNPRESSED))||(!isIn(left,TILES_WALKABLE))||isIn(left,TILES_RAISE)))&&(!isIn(tile,TILES_UNPRESSED)))||isIn(tile,TILES_DROP_UNPRESSED))\
		e((x)+0,(y)+1,58);\
	if (isIn(tile,TILES_DROP_PRESSED)) \
		e((x)+0,(y)+2,58);\
	/* debris/this */\
	if (isIn(tile,TILES_BROKENTILE)) \
		e((x)+0,(y)+0,48);\
	/* spikes/this */\
	if (isIn(tile,TILES_SPIKES)) {\
		e((x)+0,(y)+0,101);\
		drawSpike((x)+0,(y),spikeGetFrame(tile),layBack);\
	}\
	/* skeleton/this */\
	if (isIn(tile,TILES_SKELETON)) \
		e((x)+0,(y)+0,80);

#define drawAllBack(x,y,left,tile,right,dleft) \
	/* loose moving */\
	if (isIn(tile,TILES_LOOSEMOVING)) \
		drawLoose((x)+0,(y)+3,looseGetFrame(tile),layBase);\
	/* normal */\
	if (isIn(tile,TILES_PRESSABLE)) \
		/* TODO: drop has resource 59 for unpressed/raise 47? checkgame */\
		e((x)+0,(y)+(isIn(tile,TILES_UNPRESSED)?3:4),59);\
	if (isIn(tile,TILES_WALKABLE)&&(!isIn(tile,TILES_LOOSEMOVING))&&(!isIn(tile,TILES_PRESSABLE)))\
		e((x)+0,(y)+3,11);\
	/* wall */\
	/* there are 4 cases */\
	if (wallCase(WALL_LOC_WWW))\
		e((x)+0,(y)+3,65);\
	if (wallCase(WALL_LOC_SWW))\
		e((x)+0,(y)+3,71);\
	if (wallCase(WALL_LOC_WWS))\
		e((x)+0,(y)+3,67);\
	if (wallCase(WALL_LOC_SWS))\
		e((x)+0,(y)+3,69);\
	/* empty */\
	/* tile, dleft*/\
	\
	/* gate_frame/this */\
	if (isIn(tile,TILES_SPACE)&&isIn(tile,TILES_GATEFRAME)) \
		e((x)+0,(y)+3,42);\
	/* gate/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILES_DOOR)) \
		e((x)+0,(y)+3,16);\
	/* big_pillar/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILE_BP_TOP)) \
		e((x)+0,(y)+3,86);\
	/* pillar/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILES_PILLAR)) \
		e((x)+0,(y)+3,45);\
	/* wall/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILES_WALL)) \
		e((x)+0,(y)+3,64);\
\
	/* upper objects */\
	if (isIn(tile,TILE_EXIT_LEFT)) \
		e((x)+0+TILE_W,(y)-TILE_H+3,6);\
	if (isIn(tile,TILE_EXIT_RIGHT)) \
		e((x)+0+TILE_W,(y)-TILE_H+3,8);


#define drawAllFore(x,y,left,tile,right) \
	/* pillar */\
	if (isIn(tile,TILES_PILLAR)) \
		e((x)+8,(y)+0,46);\
	/* big pillar */\
	if (isIn(tile,TILE_BP_BOTTOM)) \
		e((x)+8,(y)+0,84);\
	/* wall */\
	/* there are 4 cases */\
	if (wallCase(WALL_LOC_WWW))\
		e((x)+0,(y)+0,66);\
	if (wallCase(WALL_LOC_SWW))\
		e((x)+0,(y)+0,72);\
	if (wallCase(WALL_LOC_WWS))\
		e((x)+0,(y)+0,68);\
	if (wallCase(WALL_LOC_SWS))\
		e((x)+0,(y)+0,70);\
		\
	/* the seed generation algorithm */\
	/* Upper row */\
	if (wallGetInfo(WALL_TABLE_LINE1_DARKER))\
		e((x)+0,(y)-39,75);\
	if (wallGetInfo(WALL_TABLE_LINE1_LEFT_DOWN))\
		e((x)+0,(y)-39,77);\
	if (wallGetInfo(WALL_TABLE_LINE1_RIGHT_DOWN))\
		e((x)+24,(y)-39,79);\
	if (wallGetInfo(WALL_TABLE_LINE1_RIGHT_UP))\
		e((x)+24,(y)-49,78);\
	if (wallGetInfo(WALL_TABLE_LINE1_LEFT_UP))\
		e((x)+0,(y)-54,76);\
	/* Second row */\
	if (isIn(tile,TILES_WALL))\
		e((x)+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET),(y)-18,74-wallGetInfo(WALL_TABLE_LINE2_SEP));\
	if (wallGetInfo(WALL_TABLE_LINE2_LEFT_DOWN))\
		e((x)+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)+5,(y)-39+21,77);\
	if (wallGetInfo(WALL_TABLE_LINE2_RIGHT_DOWN))\
		e((x)+24+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)-32+5,(y)-39+21,79);\
	if (wallGetInfo(WALL_TABLE_LINE2_RIGHT_UP))\
		e((x)+24+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)-32+5,(y)-49+21,78);\
	if (wallGetInfo(WALL_TABLE_LINE2_LEFT_UP))\
		e((x)+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)+5,(y)-54+21,76);\
	/* Third row TODO: send to BottomTile */\
	if (isIn(tile,TILES_WALL))\
		e((x)+3+wallGetInfo(WALL_TABLE_LINE3_OFFSET),(y)+3,74-wallGetInfo(WALL_TABLE_LINE3_SEP));\
	if (wallGetInfo(WALL_TABLE_LINE3_LEFT_UP))\
		e((x)+5+wallGetInfo(WALL_TABLE_LINE3_OFFSET)+5,(y)-55+21*2,76);\
\
	/* debris/this foreground layer */\
	if (isIn(tile,TILES_BROKENTILE)) \
		e((x)+0,(y)+0,51);\
	/* spikes/this foreground layer */\
	if (isIn(tile,TILES_SPIKES)) \
		drawSpike((x)+0,(y)+0,spikeGetFrame(tile),layFore);\

#endif

