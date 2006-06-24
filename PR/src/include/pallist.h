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
pallist.h: Princed Resources : Resource list prototypes
¯¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 05 Jun 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2005-Jun-05)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_PALLIST_H_
#define _PR_PALLIST_H_

#include "list.h"    /* tList */
typedef tList tPaletteList;

#include "common.h"  /* tResourceType */
#include "object.h"  /* tObject */
#include "types.h"   /* tResourceId */

/* types */
typedef struct {
	tResourceId        id;
	tObject            pal;
} tPaletteListItem;

tPaletteList paletteListCreate();

typedef enum {highPriority, lowPriority}tPriority;
typedef struct pln {
	tResourceId resid;
	tObject     object;
	struct pln* next;
} tPL_Node;

typedef struct {
	struct {
		int         enabled;
		tResourceId idres;
		tObject     object;
	} priority_field;
	tPL_Node*   list_first;
	tPL_Node*   list_deleted_first;
} tPL;

void pl_free  (tPL* pl);
int  pl_tryAdd(tPL* pl, tResourceId resid, tPriority p);
void pl_add   (tPL* pl, tObject o, tResourceId resid, tPriority p);
int  pl_hasPriority(tPL* pl, tResourceId resid);
tPL  pl_create();
tObject pl_get(tPL* pl, int* priorityRight, int colors);

#endif
