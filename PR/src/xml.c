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
xml.c: Princed Resources : xml handling functions
¯¯¯¯¯
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

//Includes
#include "xml.h"
#include "disk.h"
#include "pr.h"
#include <string.h>
#include <stdio.h> /* Included only for XML specific attribute code */
#include <stdlib.h>

/****************************************************************\
|                   Tag Tree Handling Functions                  |
\****************************************************************/

//private defines
#define IsSpace(c) ((c==' ')||(c==9))||(c=='\n')||(c=='\r')
#define IsChar(c) ((('a'<=c)&&(c<='z'))||(('A'<=c)&&(c<='Z'))||(c=='_')||(c=='-')||(c=='?'))

#define Separate while (IsSpace(*i)) i++
#define NextWord(i) while (IsChar(*(i))) (i)++

#define FillAttr(a,b) if (equalsIgnoreCase(attr,b)) { if ((a)!=NULL) free(a); (a)=(val); return 1;}
#define freeAllocation(m) if ((m)!=NULL) free(m)

#define ParseError return -1

tTag* getTagStructure() {
	//initializes
	tTag* t;
	t=(tTag*)malloc(sizeof(tTag));
	if (t==NULL) return NULL;

	t->child=NULL;
	t->next=NULL;
	t->tag=NULL;
	t->desc=NULL;
	t->external=NULL;
	t->file=NULL;
	t->itemtype=NULL;
	t->name=NULL;
	t->palette=NULL;
	t->type=NULL;
	t->value=NULL;
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
	freeAllocation(t->external);
	freeAllocation(t->file);
	freeAllocation(t->itemtype);
	freeAllocation(t->name);
	freeAllocation(t->palette);
	freeAllocation(t->type);
	freeAllocation(t->value);
	freeAllocation(t->version);
	freeAllocation(t->number);
	free(t);
}

int attribFill(char* attr,char* val, tTag* t) {
	/*
		0: attribute mismatch
		1: ok
	*/

	if (equalsIgnoreCase(attr,"?")) {
		free(val);
		return 1;
	}

	FillAttr(t->desc,"desc");
	FillAttr(t->external,"external");
	FillAttr(t->file,"file");
	FillAttr(t->itemtype,"itemtype");
	FillAttr(t->name,"name");
	FillAttr(t->palette,"palette");
	FillAttr(t->type,"type");
	FillAttr(t->value,"value");
	FillAttr(t->version,"version");
	FillAttr(t->number,"levelnumber"); //levelnumber is a number alias
	FillAttr(t->number,"number");

	return 0;
}

/****************************************************************\
|                     Other Modules Functions                    |
\****************************************************************/

//Taken from parser.c

int equalsIgnoreCase(const char s1[],const char s2[]) {
	int i=0;
	for (;((s1[i]&0xDF)==(s2[i]&0xDF))&&s1[i];i++);
	return !(s1[i]||s2[i]);
}

/****************************************************************\
|                       XML Parsing Functions                    |
\****************************************************************/

//Parse text functions
int parseNext(char** pString, tTag* tag) {
	/*
	  -3 Attribute not recognized
	  -2 No memory
		-1 Parse error
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

	size=(long int)i-(long int)start; //Note: casted to long for portability with 64 bits architectures
	attribute=(char*)malloc((1+size));
	if (attribute==NULL) return -2;
	memcpy(attribute,start,size);
	attribute[size]=0;

	if (*i=='=') {
		int k=0;
		//It's a text attribute
		i++;
		if (*i!='"') ParseError;
		i++;
		//Parse until the next "
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
		value=(char*)malloc((k+1));
		if (value==NULL) {
			free(attribute);
			return -2;
		}
		memcpy(value,aux,k);
		value[k]=0;
	} else {
		//It's a boolean attribute, I'll define it
		value=(char*)malloc((1));
		if (value==NULL) {
			free(attribute);
			return -2;
		}
		value[0]=0;
	}

	if (!(attribFill(attribute,value,tag))) {
		free(attribute);
		return -3;
	}
	free(attribute);
	*pString=i;
	return 0;
}


int getNextTag(char** pString, char** value) {
	/*
	  -2 No memory
		-1 Parse error
		0  if next item is a tag
		1  if it was a text
		2  if next item closes a tag
		3  End of document
		4  if there was no text
	*/
	char* i=*pString;
	int   result;
	char* start;
	char  size;

	Separate;

	if (*i=='<') {
		//it is a tag
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

		size=(char)((long int)i-(long int)start); //Note: casted to long for portability with 64 bits architectures
		*value=(char*)malloc((size));
		if (*value==NULL) return -2;
		memcpy(*value,start,size-1);
		(*value)[size-1]=0;
		*pString=i-(!result);
		return result;
	}
	start=i;
	while ((*i)&&((*i)!='<')) i++;
	if (!(*i)) return 3;
	if (start==i) return 4;
	size=(char)((long int)i-(long int)start); //Note: casted to long for portability with 64 bits architectures
	*value=(char*)malloc((1+size));
	if (*value==NULL) return -2;
	memcpy(*value,start,size);
	//memset(*value,0,size);
	(*value)[size]=0;
	*pString=i;
	return 1;
}

//Parse Tree functions
tTag* makeTree(char** p,char* name, int* error,tTag* father) {
	/* *error
		-3 Attribute not recognized
	  -2 No memory
		-1 Parse error
		0  if next item is a tag
	*/

	tTag* tag;
	tTag* children=NULL;
	char* value;
	int result;

	tag=getTagStructure();
	tag->tag=name;

	while (!((*error)=parseNext(p, tag)));

	if ((*error)<0) return NULL; //Fatal error
	/*	(*error)
			1  if tag end
			2  if end
	*/
	if ((*error)==1) {
		*error=0; //No errors, end of the tag in the same tag <tag />
		return tag;
	}

	//In case there are some empty attributes, they may be inherited
	//BEGIN specific xml tag inheritance
	{
		int x;
		char* str;
		//TODO: use macros
		//TotalInheritance(tag->palette,father->palette);
		if ((tag->palette==NULL)&&(father->palette!=NULL)) {
			x=strlen(father->palette)+1;
			tag->palette=(char*)malloc(x);
			memcpy(tag->palette,father->palette,x);
		}
		//TotalInheritance(file);
		if ((tag->file==NULL)&&(father->file!=NULL)) {
			x=strlen(father->file)+1;
			tag->file=(char*)malloc(x);
			memcpy(tag->file,father->file,x);
		}
		//TotalInheritance(itemtype);
		if ((tag->itemtype==NULL)&&(father->itemtype!=NULL)) {
			x=strlen(father->itemtype)+1;
			tag->itemtype=(char*)malloc(x);
			memcpy(tag->itemtype,father->itemtype,x);
		}
		//PartialConcatInheritance(tag->external,father->external,tag->value);
		if ((tag->value==NULL)||(tag->external!=NULL)) {
			//Make sure externals do exist
			if (father->external==NULL) {father->external=(char*)malloc(1);*(father->external)=0;}
			if (tag->external==NULL) {tag->external=(char*)malloc(1);*(tag->external)=0;}

			//Create new variable
			x=strlen(father->external)+strlen(tag->external)+2;
			str=(char*)malloc(x);
			if (str==NULL) {*error=2;return NULL;}

			//Set variable and destroy old variables
			sprintf(str,"%s/%s",father->external,tag->external);
			free(tag->external);
			if ((*(father->external))==0) free(father->external);
			tag->external=str;
			memcpy(tag->external,str,x);
		}
	}
	//END specific xml tag inheritance
	//Parse Child tags
	while (1) {
		(*error)=getNextTag(p, &value);
		if ((*error)<0) return NULL; //Fatal error
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
				if (*error) return NULL;
				//Add error handling
				break;
			case 1:
				if (tag->name!=NULL) free(tag->name);
				tag->name=value;
				break;
			case 2:
				//"no errors" or "a wrong tag is closed"
				*error=-(!(equalsIgnoreCase(value,tag->tag)));
				free(value);
				return tag;
			case 3:
				*error=-1; //this tag wasn't closed
				return tag;
				break;
		}
	}
	return NULL;
}

#if 0

void showTag(int n,tTag* t) {
	int a;
	tTag* children;

	for(a=0;a<n;a++) printf ("  ");
	if (t!=NULL) {
		printf("%s (%s)\n",t->tag,t->desc);
		children=t->child;

		while (children!=NULL) {
			showTag(n+1,children);
			children=children->next;
		}
	} else {
		printf("None\n");
	}
}

#endif

tTag* parseXmlFile(const char* vFile,int* error) {
	/* error may take the following values:
		-1 Parse error
		-2 No memory
		-3 Attribute not recognized
		-4 File not found
		0  no errors
	*/
	char* p;
	char* l;
	char* value=NULL;
	tTag* tag;
	tTag* father;

	if (!mLoadFileArray(vFile,(unsigned char**)(&l))) {
		*error=-4; //File not open
		return NULL;
	}
	p=l;

	*error=getNextTag(&p, &value);
	if ((*error)<0) {
		free(l);
		return NULL; //Fatal error will stop the execusion of the parsing
	}

	father=getTagStructure();
	tag=makeTree(&p,value,error,father);

	if (*error) {
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
		return NULL; //Fatal error will stop the execusion of the parsing
	}
	if (*error==3) {
		*error=0;
		free(father);
		return tag;
	} else {
		freeTagStructure(tag);
		free(father);
		*error=-1;
		return NULL;
	}
}
