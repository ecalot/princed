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
kernel.c: FreePrince : Main Kernel
¯¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 16 Jun 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include <SDL/SDL.h>
#include <stdlib.h>
#include "kernel.h"
#include "resources.h"
#include "output.h"
#include "input.h"
#include "titles.h"

int control(int optionflag,int level) {
	int i,location,direction,upIsPressed;
	SDL_Event e;
	tData* runningAnimation[4];
	tData* animation;
	tData* fondo;
	
	runningAnimation[0]=resLoad(RES_ANIM_RUN);
	runningAnimation[1]=resLoad(RES_ANIM_RUN|RES_MOD_RIGHT);
	runningAnimation[2]=resLoad(RES_ANIM_JUMPRUN);
	runningAnimation[3]=resLoad(RES_ANIM_JUMPRUN|RES_MOD_RIGHT);
	fondo=resLoad(RES_IMG_BACKGROUND);
	if (!fondo) {
		printf("The resource couldn't be loaded!\n");
		return 1;
	}
	/*
	printf("Resource number: %d. Frames: %d. Type: %d.\n",
		RES_ANIM_RUN_LEFT,
		runningAnimation->frames,
		runningAnimation->type
	);
	*/
	i=0;
	location=160;
	direction=0;
	animation=runningAnimation[0];
	upIsPressed=0;
	while (1) {
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					return 1;
				case SDL_KEYDOWN:
					/*fprintf(stderr, "The %s key was pressed! %d\n",
					SDL_GetKeyName(e.key.keysym.sym),e.key.keysym.sym);*/
					switch (e.key.keysym.sym) {
						case SDLK_LEFT:
							direction=0;
							break;
						case SDLK_RIGHT:
							direction=1;
							break;
						case SDLK_UP:
							i=0;
							upIsPressed=1;
							break;
						case SDLK_DOWN:
							i=0;
							break;
						case SDLK_q:
							return 1;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym) {
						case SDLK_UP:
								upIsPressed=0;
								break;
						default:
								break;
					}
			}
		}
		outputClearScreen();
		outputDrawBitmap(fondo->pFrames[0], 0, 0);
		outputDrawBitmap(animation->pFrames[i], location, 141);
		outputUpdateScreen();
		i++;
		SDL_Delay(50);
		animation=runningAnimation[(upIsPressed<<1)|(direction)];
		if (direction) {
			location+=3;
		} else {
			location-=3;
		}
		printf("up=%d\n",upIsPressed);
		if (i>animation->frames-1) i =6;
	}

	return 0;
}

/*
 * Main function
 */

int kernel(int optionflag,int level) {
/* levels=-1 is default
 * levels from 0 to n is the level number
 *  
 * optionflag may be read using hasFlag(name_flag); Note that the variable
 * must be called optionflag
 */
	
	int menuOption;
	int quit=0;
	if (outputInit()) {
		fprintf(stderr, "Unable to initialize screen: %s\n", SDL_GetError());
		exit(1);
	}

	/*
	 * Start main menu loop (story and titles)
	 */
	do {
		if (level==-1) {
			menuOption=showTitles();
			switch (menuOption) {
				case sLoad:
					level=8; /* TODO: make read level function */
					break;
				case sStart:
					level=1;
					break;
				case sQuit:
					quit=1;
					break;
			}
		}
		if (!quit) {
			quit=control(optionflag,level);
		}
	} while(!quit);
	outputStop();
	return 0;
}

