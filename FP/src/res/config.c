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
config.c: Free Prince : Configuration Handler
¯¯¯¯¯¯¯¯
 Copyright 2003,2004 Princed Development Team
  Created: 24 Jun 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "config.h"
#include "freeprince.h"
#include <stdlib.h>

typedef struct {
	int size;
	void* value;
}tConfigStructure;

static tConfigStructure options[CONFIG_MAX_ITEMS];

int configInit() {
	int i;
	
	for (i=0;i<CONFIG_MAX_ITEMS;i++) {
		options[i].size=-1;
		options[i].value=NULL;
	}

}

void configStop() {
	//use CONFIG_FILE from freeprince.h
	int i;

	for (i=0;i<CONFIG_MAX_ITEMS;i++) {
		if (options[i].value) free(options[i].value);
	}
}

void configSet(int id, void* data,int size) {
	if (options[id].value) free(options[id].value);
	options[id].value=(void*)malloc(size);
	memcpy(options[id].value,data,size);
	options[id].size=size;
}

void* configGet(int id) {
	return options[id].value;
}

void* configGetArray(int id,int *size) {
	*size=options[id].size;
	return options[id].value;
}

void configSync() {

}


