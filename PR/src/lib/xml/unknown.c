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
#include <string.h>
#include <stdlib.h>
#include "pr.h"
#include "xmlparse.h"
#include "xmlsearch.h"
#include "disk.h"
#include "memory.h"
#include "resources.h"
#include "compress.h"

extern FILE* outputStream;

/***************************************************************\
|                       Item Type Detector                      |
\***************************************************************/

int verifyLevelHeader(const unsigned char *array, int size) {
	return (((size==2306)||(size==2305))&&!(array[1698]&0x0F)&&!(array[1700]&0x0F)&&!(array[1702]&0x0F));
}

int verifyImageHeader(const unsigned char *array, int size) {
	unsigned char imageBitRate;
	imageBitRate=((unsigned char)array[6]&0xF0);
	return (size>7) && (!array[5]) && ((imageBitRate==0xB0));
	/* return (size>7) && (!array[5]) && ((imageBitRate==0xB0)||(imageBitRate==0x00)); */
}

int verifyPaletteHeader(const unsigned char *array, int size) {
	return ((size==101)&&(!array[2])&&(!array[3])&&(array[4]==0x10));
}

int verifyMidiHeader(const unsigned char *array, int size) {
	return
		(size>6) &&
		(array[1]==0x02) &&
		(array[2]=='M') &&
		(array[3]=='T') &&
		(array[4]=='h') &&
		(array[5]=='d')
	;
}

int verifyWaveHeader(const unsigned char *array, int size) {
	return
		(size>1)&&(array[1]==0x01)
	;
}

int verifySpeakerHeader(const unsigned char *array, int size) {
	return
		(size>2)&&(array[1]==0x00) /* &&!(size%3) */
	;
}

int verifyHeader(const unsigned char *array, int size) {
	if (verifyLevelHeader  (array,size)) return RES_TYPE_LEVEL;
	if (verifyMidiHeader   (array,size)) return RES_TYPE_MIDI;
	if (verifyImageHeader  (array,size)) return RES_TYPE_IMAGE;
	if (verifyPaletteHeader(array,size)) return RES_TYPE_PALETTE;
	if (verifyWaveHeader   (array,size)) return RES_TYPE_WAVE;
	if (verifySpeakerHeader(array,size)) return RES_TYPE_PCSPEAKER;
	return RES_TYPE_BINARY;
}

const char* getExtDesc(int type) {
	static const char* extarraydesc[]=RES_FILE_TYPES;
	return extarraydesc[type];
}

/***************************************************************\
|                Partial Resource List Functions                |
\***************************************************************/

static tResIdList partialList;

void parseGivenPath(char* path) {
	/*
		PRE:  partialList.list was not allocated
		POST:
		 partialList.count=0 and partialList.list=NULL if all resources
		 path was trimed in the "@"
	*/

	int i;
	int separator=0;
	int resourceValue;
	int j=0;
	unsigned char n;

	/* Check if the variable wasn't initialized before */
	if (partialList.count!=0) return;
	partialList.list=NULL;

	/* Validates the NULL path */
	if (path==NULL) return;

	/* Locate the string separation */
	while (path[separator]&&path[separator]!='@') separator++;

	/* If no separation */
	if (!path[separator]) return; /* There was no separator */

	/* Count values, separate them with '\0' and alloc memory */
	partialList.count=1;
	path[separator]=0; /* Trim the path to the separator */
	i=++separator;
	while(path[i]) {
		if (path[i]==',') {
			partialList.count++;
			path[i]=0;
		}
		i++;
	}
	partialList.list=(tResIdListItem*)malloc(sizeof(tResIdListItem)*partialList.count);

	/* Parse values and save them in the list */
	resourceValue=0;
	for(i=separator;j!=partialList.count;i++) {
		n=path[i];
		if ((0x2F<n)&&(n<0x3A)) {
			resourceValue=resourceValue*10+(n-'0');
		} else {
			if (n) {
				partialList.list[j].idType=eString;
				partialList.list[j].value=(char*)strallocandcopy(repairFolders(path+separator));
				while (path[i]) i++;
			} else {
				partialList.list[j].idType=eValue;
				partialList.list[j].value=(void*)resourceValue;
			}
			resourceValue=0;
			separator=i+1;
			j++;
		}
	}
}

int isInThePartialList(const char* vFile, int value) {
	/*
		Cases:
			"path/path@"                    all files are false
			"path/path"                     all files are true
			"path/path@12file/jjj.bmp,777"  only file "12file/jjj.bmp" and id 777 are true
			"path/path@1,2,3"               only ids 1, 2 and 3 are true
	*/
	int i;
	char* file;

	if (!partialList.count) return 1;
	file=(char*)repairFolders(vFile);

	for (i=0;i<partialList.count;i++) {
		if (partialList.list[i].idType==eValue) {
			if (value==(int)partialList.list[i].value) return 1;
		} else {
			if (file)
				if (equalsIgnoreCase(file,(char*)partialList.list[i].value)) return 1;
		}
	}
	return 0;
}

void freePartialList() {
	int i;
	for (i=0;i<partialList.count;i++) {
		if (partialList.list[i].idType==eString)
			free(partialList.list[i].value);
	}
	partialList.count=0;
}

/***************************************************************\
|                       Checksum handling                       |
\***************************************************************/

int checkSum(unsigned char* data,int size) {
	unsigned char  checksum = 1;

	/* calculates the checksum */
	while (size--) checksum+=*(data++);
	return !checksum;
}

/***************************************************************\
|                     Parsing resource file                     |
\***************************************************************/

/* Initializes the resource table */
void emptyTable(tResource* r[]) {
	int i=MAX_RES_COUNT;
	while (i--) *(r++)=NULL;
}

/* Resources input xml tree. Private+abstract variable */
static tTag* xmlStructure=NULL; /* Keeping the parsed file structure in memory will save a lot of time */

int parseStructure(const char* vFile) {
	static const char defaultXmlFile[]=RES_XML_RESOURC_XML;
	int error=0;

	/* Generate xml structure if doesn't exist */
	if (xmlStructure==NULL)	{
		/* Set default values */
		if (vFile==NULL) vFile=defaultXmlFile;
		xmlStructure=parseXmlFile(vFile,&error);
	}
	if (error) xmlStructure=NULL;
	return error;
}

/* parse file */
int parseFile(const char* vFile, const char* datFile, tResource* r[]) {
	/* Declare error variable */
	int error;
	tPassWork pass;

	/* Generate xml structure if doesn't exist */
	if ((error=(parseStructure(vFile)))) return error;

	/* Use the xml structure to Generate the resource structure of the file */
	emptyTable(r);
	pass.datFile=datFile;
	pass.r=r;
	workTree(xmlStructure,&pass,workTag);

	/* All done */
	return 0;
}

void freeParsedStructure() {
	/* Free if exist */
	if (xmlStructure!=NULL) freeTagStructure(xmlStructure);
	/* Reinitializes the variable */
	xmlStructure=NULL;
}

/***************************************************************\
|                     Unknown.xml primitives                    |
\***************************************************************/

/* Resources output to xml functions. Private+abstract variable */
static FILE* unknownXmlFile=NULL;

void AddToUnknownXml(const char* vFiledatWithPath,unsigned short id,const char* ext,char type,const char* vDirExt,unsigned short pal,const char* vFiledat,int optionflag,int count) {
	/* Open file if not open */
	if (unknownXmlFile==NULL) {
		char xmlFile[MAX_FILENAME_SIZE];
		sprintf(xmlFile,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_XML,vDirExt,vFiledatWithPath);

		/* Open file */
		if (!writeOpen(xmlFile,&unknownXmlFile,optionflag)) return;

		/* Save headers */
		if (type==6) pal=id;
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_START,
			vFiledat,vFiledatWithPath,pal
		);
	}

	/* Write item */
	fprintf(unknownXmlFile,RES_XML_UNKNOWN_ITEM,
		id,getExtDesc(type),count,ext,getExtDesc(type),getExtDesc(type),count
	); /* To the xml output */
}

static unsigned int typeCount[RES_TYPECOUNT]; /* initialized in 0 */

void endUnknownXml(int optionflag, const char* backupExtension) {
	if (unknownXmlFile!=NULL) {
		int i;
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_END);
		writeCloseOk(unknownXmlFile,optionflag,backupExtension);
		unknownXmlFile=NULL;
		for (i=0;i<RES_TYPECOUNT;i++) typeCount[i]=0;
	}
}

/***************************************************************\
|                   Resources extra functions                   |
\***************************************************************/

void getFileName(char* vFileext,const char* vDirExt,tResource* r,unsigned short id,const char* vFiledat, const char* vDatFileName,int optionflag, const char* backupExtension) {
	static const char* extarray[]=RES_FILE_EXTENSIONS;
	int pos;

	if (r->path==NULL) {
		pos=((r->type<RES_TYPECOUNT)&&(r->type>=0))?r->type:RES_TYPE_BINARY;
		typeCount[pos]++;

		/* set filename */
		sprintf(vFileext,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_FILES,vDirExt,vDatFileName,getExtDesc(pos),typeCount[pos],extarray[pos]);
		AddToUnknownXml(vDatFileName,id,extarray[pos],r->type,vDirExt,r->palette,vFiledat,optionflag,typeCount[pos]);
	} else {
		/* set filename */
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

/* Search files for the Import feature */
int importDir(const char* directory, const char* vResFile, int pOption, const char* backupExtension,const char* vDatDirectory, FILE* output) {
	/* Declare error variable */
	int error=0;
	char* datfile;
	char* recursive;
	int sizeOfPath;
	int sizeOfFile;
	int result;

	/* Generate xml structure if doesn't exist */
	if ((error=(parseStructure(vResFile)))) return error;

	/* Use the xml structure to Generate the file list */
	workTree(xmlStructure,NULL,addFileToList);

	while(datfile=getFileFromList()) {
		sizeOfPath=strlen(vDatDirectory);
		sizeOfFile=strlen(datfile);

		/* Generate full vDatDirectory/datfile path */
		recursive=(char*)malloc(sizeOfPath+sizeOfFile+2);
		memcpy(recursive,vDatDirectory,sizeOfPath);
		recursive[sizeOfPath]=DIR_SEPARATOR;
		memcpy(recursive+sizeOfPath+1,datfile,sizeOfFile+1);

		/* Run program */
		result=prMain(pOption, backupExtension,directory,vResFile,recursive,datfile,NULL,output);
		/* Free memory */
		free(datfile);
		free(recursive);
	}

	/* All done */
	return result;
}

int isntADatFile(const char* testFile, const char* vResFile) {
	/*
		Results:
			0  Is a dat file
			1  It isn't a dat file
			-1 Parse error
			-2 No memory
			-3 Attribute not recognized
			-4 File not found
	*/

	/* Declare result variable */
	int result;
	char* gottenFile;

	/* Generate xml structure if doesn't exist */
	if ((result=(parseStructure(vResFile)))) return result;

	/* Use the xml structure to Generate the file list */
	workTree(xmlStructure,NULL,addFileToList);
	while((gottenFile=(getFileFromList()))) {
		result=result||equalsIgnoreCase(gottenFile,testFile);
		free(gottenFile);
	}

	/* All done */
	return !result;
}

/***************************************************************\
|                Resource tree browsing for DLL                 |
\***************************************************************/

#ifdef DLL

tTag* resourceTreeGetRoot () {
	return xmlStructure;
}

tTag* resourceTreeGetNext (tTag* whereAmI) {
	return whereAmI->next;
}

tTag* resourceTreeGetChild(tTag* whereAmI) {
	return whereAmI->child;
}

int   resourceTreeGetInfo (tTag* whereAmI,	char** tag, char** desc, char** path, char** file, char** itemtype, char** name, char** palette, char** type, char** value, char** version, char** number) {
	if (whereAmI==NULL) return 0;
	*tag=whereAmI->tag;
	*desc=whereAmI->desc;
	*path=whereAmI->path;
	*file=whereAmI->file;
	*itemtype=whereAmI->itemtype;
	*name=whereAmI->name;
	*palette=whereAmI->palette;
	*type=whereAmI->type;
	*value=whereAmI->value;
	*version=whereAmI->version;
	*number=whereAmI->number;
	return 1;
}

#endif

