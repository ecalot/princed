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

//Includes
#include <stdio.h>
#include <stdlib.h>
#include "pr.h"
#include "xml.h"
#include "xmlsearch.h"
#include "disk.h"
#include "memory.h"
#include "resources.h"
#include "compress.h"

/***************************************************************\
|                       File format handling                    |
\***************************************************************/

int verifyLevelHeader(unsigned char* array, int size) {
	return (((size==2306)||(size==2305))&&!(array[1698]&0x0F)&&!(array[1700]&0x0F)&&!(array[1702]&0x0F));
}

int verifyImageHeader(unsigned char* array, int size) {
	//return (size>6) && (!(((!array[1])||(array[2])||(!array[3])||(array[4])||(array[5])||(((unsigned char)array[6]&0xF0)!=0xB0))));
	return (size>7) && (!array[5]) && (((unsigned char)array[6]&0xF0)==0xB0);
	//TODO: fix the expression
}

int verifyPaletteHeader(unsigned char* array, int size) {
	return ((size==101)&&(!array[2])&&(!array[3])&&(array[4]==0x10));
}

int verifyMidiHeader(unsigned char* array, int size) {
	return
	  (size>6) &&
		(array[1]==0x02) &&
		(array[2]=='M') &&
		(array[3]=='T') &&
		(array[4]=='h') &&
		(array[5]=='d')
	;
}

int verifyWaveHeader(unsigned char* array, int size) {
	return
		(size>1)&&(array[1]==0x01)
	;
}

int verifySpeakerHeader(unsigned char* array, int size) {
	return
		(size>2)&&(array[1]==0x00) /* &&!(size%3) */
	;
}

int verifyHeader(unsigned char* array, int size) {
	if (verifyLevelHeader(array,size)) return 1;
	if (verifyMidiHeader(array,size)) return 4;
	if (verifyImageHeader(array,size)) return 2;
	if (verifyPaletteHeader(array,size)) return 6;
	if (verifyWaveHeader(array,size)) return 3;
	if (verifySpeakerHeader(array,size)) return 7;
	return 05;
}

const char* getExtDesc(int type) {
	static const char* extarraydesc[]=RES_FILE_TYPES;
	return extarraydesc[type];
}

/***************************************************************\
|                      Parsing resource file                    |
\***************************************************************/

//Initializes the resource table
void emptyTable(tResource* r[]) {
	int i=MAX_RES_COUNT;
	while (i--) *(r++)=NULL;
}

//TODO: use a static XML tree and two functions: parseFile and freeXmlStructure

//Resources input xml tree. Private+abstract variable
static tTag* xmlStructure=NULL; /* Keeping the parsed file structure in memory will save a lot of time */


//parse file
char parseFile(const char* vFile, const char* datFile, tResource* r[]) {
	//Declare error variable
	int error=0;

	//Generate xml structure if doesn't exist
	if (xmlStructure==NULL)	xmlStructure=parseXmlFile(vFile,&error);
	if (error) {
		xmlStructure=NULL;
		return error;
	}

	//Use the xml structure to Generate the resource structure of the file
	emptyTable(r);
	workTree(xmlStructure,datFile,r);

	//All done
	return 0;
}

void freeParsedStructure() {
	//Free if exist
	if (xmlStructure!=NULL) freeTagStructure(xmlStructure);

	//Reinitializes the veriable
	xmlStructure=NULL;

	return;
}


//Resources output to xml functions. Private+abstract variable
static FILE* unknownXmlFile=NULL;

void AddToUnknownXml(const char* vFiledat,unsigned short id,const char* ext,char type,const char* vDirExt,unsigned short pal) {
	//Open file if not open
	if (unknownXmlFile==NULL) {
		char xmlFile[MAX_FILENAME_SIZE];
		sprintf(xmlFile,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_XML,vDirExt,vFiledat);

		//Open file
		if (!writeOpen(xmlFile,&unknownXmlFile)) return;

		//Save headers
		if (type==6) pal=id;
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_START,
			vFiledat,vFiledat,pal
		);
	}

	//Write item
	fprintf(unknownXmlFile,RES_XML_UNKNOWN_ITEM,
		id,id,ext,getExtDesc(type),id
	); //To the xml output
}

void endUnknownXml() {
	if (unknownXmlFile!=NULL) {
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_END);
		fclose(unknownXmlFile);
		unknownXmlFile=NULL;
	}
}

//Resources extra functions
void getFileName(char* vFileext,const char* vDirExt,tResource* r,short id,const char* vFiledat, const char* vDatFileName) {
	static const char extarray[8][4]=RES_FILE_EXTENSIONS;
	const char* ext;

	if (r->path==NULL) {
		ext=extarray[((r->type<=7)&&(r->type>=0))?r->type:5];
		//set filename
		sprintf(vFileext,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_FILES,vDirExt,vDatFileName,id,ext);
		AddToUnknownXml(vDatFileName,id,ext,r->type,vDirExt,r->palette);
	} else {
		//set filename
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

//Search files for the Import feature
int importDir(const char* directory, const char* vResFile, int* pOption, const char* backupExtension,const char* vDatDirectory, FILE* output) {
	//TODO send to a function called by parseFile & importDir
	//Declare error variable
	int error=0;
	char* datfile;
	char* recursive;
	int sizeOfPath;
	int sizeOfFile;
	int result;

	//Generate xml structure if doesn't exist
	if (xmlStructure==NULL)	xmlStructure=parseXmlFile(vResFile,&error);
	if (error) {
		xmlStructure=NULL;
		return error;
	}

	//Use the xml structure to Generate the file list
	getFiles(xmlStructure);

	while(datfile=getFileFromList()) {
		sizeOfPath=strlen(vDatDirectory);
		sizeOfFile=strlen(datfile);

		//Generate full vDatDirectory/datfile path
		recursive=getMemory(sizeOfPath+1+sizeOfFile);
		memcpy(recursive,vDatDirectory,sizeOfPath);
		recursive[sizeOfPath]=DIR_SEPARATOR;
		memcpy(recursive+sizeOfPath+1,datfile,sizeOfFile+1);

		//Run program
		result=prMain(pOption, backupExtension,directory,vResFile,recursive,datfile,NULL,output);
		//Free mem
		free(datfile);
		free(recursive);
	}

	//All done
	return result;
}

