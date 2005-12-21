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
#include "stringformat.h"
#include "translate.h"

/***************************************************************\
|                     Unknown.xml primitives                    |
\***************************************************************/

unsigned int typeCount[RES_TYPECOUNT]; /* initialized in 0 */

static struct {
	FILE* fd;
	char* backupExtension;
	char* currentDat;
	int optionflag;
	unsigned int typeCount[RES_TYPECOUNT]; /* initialized in 0 */
} unknownFile;

/* syntactic layer */

/* fwrite(RES_XML_UNKNOWN_END,1,sizeof(RES_XML_UNKNOWN_END)-1,unknownXmlFile);*/

#define unknown_emptyfile()\
	fprintf(unknownFile.fd, "<?xml version=\"1.0\" ?>\n<resources />\n")

#define unknown_folderclose()\
	fprintf(unknownFile.fd, "\t</folder>\n\n")

#define unknown_folder(file)\
	fprintf(unknownFile.fd, "\t<folder file=\"%s\">\n", file)

#define unknown_foot()\
	fprintf(unknownFile.fd, "</resources>\n")

#define unknown_head()\
	fprintf(unknownFile.fd, "<?xml version=\"1.0\" ?>\n<resources>\n")

#define unknown_item(path)\
	fprintf(unknownFile.fd, "\t\t<item path=\"%s\" />\n", path)


/* semantic layer */

int unknownLogStart (const char* file,int optionflag, const char* backupExtension) {
	if (unknownFile.fd) return -1; /* File already open */

	/* Use default filename if file is NULL */
	if (!file) file="unknown.xml";

	/* Remember optionflag and backupExtension */
	unknownFile.optionflag=optionflag;
	unknownFile.backupExtension=strallocandcopy(backupExtension);
	unknownFile.currentDat=NULL;

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
		unknown_foot(); /* fwrite(RES_XML_UNKNOWN_END,1,sizeof(RES_XML_UNKNOWN_END)-1,unknownXmlFile);*/
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

	return 0; /* Ok */
}

int unknownLogAppend(const char* vFiledatWithPath,tResourceId id,const char* ext,tResourceType type,const char* vDirExt,tResourceId pal,const char* vFiledat,int optionflag,int count, unsigned long int flags,const char* filename) {
	if (!unknownFile.fd) return -1; /* File not open, logging if off */

	if (!unknownFile.currentDat) { /* this is the beginning of the file */
		unknown_head();
		unknown_folder(vFiledat); 
		unknownFile.currentDat=strallocandcopy(vFiledat);
	} else if (!equalsIgnoreCase(unknownFile.currentDat,vFiledat)) {
		unknown_folderclose(); 
		unknown_folder(vFiledat); 
		freeAllocation(unknownFile.currentDat);
		unknownFile.currentDat=strallocandcopy(vFiledat);
	}

	unknown_item(ext);

	return 0;
}


char* toLower(const char* txt) { /* TODO: send to memory.c */
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



/* Resources output to xml functions. Private+abstract variable */
/*static FILE* unknownXmlFile=NULL;*/

void AddToUnknownXml(const char* vFiledatWithPath,tResourceId id,const char* ext,tResourceType type,const char* vDirExt,tResourceId pal,const char* vFiledat,int optionflag,int count, unsigned long int flags,const char* filename) {

	unknownLogAppend(vFiledatWithPath,id,ext,type,vDirExt,pal,vFiledat,optionflag,count,flags,filename);

#if 0
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
		id.value,translateInt2Ext(toLower(id.index)),filename,getExtDesc(type),flags,getExtDesc(type),count
	); /* To the xml output */
#endif
}


void endUnknownXml(int optionflag, const char* backupExtension) {
#if 0
	if (unknownXmlFile!=NULL) {
		int i;
		fwrite(RES_XML_UNKNOWN_END,1,sizeof(RES_XML_UNKNOWN_END)-1,unknownXmlFile);
		writeCloseOk(unknownXmlFile,optionflag,backupExtension);
		unknownXmlFile=NULL;
		for (i=0;i<RES_TYPECOUNT;i++) typeCount[i]=0; /* re-initialize in 0 for next file processing */
	}
#endif
}


/* Middle layer function */

void getFileName(char* vFileext,const char* vDirExt,const tResource* r,const char* vFiledat, const char* vDatFileName,int optionflag, const char* backupExtension,const char* format) {
	static const char* extarray[]=RES_FILE_EXTENSIONS;
	const char* filename;
	int pos;

	if (r->path==NULL) {
		pos=((r->type<RES_TYPECOUNT)&&(r->type>=0))?r->type:eResTypeBinary;
		typeCount[pos]++;

		/* set filename */
		if (!format) format=RES_XML_UNKNOWN_FILES;
		filename=parseformat(format,r->id.value,r->id.index,getExtDesc(pos),extarray[pos],typeCount[pos],r->id.order,r->desc);

		sprintf(vFileext,RES_XML_UNKNOWN_PATH"%s",vDirExt,vDatFileName,filename);
		AddToUnknownXml(vDatFileName,r->id,extarray[pos],r->type,vDirExt,r->palette,vFiledat,optionflag,typeCount[pos],r->flags,filename);
	} else {
		/* set filename */
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

