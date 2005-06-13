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
reslist.h: Princed Resources : Resource list
���������
 Copyright 2005 Princed Development Team
  Created: 05 Jun 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2005-Jun-05)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _RESLIST_H_
#define _RESLIST_H_

#include "list.h" /* tList */
#define tResourceList tList

/* Item Types */

typedef enum {
RES_TYPE_RAW=0,
RES_TYPE_LEVEL=1,
RES_TYPE_IMAGE=2,
RES_TYPE_WAVE=3,
RES_TYPE_MIDI=4,
RES_TYPE_BINARY=5,
RES_TYPE_PALETTE=6,
RES_TYPE_PCSPEAKER=7} tResourceType;

/* types */
typedef struct {
	unsigned short int value;
	char               index[5];
}tResourceId;

typedef struct {
	tResourceId        id;
	tResourceId        palette;
	long int           size;
	unsigned long int  offset; /* Used internally in dat.c to remember the offset */
	unsigned char      number; /* Used for level number */
	tResourceType      type;
	char*              desc;
	char*              name;
	char*              path;
	unsigned char*     data;
	unsigned long      flags;
}tResource;

void printr(const tResource* record); /* for debug purposes */

int resIdCmp(tResourceId a,tResourceId b);
int resCmp(const void* a,const void* b);
void resourceListDrop(tResourceList* r);
tResourceList resourceListCreate(int isCopy);
void resourceListAdd(tResourceList* r,const tResource* res);
void resourceListDebugPrint(tResourceList* r);
void resourceListAddInfo(tResourceList* r,tResource* res);

#endif
