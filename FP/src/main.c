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
resources.h: Princed Resources : Resource Handler headers
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Mar 2004

  Author: Endfhgfhgfhg <efghgfdht.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "input.h"
#include "interpreter.h"
#include "main.h"
#include "output.h"
#include "parse.h"
#include "resources.h"

int main(int argc,char** argv) {
	tGame game;
	int playing;

	/* Parse game */
	game=parseFile("engine.dat");
	if (!game) {
		printf("Game couldn't be loaded\n");
		return 0;
	}

	/* Initialize Devices */
	startGraphicMode();

	/* Start interpreter */
	if (!initGame(game)) return 0;

	/* Main Loop */
	while(performActions(getKey()));

	/* End */
	stopGraphicMode();
	freeGame(game);
	return 1;
}


