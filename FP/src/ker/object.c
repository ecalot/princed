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
kid.h: Free Prince : Kid and other objects
¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 19 Jul 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "kid.h"
#include "output.h"
#include "resources.h" /* resLoad/resFree */
#include "room.h"
#include "maps.h" /* getTile */
#include <stdio.h> /* NULL */
#include "states.h"

#define NEW_KERNEL

void loadGfx(int storeMirrored, tData** gfxCache, unsigned long resId) {
	gfxCache[DIR_LEFT]=resLoad(resId);
	if (storeMirrored)
		gfxCache[DIR_RIGHT]=resLoad(resId|RES_MODS_INVERT);
	else
		gfxCache[DIR_RIGHT]=NULL;
}

void objectFree(tData** gfxCache) {
	resFree(gfxCache[DIR_LEFT]);
	if (gfxCache[DIR_RIGHT]) resFree(gfxCache[DIR_RIGHT]);
}

/* TODO: make a function in maps.c that calls this one */
tObject objectCreate(int location, int floor, int direction, int stateId, unsigned long resId,int cacheMirror) {
	tObject object;

	loadGfx(cacheMirror,object.gfxCache,resId);

	object.location=location;
	object.floor=floor;
	object.direction=direction;
	object.action=createState(stateId);
	return object;
}

int objectVerifyRoom(tObject *kid,tRoom *room) {
	/* if the kid is out of the screen we need to change the screen and put
	 * the kid back again on it
	 * PRE: tObject *kid is a kid
	 */
	
	int refresh=0;
	
	/* The kid is down */
	if (kid->floor==4) {
		kid->floor=0;
		room->id=room->links[eDown];
		refresh=1;
	}
	
	/* The kid is up */
	if (kid->floor==-1) {
		printf("pasó: kf=0 ahora es 3, cambio el id del room y refresco\n");
		kid->floor=2;
		room->id=room->links[eUp];
		refresh=1;
	}

	/* The kid is left */
	if (kid->location<0) {
		kid->location+=TILE_W*10;
		room->id=room->links[eLeft];
		refresh=1;
	}

	/* The kid is right */
	if (kid->location>TILE_W*10) {
		kid->location-=TILE_W*10;
		room->id=room->links[eRight];
		refresh=1;
	}

	return refresh;
}
	
#define kid_getLocation(kid,image) ((kid).location-(outputGetWidth(image)>>1))

void objectDraw(tObject kid) {
	void* image=kid.gfxCache[kid.direction]->pFrames[stateGetImage(kid)-1];
	/* TODO: move this -1 to each script frame */
	outputDrawBitmap(
		image, 
		kid_getLocation(kid,image),
		58+kid.floor*TILE_H
	);
}

int objectMove(tObject* kid,tKey key,tRoom* room) {
	/* advance state and get the flag, then interpret the flag and do the events */
	short flags;
	int refresh=0;
	int x;
	
	flags=stateUpdate(&key,kid,room);

	if (room==NULL) return flags; /* exits if it is not associated to a room */
	
	x=kid_getLocation(*kid,kid->gfxCache[kid->direction]->pFrames[stateGetImage(*kid)-1])/TILE_W;
	
	if (flags&STATES_FLAG_P)
		refresh=mapPressedTile(
			room->level,
			roomGetTile(room,x+1,kid->floor+1),
			room->id,
			x+1,
			kid->floor+1
		);
printf("f era %d. ",kid->floor);
	if (flags&STATES_FLAG_F)
		kid->floor++;
	if (flags&STATES_FLAG_U)
		kid->floor--;
printf("f pasa a ser %d\n",kid->floor);
	refresh=objectVerifyRoom(kid,room)||refresh;
	
	if (refresh) { /* room map was changed and needs to be refreshed */
		*room=mapGetRoom(room->level,room->id);
	}
	return flags;
}

