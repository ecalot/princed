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
tasks.c: Princed Resources : Classify a DAT file
¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include "tasks.h" /* TODO: rename tasks to classify */
#include "memory.h"
#include "resources.h"
#include "dat.h"
#include "xmlparse.h"
#include "xmlsearch.h"
#include "pr.h"

/***************************************************************\
|                    Get the type of a DAT file                 |
\***************************************************************/

#ifndef IGNOREVERIFYDATTYPES

extern int pop1;

int prVerifyDatType(const char* vFiledat) {
	int                error;
	int                indexNumber;
	long int           id;
	unsigned char*     data;
	unsigned long  int size;
	int                type=RES_TYPE_BINARY;
	unsigned short int numberOfItems;

	/* Initialize abstract variables to read this new DAT file */
	if (error=mReadBeginDatFile(&numberOfItems,vFiledat)) return error;

	/* main loop */
	for (indexNumber=0;(indexNumber<numberOfItems)&&type!=RES_TYPE_BINARY;indexNumber++) {
		id=mReadGetFileInDatFile(indexNumber,&data,&size);
		if (id<0) return 0; /* Read error */
		if (id==0xFFFF) continue; /* Tammo Jan Bug fix */
		if (id>=MAX_RES_COUNT) return 0; /* A file with an ID out of range will be treated as invalid */
		type=verifyHeader(data,size);
	}
	mReadCloseDatFile();
	return pop1?type:(type+10);
}

#if 0
	/* Variables */
	FILE*              fp;
	char               ok;
	int 					     k;

	/* variables in case header is ok */
	unsigned short int offset;
	unsigned long  int size;
	unsigned char*     data;
	unsigned char      type=5;

	/* loop variables */
	unsigned long  int indexOffset;
	unsigned short int indexSize;
	unsigned short int numberOfItems;
	unsigned char*     index;


	if ((ok=(((fp=fopen(vFiledat,"rb")))!=NULL))) {
		/* verify dat format */
		ok    = fread(&indexOffset,4,1,fp);
		ok=ok&& fread(&indexSize,2,1,fp);
		ok=ok&& !fseek(fp,indexOffset,SEEK_SET);
		ok=ok&& fread(&numberOfItems,2,1,fp);
		if (!ok) {
			fclose(fp);
			return 0; /* this is not a valid prince dat file */
		}
		if ((numberOfItems*8+2)!=indexSize) {
			indexOffset+=indexSize;
			fseek(fp,0,SEEK_END);
			ok=(((unsigned long)ftell(fp))==indexOffset)?11:0; /* see if it is a pop2 file */
			fclose(fp);
			return ok; /* this is a pop2 dat file or invalid */
		}
		if ((index=getMemory(indexSize-2))==NULL) {
			fclose(fp);
			return -2; /* no memory */
		}
		ok=fread(index,indexSize-2,1,fp);

		/* main loop */
		for (k=0;ok&&(k<numberOfItems)&&(type==5);k++) {
			/* for each archived file */
			ok=ok&&!(index[k*8+4]||index[k*8+5]);
			offset=index[k*8+2]+256*index[k*8+3];
			size=index[k*8+6]+256*index[k*8+7]+1;
			ok=ok&&((data=getMemory(size))!=NULL);
			ok=ok&&(!fseek(fp,offset,SEEK_SET));
			ok=ok&&fread(data,size,1,fp);

			/* set resource information on this index entry */
			type=verifyHeader(data,(unsigned short int)size);

		}
		fclose(fp);
		return ok?type:0;
	} else {
		return -1; /* file could not be open */
	}
}
#endif
#else
extern FILE* outputStream;
void showTag(int n,tTag* t);
void compareFiles(const char* vModified,const char* vOriginal) {
	/* Declare variables */
	int error;
	tTag* modified;
	tTag* original;

	/* Parse modified file */
	modified=parseXmlFile(vModified,&error);
	fprintf(outputStream,"Modified file parsing result: %d\n",error);

	/* Parse original file */
	original=parseXmlFile(vOriginal,&error);
	fprintf(outputStream,"Original file parsing result: %d\n",error);


	fprintf(outputStream,"Original tree\n");
	showTag(0,original);
	fprintf(outputStream,"Modified tree\n");
	showTag(0,modified);


	/* Compare */
	compareXmlFile(modified,original);

	/* Free memory */
	freeTagStructure(modified);
	freeTagStructure(original);
}
#endif
