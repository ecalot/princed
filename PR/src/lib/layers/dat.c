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

#include "reslist.h"

/***************************************************************\
|                     DAT reading primitives                    |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATREAD

typedef struct {
	tPopVersion         popVersion;
	unsigned char*      highData;
	int                 highDataSize;
	int                 masterItems;
	int                 slaveItems;
	int                 currentMasterItem;
	int                 currentSlaveItem;
	unsigned char*      currentRecord;
	char                slaveIndexName[5];
} tIndexCursor;

static unsigned char* readDatFile;
static int            readDatFileSize;
static tIndexCursor   readIndexCursor;

/* private functions */
/* todo: move to datindex.c */

/* the cursor get functions */

#define dat_readCursorGetIndexName(r) (r.slaveIndexName)
#define dat_readCursorGetId(r)        (array2short(r.currentRecord))
#define dat_readCursorGetOffset(r)    (array2long(r.currentRecord+2))
#define dat_readCursorGetSize(r)      (array2short(r.currentRecord+6))
#define dat_readCursorGetFlags(r)     ((r.popVersion==pop1)?(1<<31):(r.currentRecord[8]<<16|r.currentRecord[9]<<8|r.currentRecord[10]))
#define dat_readCursorGetVersion(r)     (r.popVersion)

/* the cursor move functions */

int dat_cursorNextIndex(tIndexCursor* r) {
	if (r->popVersion==pop1) {
		/* POP1 */
		return 0;
	} else {
		/* POP2 */
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
	}
	return 1;
}

int dat_cursorNext(tIndexCursor* r) {
	if (r->popVersion==pop1) {
		/* POP1 */
		r->currentSlaveItem++;
		r->currentRecord+=8;
		if (r->currentSlaveItem==r->slaveItems)
			return 0;
	} else {
		/* POP2 */
			r->currentSlaveItem++;
			r->currentRecord+=11;
		if (r->currentSlaveItem==r->slaveItems)
			return dat_cursorNextIndex(r);
	}
	return 1;
}

void dat_cursorFirst(tIndexCursor* r) {
	if (r->popVersion==pop2) {
		/* remember the first slave index name */
		strncpy(r->slaveIndexName,(char*)(r->highData+2),4);
		r->slaveIndexName[4]=0; /* now it is a null terminated string */
		r->currentRecord=r->highData+array2short(r->highData+6)+2;
	} else {
		r->currentRecord=r->highData+2;
	}

	/* jump to the first index */
	r->currentMasterItem=0;
	r->currentSlaveItem=0;
}

int dat_cursorMoveId(tIndexCursor* r, tResourceId id) {
	dat_cursorFirst(r);
	/* first try to find the index name */

	do {
		if (!strcmp(r->slaveIndexName,id.index)) {
			/* the same index */
			do {
				if (strcmp(r->slaveIndexName,id.index)) return 0; /* in case we are passed */
				if (array2short(r->currentRecord)==id.value) return 1; /* found! */ /* TODO: check for segmentation faults here */
			} while (dat_cursorNext(r));
			return 0; /* id not found */
		}
	} while (dat_cursorNextIndex(r));
	return 0; /* index name not found */
}

int dat_cursorMove(tIndexCursor* r,int pos) {
	if (r->popVersion==pop1) {
		/* POP1 */
		if (r->slaveItems<=pos) return 0;
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
				r->slaveIndexName[4]=0; /* now it is a null terminated string */

				/* remember the new slave index size */
				r->slaveItems=itemCount;

				/* jump to next index */
				r->currentMasterItem=i;
				r->currentSlaveItem=pos;
				r->currentRecord=r->highData+array2short(r->highData+6+6*r->currentMasterItem)+2+pos*11;
				return 1;
			}
			pos-=itemCount;
		}
		return 0; /* we had read all the master index and we didn't read the pos */
	}
}

/* the detect function */

tPopVersion detectPopVersion(unsigned char* highArea,int highAreaSize,unsigned short int *numberOfItems) {
	const unsigned char* cursor;
	unsigned short numberOfRecords;

	/* create cursor */
	cursor=highArea;

	/* read number of records */
	numberOfRecords=array2short(cursor);cursor+=2;

	/* check pop1: if there are numberOfRecords records sized 8 and 2 bytes for the short numberOfRecords */
	if ((numberOfRecords*8+2)==highAreaSize) {
		*numberOfItems=numberOfRecords;
		return pop1;
	}

	/* check pop2: if there are numberOfRecords records sized 6 and 2 bytes for the short numberOfRecords */
	*numberOfItems=0;
	if ((numberOfRecords*6+2)>=highAreaSize) return none;
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

		if ((array2short(highArea+startOfSection)*11+2)!=sizeOfSection) return none;
		*numberOfItems+=array2short(highArea+startOfSection);
	}

	return pop2;
}

/* the cursor create function */

tIndexCursor dat_createCursor(unsigned char* highData,int highDataSize,unsigned short int* numberOfItems) {
	tIndexCursor r;

	/* read the high data to detect pop version and set up the number of items */
	r.popVersion=detectPopVersion(highData,highDataSize,numberOfItems);

	switch (r.popVersion) {
	case pop1:
		r.highData=highData;
		r.highDataSize=highDataSize;
		r.slaveItems=array2short(highData);

		/* remember the first slave index name */
		strcpy(r.slaveIndexName,"POP1");

		/* jump to the first index */
		r.currentSlaveItem=0;
		r.currentRecord=r.highData+2;

		return r;
	case pop2:
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
	default:
		return r;
	}
}

/* public functions */

tPopVersion mReadGetVersion() {
	return readIndexCursor.popVersion;
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
	unsigned long  int indexOffset;
	unsigned short int indexSize;

	/* Open file */
	readDatFileSize=mLoadFileArray(vFiledat,&readDatFile);
	if (!readDatFileSize) return -2;
	if (readDatFileSize<=6) {
		free(readDatFile);
		return -1;
	}

	/* read header  */
	indexOffset=array2long(readDatFile);
	indexSize=array2short(readDatFile+4);

	/* verify dat format: the index offset belongs to the file and the file size is the index size plus the index offset */
	if ((indexOffset>readDatFileSize)&&((indexOffset+indexSize)!=readDatFileSize)) {
		free(readDatFile);
		return -1; /* this is not a valid prince dat file */
	}

	/* create cursor */
	readIndexCursor=dat_createCursor(readDatFile+indexOffset,indexSize,numberOfItems);

	/* pop version check */
	if (!dat_readCursorGetVersion(readIndexCursor)) return -1;

	return 0;
}

int mReadFileInDatFileId(tResource* res) {
	if (!dat_cursorMoveId(&readIndexCursor,res->id)) return 0; /* 0 means index not found */
	/* TODO: use a function for common parts */

	/* for each archived file the index is read */
	res->id.value=        dat_readCursorGetId        (readIndexCursor);
	strncpy(res->id.index,dat_readCursorGetIndexName (readIndexCursor),5);
	res->offset=          dat_readCursorGetOffset    (readIndexCursor);
	res->size=            dat_readCursorGetSize      (readIndexCursor);
	res->flags=           dat_readCursorGetFlags     (readIndexCursor);

	/* if (offset>indexOffset) return -1; * a resourse offset is allways before the index offset TODO: move this check to detect pop version*/
	res->size++; /* add the checksum */

	res->data=readDatFile+res->offset;

	return 1;

}

int mReadFileInDatFile(tResource* res, int k) {

	if (!dat_cursorMove(&readIndexCursor,k)) return 0; /* 0 means out of range */

	/* for each archived file the index is read */
	res->id.value=        dat_readCursorGetId        (readIndexCursor);
	strncpy(res->id.index,dat_readCursorGetIndexName (readIndexCursor),5);
	res->offset=          dat_readCursorGetOffset    (readIndexCursor);
	res->size=            dat_readCursorGetSize      (readIndexCursor);
	res->flags=           dat_readCursorGetFlags     (readIndexCursor);

	/* if (offset>indexOffset) return -1; * a resourse offset is allways before the index offset TODO: move this check to detect pop version*/
	res->size++; /* add the checksum */
	res->data=readDatFile+res->offset;

	return 1;
}

#endif

/***************************************************************\
|                     DAT Writing primitives                    |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATWRITE
FILE* writeDatFile;
tResourceList resIndex;

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
		resIndex=resourceListCreate();
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		return 1;
	} else {
		return 0;
	}
}
/*
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
*/

void dat_write(const tResource* res,unsigned long off) {
	tResource insert;

	/* do the magic */
	fwrite(res->data,res->size,1,writeDatFile);

	/* remember only indexed values */
	insert.id=res->id;
	insert.size=res->size;
	insert.offset=off;

	/* TODO: use an abstract use of the list (at least macros in reslist.h) */
	list_insert(&resIndex,&insert);
}

void mWriteFileInDatFileIgnoreChecksum(const tResource* res) {
	dat_write(res,(unsigned long)ftell(writeDatFile));
}

void mWriteFileInDatFile(const tResource* res) {
	/*
		Adds a data resource to a dat file keeping
		abstractly the checksum verifications
	*/

	/* Declare variables */
	int            k        = res->size;
	unsigned char  checksum = 0;
	const unsigned char* dataAux  = res->data;
	unsigned long off;

	/* calculates the checksum */
	while (k--) checksum+=*(dataAux++);
	checksum=~checksum;

	/* writes the checksum and the data content */
	off=(unsigned long)ftell(writeDatFile);
	fwritechar(&checksum,writeDatFile);

	/* write the resource contents */
	dat_write(res,off);
}


void mWriteCloseDatFile(int dontSave,int optionflag, const char* backupExtension) {
	/*
		Closes a dat file filling the index and other structures
	*/
	unsigned short int totalItems=0;
	unsigned short int size2=2;
	unsigned long  int size1=ftell(writeDatFile);
	const tResource*   res;

	/* Write index */
	fwriteshort(&totalItems,writeDatFile); /* Junk total items count to reserve 2 bytes */

	/* TODO: use an abstract use of the list (at least macros in reslist.h) */
	list_firstCursor(&resIndex);
	while ((res=list_getCursor(&resIndex))) {
		totalItems++;
		fwriteshort(&(res->id.value),writeDatFile);
		fwritelong(&(res->offset),writeDatFile);
		fwriteshort(&(res->size),writeDatFile);

		list_nextCursor(&resIndex);
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

	/* drops the index list */
	resourceListDrop(&resIndex);
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

