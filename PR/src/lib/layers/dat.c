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
dat.c: Princed Resources : DAT library
¯¯¯¯¯
 Copyright 2004,2005 Princed Development Team
  Created: 15 Mar 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2004-Mar-15)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disk.h"
#include "common.h"
#include "dat.h"

/***************************************************************\
|                     DAT reading primitives                    |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATREAD

typedef struct {
	tPopVersion        popVersion;
	unsigned char*     highData;
	int                highDataSize;
	int                masterItems;
	int                slaveItems;
	char               recordSize;
	char               slaveIndexName[5];
	int                currentMasterItem;
	int                currentSlaveItem;
	unsigned char*     currentRecord;
} tIndexCursor;

char               recordSize;
tPopVersion        popVersion;
unsigned char*     indexPointer;
unsigned long  int indexOffset;
unsigned long  int offset;
unsigned short int indexSize;
unsigned char*     readDatFile;
int                readDatFileSize;

/* private functions */
/* todo: move to datindex.c */

int dat_cursorNext(tIndexCursor* r) {
	if (r->popVersion==pop1) {
		/* POP1 */
		if (r->currentSlaveItem==r->slaveItems) return 0;
		r->currentSlaveItem++;
		r->currentRecord+=8;
	} else {
		/* POP2 */
		if (r->currentSlaveItem==r->slaveItems) {
			if (r->currentMasterItem==r->masterItems) {
				return 0; /* its over */
			}
			
			/* remember the new slave index name */
			strncpy(r->slaveIndexName,(char*)(r->highData+2+6*r->currentMasterItem),4);
			
			/* remember the new slave index size */
			r->slaveItems=array2short(r->highData+array2short(r->highData+6+6*r->currentMasterItem));
							
			/* jump to next index */
			r->currentMasterItem++;
			r->currentSlaveItem=0;
			r->currentRecord=r->highData+array2short(r->highData+6+6*r->currentMasterItem)+2;

		} else {
			r->currentSlaveItem++;
			r->currentRecord+=11;
		}
	}
	return 1;
}

tIndexCursor dat_initPop2IndexCursor(unsigned char* highData,int highDataSize) {
	tIndexCursor r;
	r.popVersion=pop2;
	r.highData=highData;
	r.highDataSize=highDataSize;
	r.masterItems=array2short(highData);

	/* remember the first slave index name */
	strncpy(r.slaveIndexName,(char*)(highData+2),4);
	r.slaveIndexName[4]=0; /* now it is a null terminated string */
			
	/* remember the first slave index size */
	r.slaveItems=array2short(highData+array2short(highData+6));
							
	/* jump to the first index */
	r.currentMasterItem=0;
	r.currentSlaveItem=0;
	r.currentRecord=r.highData+array2short(r.highData+6)+2;
	
	return r;
}

int dat_cursorMove(tIndexCursor* r,int pos) {
	if (r->popVersion==pop1) {
		/* POP1 */
		if (r->slaveItems>pos) return 0;
		r->currentSlaveItem=pos;
		r->currentRecord=r->highData+8*pos+2;
		return 1;
	} else {
		/* POP2 */
		int i;
		for (i=0;i<r->masterItems;i++) {
			/* read how many items are in the slave index number i */
			int itemCount=array2short(r->highData+array2short(r->highData+6+6*i));
			if (pos<itemCount) {
				/* Great! we found it */
				
				/* remember the new slave index name */
				strncpy(r->slaveIndexName,(char*)(r->highData+2+6*i),4);
						
				/* remember the new slave index size */
				r->slaveItems=itemCount;
							
				/* jump to next index */
				r->currentMasterItem=i;
				r->currentSlaveItem=pos;
				r->currentRecord=r->highData+array2short(r->highData+6+6*r->currentMasterItem)+2;
				return 1;
			}
			pos-=itemCount;
		}
		return 0; /* we had read all the master index and we didn't read the pos */
	}
}
				
#define dat_readCursorGetIndexName(r) (r.slaveIndexName)
#define dat_readCursorGetId(r)        (array2short(r.currentRecord))
#define dat_readCursorGetOffset(r)    (array2long(r.currentRecord+2))
#define dat_readCursorGetSize(r)      (array2short(r.currentRecord+6))
#define dat_readCursorGetFlags(r)     ((r.popVersion==pop1)?(1<<31):(r.currentRecord[7]<<24|r.currentRecord[8]<<16|r.currentRecord[9]))

tIndexCursor dat_initPop1IndexCursor(unsigned char* highData,int highDataSize) {
	tIndexCursor r;
	r.popVersion=pop1;
	r.highData=highData;
	r.highDataSize=highDataSize;
	r.slaveItems=array2short(highData);

	/* remember the first slave index name */
	strcpy(r.slaveIndexName,"POP1");
	
	/* jump to the first index */
	r.currentSlaveItem=0;
	r.currentRecord=r.highData+2;
	
	return r;
}

tPopVersion detectPopVersion(int highArea,int highAreaSize) {
	const unsigned char* cursor;
	unsigned short numberOfRecords;

	/* create cursor */
	cursor=readDatFile+highArea;

	/* read number of records */
	numberOfRecords=array2short(cursor);cursor+=2;
	
	/* check pop1: if there are numberOfRecords records sized 8 and 2 bytes for the short numberOfRecords */
	if ((numberOfRecords*8+2)==highAreaSize) {
		indexPointer=readDatFile+highArea;
		indexSize=highAreaSize;
		recordSize=8;
		return pop1;
	}

	/* check pop2: if there are numberOfRecords records sized 6 and 2 bytes for the short numberOfRecords */
	if ((numberOfRecords*6+2)>=highAreaSize) return none;
	printf("pop2 detected with %d high sections\n",numberOfRecords);
	recordSize=0;
	for (;numberOfRecords;numberOfRecords--,cursor+=6) {
		int startOfSection;
		int endOfSection;
		int sizeOfSection;

		/* calculate section size and offset */
		if (numberOfRecords==1) { /* the last section size is calculated using the highAreaSize */
			endOfSection=highAreaSize;
		} else {
			endOfSection=array2short(cursor+10);
		}
		startOfSection=array2short(cursor+4);
		sizeOfSection=endOfSection-startOfSection;
		
		/* check section integrity */
		if (sizeOfSection<0) return none;
		
		printf("Section %c%c%c%c starts at %d, ends at %d and length %d\n",cursor[0],cursor[1],cursor[2],cursor[3],startOfSection,endOfSection,sizeOfSection);
		
		/* check for the PAHS section */	
		if (!strncmp("PAHS",(char*)cursor,4)) { /* TODO: send to define */
			indexPointer=readDatFile+highArea+startOfSection;
			indexSize=sizeOfSection;
			recordSize=11;
		}
		/* TODO: check if short(startOfSection)*11+2==sizeOfSection*/	
	}
	
	return pop2;
}

/* public functions */

tPopVersion mReadGetVersion() {
	return popVersion;
}

void mReadCloseDatFile() {
	free(readDatFile);
}

int mReadBeginDatFile(unsigned short int *numberOfItems,const char* vFiledat){
	/*
		Return Values:
			-1 Wrong Format
			-2 File not found or empty
			0 Ok
	*/

	unsigned char* readDatFilePoint;

	/* Open file */
	readDatFileSize=mLoadFileArray(vFiledat,&readDatFile);
	if (!readDatFileSize) return -2;
	if (readDatFileSize<=6) {
		free(readDatFile);
		return -1;
	}

	readDatFilePoint=readDatFile;

	/* read header  */
	indexOffset=array2long(readDatFilePoint);
	readDatFilePoint+=4;
	indexSize=array2short(readDatFilePoint);

	/* verify dat format: the index offset belongs to the file and the file size is the index size plus the index offset */
	if ((indexOffset>readDatFileSize)&&((indexOffset+indexSize)!=readDatFileSize)) {
		free(readDatFile);
		return -1; /* this is not a valid prince dat file */
	}

	/* read the high data to detect pop version and set up the indexPointer, indexSize and recordSize */
	popVersion=detectPopVersion(indexOffset,indexSize);

	/* pop version check */
	if (popVersion==none) return -1;
	if (!recordSize) {*numberOfItems=0; return 0;} /* valid dat file without an index */
	
	/* read numberOfItems */
	*numberOfItems=array2short(indexPointer);
	indexPointer+=2;

	return 0;
}

int mReadFileInDatFile(int k,unsigned char* *data,unsigned long  int *size) {
	int ok=1;
	unsigned short int id;

	/* for each archived file the index is read */
	id=    array2short(indexPointer+k*recordSize);
	offset=array2long(indexPointer+k*recordSize+2);
	*size= array2short(indexPointer+k*recordSize+6)+1;

#ifdef CHECK_POP2_PAHS_INTEGRITY
	if ( /* pop2 integrity check */
		(popVersion==pop2) &&
		(!(indexPointer[k*recordSize+8]==0x40) &&
		(!indexPointer[k*recordSize+9]) &&
		(!indexPointer[k*recordSize+10])
	)) return -1;
#endif

	if (offset>indexOffset) return -1; /* a resourse offset is allways before the index offset */
	*data=readDatFile+offset;

	return ok?id:-1;
}

int mReadInitResource(tResource** res,const unsigned char* data,long size) {
	if ((*res)==NULL) {
		(*res)=(tResource*)malloc(sizeof(tResource));
		if ((*res)==NULL) return -1; /* no memory */
		(*res)->path=NULL;
		(*res)->palAux=NULL;
		(*res)->desc=NULL;
		(*res)->name=NULL;
		(*res)->palette=0;
		(*res)->number=0;
		(*res)->size=(unsigned short int)size;
		(*res)->type=verifyHeader(data,(unsigned short int)size);
	} else { /* If resource type is invalid or 0, the type will be decided by PR */
		if (!((*res)->type)) (*res)->type=verifyHeader(data,(unsigned short int)size);
	}
	return 0;
}
#endif

/***************************************************************\
|                     DAT Writing primitives                    |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATWRITE
FILE* writeDatFile;

int mWriteBeginDatFile(const char* vFile, int optionflag) {
	/*
		Opens safely a dat file for writing mode and
		reserves space for the headers

		Return Values:
			 1 Ok
			 0 File couldn't be open

	*/
	if (writeOpen(vFile,&writeDatFile,optionflag|backup_flag)) {
		short fill=0;
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		return 1;
	} else {
		return 0;
	}
}

void mWriteInitResource(tResource** res) {
	if ((*res)==NULL) {
		(*res)=(tResource*)malloc(sizeof(tResource));
		(*res)->path=NULL;
		(*res)->palAux=NULL;
		(*res)->desc=NULL;
		(*res)->name=NULL;
	}
	(*res)->offset=(unsigned long)ftell(writeDatFile);
}

void mWriteFileInDatFile(const unsigned char* data, int size) {
	/*
		Adds a data resource to a dat file keeping
		abstractly the checksum verifications
	*/

	/* Declare variables */
	int            k        = size;
	unsigned char  checksum = 0;
	const unsigned char* dataAux  = data;

	/* calculates the checksum */
	while (k--) checksum+=*(dataAux++);
	checksum=~checksum;

	/* writes the checksum and the data content */
	fwritechar(&checksum,writeDatFile);
	fwrite(data,size,1,writeDatFile);
}

void mWriteFileInDatFileIgnoreChecksum(unsigned char* data, int size) {
	fwrite(data,size,1,writeDatFile);
}

void mWriteCloseDatFile(tResource* r[],int dontSave,int optionflag, const char* backupExtension) {
	/*
		Closes a dat file filling the index and other structures
	*/
	unsigned short int id=1;
	unsigned short int totalItems=0;
	unsigned short int size2=2;
	unsigned long  int size1=ftell(writeDatFile);

	/* Write index */
	fwriteshort(&totalItems,writeDatFile); /* Junk total items count to reserve 2 bytes */
	for (;id!=MAX_RES_COUNT;id++) {
		if (r[id]!=NULL) {
			/* the file is in the archive, so i'll add it to the index */
			totalItems++;
			fwriteshort(&id,writeDatFile);
			fwritelong(&(r[id]->offset),writeDatFile);
			fwriteshort(&(r[id]->size),writeDatFile);
		}
	}
	size2+=totalItems<<3;
	fseek(writeDatFile,size1,SEEK_SET);
	fwriteshort(&totalItems,writeDatFile); /* Definitive total items count */

	/* Write first 6 bytes header */
	fseek(writeDatFile,0,SEEK_SET);
	fwritelong(&size1,writeDatFile);
	fwriteshort(&size2,writeDatFile);

	/* Closes the file and flushes the buffer */
	writeClose(writeDatFile,dontSave,optionflag,backupExtension);
}
#endif

/***************************************************************\
|                       DAT R/W primitives                      |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATREAD
#ifdef PR_DAT_INCLUDE_DATWRITE
int mRWBeginDatFile(const char* vFile, unsigned short int *numberOfItems, int optionflag) {
	if (mReadBeginDatFile(numberOfItems,vFile)) return -2;
	if (!mWriteBeginDatFile(vFile,optionflag)) {
		mReadCloseDatFile();
		return -1;
	}
	return 0;
}
#endif
#endif
