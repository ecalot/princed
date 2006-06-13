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
pallist.c: Princed Resources : Palette list layer implementation
���������
 Copyright 2006 Princed Development Team
  Created: 09 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2006-Feb-09)

 Note:
  DO NOT remove this copyright notice
*/

/* Defines */
#include "pallist.h"
#include "reslist.h" /* resourceListCompareId */

/* palette list layer (that uses the abstract list layer primitives) */

int pallist_compare(const void* a,const void* b) {
	return resourceListCompareId(((tPaletteListItem*)a)->id,((tPaletteListItem*)b)->id);
}

tPaletteList paletteListCreate() {
	return list_create(sizeof(tPaletteListItem),pallist_compare,NULL);
}

/* Priority list */
#include <stdlib.h>

#include "object.h"

void showobj(tObject o) {
	printf("object type=%d colors=%d\n",o.type,paletteGetColors(o));
}

typedef enum {highPriority, lowPriority}tPriority;
typedef struct pln{
	tResourceId resid;			 
	tObject*    object;
	struct pln* next;
}	tPL_Node;

typedef struct {
	struct {
		tResourceId idres;			 
		tObject*    object;
	} priority_field;
	tPL_Node*   list_first;
	tPL_Node*   list_deleted_first;
}	tPL;

void pl_free  (tPL* pl);
int  pl_tryAdd(tPL* pl, tResourceId resid, tPriority p);
int  pl_add   (tPL* pl, tObject* o, tResourceId resid, tPriority p);
int  pl_hasPriority(tPL* pl, tResourceId resid);
tPL  pl_create();
tObject* pl_get(tPL* pl, int* priorityRight, int colors);

tPL  pl_create() {
	tPL r;

	r.priority_field.object=NULL;
	r.list_first=NULL;
	r.list_deleted_first=NULL;

	return r;
}

int  pl_hasPriority(tPL* pl, tResourceId resid) {
	if (!pl->priority_field.object) return 0; /* false */
	return resourceListCompareId(resid,pl->priority_field.idres)==0;
}

int  pl_tryAdd(tPL* pl, tResourceId resid, tPriority p) {
	return 0; /* always false, optimization disabled */
}

int pl_add(tPL* pl, tObject* o, tResourceId resid, tPriority p) {
	if (p==highPriority) {
		/* high priority insertion */
		if (pl->priority_field.object) { /* if there was another object proprized, move it to the list */
			tObject* obj_old_priority=pl->priority_field.object;
			if (resourceListCompareId(resid,pl->priority_field.idres)==0) return 0; /* same object, take no action */
			/* drop the object from the priority field and reinsert it with low priority */
			pl->priority_field.object=NULL;
			pl_add(pl,obj_old_priority,pl->priority_field.idres,lowPriority);
		}
		/* now, we know there is no object in the priority field, so we insert it */
		pl->priority_field.object=o;
		pl->priority_field.idres=resid;
	} else {
		/* low priority insertion */
		tPL_Node* insertNode=malloc(sizeof(tPL_Node));
		int colors=paletteGetColors(*o);
		
		while (pl->list_first && colors>=paletteGetColors(*(pl->list_first->object))) {
			printf("deleting: ");
			showobj(*pl->list_first->object);
			pl->list_first=pl->list_first->next; /* Delete */
		}
		insertNode->next=pl->list_first;
		insertNode->object=o;
		insertNode->resid=resid;
		pl->list_first=insertNode;
	}
	return 1;
}

tObject* pl_get(tPL* pl, int* priorityRight, int colors) {
	tPL_Node* node;
				
	if (pl->priority_field.object) {
		*priorityRight=(colors<=paletteGetColors(*pl->priority_field.object));
		 /* true iif the object palette has more colors than the given variable */
		return pl->priority_field.object;
	}

	node=pl->list_first;
	
	while (node && colors>paletteGetColors(*node->object))
		node=node->next;
	
	*priorityRight=1;
	return node?node->object:NULL;
}

int main(int a,char** b) {
	tObject tests[]={
		{eResTypeNone,NULL},
		{eResTypePop2PaletteNColors,(void*)((int)320)},
		{eResTypePop1Palette4bits,NULL}
	};
	tResourceId ress[]={
		{0,"POP1",4},
		{2,"LALA",6}
	};
	tPL pl=pl_create();
	
	printf("hello world\n");
	showobj(tests[1]);
	
	pl_add(&pl, tests, ress[0], lowPriority);
	pl_add(&pl, tests+1, ress[1], lowPriority);
	pl_add(&pl, tests, ress[0], lowPriority);
	pl_add(&pl, tests+2, ress[1], lowPriority);

	{
		tPL_Node* nodo=pl.list_first;
		printf("yeah list:\n");
		printf(" pri:\n");
		if (pl.priority_field.object) {
			showobj(*pl.priority_field.object);
		} else {
			printf("nada!\n");
		}
		printf(" no pri:\n");
		while (nodo) {
			showobj(*(nodo->object));
			nodo=nodo->next;
		}
	}

	{
		tObject* o;
		int p;
		o=pl_get(&pl,&p, 16);
		printf("result of 16 colors: p=%d\n",p);
		showobj(*o);
		o=pl_get(&pl,&p, 106);
		printf("result of 106 colors: p=%d\n",p);
		showobj(*o);
		o=pl_get(&pl,&p, 216);
		printf("result of 216 colors: p=%d\n",p);
		showobj(*o);
		o=pl_get(&pl,&p, 316);
		printf("result of 316 colors: p=%d\n",p);
		showobj(*o);
		o=pl_get(&pl,&p, 416);
		printf("result of 416 colors: p=%d %p\n",p,(void*)o);
		showobj(*o);

	}
	return 0;
}

