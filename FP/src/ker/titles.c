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
titles.c: FreePrince : Titles, animation and presentation
¯¯¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 18 Jul 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "output.h"
#include "titles.h"
#include "input.h"
#include <stdio.h> /* debug printf */

tMenuOption getAction(tKey key) {
	switch(key.actionPerformed) {
	case quit:
		return menuQuit;
	case load:
		return menuLoad;
	default:
		return menuStart;
	}
}

tMenuOption sleep(int ticks) {
	/* Wait ticks or a key is pressed if an action is thrown process it */
	tKey key=inputCreateKey();
	while (ticks) {
		ticks=inputDelay(&key,ticks);
		if (
			/* there are ticks remaining (a non-temporal action was thrown) */
			ticks &&
	 		/* there is an action */
			key.actionPerformed!=none &&
			/* the action wasn't control key */
			!(inputGetCtrl(key.status) && key.actionPerformed==other)
			) 
			return getAction(key); /* return the results of this action */ 
	}
	return menuNone;
}

#define goodBye {resFree(main_title);return result;}
	
tMenuOption showTitles() {
/* Show the titles animation
 * returns 0 if the user has finished the animations with quit
 * returns 1 if the user has selected to load a saved game 
 * returns 2 if the user has selected to start the game
 */

	tData *main_title;
	/*tData *main_text;*/
	tMenuOption result;

	main_title = resLoad(RES_IMG_MAIN_BACKGROUND);
	if (! main_title) {
		printf("The resource couldn't be loaded!\n");
		return menuQuit;
	}
	outputClearScreen();
	/* The main background */
	outputDrawBitmap(main_title->pFrames[0], 0, 200);
	outputUpdateScreen();
	result=sleep(12);
	if (result!=menuNone) goodBye;
	/* The presents */
	outputDrawBitmap(main_title->pFrames[1], 100, 100);
	outputUpdateScreen();
	result=sleep(12);
	if (result!=menuNone) goodBye;
	/* The author*/
	outputClearScreen();
	outputDrawBitmap(main_title->pFrames[0], 0, 200);
	outputDrawBitmap(main_title->pFrames[2], 100, 100);
	outputUpdateScreen();
	result=sleep(12);
	if (result!=menuNone) goodBye;
	/* The game name*/
	outputClearScreen();
	outputDrawBitmap(main_title->pFrames[0], 0, 200);
	outputDrawBitmap(main_title->pFrames[3], 30, 70);
	outputDrawBitmap(main_title->pFrames[4], 30, 190);
	outputUpdateScreen();
	/*SDL_Delay(1050);*/
	result=sleep(12);
	if (result!=menuNone) goodBye;

	/*
	 * The outputDraw doesn't support the text files yet
	 * 
	main_text = resLoad(RES_IMG_TEXT_BACKGROUND);
	if (! main_text) {
		printf("The resource couldn't be loaded!\n");
		return menuQuit;
	}
	*/
	/* The presents */
	/*
	outputDrawBitmap(main_text->pFrames[0], 0, 0);
	outputDrawBitmap(main_text->pFrames[1], 0, 0);
	outputUpdateScreen();
	*/
	do {
		result=sleep(1000);
	} while (result==menuNone);

	goodBye;
}	

