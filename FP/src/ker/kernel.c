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
  Created: 16 Jun 2004

  Author: Diego Essaya <efghgfdht.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "kernel.h"
#include "resources.h"

int kernel(int optionflag,int level) {
/* levels=-1 is default
 * levels from 0 to n is the level number
 *  
 * optionflag may be read using hasFlag(name_flag); Note that the variable must be called optionflag
 */
	
	tData* testResource;
	int i;
			
	printf("Hello, I'm a dummy kernel, I was called to do the stuff\nwith '%x' options and go to the level '%d'\n",optionflag,level);
	testResource=resLoad(RES_ANIM_RUN_LEFT);	
	
	/* How to use a resource */
	if (!testResource) {
		printf("The resource couldn't be loaded!\n");
		exit(1);
	}

	printf("Resource number: %d. Frames: %d. Type: %d.\n",
		RES_ANIM_RUN_LEFT,
		testResource->frames,
		testResource->type
	);
	
	for (i=0;i<testResource->frames;i++) {
		/* Note: for the moment there is a testing abstraction violation here */
		printf("frame %d: size=%d\n",i,((tMemory*)(testResource->pFrames[i]))->size);
	}
	
	return 0;
}

