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
¯¯¯¯¯¯¯¯¯
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

#ifndef DEBUG_TEST_PALLST
void showobj(tObject o) {
	printf("object type=%d colors=%d\n",o.type,paletteGetColors(o));
}
#endif

#if 0
pl_dellist_add(tPL* pl, tResourceId idres, tObject* obj) {
	/* TODO: use an ordered list */
	/*	result=resourceListCompareId(idres,);*/
	tPL_Node* node=pl->list_deleted_first;

	while (node) {
		if (resourceListCompareId(node->resid,idres)==0)e
	}
	
	

				
}

#endif

tPL  pl_create() {
	tPL r;

	r.priority_field.enabled=0;
	r.list_first=NULL;
	r.list_deleted_first=NULL;

	return r;
}

int  pl_hasPriority(tPL* pl, tResourceId resid) {
	if (!pl->priority_field.enabled) return 0; /* false */
	return resourceListCompareId(resid,pl->priority_field.idres)==0;
}

int  pl_tryAdd(tPL* pl, tResourceId resid, tPriority p) {
	return 0; /* always false, optimization disabled */
}

void pl_add(tPL* pl, tObject o, tResourceId resid, tPriority p) {
	if (p==highPriority) {
printf("adding with high priority:\n");
showobj(o);
		/* high priority insertion */
		if (pl->priority_field.enabled) { /* if there was another object proprized, move it to the list */
			tObject obj_old_priority=pl->priority_field.object;
			pl->priority_field.enabled=0;
			if (resourceListCompareId(resid,pl->priority_field.idres)==0) return; /* same object, take no action */
			/* drop the object from the priority field and reinsert it with low priority */
			pl_add(pl,obj_old_priority,pl->priority_field.idres,lowPriority);
		}
		/* now, we know there is no object in the priority field, so we insert it */
		pl->priority_field.object=o;
		pl->priority_field.idres=resid;
		pl->priority_field.enabled=1;
	} else {
		/* low priority insertion */
		tPL_Node* insertNode=malloc(sizeof(tPL_Node));
		int colors=paletteGetColors(o);
printf("adding with low priority:\n");
showobj(o);
		
		while (pl->list_first && colors>=paletteGetColors(pl->list_first->object)) {
			printf("deleting: ");
			showobj(pl->list_first->object);
			pl->list_first=pl->list_first->next; /* Delete */
		}
		insertNode->next=pl->list_first;
		insertNode->object=o;
		insertNode->resid=resid;
		pl->list_first=insertNode;
printf("inserting ");
showobj(pl->list_first->object);
	}
	return;
}

tObject pl_get(tPL* pl, int* priorityRight, int colors) {
	tPL_Node* node;
	int junk;
printf("getting PL\n");
	
	*priorityRight=1;
	if (pl->priority_field.enabled) {
		if (colors<=paletteGetColors(pl->priority_field.object)) {
showobj(pl->priority_field.object);
			return pl->priority_field.object;
		} else {
			*priorityRight=0;
		}
	}

	node=pl->list_first;
printf("first=%p with ",(void*)pl->list_first);
showobj(pl->list_first->object);
	
	while (node && colors>paletteGetColors(node->object))
	{		node=node->next; printf("next %p\n",(void*)node); }
	
	return node?node->object:getObject(NULL,&junk);
}

#ifdef DEBUG_TEST_PALLST
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
	pl_add(&pl, tests, ress[1], highPriority);
	pl_add(&pl, tests, ress[1], lowPriority);

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
#endif

