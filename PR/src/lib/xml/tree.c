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
#include "parser.h"
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
		(size>1)&&(array[1]==0x00)
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


/***************************************************************\
|                      Parsing resource file                    |
\***************************************************************/

#if 0
//Parse line
void parseResource(tResource* r[], char* line) {
	//declare variables
	int                i=0;
	int                k=0;
	unsigned short int id,ty;

	//Begin parsing
	for (;!(line[k]=='\r'||line[k]=='\n'||(!line[k]));k++);
	line[k]=0;

	if (getNumberToken(line,&id,' ',&i,6)) {
		r[id]=(tResource*)malloc(sizeof(tResource));
		getNumberToken(line,&(*(r[id])).size,' ',&i,6);
		getNumberToken(line,&(*(r[id])).offset,' ',&i,6);
		getUpperToken(line,(*(r[id])).file,' ',&i,12);
		if (getNumberToken(line,&ty,' ',&i,3)) {
			(*(r[id])).desc=(char*)malloc(255);
			if (getToken(line,(*(r[id])).desc,'#',&i,255)) {
				(*(r[id])).coms=(char*)malloc(1023);
				if (!getToken(line,(*(r[id])).coms,0,&i,1023)) {
					free ((*(r[id])).coms);
					(*(r[id])).coms=NULL;
				}
			}
		} else {
			(*(r[id])).desc=NULL;
		}
		(*(r[id])).type=(char)ty;
	}
}
#endif

void emptyTable(tResource* r[]) {
	int i=0;
	for (;i<65536;i++) r[i]=NULL;
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

//Old code (will be removed soon)
#if 0
	//declare variables
	char parsing=0;
	char line[MAX_LINE_SIZE];
	char B[]=BEGIN_TABLE;
	char E[]=END_TABLE;
	FILE* fp;

	//empty resource array
	emptyTable(r);

	//parse file
	if ((fp=fopen(vFile,"rt"))!=NULL) {
		while (fgets(line,MAX_LINE_SIZE,fp)!=NULL) {
			if (parsing) {
				if (equals(line,E)) {
					parsing=0;
				} else {
					parseResource(r,line);
				}
			} else {
				if (equals(line,B)) parsing=1;
			}
		}
		fclose(fp);
		return 1;
	} else {
		return 0;
	}
#endif
}

//generate file
char generateFile(char* vFile,tResource* r[]) {

//Old code (will be removed soon)
#if 0
	//declare variables
	FILE* fp;
	FILE* source;
	char B[]=BEGIN_TABLE;
	char E[]=END_TABLE;
	char coms[MAX_LINE_SIZE];
	char line[MAX_LINE_SIZE];
	char none[]="";
	char parsing=0;
	int id=0;

	if ((fp=fopen("res.tmp","wt"))!=NULL) {
		//insert headers
		if ((source=fopen(vFile,"rt"))!=NULL) {
			while ((parsing!=2)&&(fgets(line,MAX_LINE_SIZE,source)!=NULL)) {
				if (parsing) {
					if (equals(line,E)) {
						parsing=2;
					}
				} else {
					if (equals(line,B)) {
						parsing=1;
					} else {
						fputs(line,fp);
					}
				}
			}
		}

		//insert main body file
		fputs(B,fp);
		fputs("\n",fp);

		for (;id<65536;id++) {
			if (r[id]!=NULL) {
				if (1||((*(r[id])).desc==NULL)) { //todo see this line
					if ((*(r[id])).coms==NULL) {
						sprintf(coms,none);
					} else {
						sprintf(coms," #",(*(r[id])).coms);
					}
				} else {
					if ((*(r[id])).coms==NULL) {
						sprintf(coms," %s",(*(r[id])).desc);
					} else {
						sprintf(coms," %s#%s",(*(r[id])).desc,(*(r[id])).coms);
					}
				}
				sprintf(line,"<item itemtype=\"%d\" value=\"%d\" file=\"%s\" external=\"res%05d.bmp\">Res %d</item>\n",
					(*(r[id])).type,
					id,
				//(*(r[id])).size,
				//(*(r[id])).offset,
				(*(r[id])).file,
				id,id
				//,
				//coms
				);
				fputs(line,fp);
			}
		}
		fputs("\n",fp);
		fputs(E,fp);

		//insert footers
		if (source!=NULL) {
			while ((parsing==2) && (fgets(line,MAX_LINE_SIZE,source)!=NULL)) {
				fputs(line,fp);
			}
			fclose(source);
		}
		fclose(fp);
		remove(vFile);
		rename("res.tmp",vFile);
		return 1;
	} else {
		return 0;
	}
#else
	//New code ignores this function (resources.xml is read only)
	return 1;
#endif
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
		id,id,ext,type,id
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
