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
#include "classify.h"
#include "memory.h"
#include "resources.h"
#include "dat.h"
#include "disk.h" /* mLoadFileArray */
#include "common.h"

/***************************************************************\
|                    Get the type of a DAT file                 |
\***************************************************************/

#define READ_ERROR {mReadCloseDatFile();return 0;}

int prClassifyDat(const char* vFiledat) {
	int                indexNumber;
	long int           id;
	unsigned char*     data;
	unsigned long  int size;
	int                type=RES_TYPE_BINARY;
	unsigned short int numberOfItems;
	tPopVersion        popVersion;
	unsigned long int  flags;
	char*              indexName;

	/* Initialize abstract variables to read this new DAT file */
	if ((id=mReadBeginDatFile(&numberOfItems,vFiledat))) return id+1; /* -1 if not found or empty, 0 if invalid */

	popVersion=mReadGetVersion();

	/* main loop */
	for (id=0,indexNumber=0;(indexNumber<numberOfItems)&&(type==RES_TYPE_BINARY);indexNumber++) {
		id=mReadFileInDatFile(indexNumber,&data,&size,&flags,&indexName);
		if (id<0) READ_ERROR; /* Read error */
		if (id==0xFFFF) continue; /* Tammo Jan Bug fix */
		if (id>=MAX_RES_COUNT) READ_ERROR; /* A file with an ID out of range will be treated as invalid */
		type=verifyHeader(data,size);
	}

	mReadCloseDatFile();
	return (popVersion==pop1)?type:(type+10);
}

typedef struct {
	unsigned long int checkSum;
	long size;
}tExeClassification;

int prClassify(const char* fileName) {
	int result;

	/* 1) check if it is a dat file */
	result=prClassifyDat(fileName);

	if (!result) { /* it's not a dat file*/
		long int fileSize;
		unsigned char* fileData;
		
		/* let's get it's content and see what it is */
		fileSize=mLoadFileArray(fileName,&fileData);
		
		/* 2) let's compare the size with a .sav size */
		if (fileSize==8) {
			int framesLeft;
			/* check that the frames (seconds/12) are in the range [0*12,60*12) */
			framesLeft=fileData[2]|fileData[3]<<8;
			if (framesLeft<60*12)
				result=30; /* sav file */
		}
		
		/* 3) let's compare the size with a .hof size */
		if (fileSize==176) {
			int records;
			/* check that the number of stored records are 6 or less */
			records=fileData[0]|fileData[1]<<8;
			if (records<=6) {
				result=31; /* hof file */
				while (records) {
					int framesLeft;
					/* wrong seconds left format for this record will invalidate the whole file */
					framesLeft=fileData[29*records-2]|fileData[29*records-1]<<8;
					if (framesLeft>=60*12) result=0; 
					records--;
				}
			}
		}

		/* 4) as the last resource, check if it is an exe file */
		if (!result && fileSize>2 && fileData[0]=='M' && fileData[1]=='Z') {
			static tExeClassification x[]={
				/* install.pdm         : 41 */ {717181985,4233},
				/* prince.exe v1.0 THG : 42 */ {622612442,123335},
				{0,0}
			};
			unsigned long checkSum=0;
			int i;
			result=40; /* generic exe file */
			/* Now I'll try to recognize some known exe files */
			/* calculate checksum */
			for (i=0;i<fileSize;i++) {
				checkSum+=fileData[i]<<((3-(i%4))*8);
			}
			/* printf("{%lu,%ld},\n",checkSum,fileSize); */

			/* compare checksum*/
			for (i=0;x[i].size;i++)
				if ((x[i].checkSum==checkSum) && (x[i].size==fileSize)) {
					result=41+i;
					break;
				}
		}
		
		free(fileData);
	}
	
	return result;
}
