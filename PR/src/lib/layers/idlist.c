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
idlist.c: Princed Resources : Partial Id list
¯¯¯¯¯¯¯¯
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
#include <string.h> /* strcat strlen */
#include "disk.h"
#include "memory.h"
#include "resourcematch.h"
#include "idlist.h"

/* Id list for partial manipulation. Private type */
typedef enum {eString,eId,eIdValue,eIdIndex}tResLocationType;
/*
typedef struct {
  tResLocationType type;
  union {
    char*        text;
    tResourceId  id;
  } field;
}tResourceMatch;
*/
typedef struct {
	int             count;
	/*tResourceMatch* list;*/
	tResourceMatch* list;
}tResIdList;

static tResIdList partialList;

int partialListActive() {
	return partialList.count;
}

/***************************************************************\
|                Partial Resource List Functions                |
\***************************************************************/

void parseGivenPath(char* path) {
	/*
	 * format: \([^@]*\)/(@\([0-9]\+\)?\(:[a-z ]*\)?\(#[a-z0-9]\+\)\)?
	 * where: \1 is the dat filename, \2 is the partical list whole match if exists,
	 *        \3 is the resource id, \4 is the index name and \5 is the order
	 * examples:                         rID Ind  Ord  Restricted to
	 *  datfile.dat@111:shape#first  --> 111 shap 0    *
	 *  datfile.dat@111:shape        --> 111 shap *    *
	 *  datfile.dat@111#first        --> 111 *    0    *
	 *  datfile.dat@#first           --> *   *    0    *
	 *  datfile.dat@:shape           --> *   shap *    *
	 *  datfile.dat@111              --> 111 *    *    *
	 *  datfile.dat@#last            --> *   *    last *
	 *  datfile.dat@#second          --> *   *    1    *
	 *  datfile.dat@:shap            --> *   shap *    *
	 *  datfile.dat@:tga palette     --> *   palt *    *
	 *  datfile.dat@#785             --> *   *    785  *
	 *  datfile.dat@/a.bmp           --> *   *    *    a.bmp
	 *  datfile.dat@/img*.bmp        --> *   *    *    img*.bmp

		PRE:  partialList.list was not allocated
		POST:
		 partialList.count=0 and partialList.list=NULL if all resources
		 path was trimed in the "@"
	*/

	int i;
	int separator=0;
	int j=0;
	int size;

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
	partialList.list=(tResourceMatch*)malloc(sizeof(tResourceMatch)*partialList.count);

	/* Parse values and save them in the list */
	for(i=separator;j!=partialList.count;i++) {
		printf("hola %s j=%d\n",path+i,j);
		initRM(path+i,partialList.list+j); /* parsing error*/
/*		unsigned int value;
		int converted;
		char index[5];
		converted=sscanf(path+i,"%u:%5s",&value,index); * TODO: support order *
		switch (converted) {
		case 2:
			partialList.list[j].type=eId;
			partialList.list[j].field.id.value=value;
			str5lowercpy(partialList.list[j].field.id.index,index);
			partialList.list[j].field.id.order=0;
			break;
		case 1:
			partialList.list[j].type=eIdValue;
			partialList.list[j].field.id.value=value;
			partialList.list[j].field.id.order=0;
			break;
		default:
			* TODO: test this *
			if (sscanf(path+i,":%5s",index)) {
				partialList.list[j].type=eIdIndex;
				strncpy(partialList.list[j].field.id.index,index,5); * TODO: check str5lowercpy *
				partialList.list[j].field.id.order=0;
			} else {
				char* aux;
				partialList.list[j].type=eString;
				aux=malloc(strlen(path+i)+2);
				aux[0]='/';
				strcpy(aux+1,path+i);
				partialList.list[j].field.text=strallocandcopy(repairFolders(aux));
				free(aux);
			}
			break;
		}
		*/
		while (path[i]) i++;
		j++;
	}
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
/*		switch (partialList.list[i].type) {
		case eIdValue:
			if (id.value==partialList.list[i].field.id.value) return 1;
			break;
		case eIdIndex:
			if (!strncmp(id.index,partialList.list[i].field.id.index,4)) return 1;
			break;
		case eString:
			if (vFile && matchesIn(repairFolders(vFile),partialList.list[i].field.text)) return 1;
			break;
		case eId:
			if (!resIdCmp(id,partialList.list[i].field.id)) return 1;
			break;
		}*/
		if (runRM(partialList.list+i,repairFolders(vFile),&id)) return 1;
	}
	return 0;
}

void freePartialList() {
	void* aux=partialList.list;
	while (partialList.count--) freeRM(partialList.list++);
	free(aux);
}

