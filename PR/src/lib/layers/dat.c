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
 Copyright 2004 Princed Development Team
  Created: 15 Mar 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2004-Mar-15)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <string.h>
#include "pr.h"

#include "resources.h"
#include "disk.h"
#include "memory.h"

/***************************************************************\
|                     DAT reading primitives                    |
\***************************************************************/

char               recordSize;
int                ofk=0;
int                pop1;
unsigned char*     indexPointer;
unsigned long  int indexOffset;
unsigned long  int offset;
unsigned short int indexSize;
unsigned char*     readDatFile;
int                readDatFileSize;

void mReadCloseDatFile() {
	free(readDatFile);
}

int mReadBeginDatFile(unsigned short int *numberOfItems,const char* vFiledat){
	/* 
		Return Values:
			-1 Wrong format
	 *
	 *
	 *
	 *
	*/

	int ok;
	unsigned char* readDatFilePoint;

	/* Open file */
	readDatFileSize=mLoadFileArray(vFiledat,&readDatFile);
	if (!readDatFileSize) return -1;
	if (readDatFileSize<=6) return -1;

	readDatFilePoint=readDatFile;

	/* verify dat format */
	indexOffset=array2long(readDatFilePoint);
	readDatFilePoint+=4;
	indexSize=array2short(readDatFilePoint);

	if ((indexOffset>readDatFileSize)&&((indexOffset+indexSize)!=readDatFileSize)) {
		free(readDatFile);
		return -1; /* this is not a valid prince dat file */
	}

	indexPointer=readDatFile+indexOffset;
	*numberOfItems=array2short(indexPointer);
	indexPointer+=2;
	pop1=(((*numberOfItems)*8+2)==indexSize);

	if (!pop1) { /* verify if pop2 */
		ofk=(*numberOfItems)*6+2+((*numberOfItems)-2)*13;
		(*numberOfItems)=((indexSize-6-((*numberOfItems)*6)-(((*numberOfItems)-2)*13))/11);
	} else {
		ofk=0;
	}
	recordSize=pop1?8:11;

	return 0;
}

int mReadGetFileInDatFile(int k,unsigned char* *data,unsigned long  int *size) {
	int ok=1;
	unsigned short int id;

	/* for each archived file the index is read */
	id=    (indexPointer[ofk+k*recordSize])+(indexPointer[ofk+k*recordSize+1]<<8);
	offset=(indexPointer[ofk+k*recordSize+2])+(indexPointer[ofk+k*recordSize+3]<<8)+(indexPointer[ofk+k*recordSize+4]<<16)+(indexPointer[ofk+k*recordSize+5]<<24);
	*size= (indexPointer[ofk+k*recordSize+6])+(indexPointer[ofk+k*recordSize+7]<<8)+1;
	if (!pop1) ok=(indexPointer[ofk+k*recordSize+8]==0x40)&&(!indexPointer[ofk+k*recordSize+9])&&(!indexPointer[ofk+k*recordSize+10]);
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
		(*res)->offset=(unsigned short)offset;
		(*res)->type=verifyHeader(data,(unsigned short int)size);
	} else { /* If resource type is invalid or 0, the type will be decided by PR */
		if (!((*res)->type)) (*res)->type=verifyHeader(data,(unsigned short int)size);
	}
	return 0;
}

/***************************************************************\
|                     DAT Writing primitives                    |
\***************************************************************/

FILE* writeDatFile;

int mWriteBeginDatFile(const char* vFile, int optionflag) {
	/*
		Opens safely a dat file for writing mode and
		reserves space for the headers
	*/

	if (writeOpen(vFile,&writeDatFile,optionflag|backup_flag)) {
		fseek(writeDatFile,6,SEEK_SET);
		return 1;
	} else {
		return 0;
	}
}

void mWriteInitResource(tResource** res) {
	(*res)->offset=(unsigned long)ftell(writeDatFile);
}

void mWriteSetFileInDatFile(unsigned char* data, int size) {
	/*
		Adds a data resource to a dat file keeping
		abstractly the checksum ver	ifications
	*/

	/* Declare variables */
	int            k        = size;
	unsigned char  checksum = 0;
	unsigned char* dataAux  = data;

	/* calculates the checksum */
	while (k--) checksum+=*(dataAux++);
	checksum=~checksum;

	/* writes the checksum and the data content */
	fwrite(&checksum,1,1,writeDatFile);
	fwrite(data,size,1,writeDatFile);
}

void mWriteSetFileInDatFileIgnoreChecksum(unsigned char* data, int size) {
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
	fwrite(&totalItems,2,1,writeDatFile); /* Junk total items count to reserve 2 bytes */
	for (;id!=MAX_RES_COUNT;id++) {
		if (r[id]!=NULL) {
			/* the file is in the archive, so i'll add it to the index */
			totalItems++;
			fwrite(&id,2,1,writeDatFile);
			fwrite(&(r[id]->offset),4,1,writeDatFile);
			fwrite(&(r[id]->size),2,1,writeDatFile);
		}
	}
	size2+=totalItems<<3;
	fseek(writeDatFile,size1,SEEK_SET);
	fwrite(&totalItems,2,1,writeDatFile); /* Definitive total items count */

	/* Write first 6 bytes header */
	fseek(writeDatFile,0,SEEK_SET);
	fwrite(&size1,4,1,writeDatFile);
	fwrite(&size2,2,1,writeDatFile);

	/* Closes the file and flushes the buffer */
	writeClose(writeDatFile,dontSave,optionflag,backupExtension);
}

