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
unknown.c: Princed Resources : Unknown resources handler
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

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
#include "disk.h"
#include "unknown.h"
#include "memory.h"
#include "parse.h" /* For the moment just testing */
#include "stringformat.h"
#include "translate.h"

/***************************************************************\
|                     Unknown.xml primitives                    |
\***************************************************************/

/* XML generation defines */

#define RES_XML_UNKNOWN_FILES "%t%03n.%e"

#define XML_HEADER \
	"<!DOCTYPE resources SYSTEM \"http://www.princed.com.ar/standards/xml/resources/std1.dtd\">\n"\
	"<?xml version=\"1.0\" ?>\n"

static struct {
	char*        backupExtension;
	char*        currentDat;
	FILE*        fd;
	tTag*        folderCursor;
	tTag*        folderFirst;
	tTag*        itemCursor;
	tTag*        tree;
	unsigned int optionflag;
	unsigned int typeCount[RES_TYPECOUNT]; /* initialized in 0 */
} unknownFile;

/***************************************************************\
|                           Tree Layer                          |
\***************************************************************/

void unknown_folder(const char* path, const char* file, int palette, const char* paletteindex) {
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
	
	if (!unknownFile.folderFirst) {
		unknownFile.folderFirst=folder;
	} else {
		unknownFile.folderCursor->next=folder;
	}
	unknownFile.folderCursor=folder;
	unknownFile.itemCursor=NULL;
}

void unknown_item(int value,const char* index,const char* path,const char* type,unsigned long int flags,const char* typedesc,int count) {
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
	
	if (!unknownFile.itemCursor) {
		unknownFile.folderCursor->child=item;
	} else {
		unknownFile.itemCursor->next=item;
	}
	unknownFile.itemCursor=item;
}

/* memory tree --> xml, TODO send to other module */
#define outputStream unknownFile.fd

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

/* file deletion */

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

/* inheritance fixing */
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

/***************************************************************\
|                           Logging Layer                       |
\***************************************************************/

int unknownLogStart (const char* file,int optionflag, const char* backupExtension) {
	if (unknownFile.fd) return PR_RESULT_ERR_XML_ALREADY_OPEN; /* File already open */

	/* Use default filename if file is NULL */
	if (!file) file=RES_XML_UNKNOWN_XML;

	/* Remember optionflag and backupExtension */
	unknownFile.optionflag=optionflag;
	unknownFile.backupExtension=strallocandcopy(backupExtension);
	unknownFile.currentDat=NULL;

	/* Read the previous file if exists */
	{
		int error;

		unknownFile.folderFirst=NULL;
		unknownFile.folderCursor=NULL;
		unknownFile.tree=parseXmlFile(file,&error);
		if (error) {
			unknownFile.tree=malloc(sizeof(tTag));
			memset(unknownFile.tree,0,sizeof(tTag));
			unknownFile.tree->version=strallocandcopy("generated");
			unknownFile.tree->tag=strallocandcopy("resources");
		}
	}
	
	/* Open the file */
	if (!writeOpen(file,&unknownFile.fd,optionflag)) return PR_RESULT_ERR_XML_FILE; /* file not open */

	return PR_RESULT_SUCCESS; /* Ok */
}

int unknownLogStop () {
	int i;
	tTag* t;

	if (!unknownFile.fd) return PR_RESULT_ERR_XML_NOT_OPEN; /* File not open */

	/* it is time to fix the inheritances */
	unknown_fixtreeinheritances();
	
	/* now we'll add the new generated part of the tree at the end of the second level (resources id the first) */
	if (unknownFile.tree) {
		if (unknownFile.tree->child) {
			for (t=unknownFile.tree->child;t->next;t=t->next);
			t->next=unknownFile.folderFirst; /* the first folder of the new tree */
		} else {
			unknownFile.tree->child=unknownFile.folderFirst; /* the first folder of the new tree */
		}
	}

	/* TODO: create common factor tree reducing function */
	
	/* generate the xml file */
	generateXML(0,unknownFile.tree);

	/* it's time to free the tree */
	freeParsedStructure (&unknownFile.tree);
	
	/* and close the file */
	writeCloseOk(unknownFile.fd,unknownFile.optionflag,unknownFile.backupExtension);

	/* Free structures */
	freeAllocation(unknownFile.currentDat);
	freeAllocation(unknownFile.backupExtension);
	unknownFile.backupExtension=NULL;
	unknownFile.fd=NULL;
	for (i=0;i<RES_TYPECOUNT;i++) unknownFile.typeCount[i]=0; /* re-initialize in 0 for next file processing */

	return PR_RESULT_SUCCESS; /* Ok */
}

int unknownLogAppend(const char* vFiledat,tResourceId id,const char* ext,tResourceType type,const char* vDirExt,tResourceId pal,const char* vFiledatWithPath,int optionflag,int count, unsigned long int flags,const char* filename) {
	if (!unknownFile.fd) return PR_RESULT_ERR_XML_NOT_OPEN; /* File not open, logging is off, just a warning */

	if (!unknownFile.currentDat) { /* this is the beginning of the file */
		unknown_folder(vFiledatWithPath,vFiledat,pal.value,translateInt2Ext(toLower(pal.index)));
		unknownFile.currentDat=strallocandcopy(vFiledat);
		/* TODO: move here the read-parsing-loading and write-opening */
		unknown_deletetreefile(vFiledat);
	} else if (!equalsIgnoreCase(unknownFile.currentDat,vFiledat)) {
		int i;
		unknown_folder(vFiledatWithPath,vFiledat,pal.value,translateInt2Ext(toLower(pal.index)));
		freeAllocation(unknownFile.currentDat);
		unknownFile.currentDat=strallocandcopy(vFiledat);
		unknown_deletetreefile(vFiledat);
		for (i=0;i<RES_TYPECOUNT;i++) unknownFile.typeCount[i]=0; /* re-initialize in 0 for next file processing */
	}

	unknown_item(id.value,translateInt2Ext(toLower(id.index)),filename,getExtDesc(type),flags,getExtDesc(type),count);

	return PR_RESULT_SUCCESS;
}

/***************************************************************\
|                     Middle layer function                     |
\***************************************************************/

void getFileName(char* vFileext,const char* vDirExt,const tResource* r,const char* vFiledat, const char* vDatFileName,int optionflag, const char* backupExtension,const char* format) {
	static const char* extarray[]=RES_FILE_EXTENSIONS;
	const char* filename;
	int pos;

	if (r->path==NULL) {
		pos=((r->type<RES_TYPECOUNT)&&(r->type>=0))?r->type:eResTypeBinary;
		unknownFile.typeCount[pos]++;

		/* set filename */
		if (!format) format=RES_XML_UNKNOWN_FILES;
		filename=parseformat(format,r->id.value,r->id.index,getExtDesc(pos),extarray[pos],unknownFile.typeCount[pos],r->id.order,r->desc);

		sprintf(vFileext,"%s/"RES_XML_UNKNOWN_PATH"/%s/%s",vDirExt,vDatFileName,filename);
		unknownLogAppend(vDatFileName,r->id,extarray[pos],r->type,vDirExt,r->palette,vFiledat,optionflag,unknownFile.typeCount[pos],r->flags,filename);
	} else {
		/* set filename */
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

