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

static struct {
	FILE* fd;
	char* backupExtension;
	char* currentDat;
	int optionflag;
	unsigned int typeCount[RES_TYPECOUNT]; /* initialized in 0 */
	tTag* tree;
} unknownFile;

/***************************************************************\
|                        Syntactic Layer                        |
\***************************************************************/

#define unknown_emptyfile() /* TODO: don't affect this file if empty */  fprintf(unknownFile.fd,\
	"<!DOCTYPE resources SYSTEM \"http://www.princed.com.ar/standards/xml/resources/std1.dtd\">\n"\
	"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<resources version=\"generated\" />\n") 

#define unknown_folderclose() fprintf(unknownFile.fd,\
	"\t</folder>\n\n")

#define unknown_folder(path, file, palette, paletteindex) fprintf(unknownFile.fd,\
	"\t<folder name=\"unknown\" path=\"%s\" file=\"%s\" palette=\"%d\" paletteindex=\"%s\">\n",\
	                            path,       file,       palette,       paletteindex)

#define unknown_foot() fprintf(unknownFile.fd,\
	"</resources>\n")

#define unknown_head() fprintf(unknownFile.fd,\
	"<!DOCTYPE resources SYSTEM \"http://www.princed.com.ar/standards/xml/resources/std1.dtd\">\n"\
	"<?xml version=\"1.0\" ?>\n<resources version=\"generated\">\n")

#define unknown_item(value,index,path,type,flags,typedesc,count) fprintf(unknownFile.fd,\
	"\t\t<item value=\"%d\" index=\"%s\" path=\"%s\" type=\"%s\" flags=\"0x%lx\">Unknown %s %d</item>\n",\
	           value,       index,       path,       type,       flags,          typedesc, count)


/* tree module */
#define outputStream stdout

void showTag(int n,tTag* t) {
	int a;
	tTag* children;
	for(a=0;a<n;a++) fprintf (outputStream,"\t");
	if (t!=NULL) {
		fprintf(outputStream,"<%s",t->tag);

#define FillAttr(a,b) if (a) fprintf(outputStream," %s=\"%s\"",b,a)

	FillAttr(t->desc,"desc");
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
				showTag(n+1,children);
				children=children->next;
			}
			for(a=0;a<n;a++) fprintf (outputStream,"\t");
			fprintf(outputStream,"</%s>\n",t->tag);
		} else {
			fprintf(outputStream," />\n");
		}
	}
}


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
	printf("deleting file %s from tree\n",file);
/*	showTag(0,unknownFile.tree);*/
	if (unknownFile.tree)
		rec_tree(&(unknownFile.tree->child),file);

/*	showTag(0,unknownFile.tree);*/

}

/***************************************************************\
|                          Semantic Layer                       |
\***************************************************************/

int unknownLogStart (const char* file,int optionflag, const char* backupExtension) {
	if (unknownFile.fd) return -1; /* File already open */

	/* Use default filename if file is NULL */
	if (!file) file=RES_XML_UNKNOWN_XML;

	/* Remember optionflag and backupExtension */
	unknownFile.optionflag=optionflag;
	unknownFile.backupExtension=strallocandcopy(backupExtension);
	unknownFile.currentDat=NULL;

	/* Read the previous file if exists */
	{
		int error;

		unknownFile.tree=parseXmlFile(file,&error);
		if (!error) {
		/*	showTag(0,unknownFile.tree);*/
		} else {
			unknownFile.tree=NULL;
			printf("Error parsing %s (code=%d)\n",file,error);
		}
	}
	
	/* Open the file */
	if (!writeOpen(file,&unknownFile.fd,optionflag)) return -2; /* file not open */

	return 0; /* Ok */
}

int unknownLogStop () {
	int i;

	if (!unknownFile.fd) return -1; /* File not open */

	/* close a folder if it is open */
	if (unknownFile.currentDat) { /* there is a folder open */
		unknown_folderclose();
		unknown_foot(); 
	} else {
		unknown_emptyfile();
	}
	
	/* Close the file */
	writeCloseOk(unknownFile.fd,unknownFile.optionflag,unknownFile.backupExtension);

	/* Free structures */
	freeAllocation(unknownFile.currentDat);
	freeAllocation(unknownFile.backupExtension);
	unknownFile.backupExtension=NULL;
	unknownFile.fd=NULL;
	for (i=0;i<RES_TYPECOUNT;i++) unknownFile.typeCount[i]=0; /* re-initialize in 0 for next file processing */
	showTag(0,unknownFile.tree);
	freeParsedStructure (&unknownFile.tree);

	return 0; /* Ok */
}

int unknownLogAppend(const char* vFiledat,tResourceId id,const char* ext,tResourceType type,const char* vDirExt,tResourceId pal,const char* vFiledatWithPath,int optionflag,int count, unsigned long int flags,const char* filename) {
	if (!unknownFile.fd) return -1; /* File not open, logging if off */

	if (!unknownFile.currentDat) { /* this is the beginning of the file */
		unknown_head();
		unknown_folder(vFiledatWithPath,vFiledat,pal.value,translateInt2Ext(toLower(pal.index)));
		unknownFile.currentDat=strallocandcopy(vFiledat);
		/* TODO: move here the read-parsing-loading and write-opening */
		unknown_deletetreefile(vFiledat);
	} else if (!equalsIgnoreCase(unknownFile.currentDat,vFiledat)) {
		int i;
		unknown_folderclose(); 
		unknown_folder(vFiledatWithPath,vFiledat,pal.value,translateInt2Ext(toLower(pal.index)));
		freeAllocation(unknownFile.currentDat);
		unknownFile.currentDat=strallocandcopy(vFiledat);
		unknown_deletetreefile(vFiledat);
		for (i=0;i<RES_TYPECOUNT;i++) unknownFile.typeCount[i]=0; /* re-initialize in 0 for next file processing */
	}

	unknown_item(id.value,translateInt2Ext(toLower(id.index)),filename,getExtDesc(type),flags,getExtDesc(type),count);

	return 0;
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

