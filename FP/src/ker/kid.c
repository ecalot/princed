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
kid.c: Free Prince : Kid object
¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 19 Jul 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "kid.h"
#include "room.h"
#include <stdio.h> /* DEBUG printf */
#include "states.h"
/* #include "maps.h" * mapPressedTile */


int kidVerifyRoom(tObject *kid,tRoom *room) {
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
	
int kidMove(tObject* kid,short flags,tRoom* room) {
	int refresh=0;
	
/*	x=object_getLocation(*kid,kid->gfxCache[kid->direction]->pFrames[stateGetImage(*kid)-1])/TILE_W;*/
	
	if (flags&STATES_FLAG_P)
/*		refresh=mapPressedTile(
			room->level,
			roomGetTile(room,x+1,kid->floor+1),
			room->id,
			x+1,
			kid->floor+1
		);*/
		refresh=roomPress(room,kid);
printf("f era %d. ",kid->floor);
	if (flags&STATES_FLAG_F)
		kid->floor++;
	if (flags&STATES_FLAG_U)
		kid->floor--;
printf("f pasa a ser %d\n",kid->floor);
	return kidVerifyRoom(kid,room)||refresh;
}
	
