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
/*
typedef struct {
	int location;
	int floor;
	int direction;
	int framesLeft;
	tData* action;
} tKid;
*/

#define KID_ACTION_NOTHING  0
#define KID_ACTION_WALKING  1
#define KID_ACTION_CLIMBING 2
#define KID_ACTION_RUNNING  3

#define KID_ACTION_JUMPING  4
#define KID_ACTION_RIGHT    8
#define KID_ACTION_STARTING 16
#define KID_ACTION_ENDING   32

static tData kidGfx[48];

/* TODO: send this function to maps.c*/
void kidInit(tKid* kid) {
	kid->location=100;
	kid->floor=0;
	kid->direction=1;
	kid->framesLeft=0;
/*	if (kidGfx[0]==NULL) loadGfx();*/
	action=&(kidGfx[KID_ACTION_NOTHING|KID_ACTION_RIGHT]);
}


int kidDraw(tKid* kid) {
	/* Returns 1 if the action is done
	 * returns 0 if the action needs more time */

	outputDraw(location,floor*20,kid->action->pFrames[frame]);
	frame++;
	if (frame==kid->action->frames) {
		frame=0;
		return 1;
	} else {
		return 0;
	}
}

void kidMove(tKid* kid,tKey* key/*,tRoom room*/) {

}

