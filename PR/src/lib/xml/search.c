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
search.c: Princed Resources : specific xml handling functions
��������
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

/* Includes */
#include "common.h"
#include "parse.h"
#include "search.h"
#include "memory.h"
#include <string.h>
#include "translate.h" /* to translate indexes */

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

int getOrder(const char* order) {
	if (order) {
		if (equalsIgnoreCase(order,"first")) {
			return 0;
		} else if (equalsIgnoreCase(order,"second")) {
			return 1;
		} else if (equalsIgnoreCase(order,"last")) {
			return 65535;
		} else {
			return atoi(order);
		}
	} else {
		return 0;
	}
}

/****************************************************************\
|                   Tag Tree Searching Functions                 |
\****************************************************************/

#define ptoi(p) ((p!=NULL)?atoi(p):0)

#define keepStringAttribute(attribute) res.attribute=strallocandcopy(t->attribute)
#define keepIntAttribute(attribute,type) res.attribute=(type)ptoi(t->attribute);
#define keepIdAttributes(attribute,idnum,idindex) res.attribute.value=(unsigned short int)ptoi(t->idnum);\
                                             if (t->idindex) str5lowercpy(res.attribute.index,translateExt2Int(t->idindex));\
																	           else res.attribute.index[0]=0
																	 
#define keepIdAttributesElse(attribute,idnum,idindex,idelse) res.attribute.value=(unsigned short int)ptoi(t->idnum);\
                                             if (t->idindex) str5lowercpy(res.attribute.index,translateExt2Int(t->idindex));\
																	           else str5lowercpy(res.attribute.index,t->idelse)
void workTag(const tTag* t,void* pass) {
	/*
		If the tag matches, it is converted to resource and added to the array
	*/

	/* Declare variables */
	const char* datFile=((tPassWork*)pass)->datFile;
	tResourceList* r=((tPassWork*)pass)->r;
	tResource res;
	char* end;

	/* Skipping conditions */
	if (!equalsIgnoreCase(t->file,datFile))   return; /* If it doesn't belong to the given dat file */
	if (!t->value) return;                            /* If there was not number id */
	if (!t->index) return;                            /* If there was not index id */
	if (!equalsIgnoreCase(t->tag,"item"))     return; /* If the tag isn't an item */

	/* Process tag and copy values to resource: */

	/* Get string type and convert into the typeId */
	res.type=0;
#ifndef IGNORERESOURCEFUNCTIONS
	if (t->type!=NULL) { /* NULL tells the extractor that the type should be auto detected */
		int i=RES_TYPECOUNT;
		while((!res.type)&&(i--))
			if (equalsIgnoreCase(t->type,getExtDesc(i)))
				res.type=i;
		/* If error it returns 0 and the verifyHeader will try to detect the type */
	}
#endif

	/* Get the order */
	res.id.order=getOrder(t->order);
	res.palette.order=getOrder(t->paletteorder);
	
	/* Copy id and palette id */	
	keepIdAttributes(id,value,index);
	keepIdAttributesElse(palette,palette,paletteindex,index);
	
	/* Copy number, title, desc and path */
	keepIntAttribute(number,unsigned char);    /* Transforms the char* levelnumer/number attribute into a char value, if error, demo level is used */
	if (t->flags) {
		res.flags=strtol(t->flags,&end,0);
		if (*end) return;
	} else {
		res.flags=0;
	}
	keepStringAttribute(desc);  /* only for plv */
	keepStringAttribute(name);  /* only for plv */
	keepStringAttribute(path);

	resourceListAdd(r,&res);
}

void workTree(const tTag* t,void* pass, void (*function)(const tTag*,void*)) {
	/*
		Runs the given function for each matching tag
	*/
	tTag* children;

	if (t!=NULL) {
		if (t->file!=NULL) (*function)(t,pass);
		children=t->child;

		while (children!=NULL) {
			workTree(children,pass,function);
			children=children->next;
		}
	}
}

/****************************************************************\
|                       Compare two XML files                    |
\****************************************************************/

#ifdef DO_NOT_IGNORE_COMPARATION

static int compareStatisticsOk=0;
static int compareStatisticsTotals=0;
static int compareStatisticsWarnings=0;
extern FILE* outputStream;

const tTag* searchTree(const tTag* t,const char* datFile, const char* id) {
	/* tTag*
		tag pointer if found
		NULL if not found
	*/
	tTag* children;
	const tTag* result;

	if (t!=NULL) {
		if (((t->file)!=NULL)&&((t->value)!=NULL)) {
			if ((equalsIgnoreCase(t->file,datFile))&&(equalsIgnoreCase(t->value,id))) return t;
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

void compareXmlFileForTag(const tTag* tag,void* pass) {
	const tTag* modified;
	const tTag* result;

	modified=((tPassCompare*)pass)->tag;

	if ((tag->file)&&(tag->value)) {
		result=searchTree(modified,tag->file,tag->value);
		if (!result) {
			fprintf(outputStream,"Error: Item not found: '%s@%s'\n",tag->value,tag->file);
			fprintf(outputStream,"-> <item value=\"%s\" path=\"%s\" type=\"%s\" palette=\"%s\">%s</item>\n",
				tag->value,
				tag->path,
				tag->type,
				tag->palette,
				tag->desc
			);
		} else {
			fprintf(outputStream,"Item found: '%s@%s' %s\n",tag->value,tag->file,result->file);
			if (!equalsIgnoreCase(tag->type,result->type)) {
				compareStatisticsWarnings++;
				fprintf(outputStream,"Warning: Type mismatch in '%s@%s' (%s!=%s)\n",tag->value,tag->file,tag->type,result->type);
			}
			compareStatisticsOk++;
		}
		compareStatisticsTotals++;
	}
}

void compareXmlFile(tTag* modified,tTag* original) {
	tPassCompare pass;
	pass.tag=modified;

	workTree(original,&pass,compareXmlFileForTag);
	fprintf(outputStream,"Statistics:\n Totals: %d\n Working: %d (%5.2f%%)\n Warnings: %d\n Missing: %d (%5.2f%%)\n",
		compareStatisticsTotals,
		compareStatisticsOk,(float)(100*(float)compareStatisticsOk/compareStatisticsTotals),
		compareStatisticsWarnings,
		compareStatisticsTotals-compareStatisticsOk,(float)(100*(float)(compareStatisticsTotals-compareStatisticsOk)/compareStatisticsTotals)
	);
}

#endif

