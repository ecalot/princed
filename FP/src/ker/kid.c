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
kid.h: Free Prince : Kid object
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

static struct {
	tData* kid[2];
} kidGfx;

void loadGfx(int storeMirrored) {
	kidGfx.kid[DIR_LEFT]=resLoad(RES_IMG_ALL_KID);
	if (storeMirrored)
		kidGfx.kid[DIR_RIGHT]=resLoad(RES_IMG_ALL_KID|RES_MODS_INVERT);
}

void kidFree() {
	resFree(kidGfx.kid[DIR_LEFT]);
	resFree(kidGfx.kid[DIR_RIGHT]);
}

/* TODO: send this function to maps.c */
tKid kidCreate() {
	tKid kid;

	if (kidGfx.kid[0]==NULL) loadGfx(1);

	kid.location=30;
	kid.floor=1;
	kid.direction=DIR_RIGHT;
	kid.action=createState(stateKidInLevel(1)); /* level 1 */
	return kid;
}

int kidVerifyRoom(tKid *kid,tRoom *room) {
	/* if the kid is out of the screen we need to change the screen and put
	 * the kid back again on it
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

void kidDraw(tKid kid) {
	void* image=kidGfx.kid[kid.direction]->pFrames[stateGetImage(kid)-1];
	/* TODO: move this -1 to each script frame */
	outputDrawBitmap(
		image, 
		kid_getLocation(kid,image),
		58+kid.floor*TILE_H
	);
}

int kidMove(tKid* kid,tKey key,tRoom* room) {
	/* advance state and get the flag, then interpret the flag and do the events */
	short flags;
	int refresh=0;
	int x;
	
	flags=stateUpdate(&key,kid,room);

	if (room==NULL) return flags;
	
	x=kid_getLocation(*kid,kidGfx.kid[kid->direction]->pFrames[stateGetImage(*kid)-1])/TILE_W;
	
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
	refresh=kidVerifyRoom(kid,room)||refresh;
	
	if (refresh) { /* room map was changed and needs to be refreshed */
		*room=mapGetRoom(room->level,room->id);
	}
	return flags;
}

