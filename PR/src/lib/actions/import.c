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
compile.c: Princed Resources : DAT Compiler
¯¯¯¯¯¯¯¯¯
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

#include <string.h>
#include "compile.h"
#include "memory.h"
#include "bmp.h"
#include "mid.h"
#include "wav.h"
#include "pal.h"
#include "plv.h"
#include "disk.h"
#include "pr.h"

/***************************************************************\
|                     Dat compiling primitives                  |
\***************************************************************/

char mBeginDatFile(FILE* *fp,const char* vFile) {
	/*
		Opens safely a dat file for writing mode and
		reserves space for the headers
	*/

	if (writeOpen(vFile,fp)) {
		fseek(*fp,6,SEEK_SET);
		return 1;
	} else {
		return 0;
	}
}

void mAddFileToDatFile(FILE* fp, unsigned char* data, int size) {
	/*
		Adds a data resource to a dat file keeping
		abstratly the checksum verifications
	*/

	//Declare variables
	int            k        = size;
	unsigned char  checksum = 0;
	unsigned char* data2    = data;

	//calculates the checksum
	while (k--) checksum+=*(data2++);
	checksum=~checksum;

	//writes the checksum and the data content
	fwrite(&checksum,1,1,fp);
	fwrite(data,size,1,fp);
}

void mSetEndFile(FILE* fp, tResource* r[]) {
	/*
		Closes a dat file filling the index and other structures
	*/
	unsigned short int i=1;
	unsigned short int totalItems=0;
	unsigned short int size2=2;
	unsigned long  int size1=ftell(fp);

	//Write index
	fwrite(&totalItems,2,1,fp); //Junk total items count to reserve 2 bytes
	for (;i!=MAX_RES_COUNT;i++) {
		if (r[i]!=NULL) {
			//the file is in the archive, so I'll add it to the index
			size2+=8;
			totalItems++;
			fwrite(&i,2,1,fp);
			fwrite(&((*r[i]).offset),4,1,fp);
			fwrite(&((*r[i]).size),2,1,fp);
		}
	}
	fseek(fp,size1,SEEK_SET);
	fwrite(&totalItems,2,1,fp); //Definitive total items count

	//Write first 6 bytes header
	fseek(fp,0,SEEK_SET);
	fwrite(&size1,4,1,fp);
	fwrite(&size2,2,1,fp);

	//Closes the file and flushes the buffer
	fclose(fp);
}

//Format detection function (private function, not in header file)
int mAddCompiledFileToDatFile(FILE* fp,unsigned char** data, tResource *res) {
	//return 1 if ok, 0 if error
	switch (res->type) {
		case 1: //compile levels
			return mFormatCompilePlv(*data,fp,res);
		case 2: //compile bitmap
			return mFormatCompileBmp(*data,fp,res);
		case 3: //compile wave
			return mFormatCompileWav(*data,fp,res);
		case 7: //compile pcs
		case 4: //compile midi
			return mFormatCompileMid(*data,fp,res);
		case 6: //compile palette
			//TODO: make mFormatCompilePal and stop using char** for data
			if (!mImportPalette(data,&((*res).size))) {
				return 0;
			}
		case 5:
		default:
			mAddFileToDatFile(fp,*data,(*res).size);
			break;
	}
	return 1;
}

/***************************************************************\
|                    M A I N   F U N C T I O N                  |
\***************************************************************/

int compile(const char* vFiledat, const char* vDirExt, tResource* r[], int opt, const char* vDatFileName) {
	/*
		Return values:
			-1 File couldn't be open for writing
			00 File succesfully compiled
			positive number: number of missing files
	*/

	FILE* fp;
	char vFileext[200];
	int ok=0;
	unsigned char* data;
	unsigned short int i=1;

	if (!mBeginDatFile(&fp,vFiledat)) {
		return -1; //File couldn't be open
	}

	//getUpperFolder(vUpperFile,vFiledat);

	for (;i!=MAX_RES_COUNT;i++) {
		if (r[i]!=NULL) {
			if (opt&raw_flag) r[i]->type=0; //compile from raw
			getFileName(vFileext,vDirExt,r[i],i,vFiledat,vDatFileName);
			//the file is in the archive, so I'll add it to the main dat body
			if (r[i]->size=(unsigned short)mLoadFileArray(vFileext,&data)) {
				r[i]->offset=(unsigned short)ftell(fp);
				if (!mAddCompiledFileToDatFile(fp,&data,r[i])) {
					if (opt&verbose_flag) printf("'%s' has errors, skipped\n",getFileNameFromPath(vFileext));
					ok++;
				} else {
					if (opt&verbose_flag) printf("'%s' succesfully compiled\n",getFileNameFromPath(vFileext));
				}
				free(data);
			} else {
				if (opt&verbose_flag) printf("'%s' not open, skipped\n",/*getFileNameFromPath(*/vFileext/*)*/);
				ok++;
			}
		}
	}
	mSetEndFile(fp,r);
	//TODO: if file size if 8, then erase it
	if (opt&verbose_flag) printf("Compilation done\n");
	return ok;
}
