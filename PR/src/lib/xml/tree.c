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
resources.c: Princed Resources : Resource Handler
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

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
#include "resources.h"
#include "search.h"
#include "translate.h"

/***************************************************************\
|            Filtering xml structure to tResourceList           |
\***************************************************************/

/* parse file */
int parseFile(const char* vFile, const char* datFile, tResourceList *r) {
	/* Declare error variable */
	int error;
	tPassWork pass;
	tTag* structure;

	/* Generate xml structure if doesn't exist */
	if ((error=parseStructure(vFile,&structure))) return error;

	/* Use the xml structure to Generate the resource structure of the file */
	pass.datFile=datFile;
	pass.r=r;
	workTree(structure,&pass,workTag);

	/* All done */
	return PR_RESULT_SUCCESS;
}

/***************************************************************\
|                     Unknown.xml primitives                    |
\***************************************************************/

/* Resources output to xml functions. Private+abstract variable */
static FILE* unknownXmlFile=NULL;

char* toLower(const char* txt) {
	static char ret[5];
	char* r=ret;
	while (*txt) {
		*r=(('A'<=(*txt)) && ((*txt)<='Z'))?(*txt)|0x20:*txt;
		r++;
		txt++;
	}
	*r=0;
	return ret;			
}

void AddToUnknownXml(const char* vFiledatWithPath,tResourceId id,const char* ext,tResourceType type,const char* vDirExt,tResourceId pal,const char* vFiledat,int optionflag,int count, unsigned long int flags) {
	/* Open file if not open */
	if (unknownXmlFile==NULL) {
		char xmlFile[MAX_FILENAME_SIZE];
		sprintf(xmlFile,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_XML,vDirExt,vFiledatWithPath);

		/* Open file */
		if (!writeOpen(xmlFile,&unknownXmlFile,optionflag)) return;

		/* Save headers */
		if (type==eResTypePalette) pal=id;
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_START,
			vFiledat,vFiledatWithPath,pal.value,translateInt2Ext(toLower(pal.index))
		);
	}

	/* Write item */
	fprintf(unknownXmlFile,RES_XML_UNKNOWN_ITEM,
		id.value,translateInt2Ext(toLower(id.index)),getExtDesc(type),count,ext,getExtDesc(type),flags,getExtDesc(type),count
	); /* To the xml output */
}

static unsigned int typeCount[RES_TYPECOUNT]; /* initialized in 0 */

void endUnknownXml(int optionflag, const char* backupExtension) {
	if (unknownXmlFile!=NULL) {
		int i;
		fwrite(RES_XML_UNKNOWN_END,1,sizeof(RES_XML_UNKNOWN_END)-1,unknownXmlFile);
		writeCloseOk(unknownXmlFile,optionflag,backupExtension);
		unknownXmlFile=NULL;
		for (i=0;i<RES_TYPECOUNT;i++) typeCount[i]=0;
	}
}

void getFileName(char* vFileext,const char* vDirExt,const tResource* r,const char* vFiledat, const char* vDatFileName,int optionflag, const char* backupExtension) {
	static const char* extarray[]=RES_FILE_EXTENSIONS;
	int pos;

	if (r->path==NULL) {
		pos=((r->type<RES_TYPECOUNT)&&(r->type>=0))?r->type:eResTypeBinary;
		typeCount[pos]++;

		/* set filename */
		sprintf(vFileext,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_FILES,vDirExt,vDatFileName,getExtDesc(pos),typeCount[pos],extarray[pos]);
		AddToUnknownXml(vDatFileName,r->id,extarray[pos],r->type,vDirExt,r->palette,vFiledat,optionflag,typeCount[pos],r->flags);
	} else {
		/* set filename */
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

