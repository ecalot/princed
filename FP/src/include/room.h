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
room.h: FreePrince : Draw Screen
¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 21 Jul 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _ROOM_H_
#define _ROOM_H_

#define TILE_W 32
#define TILE_H 63

#include "types.h"

tTile roomGetTile(tRoom* room,int x, int y);
void roomDrawBackground(tRoom* room);
void roomDrawForeground(tRoom* room);
void roomLoadGfx(long environment); /* Only to initialize environment by mapStart */

#define w1_darker(a)     ((a)&1)
#define w1_left_down(a)  ((a>>1)&1)
#define w1_right_down(a) ((a>>2)&1)
#define w1_right_up(a)   ((a>>3)&1)
#define w2_left_down(a)  ((a>>4)&1)
#define w2_left_up(a)    ((a>>5)&1)
#define w2_right_down(a) ((a>>6)&1)
#define w2_right_up(a)   ((a>>7)&1)

#define w2_sep(a)        ((a>>8)&1)
#define w3_sep(a)        ((a>>9)&1)
#define w2_offset(a)     ((a>>10)&7)
#define w3_offset(a)     ((a>>13)&7)

#define WALL_PROPERTIES {46592, 37187, 27904, 28168, 9217, 46208, 37632, 18752, 9760, 9768, 46338, 28544, 19200, 9217, 46592, 37632, 27905, 19200, 46592, 36865, 768, 768, 384, 512, 64, 256, 772, 256, 128, 512, 264, 257, 772, 16, 65, 770, 264, 256, 640, 0, 768, 768, 0, 512, 514, 256, 784, 512, 0, 513, 768, 256, 512}

#endif

