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
#include <stdio.h> /* NULL */

static struct {
	tData* turning[2];
	tData* normal[2];
	tData* couch[2];
	tData* jump[2];
	tData* walking[2];
	tData* running[2];
} kidGfx;

void loadGfx() {
	kidGfx.turning[DIR_LEFT]=resLoad(RES_ANIM_TURNING|RES_MOD_LEFT);
	kidGfx.turning[DIR_RIGHT]=resLoad(RES_ANIM_TURNING|RES_MOD_RIGHT);
	kidGfx.normal[DIR_LEFT]=resLoad(RES_ANIM_NORMAL|RES_MOD_LEFT);
	kidGfx.normal[DIR_RIGHT]=resLoad(RES_ANIM_NORMAL|RES_MOD_RIGHT);
	kidGfx.walking[DIR_LEFT]=resLoad(RES_ANIM_WALKING|RES_MOD_LEFT);
	kidGfx.walking[DIR_RIGHT]=resLoad(RES_ANIM_WALKING|RES_MOD_RIGHT);
	kidGfx.couch[DIR_LEFT]=resLoad(RES_ANIM_COUCH|RES_MOD_LEFT);
	kidGfx.couch[DIR_RIGHT]=resLoad(RES_ANIM_COUCH|RES_MOD_RIGHT);
	kidGfx.jump[DIR_LEFT]=resLoad(RES_ANIM_JUMP|RES_MOD_LEFT);
	kidGfx.jump[DIR_RIGHT]=resLoad(RES_ANIM_JUMP|RES_MOD_RIGHT);
	kidGfx.running[DIR_LEFT]=resLoad(RES_ANIM_RUN|RES_MOD_LEFT);
	kidGfx.running[DIR_RIGHT]=resLoad(RES_ANIM_RUN|RES_MOD_RIGHT);
}


/* TODO: send this function to maps.c*/
tKid kidCreate() {
	tKid kid;
	kid.location=100;
	kid.floor=0;
	kid.direction=DIR_LEFT;
	kid.frame=0;
	if (kidGfx.turning[0]==NULL) loadGfx();
	kid.action=kidGfx.normal[DIR_LEFT];
	kid.nextAction=stay;
	kid.velocity=0;
	return kid;
}


void kidDraw(tKid kid) {
	outputDrawBitmap(kid.action->pFrames[kid.frame],kid.location,kid.floor*20);
}

int kidMove(tKid* kid,tKey key/*,tRoom room*/) {
	/* Returns 1 if the action is done
	 * returns 0 if the action needs more time */

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
		printf("next action=%d direction=%d ",kid->nextAction,kid->direction);
		if (kid->nextAction==stay) {
			if (key.status==(K_Shift|K_Left)) {
				if (kid->direction==DIR_LEFT) {
					/* walk left */
								printf("WALK! ");
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
				kid->action=kidGfx.couch[kid->direction];
				kid->velocity=0;
			} else if (key.status&K_Up) {
				/* jump */
				kid->action=kidGfx.jump[kid->direction];
				kid->velocity=0;
			} else if (!key.status) {
					/* normal */
					kid->velocity=0;
					kid->action=kidGfx.normal[kid->direction];
			} else if (key.status&K_Left) {
				if (kid->direction==DIR_LEFT) {
					/* run left */
					kid->action=kidGfx.running[DIR_LEFT];
					kid->velocity=-6;
				} else {
					/* turn left to right */
					kid->action=kidGfx.turning[DIR_LEFT]; /* RIGHT to LEFT */
					kid->velocity=0;
					kid->direction=DIR_LEFT;
				}
			} else if (key.status==K_Right) {
				if (kid->direction==DIR_RIGHT) {
					/* run right */
					kid->action=kidGfx.running[DIR_RIGHT];
					kid->velocity=+6;
				} else {
					/* turn right to left */
					kid->action=kidGfx.turning[DIR_RIGHT]; /* LEFT to RIGHT */
					kid->velocity=0;
					kid->direction=DIR_RIGHT;
				}
			}
			/* TODO: elseif for the K_Left and K_Right to start jumping. Change nextAction */
		}
		printf("Velocity=%d\n",kid->velocity);
		return 1;
	} else {
		/* TODO: tile traspassing and validations here
		 * use roomStep(tRoom room,x,y) roomTouch(tRoom room,x,y) to activate tile events */
		kid->location+=kid->velocity;
		return 0;
	}
}

