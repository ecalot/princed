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
#include <stdio.h> /* NULL */

static struct {
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
} kidGfx;

void loadGfx() {
	kidGfx.turning[DIR_LEFT]=resLoad(RES_ANIM_TURNING|RES_MODS_LEFT);
	kidGfx.turning[DIR_RIGHT]=resLoad(RES_ANIM_TURNING|RES_MODS_RIGHT);
	kidGfx.normal[DIR_LEFT]=resLoad(RES_ANIM_NORMAL|RES_MODS_LEFT);
	kidGfx.normal[DIR_RIGHT]=resLoad(RES_ANIM_NORMAL|RES_MODS_RIGHT);
	kidGfx.walking[DIR_LEFT]=resLoad(RES_ANIM_WALKING|RES_MODS_LEFT);
	kidGfx.walking[DIR_RIGHT]=resLoad(RES_ANIM_WALKING|RES_MODS_RIGHT);
	kidGfx.couching[DIR_LEFT]=resLoad(RES_ANIM_CROUCH|RES_MODS_LEFT);
	kidGfx.couching[DIR_RIGHT]=resLoad(RES_ANIM_CROUCH|RES_MODS_RIGHT);
	kidGfx.jumping[DIR_LEFT]=resLoad(RES_ANIM_JUMP|RES_MODS_LEFT);
	kidGfx.jumping[DIR_RIGHT]=resLoad(RES_ANIM_JUMP|RES_MODS_RIGHT);
	kidGfx.running[DIR_LEFT]=resLoad(RES_ANIM_RUN|RES_MODS_LEFT);
	kidGfx.running[DIR_RIGHT]=resLoad(RES_ANIM_RUN|RES_MODS_RIGHT);
	kidGfx.startrunning[DIR_LEFT]=resLoad(RES_ANIM_RUN_START|RES_MODS_LEFT);
	kidGfx.startrunning[DIR_RIGHT]=resLoad(RES_ANIM_RUN_START|RES_MODS_RIGHT);
	kidGfx.stoprunning[DIR_LEFT]=resLoad(RES_ANIM_RUN_STOP|RES_MODS_LEFT);
	kidGfx.stoprunning[DIR_RIGHT]=resLoad(RES_ANIM_RUN_STOP|RES_MODS_RIGHT);
	kidGfx.turnrunning[DIR_LEFT]=resLoad(RES_ANIM_RUN_TURN|RES_MODS_LEFT);
	kidGfx.turnrunning[DIR_RIGHT]=resLoad(RES_ANIM_RUN_TURN|RES_MODS_RIGHT);
	kidGfx.jumprunning[DIR_LEFT]=resLoad(RES_ANIM_JUMPRUN|RES_MODS_LEFT);
	kidGfx.jumprunning[DIR_RIGHT]=resLoad(RES_ANIM_JUMPRUN|RES_MODS_RIGHT);
}

void kidFree() {
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
}

/* TODO: send this function to maps.c */
tKid kidCreate() {
	tKid kid;

	if (kidGfx.turning[0]==NULL) loadGfx();

	kid.location=100;
	kid.floor=0;
	kid.direction=DIR_LEFT;
	kid.frame=0;
	kid.action=kidGfx.normal[DIR_LEFT];
	kid.nextAction=stay;
	kid.velocity=0;
	return kid;
}

void kidDraw(tKid kid) {
	outputDrawBitmap(kid.action->pFrames[kid.frame],kid.location,110+kid.floor*60); /* TODO: use TILE_H */
}

int kidMove(tKid* kid,tKey key/*,tRoom room*/) {
	/* Returns 1 if the action is done
	 * returns 0 if the action needs more time */

	key.status=key.status&(~K_Ctrl); /* Ignore control key */
				
	kid->frame++;
	if (kid->frame==kid->action->frames) {
		kid->frame=0;
		/* keylogIntercept(&key) --> here is the right place */
		/* Switch the right action knowing the actual status and set the velocity and action */
		/* stay context (walk,stay,stand,jump,turn)
		 * stay --> stay
		 * stay --> turn 
		 * stay --> run
		 * stay --> walk
		 * stay --> couch
		 * stay --> jump
		 * run context 
		 * run --> turnrun
		 * run --> run
		 * run --> jumprun
		 * run --> couchrun
		 *
		 * Note: falling and climbing context are ignored
		 */
		/*printf("next action=%d direction=%d keystatus=%x ",kid->nextAction,kid->direction,key.status);*/

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
		return 1;
	} else {
		/* TODO: tile traspassing and validations here
		 * use roomStep(tRoom room,x,y) roomTouch(tRoom room,x,y) to activate tile events
		 * use int roomKidValid(tRoom room,tKid kid) to validate kid position
		 */
		kid->location+=kid->velocity;
		return 0;
	}
}

