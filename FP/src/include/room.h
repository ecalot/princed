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
		e(15,(x)+0,(y)+2);\
		drawGate((x)+0,(y)-TILE_H+3,gateGetFrame(left));\
	}\
	/* normal/left */\
	if (isIn(left,TILES_FLOOR)) \
		e(10,(x)+0,(y)+2);\
	/* loose moving/left */\
	if (isIn(left,TILES_LOOSEMOVING)) \
		drawLoose((x)+0,(y)+2,looseGetFrame(left),layTritop);\
	/* exit_left/left */\
	if (isIn(left,TILE_EXIT_RIGHT)) \
		e(7,(x)+0,(y)+2);\
	/* pillar/left */\
	if (isIn(left,TILES_PILLAR)) \
		e(44,(x)+0,(y)+2);\
	/* pillar_big_up/left */\
	if (isIn(left,TILE_BP_BOTTOM)) \
		e(83,(x)+0,(y)+2);\
	if (isIn(left,TILE_BP_TOP)) \
		e(85,(x)+0,(y)+3);\
	/* pressable/left */\
	if (isIn(left,TILES_RAISE_PRESSED)) \
		e(10,(x)+0,(y)+2);\
	if (isIn(left,TILES_RAISE_UNPRESSED)) \
		e(10,(x)+0,(y)+1);\
	if (isIn(left,TILES_DROP_PRESSED)) \
		e(10,(x)+0,(y)+3);\
	if (isIn(left,TILES_DROP_UNPRESSED)) \
		e(10,(x)+0,(y)+2);\
	/* debris/left */\
	if (isIn(left,TILES_BROKENTILE)) \
		e(49,(x)+0,(y)+2);\
	/* spikes/left */\
	if (isIn(left,TILES_SPIKES)) {\
		e(107,(x)+0,(y)+2);\
		drawSpike((x)-TILE_W+0,(y),spikeGetFrame(left),layRight);\
	}\
	/* skeleton/left */\
	if (isIn(left,TILES_SKELETON)) \
		e(81,(x)+0,(y)+2);\
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
		e(63,(x)+0,(y)+2);\
	/* torch/this */\
	if (isIn(tile,TILES_TORCH)) { /* animation */\
		drawTorchFire((x)+11+TILE_W,(y)-39); \
		/* base */\
		e(56,(x)+3+TILE_W,(y)-24);\
	}\
	/* chopper/this */\
	if (isIn(tile,TILE_CHOPPER)) \
		drawChopper((x)+0,(y),chopperGetFrame(tile),layCBack);\
	/* empty_bricks/this */\
	if (isIn(tile,TILES_BRICKE1)) \
		e(52,(x)+TILE_W+0,(y)-18);\
	if (isIn(tile,TILES_BRICKE2)) \
		e(53,(x)+TILE_W+0,(y)-18);\
	if (isIn(tile,TILES_WINDOW)) \
		e(54,(x)+TILE_W+0,(y)-18);\
	/* floor_bricks/this */\
	if (isIn(tile,TILES_BRICKF1)) \
		e(12,(x)+TILE_W+0,(y)-18);\
	if (isIn(tile,TILES_BRICKF2)) \
		e(13,(x)+TILE_W+0,(y)-18);\
	/* gate/this */\
	if (isIn(tile,TILES_DOOR)) \
		e(14,(x)+0,(y)+0);\
	/* gate_frame/this */\
	if (isIn(tile,TILES_GATEFRAME)) \
		e(17,(x)+24,(y)+0);\
	/* normal/this */\
	if (isIn(tile,TILES_FLOOR)) \
		e(9,(x)+0,(y)+0);\
	/* loose moving/this */\
	if (isIn(tile,TILES_LOOSEMOVING)) \
		drawLoose((x)+0,(y)+0,looseGetFrame(tile),layTribot);\
	/* exit_left/this */\
	if (isIn(tile,TILE_EXIT_RIGHT)) \
		e(5,(x)+0,(y)+0);\
	if (isIn(tile,TILE_EXIT_LEFT)) {\
		e(9,(x)+0,(y)+0);\
		drawExit((x)+TILE_W+8,(y)-TILE_H-1,gateGetFrame(tile));\
	}\
	/* pillar/this */\
	if (isIn(tile,TILES_PILLAR)) \
		e(43,(x)+0,(y)+0);\
	/* big_pillar/this */\
	if (isIn(tile,TILE_BP_BOTTOM)) \
		e(82,(x)+0,(y)+0);\
	if (isIn(tile,TILE_BP_TOP)) \
		e(87,(x)+8,(y)+3);\
	/* pressable/this */\
	/* TODO: use boolean algebra to simplify this */\
	if (isIn(tile,TILES_RAISE_UNPRESSED)&&isIn(left,TILES_WALKABLE)&&(!isIn(left,TILES_RAISE)))\
		e(57,(x)+0,(y));\
	if ((isIn(tile,TILES_RAISE)&&((!isIn(tile,TILES_UNPRESSED))||(!isIn(left,TILES_WALKABLE))||isIn(left,TILES_RAISE)))&&(isIn(tile,TILES_UNPRESSED))) \
		e(58,(x)+0,(y));\
	if (((isIn(tile,TILES_RAISE)&&((!isIn(tile,TILES_UNPRESSED))||(!isIn(left,TILES_WALKABLE))||isIn(left,TILES_RAISE)))&&(!isIn(tile,TILES_UNPRESSED)))||isIn(tile,TILES_DROP_UNPRESSED))\
		e(58,(x)+0,(y)+1);\
	if (isIn(tile,TILES_DROP_PRESSED)) \
		e(58,(x)+0,(y)+2);\
	/* debris/this */\
	if (isIn(tile,TILES_BROKENTILE)) \
		e(48,(x)+0,(y)+0);\
	/* spikes/this */\
	if (isIn(tile,TILES_SPIKES)) {\
		e(101,(x)+0,(y)+0);\
		drawSpike((x)+0,(y),spikeGetFrame(tile),layBack);\
	}\
	/* skeleton/this */\
	if (isIn(tile,TILES_SKELETON)) \
		e(80,(x)+0,(y)+0);

#define drawAllBack(x,y,left,tile,right,dleft) \
	/* loose moving */\
	if (isIn(tile,TILES_LOOSEMOVING)) \
		drawLoose((x)+0,(y)+3,looseGetFrame(tile),layBase);\
	/* normal */\
	if (isIn(tile,TILES_PRESSABLE)) \
		/* TODO: drop has resource 59 for unpressed/raise 47? checkgame */\
		e(59,(x)+0,(y)+(isIn(tile,TILES_UNPRESSED)?3:4));\
	if (isIn(tile,TILES_WALKABLE)&&(!isIn(tile,TILES_LOOSEMOVING))&&(!isIn(tile,TILES_PRESSABLE)))\
		e(11,(x)+0,(y)+3);\
	/* wall */\
	/* there are 4 cases */\
	if (wallCase(WALL_LOC_WWW))\
		e(65,(x)+0,(y)+3);\
	if (wallCase(WALL_LOC_SWW))\
		e(71,(x)+0,(y)+3);\
	if (wallCase(WALL_LOC_WWS))\
		e(67,(x)+0,(y)+3);\
	if (wallCase(WALL_LOC_SWS))\
		e(69,(x)+0,(y)+3);\
	/* empty */\
	/* tile, dleft*/\
	\
	/* gate_frame/this */\
	if (isIn(tile,TILES_SPACE)&&isIn(tile,TILES_GATEFRAME)) \
		e(42,(x)+0,(y)+3);\
	/* gate/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILES_DOOR)) \
		e(16,(x)+0,(y)+3);\
	/* big_pillar/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILE_BP_TOP)) \
		e(86,(x)+0,(y)+3);\
	/* pillar/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILES_PILLAR)) \
		e(45,(x)+0,(y)+3);\
	/* wall/left */\
	if (isIn(tile,TILES_SPACE)&&isIn(dleft,TILES_WALL)) \
		e(64,(x)+0,(y)+3);\
\
	/* upper objects */\
	if (isIn(tile,TILE_EXIT_LEFT)) \
		e(6,(x)+0+TILE_W,(y)-TILE_H+3);\
	if (isIn(tile,TILE_EXIT_RIGHT)) \
		e(8,(x)+0+TILE_W,(y)-TILE_H+3);


#define drawAllFore(x,y,left,tile,right) \
	/* pillar */\
	if (isIn(tile,TILES_PILLAR)) \
		e(46,(x)+8,(y)+0);\
	/* big pillar */\
	if (isIn(tile,TILE_BP_BOTTOM)) \
		e(84,(x)+8,(y)+0);\
	/* wall */\
	/* there are 4 cases */\
	if (wallCase(WALL_LOC_WWW))\
		e(66,(x)+0,(y)+0);\
	if (wallCase(WALL_LOC_SWW))\
		e(72,(x)+0,(y)+0);\
	if (wallCase(WALL_LOC_WWS))\
		e(68,(x)+0,(y)+0);\
	if (wallCase(WALL_LOC_SWS))\
		e(70,(x)+0,(y)+0);\
		\
	/* the seed generation algorithm */\
	/* Upper row */\
	if (wallGetInfo(WALL_TABLE_LINE1_DARKER))\
		e(75,(x)+0,(y)-39);\
	if (wallGetInfo(WALL_TABLE_LINE1_LEFT_DOWN))\
		e(77,(x)+0,(y)-39);\
	if (wallGetInfo(WALL_TABLE_LINE1_RIGHT_DOWN))\
		e(79,(x)+24,(y)-39);\
	if (wallGetInfo(WALL_TABLE_LINE1_RIGHT_UP))\
		e(78,(x)+24,(y)-49);\
	if (wallGetInfo(WALL_TABLE_LINE1_LEFT_UP))\
		e(76,(x)+0,(y)-54);\
	/* Second row */\
	if (isIn(tile,TILES_WALL))\
		e(74-wallGetInfo(WALL_TABLE_LINE2_SEP),(x)+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET),(y)-18);\
	if (wallGetInfo(WALL_TABLE_LINE2_LEFT_DOWN))\
		e(77,(x)+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)+5,(y)-39+21);\
	if (wallGetInfo(WALL_TABLE_LINE2_RIGHT_DOWN))\
		e(79,(x)+24+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)-32+5,(y)-39+21);\
	if (wallGetInfo(WALL_TABLE_LINE2_RIGHT_UP))\
		e(78,(x)+24+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)-32+5,(y)-49+21);\
	if (wallGetInfo(WALL_TABLE_LINE2_LEFT_UP))\
		e(76,(x)+7+wallGetInfo(WALL_TABLE_LINE2_OFFSET)+5,(y)-54+21);\
	/* Third row TODO: send to BottomTile */\
	if (isIn(tile,TILES_WALL))\
		e(74-wallGetInfo(WALL_TABLE_LINE3_SEP),(x)+3+wallGetInfo(WALL_TABLE_LINE3_OFFSET),(y)+3);\
	if (wallGetInfo(WALL_TABLE_LINE3_LEFT_UP))\
		e(76,(x)+5+wallGetInfo(WALL_TABLE_LINE3_OFFSET)+5,(y)-55+21*2);\
\
	/* debris/this foreground layer */\
	if (isIn(tile,TILES_BROKENTILE)) \
		e(51,(x)+0,(y)+0);\
	/* spikes/this foreground layer */\
	if (isIn(tile,TILES_SPIKES)) \
		drawSpike((x)+0,(y)+0,spikeGetFrame(tile),layFore);\

#endif

