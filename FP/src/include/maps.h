/*  FreePrince - POP1 remake
    Copyright (C) 2003, 2004 Princed Development Team

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
maps.c: Freeprince : Map handling library
¯¯¯¯¯¯
 Copyright 2003,2004 Princed Development Team
  Created: 20 Jun 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _MAPS_H_
#define _MAPS_H_

#include "map_defs.h"

typedef enum {eLeft=0,eRight=1,eUp=2,eDown=3}tDirection;

int levelUse(void* level);
/* Sets the level for future use using the resource given
 * Initializes the position in the beginning of the level
 *
 * Return 0 on success
 */

int levelGetScreenWalls(unsigned char* data,unsigned char* borders);
/* nt levelGetRoom (tRoom* room); */
/* This will save a screen map into data.
 * 
 * data is a pre-allocated array with 10x3=30 bytes with the screen map
 * dumped over it in left-to-right/upper-to-lower oreder. 
 *
 * borders is another pre-allocated array with 10 upper bytes, 10 bottom
 * bytes, 3 left-side bytes, 3 right-side bytes and 4 bytes for the
 * corners. This is another 30 bytes. The order has to be determined!
 * In case the screen doesn't exist the 0x00 (free foreground) byte will
 * be returned for the bottom and right sides, the wall foreground for
 * the left side and the simple tile byte for the top side.
 */

int levelGetScreenItems(unsigned char* data,unsigned char* borders);
/* This will save a screen map modifiers into data.
 *
 * data and borders are in the same form as Walls but they contain
 * additional modifiers that must be defined in maps.h.
 * e.g. MAPS_ITEMS_DOOROPEN 0x01
 */

int levelMoveScreen(tDirection direction);
/* Moves the position screen that is inside the library
 *
 * Returns 0 id the screen didn't exist and the screen id
 * in case the screen was moved
 */
 
int levelGetGuards(/* TODO: add modifiers */);
int levelGetDoorMap(/* TODO: idem */);

int levelGetStarPosition(int* screen, int* position); /* TODO: define
		         position as an int or using x,y coordinate system*/

int levelGetInformation(int *thisScreen, unsigned char* LRUDscreens, unsigned char* cornerScreens);
/* TODO: define the format of cornerscreens */

#endif

