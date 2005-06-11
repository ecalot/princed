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
#include "common.h"
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
	/* this is only pop2 palette */
	return ((size==101)&&(!array[2])&&(!array[3])&&(array[4]==0x10));
}

int verifySpeakerHeader(const unsigned char *array, int size) {
	/* format: (checksum)+(0x00)+(even number)+3 bytes per note */
	return
		(size>4)&&(array[1]==0x00)&&(!(array[2]%2))&&(!(size%3))
	;
}

int verifyWaveHeader(const unsigned char *array, int size) {
	/* format: (checksum)+(0x01)+raw wave */
	return
		(size>1)&&(array[1]==0x01)&&((size%3)==2)
	;
}

int verifyMidiHeader(const unsigned char *array, int size) {
	/* format: (checksum)+(0x02)+"MThd"... */
	return
		(size>6) &&
		(array[1]==0x02) &&
		(array[2]=='M') &&
		(array[3]=='T') &&
		(array[4]=='h') &&
		(array[5]=='d')
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

int partialListActive() {
	return partialList.count;
}

#define PARSING_MAX_TOKEN_SIZE 200

void parseGivenPath(char* path) {
	/*
		PRE:  partialList.list was not allocated
		POST:
		 partialList.count=0 and partialList.list=NULL if all resources
		 path was trimed in the "@"
	*/

	int i;
	int separator=0;
	int j=0;
	int size;
	char aux[PARSING_MAX_TOKEN_SIZE];

	/* Check if the variable wasn't initialized before */
	if (partialList.count!=0) return;
	partialList.list=NULL;

	/* Validates the NULL path */
	if (path==NULL) return;

	/* Erase the last "/" if exists. */
	if (path) {
		size=strlen(path);
		if (size>0) {
			size--;
			if (isDirSep(path,size)) path[size]=0;
		}
	}
	
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
	for(i=separator;j!=partialList.count;i++) {
		unsigned int value;
		int converted;
		char index[5];
		converted=sscanf(path+i,"%u:%5s",&value,index);
		switch (converted) {
		case 2:
			partialList.list[j].type=eId;
			partialList.list[j].field.id.value=value;
			str5uppercpy(partialList.list[j].field.id.index,index);
			break;
		case 1:
			partialList.list[j].type=eIdValue;
			partialList.list[j].field.id.value=value;
			break;
		default:
			partialList.list[j].type=eString;
			aux[0]='/';aux[1]=0;
			strcat(aux,path+i);
			partialList.list[j].field.text=strallocandcopy(repairFolders(aux));
			break;
		}
		while (path[i]) i++;
		j++;
	}
}

/* Code taken from PR 0.1 */
int getMaskToken(const char texto[],char token[],int* i,int k) {
	int j=0;
	/* Copy text string until a space is found or the end of line is reached.
	 * In case the text diesn't fit inside the token it is truncated, but i is increased until the end of the token */
	char entreComillas=0; /* flag indicating if we are inside the quotation marks */
	while ((((entreComillas^=(texto[(*i)]=='"')) || (texto[*i]!=' ')) && ((j<k)?(token[j++]=texto[(*i)++]):(texto[(*i)++]))));
	token[j]='\0';
	return (texto[((*i)++)-1]);
}

int matches1(const char* text,const char* mask,int ptext, int pmask) {
	/*
	 * Verify if the text matches in the mask.
	 * In case a wildcar * is found it recurses to itself
	 * Optimization:
	 *  i) multiple contiguous wildcars are taken as single to avoid
	 *     executing the recursivity of order O(n²) more extra times.
	 *  ii) a wildcard at the end of the mask is taken as the end of the
	 *     comparission. No recursivity is needed to know that is true.
	 *  
	 *  " are ignored
	 *  ? is interpreted as any character (including * and ? itself)
	 *
	 *  Returns true in case it matches and false in case it doesn't
	 */

	while (text[ptext]||mask[pmask]) {
		if (mask[pmask]=='"') {
			pmask++;
		} else if (mask[pmask]=='?') {
			pmask++;
			if (!text[ptext]) return 0;
			ptext++;
		} else if (mask[pmask]=='*') {
			int aux;
			while (mask[pmask++]=='*'); /* optimization i */
			pmask--;
			if (!mask[pmask]) return 1; /* optimization ii */
			while ((text[ptext]) && !(aux=matches1(text,mask,ptext++,pmask)));
			return aux;
		} else {
			if (text[ptext]!=mask[pmask]) return 0;
			ptext++;
			pmask++;
		}
	}
	return (text[ptext]==mask[pmask]);
}

int matchesIn(const char* text,const char* mask) {
	int i=0;
	char token[PARSING_MAX_TOKEN_SIZE];
	char valor=0;

	/*
	 * matches in main function: partitionates the text using spaces and matches
	 * each token against the text
	 */
	while (getMaskToken(mask,token,&i,PARSING_MAX_TOKEN_SIZE) && !(valor=(valor || matches1(text,token,0,0))));
	return valor?1:matches1(text,token,0,0);
}

int isInThePartialList(const char* vFile, tResourceId id) {
	/*
		Cases:
			"path/path@"                     all files are false
			"path/path"                      all files are true
			"path/path@12file/jjj.bmp,777"   only file "12file/jjj.bmp" and id 777 are true
			"path/path@1,2,3"                only ids 1, 2 and 3 are true
			"path/path@12file/?mage1*.bmp"   each file matching "12file/?mage1*.bmp" is true
	*/
	int i;

	if (!partialList.count) return 1;

	for (i=0;i<partialList.count;i++) {
		switch (partialList.list[i].type) {
		case eIdValue:
			if (id.value==partialList.list[i].field.id.value) return 1;
			break;
		case eString:
			if (vFile && matchesIn(repairFolders(vFile),partialList.list[i].field.text)) return 1;
			break;
		case eId:
			if (!resIdCmp(id,partialList.list[i].field.id)) return 1;
			break;
		}
	}
	return 0;
}

void freePartialList() {
	int i;
	for (i=0;i<partialList.count;i++) {
		if (partialList.list[i].type==eString)
			free(partialList.list[i].field.text);
	}
	free(partialList.list);
	partialList.count=0;
}

/***************************************************************\
|                       Checksum handling                       |
\***************************************************************/

int checkSum(const unsigned char* data,int size) {
	unsigned char  checksum = 1;

	/* validates the checksum */
	while (size--) checksum+=*(data++);
	return !checksum;
}

/***************************************************************\
|                     Parsing resource file                     |
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

void AddToUnknownXml(const char* vFiledatWithPath,tResourceId id,const char* ext,char type,const char* vDirExt,tResourceId pal,const char* vFiledat,int optionflag,int count) {
	/* Open file if not open */
	if (unknownXmlFile==NULL) {
		char xmlFile[MAX_FILENAME_SIZE];
		sprintf(xmlFile,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_XML,vDirExt,vFiledatWithPath);

		/* Open file */
		if (!writeOpen(xmlFile,&unknownXmlFile,optionflag)) return;

		/* Save headers */
		if (type==6) pal=id;
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_START,
			vFiledat,vFiledatWithPath,pal.value,toLower(pal.index)
		);
	}

	/* Write item */
	fprintf(unknownXmlFile,RES_XML_UNKNOWN_ITEM,
		id.value,toLower(id.index),getExtDesc(type),count,ext,getExtDesc(type),getExtDesc(type),count
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

/***************************************************************\
|                   Resources extra functions                   |
\***************************************************************/

void getFileName(char* vFileext,const char* vDirExt,const tResource* r,const char* vFiledat, const char* vDatFileName,int optionflag, const char* backupExtension) {
	static const char* extarray[]=RES_FILE_EXTENSIONS;
	int pos;

	if (r->path==NULL) {
		pos=((r->type<RES_TYPECOUNT)&&(r->type>=0))?r->type:RES_TYPE_BINARY;
		typeCount[pos]++;

		/* set filename */
		sprintf(vFileext,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_FILES,vDirExt,vDatFileName,getExtDesc(pos),typeCount[pos],extarray[pos]);
		AddToUnknownXml(vDatFileName,r->id,extarray[pos],r->type,vDirExt,r->palette,vFiledat,optionflag,typeCount[pos]);
	} else {
		/* set filename */
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

