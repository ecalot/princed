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
reslist.c: Princed Resources : Ordered Read-Only list implementarion
¯¯¯¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 04 Jun 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2005-Jun-04)

 Note:
  DO NOT remove this copyright notice
*/

/* Defines */
#include "reslist.h"
#include <string.h> /* strncmp */
#include <stdio.h> /* debug */

/* resource list layer (that uses the abstract list layer primitives) */

int resIdCmp(tResourceId a,tResourceId b) {
	/* the number has the priority */
	if (a.value>b.value) return GT;
	if (a.value<b.value) return LT;
	/* at this point, the numbers are the same, so the index name is the comparation */
	return strncmp(a.index,b.index,4);	
}

int resCmp(const void* a,const void* b) {
	return resIdCmp(((tResource*)a)->id,((tResource*)b)->id);
}

void resourceListDrop(tResourceList* r) {
	list_drop(r);
}

tResourceList resourceListCreate() {
	return list_create(sizeof(tResource),resCmp);
}

void resourceListAdd(tResourceList* r,const tResource* res) {
	/* first try to detect if the resource exists */
	if (!list_moveCursor(r,res))
		list_insert(r,res);
}

void printr(const tResource* record) {
		printf("id=(%d,%s)\n",record->id.value,record->id.index);
		printf("palette=(%d,%s)\n",record->palette.value,record->palette.index);
		printf("size=%d offset=%lu\n",record->size,record->offset);
		printf("number=%d type=%d\n",record->number,record->type);
		printf("path='%s' name='%s' desc='%s'\n\n",record->path,record->name,record->desc);
}

void resourceListDebugPrint(tResourceList* r) {
	const tResource* record;
	list_firstCursor(r);
	while ((record=list_getCursor(r))) {
		printr(record);
		list_nextCursor(r);
	}
	list_firstCursor(r);
}

void resourceListAddInfo(tResourceList* r,tResource* res) {
	if (!list_moveCursor(r,res)) {
		res->path=NULL;
		res->palAux=NULL;
		res->desc=NULL;
		res->name=NULL;
		res->palette.value=0;
		memset(res->palette.index,0,5);
		res->number=0;
		res->type=0;
	} else { 
		const tResource* resInfo=list_getCursor(r);
		/* copy only the list information */
		res->path=resInfo->path;
		res->palAux=resInfo->palAux;
		res->desc=resInfo->desc;
		res->name=resInfo->name;
		res->palette=resInfo->palette;
		res->number=resInfo->number;
		/* If resource type is invalid or 0, the type will be decided by PR */
		res->type=resInfo->type;
	}
}

