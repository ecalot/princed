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
xmlsearch.c: Princed Resources : specific xml handling functions
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 23 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-25)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/
#include <stdio.h> /* only on debugging purposes */

//Includes
#include "xml.h"
#include "resources.h"
#include "xmlsearch.h"
#include <string.h>
#include <stdlib.h>

/****************************************************************\
|                   Tag Tree Searching Functions                 |
\****************************************************************/

#define ptoi(p) ((p!=NULL)?atoi(p):0)

tTag* searchTree(tTag* t,const char* datFile, const char* id) {
	/* tTag*
		tag pointer if found
		NULL if not found
	*/
	tTag* children;
	tTag* result;

	if (t!=NULL) {
		if (((t->file)!=NULL)&&((t->value)!=NULL)) {
			if ((equalsIgnoreCase(t->file,datFile))&&(equalsIgnoreCase(t->value,id))) {
				return t;
			}
		}
		children=t->child;

		while (children!=NULL) {
			if (NULL!=(result=searchTree(children,datFile,id))) {
				return result;
			}
			children=children->next;
		}
	}
	return NULL;
}

void workTag(const tTag* t, tResource* r[]) {
	//Declare variables
	unsigned short id;
	unsigned short size;
	int i;

	id=(unsigned short)ptoi(t->value);
	if (!id) return; //If there was not id or id contained wrong values, skip

	//Process tag and copy values to resource
	//Create Resource
	if (r[id]!=NULL) return;
	r[id]=(tResource*)malloc(sizeof(tResource));
	if (r[id]==NULL) return;

	//Get type and palette from tag
	if (t->itemtype==NULL) {
		r[id]->type=0;
	} else {
		for (i=0;i<8;i++) if (equalsIgnoreCase(t->itemtype,getExtDesc(i))) {r[id]->type=i;break;}
	}
	if (i==8) r[id]->type=(char)atoi(t->itemtype); //If error it returns 0 and the verifyHeader will try to detect the type
	r[id]->palette=(unsigned short)ptoi(t->palette); //Transforms the char* palette into a short ID value, if 0 or error no palette is needed
	r[id]->number=(unsigned char)ptoi(t->number); //Transforms the char* levelnumer/number attribute into a char value, if error, demo level is used

	//Title and description (only supported in level plv format
	if (t->desc!=NULL) {
		size=strlen(t->desc)+1;
		r[id]->desc=(char*)malloc(size);
		if (r[id]->desc==NULL) return;
		memcpy(r[id]->desc,t->desc,size);
	} else {
		r[id]->desc=NULL;
	}

	if (t->name!=NULL) {
		size=strlen(t->name)+1;
		r[id]->title=(char*)malloc(size);
		if (r[id]->title==NULL) return;
		memcpy(r[id]->title,t->name,size);
	} else {
		r[id]->title=NULL;
	}

	//get external and copy it to the resource path
	if (t->external!=NULL) {
		size=strlen(t->external)+1;
		r[id]->path=(char*)malloc(size);
		if (r[id]->path!=NULL) memcpy(r[id]->path,t->external,size);
	} else {
		r[id]->path=NULL;
	}
}

void workTree(const tTag* t,const char* datFile, tResource* r[]) {
	/*
		Runs workTag for each matching tag
	*/
	tTag* children;

	if (t!=NULL) {
		if (t->file!=NULL) {
			if (equalsIgnoreCase(t->file,datFile)) {
				workTag(t,r);
			}
		}
		children=t->child;

		while (children!=NULL) {
			workTree(children,datFile,r);
			children=children->next;
		}
	}
}

