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
parse.c: Princed Resources : xml parsing functions
¯¯¯¯¯¯¯
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

/* Includes */
#include "xmlparse.h"
#include "disk.h"
#include "memory.h"
#include "common.h"
#include <string.h>
#include <stdio.h> /* Included only for XML specific attribute code */
#include <stdlib.h>

extern FILE* outputStream;

/****************************************************************\
|                   Tag Tree Handling Functions                  |
\****************************************************************/

const char* getExtDesc(int type) {
	static const char* extarraydesc[]=RES_FILE_TYPES;
	return extarraydesc[type];
}

/* private defines */
#define IsSpace(c) ((c==' ')||(c==9))||(c=='\n')||(c=='\r')
#define IsChar(c) ((('a'<=c)&&(c<='z'))||(('A'<=c)&&(c<='Z'))||(c=='_')||(c=='-')||(c=='?'))

#define Separate while (IsSpace(*i)) i++
#define NextWord(i) while (IsChar(*(i))) (i)++

#define FillAttr(a,b) if (equalsIgnoreCase(attr,b)) { freeAllocation(a); (a)=(val); return 0;}

#define TotalInheritance(attribute) \
		if ((tag->attribute==NULL)&&(father->attribute!=NULL)) {\
			x=strlen(father->attribute)+1;\
			tag->attribute=(char*)malloc(x);\
			memcpy(tag->attribute,father->attribute,x);\
		}

#define ParseError return PR_RESULT_ERR_XML_PARSING

tTag* getTagStructure() {
	/* initializes */
	tTag* t;
	t=(tTag*)malloc(sizeof(tTag));
	if (t==NULL) return NULL;

	t->child=NULL;
	t->next=NULL;
	t->tag=NULL;
	t->desc=NULL;
	t->path=NULL;
	t->file=NULL;
	t->type=NULL;
	t->name=NULL;
	t->palette=NULL;
	t->value=NULL;
	t->index=NULL;
	t->paletteindex=NULL;
	t->version=NULL;
	t->number=NULL;

	return t;
}

void freeTagStructure(tTag* t) {
	if (t==NULL) return;
	freeTagStructure(t->child);
	freeTagStructure(t->next);
	freeAllocation(t->tag);
	freeAllocation(t->desc);
	freeAllocation(t->path);
	freeAllocation(t->file);
	freeAllocation(t->type);
	freeAllocation(t->name);
	freeAllocation(t->palette);
	freeAllocation(t->value);
	freeAllocation(t->index);
	freeAllocation(t->paletteindex);
	freeAllocation(t->version);
	freeAllocation(t->number);
	free(t);
}

int attribFill(char* attr,char* val, tTag* t) {
	/*
		PR_RESULT_ERR_XML_ATTR: attribute mismatch
		0: ok
	*/

	if (equalsIgnoreCase(attr,"?")) {
		free(val);
		return 0;
	}

	FillAttr(t->desc,"desc");
	FillAttr(t->path,"external"); /* external is a path alias for old compatibilities */
	FillAttr(t->path,"path");
	FillAttr(t->file,"file");
	FillAttr(t->type,"type");
	FillAttr(t->type,"itemtype"); /* alias fro type */
	FillAttr(t->name,"name");
	FillAttr(t->name,"title"); /* title is a name alias */
	FillAttr(t->palette,"palette");
	FillAttr(t->value,"value");
	FillAttr(t->index,"index");
	FillAttr(t->paletteindex,"paletteindex");
	FillAttr(t->version,"version");
	FillAttr(t->number,"levelnumber"); /* levelnumber is a number alias */
	FillAttr(t->number,"number");

	return PR_RESULT_ERR_XML_ATTR;
}

/****************************************************************\
|                     Other Modules Functions                    |
\****************************************************************/

/* Taken from parser.c */

int equalsIgnoreCase(const char s1[],const char s2[]) {
	int i=0;
	for (;((s1[i]&0xDF)==(s2[i]&0xDF))&&s1[i];i++);
	return !(s1[i]||s2[i]);
}

/****************************************************************\
|                       XML Parsing Functions                    |
\****************************************************************/

/* Parse text functions */
int parseNext(char** pString, tTag* tag) {
	/*
	  PR_RESULT_ERR_XML_ATTR Attribute not recognized
	  PR_RESULT_ERR_MEMORY No memory
		PR_RESULT_ERR_XML_PARSING Parse error
		0  if continue
		1  if tag end
		2  if end
	*/

	char* start;
	char* attribute;
	char* value;
	char aux[MAX_VALUE_SIZE];
	int size;
	char* i=*pString;

	Separate;

	if (*i=='>') {
		*pString=i+1;
		return 2;
	}
	if (*i=='/') {
		i++;
		Separate;
		if (*i=='>') {
			*pString=i+1;
			return 1;
		} else {
			ParseError;
		}
	}

	start=i;
	NextWord(i);
	if (start==i) ParseError;
	if (*i==0)    ParseError;

	if (!(IsSpace(*i)||(*i=='=')||(*i=='>'))) ParseError;

	size=(long int)i-(long int)start; /* Note: casted to long for portability with 64 bits architectures */
	attribute=(char*)malloc(1+size);
	if (attribute==NULL) return PR_RESULT_ERR_MEMORY;
	memcpy(attribute,start,size);
	attribute[size]=0;

	if (*i=='=') {
		int k=0;
		/* It's a text attribute */
		i++;
		if (*i!='"') ParseError;
		i++;
		/* Parse until the next " */
		for(start=i; (k<MAX_VALUE_SIZE)&&(*i!='"')&&(*i!=0) ;i++) {
			aux[k]=*i;
			if (aux[k]=='\\') {
				i++;
				switch (*i) {
					case 'n':
						aux[k]='\n';
						break;
					case 'r':
						aux[k]='\r';
						break;
					case 't':
						aux[k]='\t';
						break;
					default:
						aux[k]=*i;
						break;
				}
			}
			k++;
		}
		if ((*i)!='"') {
			free(attribute);
			ParseError;
		}
		i++;
		value=(char*)malloc(k+1);
		if (value==NULL) {
			free(attribute);
			return PR_RESULT_ERR_MEMORY;
		}
		memcpy(value,aux,k);
		value[k]=0;
	} else {
		/* It's a boolean attribute, I'll define it */
		value=(char*)malloc(1);
		if (value==NULL) {
			free(attribute);
			return PR_RESULT_ERR_MEMORY;
		}
		value[0]=0;
	}

	if (attribFill(attribute,value,tag)) {
		free(attribute);
		free(value);
		return PR_RESULT_ERR_XML_ATTR;
	}
	free(attribute);
	*pString=i;
	return 0;
}

int getNextTag(char** pString, char** value) {
	/*
	  PR_RESULT_ERR_MEMORY No memory
		PR_RESULT_ERR_XML_PARSING Parse error
		0  if next item is a tag (value allocated)
		1  if it was a text (value allocated)
		2  if next item closes a tag (value allocated)
		3  End of document (value not allocated)
		4  if there was no text (value not allocated)
	*/
	char* i=*pString;
	int   result;
	char* start;
	int   size;

	Separate;

	if (*i=='<') {
		/* it is a tag */
		i++;
		Separate;
		if (*i=='/') {
			result=2;
			i++;
		} else {
			if ((*i=='!')||(*i=='?')) {
				while ((*i)&&((*i)!='>')) i++;
				if (!(*i)) ParseError;
				i++;
				if (!(*i)) return 3;
				result=getNextTag(&i,value);
				*pString=i;
				return result;
			} else {
				result=0;
			}
		}
		start=i;
		NextWord(i);
		if (start==i) ParseError;
		if (*i==0)    ParseError;
		i++;

		size=(int)((long int)i-(long int)start); /* Note: casted to long for portability with 64 bits architectures */
		*value=(char*)malloc(size);
		if (*value==NULL) return PR_RESULT_ERR_MEMORY;
		memcpy(*value,start,size-1);
		(*value)[size-1]=0;
		*pString=i-(!result);
		return result;
	}
	start=i;
	while ((*i)&&((*i)!='<')) i++;
	if (!(*i)) return 3;
	if (start==i) return 4;
	size=(int)((long int)i-(long int)start); /* Note: casted to long for portability with 64 bits architectures */
	*value=(char*)malloc(1+size);
	if (*value==NULL) return PR_RESULT_ERR_MEMORY;
	memcpy(*value,start,size);
	(*value)[size]=0;
	*pString=i;
	return 1;
}

/* Parse Tree functions */
tTag* makeTree(char** p,char* name, int* error,tTag* father) {
	/* *error
		PR_RESULT_ERR_XML_ATTR Attribute not recognized
	  PR_RESULT_ERR_MEMORY No memory
		PR_RESULT_ERR_XML_PARSING Parse error
		0  if next item is a tag
	*/

	tTag* tag;
	tTag* children=NULL;
	char* value;
	int result;

	tag=getTagStructure();
	tag->tag=name;

	while (!((*error)=parseNext(p, tag)));

	if ((*error)<0) {freeTagStructure(tag);return NULL;} /* Fatal error */
	/*	(*error)
			1  if tag end
			2  if end
	*/
	if ((*error)==1) {
		*error=0; /* No errors, end of the tag in the same tag <tag /> */
		return tag;
	}

	/* In case there are some empty attributes, they may be inherited */
	/* BEGIN specific xml tag inheritance */
	{
		int x;
		char* str;

		TotalInheritance(palette);
		TotalInheritance(paletteindex);
		TotalInheritance(type);
		TotalInheritance(file);
		TotalInheritance(index);
		/* PartialConcatInheritance(tag->path,father->path,tag->value); */
		if ((tag->value==NULL)||(tag->path!=NULL)) {
			/* Make sure paths do exist */
			if (father->path==NULL) {father->path=(char*)malloc(1);*(father->path)=0;}
			if (tag->path==NULL) {tag->path=(char*)malloc(1);*(tag->path)=0;}

			/* Create new variable */
			x=strlen(father->path)+strlen(tag->path)+2;
			str=(char*)malloc(x);
			if (str==NULL) {*error=PR_RESULT_ERR_MEMORY;return NULL;}

			/* Set variable and destroy old variables */
			sprintf(str,"%s/%s",father->path,tag->path);
			free(tag->path);
			if ((*(father->path))==0) {free(father->path);father->path=NULL;}
			tag->path=str;
		}
	}
	/* END specific xml tag inheritance */
	/* Parse Child tags */
	while (1) {
		(*error)=getNextTag(p, &value);
		if ((*error)<0) return NULL; /* Fatal error */
		/*	(*error)
				0  if next item is a tag
				1  if it was a text
				2  if next item closes a tag
				3  End of document
				4  if there was no text
		*/
		result=(*error);
		switch (result) {
			case 0:
				if (children==NULL) {
					tag->child=makeTree(p,value,error,tag);
					children=tag->child;
				} else {
					children->next=makeTree(p,value,error,tag);
					children=children->next;
				}
				if (*error) {freeTagStructure(tag); return NULL;}
				break;
			case 1:
				freeAllocation(tag->name);
				tag->name=value;
				break;
			case 2:
				/* "no errors" or "a wrong tag is closed" */
				*error=(equalsIgnoreCase(value,tag->tag))?0:PR_RESULT_ERR_XML_PARSING;
				free(value);
				return tag;
			case 3:
				*error=PR_RESULT_ERR_XML_PARSING; /* this tag wasn't closed */
				return tag;
				break;
		}
	}
	return NULL;
}

#ifdef IGNOREVERIFYDATTYPES

void showTag(int n,tTag* t) {
	int a;
	tTag* children;

	for(a=0;a<n;a++) fprintf (outputStream,"  ");
	if (t!=NULL) {
		fprintf(outputStream,"%s (%s@%s)\n",t->tag,t->file,t->value);
		children=t->child;

		while (children!=NULL) {
			showTag(n+1,children);
			children=children->next;
		}
	} else {
		fprintf(outputStream,"None\n");
	}
}

#endif

tTag* parseXmlFile(const char* vFile,int* error) {
	/* error may take the following values:
		PR_RESULT_ERR_XML_PARSING Parse error
		PR_RESULT_ERR_MEMORY No memory
		PR_RESULT_ERR_XML_ATTR Attribute not recognized
		PR_RESULT_ERR_XML_FILE File not found
		0  no errors
	*/
	char* p;
	char* l;
	char* value=NULL;
	tTag* tag;
	tTag* father;
	int size;

	if (((size=mLoadFileArray(vFile,(unsigned char**)(&l)))<=0)) {
		*error=PR_RESULT_ERR_XML_FILE; /* File not open */
		return NULL;
	}
	/* bugfix moved to loadArray */ l[size-1]=0; /* The last character must be an end of line (the > is forbiden) */ 
	p=l;

	*error=getNextTag(&p, &value);
	if ((*error)<0) {
		free(l);
		return NULL; /* Fatal error will stop the execusion of the parsing */
	}

	father=getTagStructure();
	tag=makeTree(&p,value,error,father);

	if ((*error)<0) {
		freeTagStructure(tag);
		free(l);
		free(father);
		return NULL;
	}
	
	*error=getNextTag(&p, &value);
	
	free(l);
	if (*error<0) {
		freeTagStructure(tag);
		free(father);
		return NULL; /* Fatal error will stop the execusion of the parsing */
	}
	if (*error==3) { /* 3 means end of document */
		*error=PR_RESULT_SUCCESS;
		free(father);
		return tag;
	} else {
		freeTagStructure(tag);
		free(father);
		*error=PR_RESULT_ERR_XML_PARSING;
		return NULL;
	}
}

static tTag* xmlStructure=NULL; /* Keeping the parsed file structure in memory will save a lot of time */
static char lastFile[256]="";
static int xmlStructureError=0;

/* cache parsed structure. If null is passed the default name will be used */
int parseStructure(const char* vFile,tTag** structure) {
	/* Resources input xml tree. Private+abstract variable */
	static const char defaultXmlFile[]=RES_XML_RESOURC_XML;

	if (vFile==NULL) vFile=defaultXmlFile;
	
	/* Generate xml structure if doesn't exist */
	if (strcmp(lastFile,vFile)) {
		/* if the file is different than the cached file */
		freeParsedStructure(&xmlStructure);
		xmlStructure=parseXmlFile(vFile,&xmlStructureError);
		strncpy(lastFile,vFile,256); /* remember the new cached filename */
	}

	if (!xmlStructureError) {
		*structure=xmlStructure;
	} else {
		*structure=NULL;
	}

	return xmlStructureError;
}

void freeParsingCache() {
	freeParsedStructure(&xmlStructure);
	lastFile[0]=0;
}

void freeParsedStructure(tTag** structure) {
	/* Free it only if exists */
	if (*structure!=NULL) freeTagStructure(*structure);
	/* Reinitializes the variable */
	*structure=NULL;
}

/***************************************************************\
|                Resource tree browsing for DLL                 |
\***************************************************************/

tTag* resourceTreeGetNext (tTag* whereAmI) {
	return whereAmI->next;
}

tTag* resourceTreeGetChild(tTag* whereAmI) {
	return whereAmI->child;
}

int   resourceTreeGetInfo (tTag* whereAmI,	char** tag, char** desc, char** path, char** file, char** type, char** name, char** palette, char** value, char** version, char** number) {
	if (whereAmI==NULL) return 0;
	*tag=whereAmI->tag;
	*desc=whereAmI->desc;
	*path=whereAmI->path;
	*file=whereAmI->file;
	*type=whereAmI->type;
	*name=whereAmI->name;
	*palette=whereAmI->palette;
	*value=whereAmI->value;
	*version=whereAmI->version;
	*number=whereAmI->number;
	return 1;
}

