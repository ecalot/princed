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
resources.h: Free Prince : Resource Handler
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2003,2004 Princed Development Team
  Created: 23 Mar 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _RESOURCES_H_
#define _RESOURCES_H_

int resOpenFile(const char* file); 
	/* Opens file, loads the data into memory, closes the file and
	 * returns an ID number (data descriptor) for the new structure. */
resCloseFile(int dd); /* dd = data descriptor */
	/* Frees the data from memory. */
resCloseAllFiles();

/* Functions for getting data previously loaded into memory. Each dd points
 * to an array of elements, and resId indicates the index in this array. */
tMidi resGetMidi(int dd, int resId);
tWave resGetWave(int dd, int resId);
tImage resGetImage(int dd, int resId);

#endif
