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
types.h: FreePrince : Kernel types
¯¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 23 Aug 2004

  Author:  Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _TYPES_H_
#define _TYPES_H_

#include "resources.h"

typedef char tRoomId;
typedef unsigned char tTileId;
typedef unsigned char tModId;

typedef struct {
	int status;
	int action;
} tGate;

typedef struct {
	tGate* gate;
	int triggerNext;
} tEvent;

typedef struct {
	tRoomId        links[4*24];
	unsigned char  fore [24*30];
	unsigned char  back [24*30];
	tGate**        screenGates[24];
	tGate*         gates;
	/*
	 * Active door handling here
	 * */
	int time;
	unsigned char start[3];
} tMap;

typedef struct {
	tMap*          level;
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
	int hasGateFrame;
	int hasSkeleton;
	int hasChopper;
	int hasBigPillar;
	int isExit;
	int isGate;
	int gateStatus;
	int bricks;
	int isPressable;
	int hasSpikes;
	int hasPillar;
	int hasFloor;
	int hasBrokenTile;
	int isWall;
	int hasSword;

	char code;
} tTile;

typedef struct {
	int location;
	int floor;
	int direction;
	int frame;
	int velocity;
	enum {stay,run}nextAction;
	tData* action;
} tKid;

typedef enum {eLeft=0,eRight=1,eUp=2,eDown=3}tDirection;

#endif
