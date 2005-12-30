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
#include "unknown.h" /* typedef tUnknownFile */
#include "parse.h" /* RES_TYPECOUNT */

/***************************************************************\
|                     Unknown.xml primitives                    |
\***************************************************************/

/* XML generation defines */

#define XML_HEADER \
	"<!DOCTYPE resources SYSTEM \"http://www.princed.com.ar/standards/xml/resources/std1.dtd\">\n"\
	"<?xml version=\"1.0\" ?>\n"

extern tUnknownFile unknownFile;

/***************************************************************\
|                           Tree Layer                          |
\***************************************************************/

/* TODO: send this layer to other module */

/* Common factor tree reducing routines */

/* TODO */

/* Tag generation routines */
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

/* memory tree --> xml */
#define outputStream unknownFile.fd /**/

void generateXML(int n,tTag* t) {
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
				generateXML(n+1,children);
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

/* file attribute deletion routines */

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
			/* freeTag(aux); */
			if (*t) rec_tree(t,file);
		} else {
			if ((*t) && (*t)->next) rec_tree(&((*t)->next),file);
		}
	}
	
	if ((*t) && (*t)->child) rec_tree(&((*t)->child),file);
}

void unknown_deletetreefile(const char* file) {
	/*printf("deleting file %s from tree\n",file);*/
	if (unknownFile.tree)
		rec_tree(&(unknownFile.tree->child),file);
}

/* inheritance fixing routines */
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

void unknown_fixtreeinheritances() {
	/*printf("fixing inheritances in tree\n");*/
	if (unknownFile.tree) {
		rec_tree_fix(NULL,unknownFile.tree);
		
		freeAllocation(unknownFile.tree->path);
		unknownFile.tree->path=NULL;		
	}
}

