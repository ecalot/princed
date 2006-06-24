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
level.c: Princed Resources : Common POP level object implementation
¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 5 Mar 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2006-Mar-05)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include "binary.h" /* tBinary */
#include "common.h"
#include "dat.h"    /* WriteDat */
#include "disk.h"   /* getFileNameFromPath */
#include "plv.h"
#include <stdlib.h>
#include "types.h"  /* tResources */

/***************************************************************\
|                         Level Object                         |
\***************************************************************/

typedef struct {
	tBinary content;
	int number;
	const char* datfile;
	const char* name;
	const char* desc;
	const char* datAuthor;
}tPop1Level;

void* objectLevelCreate(tBinary content,int number,const char* datfile,const char* name,const char* desc,const char* datAuthor,int *error) {
	tPop1Level* r;
	*error=PR_RESULT_SUCCESS;
	/*TODO: fix the original file name path (lala///lele/demo.plv--> demo.plv) */

	r=(tPop1Level*)malloc(sizeof(tPop1Level));
	r->content=content;
	r->number=number;
	r->datfile=datfile;
	r->name=name;
	r->desc=desc;
	r->datAuthor=datAuthor;
	return (void*)r;
}

int objectLevelWrite(void* o, const char* file, int optionflag, const char* backupExtension,int version) {
	tPop1Level* b=o;
	return writePlv(file,b->content,version,b->datfile,b->number,getFileNameFromPath(file),b->desc,b->name,b->datAuthor,optionflag,backupExtension);
}

/*
void* objLevelRead(const char* file,int *result) {
	tBinary o=mLoadFileArray(file);
	if (o.size<0) {
		*result=o.size;
		return NULL;
	}
	return objectLevelPop1Create(o,result);
}

int objLevelSet(void* o,tResource* res) {
	tBinary* bin=o;
	res->content=*bin;
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}
*/

