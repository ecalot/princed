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

#include <stdlib.h>
#include <stdio.h>
#include "kernel.h"
#include "resources.h"
#include "output.h"
#include "input.h"
#include "titles.h"
#include "kid.h"
#include "room.h"

/*
 * Main game control function
 */

int playgame(int optionflag,int level) {
	tKey   key=inputCreateKey();
	tKid   kid=kidCreate();

/* Game loop here */
	drawScreen(4,level);
/* Level loop here */
	outputClearScreen(); /* TODO: send to drawScreen(0) */
	/*drawScreen(0); TODO: try to optimize what to draw */
	while (1) {
		if (inputGetEvent(&key)) {
			/* Time event */

			/* Moving objects */
			/* keylogIntercept(&key);
			 * TODO: send to the real place where
			 * the key is interpreted in kid object
			 */
			kidMove(&kid,key);
			
			/* Drawing functions */
			outputClearScreen(); /* TODO: send to drawScreen(0) */
			drawScreen(0,0);
			drawScreen(1,0);
			drawScreen(2,0);
			kidDraw(kid);
			drawScreen(3,0);
			outputUpdateScreen();
		} else {
			/* Action event */
			switch (key.actionPerformed) {
			case quit:
				return 1;
			case gotoTitles:
				return 0;
			default:
				break;
			}
		}
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
		fprintf(stderr, "Unable to initialize screen\n");
		exit(1);
	}
	inputInitTimer();

	/*
	 * Start main menu loop (story and titles)
	 */
	do {
		if (level==-1) {
			menuOption=showTitles();
			switch (menuOption) {
				case menuLoad:
					level=8; /* TODO: make read level function */
					break;
				case menuStart:
					level=1;
					break;
				case menuQuit:
					quit=1;
					break;
			}
		}
		if (!quit) {
			quit=playgame(optionflag,level);
			level = -1;
		}
	} while(!quit);

	kidFree();
	inputStopTimer();
	outputStop();
	return 0;
}

