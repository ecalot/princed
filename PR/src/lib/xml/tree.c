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
 Copyright 2005 Princed Development Team
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

typedef struct {
	const char* attr;
	int count;
}tAttrCount;

int attrcmp(const void* x,const void* y) {
	register const tAttrCount* a=x;
	register const tAttrCount* b=y;
	/* the index has the priority */
	int c=strcmp(a->attr,b->attr);
	if (c>0) return GT;
	if (c<0) return LT;
	return EQ;
}

void increase(const char* attr,tList* l) {
	tAttrCount a;
	tAttrCount* aux;

	printf("increase: %s\n",attr);
	a.attr=attr;
	a.count=1; /* if it appeared for the first time */
	if (list_moveCursor(l,&a)) {
		aux=list_getCursor(l);
		aux->count++;
	} else {
		list_insert(l,&a);
	}
}

#define attributeCount 8
/* this is the most ugly thing I ever made... nevertheless it was the only way to make it abstract */
#define bindAttr(name,i) attrInfo[i].offset=(long)(&(parent->name))-(long)(parent)
#define getAttr(tag) (*( (char**)((long)(tag)+attrInfo[i].offset) ))

void commonFactor(tTag* parent) {
	tTag* child;
	const tAttrCount* a;
	int i;
	int max;
	const char* result;
	struct attributeInfo {
		int c;
		tList l;
		long offset;
	} attrInfo[attributeCount];

	bindAttr(palette,0);
	bindAttr(paletteindex,1);
	bindAttr(paletteorder,2);
	bindAttr(type,3);
	bindAttr(file,4);
	bindAttr(index,5);
	bindAttr(order,6);
	bindAttr(flags,7);

	/* initialize counting list */
	for (i=0;i<attributeCount;i++) {
		attrInfo[i].l=list_create(sizeof(tAttrCount),attrcmp,NULL);
		attrInfo[i].c=0;
	}

	for (child=parent->child;child;child=child->next) {
		for (i=0;i<attributeCount;i++) {
			if (getAttr(child))
				increase(getAttr(child),&(attrInfo[i].l));
			else
				increase(getAttr(child),&(attrInfo[i].l));
			attrInfo[i].c++;
		}
	}

	for (i=0;i<attributeCount;i++) {
		max=0;
		result=NULL;
		list_firstCursor(&(attrInfo[i].l));
		while ((a=list_getCursor(&(attrInfo[i].l)))) {
			printf("running through %s (%d)\n",a->attr,a->count);
			if (a->count>attrInfo[i].c/2 && a->count>max) {
				max=a->count;
				result=a->attr;
			}
			list_nextCursor(&(attrInfo[i].l));
		}

		if (result) {
			if (getAttr(parent)!=result) { /* it is possible, and is the most probable case, that the parent was already the best choice. In that case, do nothing */
				freeAllocation(getAttr(parent));
				getAttr(parent)=strallocandcopy(result); /* TODO: assign result but avoid releasing below */
			}
			for (child=parent->child;child;child=child->next) {
				if (getAttr(child)&&!strcmp(getAttr(child),getAttr(parent))) {
					freeAllocation(getAttr(child)); /* the first time this is ran, result is released! */
					getAttr(child)=NULL;
				}
			}
		}
		list_drop(&(attrInfo[i].l));
	}
}

void resourceTreeCommonFactor(tTag* tag) {
	if (tag) {
		resourceTreeCommonFactor(tag->next);
		resourceTreeCommonFactor(tag->child);
		commonFactor(tag); /* post order */
	}
}

#ifndef DEBUG_CF
void test() {
	tTag tr[100];
	memset(tr,sizeof(tr),0);

	tr[0].child=&(tr[1]);

	tr[1].next =&(tr[2]);
	tr[1].child=&(tr[6]);
	tr[2].next =&(tr[3]);
	tr[3].next =&(tr[4]);
	tr[4].next =&(tr[5]);
	tr[5].next =NULL;

	tr[6].next =&(tr[7]);
	tr[7].next =&(tr[8]);
	tr[8].next =&(tr[9]);
	tr[8].child=&(tr[11]);
	tr[9].next =&(tr[10]);
	tr[10].next =NULL;

	tr[11].next =&(tr[12]);
	tr[12].next =&(tr[13]);
	tr[13].next =&(tr[14]);
	tr[14].child=&(tr[15]);
	tr[15].next =&(tr[16]);
	tr[16].next =NULL;

	tr[0].palette=strallocandcopy("joj0");
	tr[1].palette=strallocandcopy("joj1");
	tr[2].palette=strallocandcopy("joj0");
	tr[3].palette=strallocandcopy("joj1");
	tr[4].palette=strallocandcopy("joj0");
	tr[5].palette=strallocandcopy("joj1");
	tr[6].palette=strallocandcopy("joj1");
	tr[7].palette=strallocandcopy("joj0");
	tr[8].palette=strallocandcopy("joj0");
	tr[9].palette=strallocandcopy("joj1");
	tr[10].palette=strallocandcopy("joj11");
	tr[11].palette=strallocandcopy("joj11");
	tr[12].palette=strallocandcopy("joj10");
	tr[13].palette=strallocandcopy("joj10");
	tr[14].palette=strallocandcopy("joj11");
	tr[15].palette=strallocandcopy("joj11");
	tr[16].palette=strallocandcopy("joj11");

	generateXML(0,tr,stdout);

	resourceTreeCommonFactor(tr);

	generateXML(0,tr,stdout);

}
#endif

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

void generateXML(int n,/*const*/ tTag* t,FILE* outputStream) {
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

int generateXMLfile(const char* vFile,/*const*/ tTag* t) {
	FILE* fd;
	if (!(fd=fopen(vFile,"wb"))) return PR_RESULT_ERR_XML_NOT_OPEN;
	generateXML(0,t,fd);
	return PR_RESULT_SUCCESS;
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

void resourceTreeFixInheritances(tTag* *tree) {
	/*printf("fixing inheritances in tree\n");*/
	if (*tree) {
		rec_tree_fix(NULL,(*tree));

		freeAllocation((*tree)->path);
		(*tree)->path=NULL;
	}
	test();
}

