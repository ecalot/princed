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
list.c: Princed Resources : Ordered Read-Only list implementarion
¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 04 Jun 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2005-Jun-04)

 Note:
  DO NOT remove this copyright notice
*/

/* Defines */
#include <stdlib.h>
#include <string.h> /* memcpy */
#include "list.h"

tList list_create(int dataSize,int dataCmp(const void*,const void*),void dataFree(void* a)) {
	tList r;
	r.size=dataSize;
	r.cmp=dataCmp;
	r.free=dataFree;
	r.cursor=NULL;
	r.first=NULL;
	return r;
}

void list_insert(tList *list,const void* data) {
	/* I will assume sequential access is very common,
	 * so it is very possible that data is the next element.
	 * I will check that and if not I'll start a sequential search */
	tListNode *node; 
	node=(tListNode*)malloc(sizeof(tListNode));
	node->data=malloc(list->size);
	memcpy(node->data,data,list->size);

	if (!list->first) {
		/* insert first */
		list->first=node;
		node->next=NULL;
	} else {
		/* if the data was lower, start searching from the beginning */
		if (list->cmp(list->cursor->data/*>*/,data)==GT) list->cursor=list->first;
		/* in case the first record is higher than the data, we will insert the data as the first */
		if ((list->cursor==list->first) && (list->cmp(list->first->data/*>=*/,data)!=LT)) {
			node->next=list->first;
			list->first=node;
		} else {
			/* search until we find the first higher record or the end of the list */
			while ((list->cursor->next) && (list->cmp(list->cursor->next->data/*>*/,data)!=GT)) 
				list->cursor=list->cursor->next;

			node->next=list->cursor->next;
			list->cursor->next=node;
		}
	}

	list->cursor=node;
}

void list_drop(tList *list) {
	tListNode* aux;
	list->cursor=list->first;
	while (list->cursor) {
		aux=list->cursor->next;
		if (list->free) list->free(list->cursor->data);
		free(list->cursor->data);
		free(list->cursor);
		list->cursor=aux;
	}
	list->first=NULL;
}

void list_nextCursor(tList* list) {
	if (list->cursor) list->cursor=list->cursor->next;
} 

void list_firstCursor(tList* list) {
	list->cursor=list->first;	
} 

int list_moveCursor(tList* list,const void* data) {
	if (!list->first) {
		return 0;
	} else {
		/* if the data was lower, start searching from the beginning */
		if ((!list->cursor)||(list->cmp(list->cursor->data/*>*/,data)==GT)) list->cursor=list->first;
		/* in case the first record is higher than the data, we will move the cursor to the first and return 0 */
		if ((list->cursor==list->first) && (list->cmp(list->first->data/*>=*/,data)!=LT)) {
			return list->cmp(list->first->data/*==*/,data)==EQ;
		} else {
			/* search until we find the first higher record or the end of the list */
			while ((list->cursor->next) && (list->cmp(list->cursor->next->data/*>*/,data)!=GT)) 
				list->cursor=list->cursor->next;
			/* if the prior to the first higher record is equal to data, we've found it, if not 0 will be returned;
			 * of course the cursor will be set to the prior to the first higher record in both cases */
			return (list->cmp(list->cursor->data/*>*/,data)==EQ);
		}
	}
} 

void* list_getCursor(tList* list) {
	if (!list->cursor) return NULL;
	return list->cursor->data;
} 

