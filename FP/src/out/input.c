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
input.c: FreePrince : Input interface
¯¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 19 Jun 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include <SDL/SDL.h>
#include "input.h"
#include <stdio.h> /* only for the printf debug */

void inputInitKey(tKey* key) {
	key->shiftPressed=0;
	key->controlPressed=0;
	key->upPressed=0;
	key->leftPressed=0;
	key->rightPressed=0;
	key->actionPerformed=none;
}

void editKey(tKey* key,SDLKey sdlkey,int status) {
	switch (sdlkey) {
		/* keys */
		case SDLK_UP:
			key->upPressed=status;
			break;
		case SDLK_DOWN:
			key->downPressed=status;
			break;					
		case SDLK_RIGHT:
			key->rightPressed=status;
			break;					
		case SDLK_LEFT:
			key->leftPressed=status;
			break;					
		case SDLK_HOME:
			key->upPressed=status;
			key->leftPressed=status;
			break;					
		case SDLK_END:
			key->downPressed=status;
			key->leftPressed=status;
			break;					
		case SDLK_PAGEUP:
			key->upPressed=status;
			key->rightPressed=status;
			break;					
		case SDLK_PAGEDOWN:
			key->downPressed=status;
			key->rightPressed=status;
			break;					
		/* mods*/
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			key->shiftPressed=status;
			break;					
		case SDLK_RCTRL:
		case SDLK_LCTRL:
			key->controlPressed=status;
			break;
		default:
			break;
	}	
}

int inputGetEvent(tKey* key) {
	SDL_Event event;
	
	while(SDL_WaitEvent(&event))
	{
		switch (event.type) {
		case SDL_KEYDOWN:
			/* if (editAction(key,event.key.keysym.sym)) return 0;  */
		case SDL_KEYUP:
			editKey(key,event.key.keysym.sym,event.type==SDL_KEYDOWN);
			break;
		case SDL_USEREVENT:
			return 1; /* A new time cicle has started! */
		case SDL_QUIT:
			key->actionPerformed=quit;
			return 0;
		}
	}
	printf("inputGetEvent: unreachable code\n");
	return 0;	
}

static SDL_TimerID timer;

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

void inputInitTimer()
{
	timer=SDL_AddTimer(83, timer_callback, NULL);
}

void inputStopTimer()
{
	SDL_RemoveTimer(timer);
}
