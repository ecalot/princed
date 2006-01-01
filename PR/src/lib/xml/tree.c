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
tree.c: Princed Resources : Tree handling routines
¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 28 Dec 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)
  Version: 1.50 (2005-Dec-19)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/* Includes */
#include <stdio.h>
#include "common.h"
#include "memory.h"
#include "list.h" /* list primitives needed by the Common Factor routines */
#include "unknown.h" /* typedef tUnknownFile */

/***************************************************************\
|                     XML generation defines                    |
\***************************************************************/

#define XML_HEADER \
	"<!DOCTYPE resources SYSTEM \"http://www.princed.com.ar/standards/xml/resources/std1.dtd\">\n"\
	"<?xml version=\"1.0\" ?>\n"

/***************************************************************\
|              Common factor tree reducing routines             |
\***************************************************************/
/*
tList list_create(int dataSize,int dataCmp(const void*,const void*),void dataFree(void* a));
void  list_insert(tList *list,const void* data);
void  list_drop(tList *list);
void  list_nextCursor(tList* list);
void  list_firstCursor(tList* list);
int   list_moveCursor(tList* list,const void* data);
const void* list_getCursor(tList* list);
*/

typedef struct {
	char* attr;
	int count;
}tAttrCount;

void attrfree() {}

int attrcmp(const tAttrCount* a,const tAttrCount* b) {
	/* the index has the priority */
	int c=strcmp(a->attr,b->attr);
	if (c>0) return GT;
	if (c<0) return LT;
	return EQ;
}

void increase(char* attr,tList* l) {
	tAttrCount a;
	a.attr=attr;
	a.count=0;
	if (list_moveCursor(l,&a)) {
		/* TODO: int count */
	} else {
		list_insert(l,&a);
	}
}

void commonFactor(tTag* parent) {
	tTag* child;
	const tAttrCount* a;
	int c=0;
	int max=0;
	const char* result=NULL;
	tList l=list_create(sizeof(tAttrCount),attrcmp,attrfree);
	
	for (child=parent->child;child;child=child->next) {
		increase(child->palette,&l); /* TODO: use an array of attributes */
		c++;
	}
	
	list_firstCursor(&l);
	while ((a=list_getCursor(&l))) {
		if (a->count>c/2) {
			if (a->count>max) {
				max=a->count;
				result=a->attr;
			}
		}
		list_nextCursor(&l);
	}
	if (result) {
		freeAllocation(parent->palette);
		parent->palette=strallocandcopy(result);
		for (child=parent->child;child;child=child->next) {
			freeAllocation(child->palette);
			child->palette=NULL;
		}
	}
	list_drop(&l);
	
	
}

/***************************************************************\
|                     Tag generation routines                   |
\***************************************************************/

void unknown_folder(const char* path, const char* file, int palette, const char* paletteindex, tTreeStatus* status) {
	char number[10];
	tTag* folder=malloc(sizeof(tTag));	

	memset(folder,0,sizeof(tTag));
	sprintf(number,"%d",palette);
	folder->tag=strallocandcopy("folder");
	folder->name=strallocandcopy("unknown");
	folder->path=strallocandcopy(path);
	folder->file=strallocandcopy(file);
	folder->palette=strallocandcopy(number);
	folder->paletteindex=strallocandcopy(paletteindex);
	
	if (!status->folderFirst) {
		status->folderFirst=folder;
	} else {
		status->folderCursor->next=folder;
	}
	status->folderCursor=folder;
	status->itemCursor=NULL;
}

void unknown_item(int value,const char* index,const char* path,const char* type,unsigned long int flags,const char* typedesc,int count, tTreeStatus* status) {
	char aux[100];
	tTag* item=malloc(sizeof(tTag));	

	memset(item,0,sizeof(tTag));
	sprintf(aux,"%d",value);
	item->tag=strallocandcopy("item");
	item->value=strallocandcopy(aux);
	item->index=strallocandcopy(index);
	item->path=strallocandcopy(path);
	item->type=strallocandcopy(type);
	sprintf(aux,"0x%lx",flags);
	item->flags=strallocandcopy(aux);
	sprintf(aux,"Unknown %s %d",typedesc, count);
	item->desc=strallocandcopy(aux);
	
	if (!status->itemCursor) {
		status->folderCursor->child=item;
	} else {
		status->itemCursor->next=item;
	}
	status->itemCursor=item;
}

/***************************************************************\
|                       Memory tree --> xml                     |
\***************************************************************/

void generateXML(int n,tTag* t,FILE* outputStream) {
	int a;
	tTag* children;

	if (!n) fprintf(outputStream,XML_HEADER);
	for(a=0;a<n;a++) fprintf (outputStream,"\t");
	if (t!=NULL) {
		fprintf(outputStream,"<%s",t->tag);

#define FillAttr(a,b) if (a) fprintf(outputStream," %s=\"%s\"",b,a)

	if (t->child) FillAttr(t->desc,"desc");
	FillAttr(t->path,"path");
	FillAttr(t->file,"file");
	FillAttr(t->type,"type");
	FillAttr(t->name,"name");
	FillAttr(t->palette,"palette");
	FillAttr(t->value,"value");
	FillAttr(t->index,"index");
	FillAttr(t->order,"order");
	FillAttr(t->paletteindex,"paletteindex");
	FillAttr(t->paletteorder,"paletteorder");
	FillAttr(t->version,"version");
	FillAttr(t->number,"number");
	FillAttr(t->flags,"flags");

#undef FillAttr

		if ((children=t->child)) {
			fprintf(outputStream,">\n");
			while (children!=NULL) {
				generateXML(n+1,children,outputStream);
				children=children->next;
			}
			for(a=0;a<n;a++) fprintf (outputStream,"\t");
			fprintf(outputStream,"</%s>\n",t->tag);
		} else {
			if (t->desc) {
				fprintf(outputStream,">%s</%s>\n",t->desc,t->tag);
			} else {
				fprintf(outputStream," />\n\n");
			}
		}
	}
}

/***************************************************************\
|                File attribute deletion routines               |
\***************************************************************/

void rec_tree(tTag* *t,const char* file) {
	tTag* aux=*t;
	tTag* aux2;

	if (*t && (*t)->file) {
		if (equalsIgnoreCase((aux->file),file)) {
			if (aux->child) {
				*t=aux->child; /* the children are now replacing his parent */
				if ((*t)->next) {
					for(aux2=*t;aux2->next;aux2=aux2->next); /* go to the last child */
				 	aux2->next=aux->next; /* and set the next siebling as the parent next siebling */
				}
				aux->child=NULL;
			} else {
				*t=aux->next;
			}
			aux->next=NULL;
			if (*t) rec_tree(t,file);
		} else {
			if ((*t) && (*t)->next) rec_tree(&((*t)->next),file);
		}
	}
	
	if ((*t) && (*t)->child) rec_tree(&((*t)->child),file);
}

void unknown_deletetreefile(const char* file, tTag* tree) {
	if (tree)
		rec_tree(&(tree->child),file);
}

/***************************************************************\
|                  Inheritance fixing routines                  |
\***************************************************************/

#define TotalInheritance(a) if (parent->a&&child->a&&equalsIgnoreCase(parent->a,child->a)) {freeAllocation(child->a);child->a=NULL;}

void rec_tree_fix(tTag* parent,tTag* child) {
	if (child->next) rec_tree_fix(parent,child->next);
	if (child->child) rec_tree_fix(child,child->child);

	if (parent) {
		TotalInheritance(palette);
		TotalInheritance(paletteindex);
		TotalInheritance(paletteorder);
		TotalInheritance(type);
		TotalInheritance(file);
		TotalInheritance(index);
		TotalInheritance(order);
		TotalInheritance(flags);

		/* partial */
		if ((parent->path!=NULL)&&(child->path!=NULL)) {
			char *p,*c,*aux;
			
			for (p=parent->path,c=child->path;*p&&*c&&*p==*c;p++,c++);
			if (*c=='/') c++;
			aux=strallocandcopy(c);
			free(child->path);
			child->path=aux;
		}

	}
	
}

void unknown_fixtreeinheritances(tTag* *tree) {
	/*printf("fixing inheritances in tree\n");*/
	if (*tree) {
		rec_tree_fix(NULL,(*tree));
		
		freeAllocation((*tree)->path);
		(*tree)->path=NULL;		
	}
}

