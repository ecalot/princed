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
drawscreen.c: FreePrince : Draw Screen
¯¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 18 Jul 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include <stdlib.h>
#include "kernel.h"
#include "resources.h"
#include "output.h"
#include "room.h"
#include "maps.h"

void drawScreen(/*tRoom room, */int layer,int level) { /* TODO: rename it to roomDraw */
	static int frame=0;
	int i,x,y,fase;
	tTile tile;
	static tData* torch;
	static tData* environment;

/* draws the screen where the kid is
 * if layer is 0 the background is loaded
 * if layer is 1 the foreground columns are loaded
 * if layer is 2 the torches are drawn
 * if layer is 3 the columns that need to be redrawn are drawn
 *
 * if layer is 4 the function loads the vdungeon enviorment
 *             5                        vpalace
 */

	switch (layer&0x0f) {
		case 0:
			/* Draw and initialize the background */
			frame=0;
			for(x=0;x<5;x++) {
				for (y=0;y<12;y++) {
					/*tile=levelGetTile(room,x,y);*/
					printf("%c",'A'+tile.code);
				/*drawTile((i%10)*20,(i/10)*20,map[i]);*/
				}
				printf("\n");
			}
			break;
		case 1:
			break;
		case 2:
			if (frame==torch->frames) frame=0;
			fase=0;
			for (i=0;i<30;i++) {
/*				if (map[i]==torch) {								
					outputDraw((i%10)*20,(i/10)*20,torch.pFrame[(frame+fase)%torch.frames]);
					fase+=2;
				}*/
			}
			frame++;
			break;
		case 3:
			break;
		case 4:
		case 5:
			if (environment!=NULL) {
				resFree(environment);
				if (layer&0x1) {
					environment=resLoad(RES_IMG_ENV_DUNGEON);
				} else {
					/*environment=resLoad(RES_IMG_ENV_PALACE);*/
				}
			}
			if (torch==NULL) {
				torch=resLoad(RES_ANIM_TORCH);
			}
			break;
		case 6:
			if (torch!=NULL) {
				resFree(torch);
				torch=NULL;
			}
			if (environment!=NULL) {
				resFree(environment);
				environment=NULL;
			}
			break;
	}

	
}

tTile roomGetTile(tRoom* room,int x, int y) {
	tTile result;
	return result;
}

