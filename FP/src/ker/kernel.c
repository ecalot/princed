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
#include "titles.h"

int control(int optionflag,int level) {
	int i,location,direction,upIsPressed;
	SDL_Event e;
	tData* runningAnimation[4];
	tData* animation;
	tData* fondo;
	
	runningAnimation[0]=resLoad(RES_ANIM_RUN_LEFT);
	runningAnimation[1]=resLoad(RES_ANIM_RUN_RIGHT);
	runningAnimation[2]=resLoad(RES_ANIM_JUMPRUN_LEFT);
	runningAnimation[3]=resLoad(RES_ANIM_JUMPRUN_RIGHT);
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



#if 0
#include <stdio.h>
#include <SDL/SDL.h>

void quit()
{
	SDL_Quit();
	exit(0);
}

/* Initialize SDL: */
void init()
{
	if (SDL_Init(SDL_INIT_TIMER) == -1)
	{
		fprintf(stderr, "Unable to initialize SDL.");
		exit(1);
	}
}

/* Initialize screen: */
void start_screen()
{
	SDL_Surface *screen;

	screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		quit();
	}
}

/* Event loop: */	
void event_loop()
{
	SDL_Event event;
	
	while(SDL_WaitEvent(&event))
	{
		switch (event.type) {
		case SDL_KEYDOWN:
			fprintf(stderr, "The %s key was pressed! %d\n",
			SDL_GetKeyName(event.key.keysym.sym),event.key.keysym.sym);
			
			break;
		case SDL_KEYUP:
			fprintf(stderr, "The %s key was unpressed!\n",
			SDL_GetKeyName(event.key.keysym.sym));
			break;	
		case SDL_USEREVENT:
			fprintf(stderr, "Timer event!\n");
			break;
		case SDL_QUIT:
			fprintf(stderr, "Program exited normally.");
			return;
		}
	}
	fprintf(stderr, "Abnormal termination.");
}

Uint32 timer_callback(Uint32 interval, void *param)
{
	SDL_Event event;

	/* Push a user-defined event on the event queue: */
	event.type = SDL_USEREVENT;
	event.user.code = 0;
	event.user.data1 = NULL;
	event.user.data2 = NULL;
	SDL_PushEvent(&event);

	return interval; /* Timer must raise an alarm again after 'interval' ms. */
}

SDL_TimerID start_timer()
{
	return SDL_AddTimer(83, timer_callback, NULL);
}

void stop_timer(SDL_TimerID id)
{
	SDL_RemoveTimer(id);
}

int main(int argc, char argv[])
{
	SDL_TimerID id;

	init();
	start_screen();
	id = start_timer();

	/* Main loop: */
	event_loop();

	stop_timer(id);
	quit();
}
#endif
