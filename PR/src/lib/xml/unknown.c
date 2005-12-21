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

/* XML generation defines */

#define RES_XML_UNKNOWN_PATH  "%s/unknown/%s/"
#define RES_XML_UNKNOWN_FILES "%t%03n.%e"

static struct {
	FILE* fd;
	char* backupExtension;
	char* currentDat;
	int optionflag;
	unsigned int typeCount[RES_TYPECOUNT]; /* initialized in 0 */
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

/***************************************************************\
|                          Semantic Layer                       |
\***************************************************************/

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

	return 0; /* Ok */
}

int unknownLogAppend(const char* vFiledatWithPath,tResourceId id,const char* ext,tResourceType type,const char* vDirExt,tResourceId pal,const char* vFiledat,int optionflag,int count, unsigned long int flags,const char* filename) {
	if (!unknownFile.fd) return -1; /* File not open, logging if off */

	if (!unknownFile.currentDat) { /* this is the beginning of the file */
		unknown_head();
		unknown_folder(vFiledat,vFiledatWithPath,pal.value,translateInt2Ext(toLower(pal.index)));
		unknownFile.currentDat=strallocandcopy(vFiledat);
	} else if (!equalsIgnoreCase(unknownFile.currentDat,vFiledat)) {
		unknown_folderclose(); 
		unknown_folder(vFiledat,vFiledatWithPath,pal.value,translateInt2Ext(toLower(pal.index)));
		freeAllocation(unknownFile.currentDat);
		unknownFile.currentDat=strallocandcopy(vFiledat);
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

		sprintf(vFileext,RES_XML_UNKNOWN_PATH"%s",vDirExt,vDatFileName,filename);
		unknownLogAppend(vDatFileName,r->id,extarray[pos],r->type,vDirExt,r->palette,vFiledat,optionflag,unknownFile.typeCount[pos],r->flags,filename);
	} else {
		/* set filename */
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

