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
object.h: Free Prince : Objects
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
#include "maps.h" /* mapGetRoom getTile */
#include <stdio.h> /* NULL */
#include "states.h"
#include "object.h"

void loadGfx(int storeMirrored, tData** gfxCache, unsigned long resId) {
	gfxCache[DIR_LEFT]=resLoad(resId);
	if (storeMirrored)
		gfxCache[DIR_RIGHT]=resLoad(resId|RES_MODS_INVERT);
	else
		gfxCache[DIR_RIGHT]=NULL;
}

void objectFree(tObject obj) {
	resFree(obj.gfxCache[DIR_LEFT]);
	if (obj.gfxCache[DIR_RIGHT]) resFree(obj.gfxCache[DIR_RIGHT]);
}

/* TODO: make a function in maps.c that calls this one for the kid */
tObject objectCreate(int location, int floor, int direction, int stateId, unsigned long resId, int cacheMirror, tObjectType type) {
	tObject object;

	loadGfx(cacheMirror,object.gfxCache,resId);

	object.location=location;
	object.floor=floor;
	object.type=type;
	object.direction=direction;
	object.action=createState(stateId);
	return object;
}

void objectDraw(tObject object) {
	void* image=object.gfxCache[object.direction]->pFrames[stateGetImage(object)-1];
	/* TODO: move this -1 to each script frame */
	outputDrawBitmap(
		image, 
		object_getLocation(object,image),
		stateGetBottom(object)+58+object.floor*TILE_H
	);
}

int objectMove(tObject* object,tKey key,tRoom* room) {
	/* advance state and get the flag, then interpret the flag and do the events */
	short flags;
	int refresh;
	
	flags=stateUpdate(&key,object,room);

	if (room==NULL) return flags; /* exits if it is not associated to a room */

/* a static variable type in the tObject determinates what objet is it about. This is to simulate polymorphism.
 * call a function that performs all the actions knowing the room, the object and the flags. Returns refresh. */	
	switch (object->type) {
		case oKid:
			refresh=kidMove(object,flags,room);
			break;
		case oGeneric:
		default:
			refresh=0;
			break;
	}
	
	if (refresh) { /* room map was changed and needs to be refreshed */
		*room=mapGetRoom(room->level,room->id);
	}
	return flags;
}

