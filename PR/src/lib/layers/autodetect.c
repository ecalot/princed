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

char verifyLevelHeader(char* array, int size) {
	return (((size==2306)||(size==2305))&&!(array[1698]&0x0F)&&!(array[1700]&0x0F)&&!(array[1702]&0x0F));
}

char verifyImageHeader(char* array, int size) {
	//return (size>6) && (!(((!array[1])||(array[2])||(!array[3])||(array[4])||(array[5])||(((unsigned char)array[6]&0xF0)!=0xB0))));
	return (size>7) && (!array[5]) && (((unsigned char)array[6]&0xF0)==0xB0);
	//todo: fix the expression
}

char verifyPaletteHeader(char* array, int size) {
	return ((size==101)&&(!array[2])&&(!array[3])&&(array[4]==0x10));
}

char verifyMidiHeader(char* array, int size) {
	return
	  (size>6) &&
		(array[1]==0x02) &&
		(array[2]=='M') &&
		(array[3]=='T') &&
		(array[4]=='h') &&
		(array[5]=='d')
	;
}

char verifyWaveHeader(char* array, int size) {
	return
		(size>1)&&(array[1]==0x01)
	;
}

char verifySpeakerHeader(char* array, int size) {
	return
		(size>2)&&(array[1]==0x00)
	;
}

char verifyHeader(char* array, int size) {
	if (verifyLevelHeader(array,size)) return 1;
	if (verifyMidiHeader(array,size)) return 4;
	if (verifyImageHeader(array,size)) return 2;
	if (verifyPaletteHeader(array,size)) return 6;
	if (verifyWaveHeader(array,size)) return 3;
	if (verifySpeakerHeader(array,size)) return 7;
	return 05;
}

const char* getExtDesc(int type) {
	static const char extarraydesc[8][10]={"raw","level","image","wave","midi","unknown","palette","pcspeaker"};
	return extarraydesc[type];
}

/***************************************************************\
|                      Parsing resource file                    |
\***************************************************************/

//Initializes the resource table
void emptyTable(tResource* r[]) {
	int i=0;
	for (;i<MAX_RES_COUNT;i++) r[i]=NULL;
}

//parse file
char parseFile(char* vFile, char* datFile, tResource* r[]) {

	tTag* tree;
	int error;

	tree=parseXmlFile(vFile,&error);
	if (error) return error;
	emptyTable(r);
	workTree(tree,datFile,r);
	freeTagStructure(tree);

	return 0;
}

//Resources output to xml functions
static FILE* unknownXmlFile=NULL;

void AddToUnknownXml(const char* vFiledat,unsigned short id,const char* ext,char type,const char* vDirExt,unsigned short pal) {
	if (unknownXmlFile==NULL) {
		char xmlFile[260];
		sprintf(xmlFile,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_XML,vDirExt,vFiledat);
		//Create base dir
		repairFolders(xmlFile);
		makebase(xmlFile);

		//Open file
		if ((unknownXmlFile=fopen(xmlFile,"wt"))==NULL) return;

		//Save headers
		if (type==6) pal=id;
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_START,
			vFiledat,vFiledat,pal
		);
	}
	//TODO: set itemtype in words
	fprintf(unknownXmlFile,RES_XML_UNKNOWN_ITEM,
		id,id,ext,getExtDesc(type),id
	); //To the xml output
}

void endUnknownXml() {
	if (unknownXmlFile!=NULL) {
		fprintf(unknownXmlFile,RES_XML_UNKNOWN_END);
		fclose(unknownXmlFile);
	}
}

//Resources extra functions
void getFileName(char* vFileext,char* vDirExt,tResource* r,short id,char* vFiledat) {
	static const char extarray[8][4]={"raw","pet","bmp","wav","mid","ext","pal","pcs"};
	const char* ext;
	ext=extarray[((r->type<=7)&&(r->type>=0))?r->type:5];
	if (r->path==NULL) {
		//set filename
		sprintf(vFileext,RES_XML_UNKNOWN_PATH""RES_XML_UNKNOWN_FILES,vDirExt,vFiledat,id,ext);
		AddToUnknownXml(vFiledat,id,ext,r->type,vDirExt,r->palette);
	} else {
		//set filename
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}
