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
#ifdef NEW_KERNEL
	tData* kid[2];
#else
	tData* turning[2];
	tData* normal[2];
	tData* couching[2];
	tData* jumping[2];
	tData* walking[2];
	tData* running[2];
	tData* startrunning[2];
	tData* stoprunning[2];
	tData* turnrunning[2];
	tData* jumprunning[2];
#endif
} kidGfx;

void loadGfx() {
#ifdef NEW_KERNEL
	kidGfx.kid[DIR_LEFT]=resLoad(RES_IMG_ALL_KID);
	kidGfx.kid[DIR_RIGHT]=resLoad(RES_IMG_ALL_KID|RES_MODS_INVERT);
#else
	kidGfx.turning[DIR_LEFT]=resLoad(RES_ANIM_TURNING);
	kidGfx.turning[DIR_RIGHT]=resLoad(RES_ANIM_TURNING|RES_MODS_INVERT);
	kidGfx.normal[DIR_LEFT]=resLoad(RES_ANIM_NORMAL);
	kidGfx.normal[DIR_RIGHT]=resLoad(RES_ANIM_NORMAL|RES_MODS_INVERT);
	kidGfx.walking[DIR_LEFT]=resLoad(RES_ANIM_WALKING);
	kidGfx.walking[DIR_RIGHT]=resLoad(RES_ANIM_WALKING|RES_MODS_INVERT);
	kidGfx.couching[DIR_LEFT]=resLoad(RES_ANIM_CROUCH);
	kidGfx.couching[DIR_RIGHT]=resLoad(RES_ANIM_CROUCH|RES_MODS_INVERT);
	kidGfx.jumping[DIR_LEFT]=resLoad(RES_ANIM_JUMP);
	kidGfx.jumping[DIR_RIGHT]=resLoad(RES_ANIM_JUMP|RES_MODS_INVERT);
	kidGfx.running[DIR_LEFT]=resLoad(RES_ANIM_RUN);
	kidGfx.running[DIR_RIGHT]=resLoad(RES_ANIM_RUN|RES_MODS_INVERT);
	kidGfx.startrunning[DIR_LEFT]=resLoad(RES_ANIM_RUN_START);
	kidGfx.startrunning[DIR_RIGHT]=resLoad(RES_ANIM_RUN_START|RES_MODS_INVERT);
	kidGfx.stoprunning[DIR_LEFT]=resLoad(RES_ANIM_RUN_STOP);
	kidGfx.stoprunning[DIR_RIGHT]=resLoad(RES_ANIM_RUN_STOP|RES_MODS_INVERT);
	kidGfx.turnrunning[DIR_LEFT]=resLoad(RES_ANIM_RUN_TURN);
	kidGfx.turnrunning[DIR_RIGHT]=resLoad(RES_ANIM_RUN_TURN|RES_MODS_INVERT);
	kidGfx.jumprunning[DIR_LEFT]=resLoad(RES_ANIM_JUMPRUN);
	kidGfx.jumprunning[DIR_RIGHT]=resLoad(RES_ANIM_JUMPRUN|RES_MODS_INVERT);
#endif
}

void kidFree() {
#ifdef NEW_KERNEL
	resFree(kidGfx.kid[DIR_LEFT]);
	resFree(kidGfx.kid[DIR_RIGHT]);
#else
	resFree(kidGfx.turning[DIR_LEFT]);
	resFree(kidGfx.turning[DIR_RIGHT]);
	resFree(kidGfx.normal[DIR_LEFT]);
	resFree(kidGfx.normal[DIR_RIGHT]);
	resFree(kidGfx.walking[DIR_LEFT]);
	resFree(kidGfx.walking[DIR_RIGHT]);
	resFree(kidGfx.couching[DIR_LEFT]);
	resFree(kidGfx.couching[DIR_RIGHT]);
	resFree(kidGfx.jumping[DIR_LEFT]);
	resFree(kidGfx.jumping[DIR_RIGHT]);
	resFree(kidGfx.running[DIR_LEFT]);
	resFree(kidGfx.running[DIR_RIGHT]);
	resFree(kidGfx.startrunning[DIR_LEFT]);
	resFree(kidGfx.startrunning[DIR_RIGHT]);
	resFree(kidGfx.stoprunning[DIR_LEFT]);
	resFree(kidGfx.stoprunning[DIR_RIGHT]);
	resFree(kidGfx.turnrunning[DIR_LEFT]);
	resFree(kidGfx.turnrunning[DIR_RIGHT]);
	resFree(kidGfx.jumprunning[DIR_LEFT]);
	resFree(kidGfx.jumprunning[DIR_RIGHT]);
#endif
}

/* TODO: send this function to maps.c */
tKid kidCreate() {
	tKid kid;

	if (kidGfx.kid[0]==NULL) loadGfx();

	kid.location=30;
	kid.floor=1;
	kid.direction=DIR_RIGHT;
/*	kid.frame=0;
	kid.action=kidGfx.normal[DIR_LEFT];
	kid.nextAction=stay;
	kid.velocity=0;
*/
	kid.action=createState(1); /* level 1 */
	return kid;
}

void kidDraw(tKid kid) {
	outputDrawBitmap(
		kidGfx.kid[kid.direction]->pFrames[stateGetImage(kid)-1], /* TODO: move this -1 to each script frame */
		(kid.location*32)/10,
		58+kid.floor*TILE_H
	);
}

int kidMove(tKid* kid,tKey key,tRoom* room) {
#ifdef NEW_KERNEL
	short flags;
	flags=stateUpdate(&key,kid,room);
	if (flags&STATES_FLAG_P)
		mapPressedTile(
			room->level,
			roomGetTile(room,(kid->location/10)+1,kid->floor+1),
			room->id,
			(kid->location/TILE_W)+1,
			kid->floor+1
		);
	if (flags&STATES_FLAG_F)
		kid->floor++;
	return flags;
#else
	int result;
	tTile tile;
	/* Returns 1 if the action is done
	 * returns 0 if the action needs more time */

	key.status=key.status&(~K_Ctrl); /* Ignore control key */
				
	kid->frame++;
	if (kid->frame==kid->action->frames) {
		kid->frame=0;
		/* keylogIntercept(&key) --> here is the right place */
		/* Switch the right action knowing the actual status and set the velocity and action */
		/* STAY events
		 */
		
		if (kid->nextAction==stay) {
			if (key.status==(K_Shift|K_Left)) {
				if (kid->direction==DIR_LEFT) {
					/* walk left */
					kid->action=kidGfx.walking[DIR_LEFT];
					kid->velocity=-3;
				} else {
					/* turn left to right */
					kid->action=kidGfx.turning[DIR_LEFT]; /* RIGHT to LEFT */
					kid->velocity=0;
					kid->direction=DIR_LEFT;
				}
			} else if (key.status==(K_Shift|K_Right)) {
				if (kid->direction==DIR_RIGHT) {
					/* walk right */
					kid->action=kidGfx.walking[DIR_RIGHT];
					kid->velocity=+3;
				} else {
					/* turn right to left */
					kid->action=kidGfx.turning[DIR_RIGHT]; /* LEFT to RIGHT */
					kid->velocity=0;
					kid->direction=DIR_RIGHT;
				}
			} else if (key.status&K_Down) {
				/* couch */
				kid->action=kidGfx.couching[kid->direction];
				kid->velocity=0;
			} else if (key.status&K_Up) {
				/* jump */
				kid->action=kidGfx.jumping[kid->direction];
				kid->floor--;
				kid->velocity=0;
			} else if (key.status&K_Left) {
				if (kid->direction==DIR_LEFT) {
					/* run left */
					kid->action=kidGfx.startrunning[DIR_LEFT];
					kid->velocity=-6;
					kid->nextAction=run;
				} else {
					/* turn left to right */
					kid->action=kidGfx.turning[DIR_LEFT]; /* RIGHT to LEFT */
					kid->velocity=0;
					kid->direction=DIR_LEFT;
				}
			} else if (key.status==K_Right) {
				if (kid->direction==DIR_RIGHT) {
					/* run right */
					kid->action=kidGfx.startrunning[DIR_RIGHT];
					kid->velocity=+6;
					kid->nextAction=run;
				} else {
					/* turn right to left */
					kid->action=kidGfx.turning[DIR_RIGHT]; /* LEFT to RIGHT */
					kid->velocity=0;
					kid->direction=DIR_RIGHT;
				}
			} else {
				/* normal */
				kid->velocity=0;
				kid->action=kidGfx.normal[kid->direction];
			}
			/* TODO: elseif for the K_Left and K_Right to start jumping. Change nextAction */
		} else if (kid->nextAction==run) {

			/* RUN events
			 */

			if (key.status&K_Left) {
				if (kid->direction==DIR_LEFT) {
					/* run left */
					kid->action=kidGfx.running[DIR_LEFT];
					kid->velocity=-6;
					kid->nextAction=run;
				} else {
					/* turn left to right */
					kid->action=kidGfx.turnrunning[DIR_RIGHT]; /* RIGHT to LEFT */
					kid->velocity=0;
					kid->direction=DIR_LEFT;
				}
			} else if (key.status==K_Right) {
				if (kid->direction==DIR_RIGHT) {
					/* run right */
					kid->action=kidGfx.running[DIR_RIGHT];
					kid->velocity=+6;
					kid->nextAction=run;
				} else {
					/* turn right to left */
					kid->action=kidGfx.turnrunning[DIR_LEFT]; /* LEFT to RIGHT */
					kid->velocity=0;
					kid->direction=DIR_RIGHT;
				}
			} else if (!key.status) {
				/* Stop running */
				kid->velocity=kid->velocity/3;
				kid->action=kidGfx.stoprunning[kid->direction];
				kid->nextAction=stay;
			} else if ((key.status&K_Up)==K_Up) {
				/* Jump running */
				kid->action=kidGfx.jumprunning[kid->direction];
				kid->velocity+=kid->velocity/2; /* 50% faster */
			} else if ((key.status&K_Down)==K_Down) {
				/* Coach running */
				kid->action=kidGfx.couching[kid->direction];
				kid->velocity=kid->velocity/2; /* 50% slower */
			}
		}
		result=1;
	} else {
		kid->location+=kid->velocity;
		result=0;
	}

	/* TODO: tile traspassing and validations here
	 * use roomStep(tRoom room,x,y) roomTouch(tRoom room,x,y) to activate tile events
	 * use int roomKidValid(tRoom room,tKid kid) to validate kid position
	 */
	if (kid->location<0) {
		kid->location=10*TILE_W;
		*room=mapGetRoom((void*)(room->level),room->links[eLeft]);
	} else if (kid->location>10*TILE_W) {
		*room=mapGetRoom((void*)(room->level),room->links[eRight]);
		kid->location=0;
	}
	tile=roomGetTile(room,(kid->location/TILE_W)+1,kid->floor+1);
	if (!tile.walkable) {
		fprintf(stderr,"kidMove: Tile not walkable, falling\n");
		kid->floor++;
	} else {
		mapPressedTile(room->level, tile, room->id, (kid->location/TILE_W)+1, kid->floor+1);
	}
	if (kid->floor<0) {
		kid->floor=2;
		*room=mapGetRoom((void*)(room->level),room->links[eUp]);
	} else if (kid->floor>2) {
		*room=mapGetRoom((void*)(room->level),room->links[eDown]);
		kid->floor=0;
	}
	return result;
#endif
}

