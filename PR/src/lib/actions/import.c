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
#include "parser.h"
#include "disk.h"

/***************************************************************\
|                  Dat compiling primitives                     |
\***************************************************************/

char mBeginDatFile(FILE* *fp,char* vFile) {
	//opens a file and reserves space for the headers
	*fp=fopen(vFile,"wb");
	if (*fp!=NULL) {
		fseek(*fp,6,SEEK_SET);
		return 1;
	} else {
		return 0;
	}
}

void mAddFileToDatFile(FILE* fp, char* data, int size) {
	//calculates the checksum of a file
	unsigned char checksum=0;
	int k;
	for (k=0;k<size;k++) checksum+=data[k];
	checksum=~checksum;
	//writes the checksum and the data content
	fwrite(&checksum,1,1,fp);
	fwrite(data,size,1,fp);
}

void mSetEndFile(FILE* fp,int sizeOfIndex) {
	//sets the headers
	short int size1,zero,size2;
	fseek(fp,0,SEEK_END);
	size1=ftell(fp)-(size2=sizeOfIndex);
	zero=0;
	fseek(fp,0,SEEK_SET);
	fwrite(&size1,2,1,fp);
	fwrite(&zero,2,1,fp);
	fwrite(&size2,2,1,fp);
	fclose(fp);
}

int mCreateIndexInDatFile(FILE* fp, tResource* r[]) {
	//Add extra text at the end of the file
	unsigned short int i=0;
	unsigned short int junk=0;
	unsigned short int tot=0;
	int k=2;
	int pos=ftell(fp);

	fwrite(&tot,2,1,fp);
	for (;i!=65000;i++) { //TODO: add define 65000
		if (r[i]!=NULL) {
//			if (equals((*r[i]).file,vUpperFile)) {
				//the file is in the archive, so I'll add it to the index
				k+=8;
				tot++;
				fwrite(&i,2,1,fp);
				fwrite(&((*r[i]).offset),2,1,fp);
				fwrite(&junk,2,1,fp);
				fwrite(&((*r[i]).size),2,1,fp);
//			}
		}
	}
	fseek(fp,pos,SEEK_SET);
	fwrite(&tot,2,1,fp);
	return k;
}

//Format detection function (private function, not in header file)
char mAddCompiledFileToDatFile(FILE* fp,unsigned char** data, tResource *res) {
	switch ((*res).type) {
		case 2: //compile bitmap
			return mFormatCompileBmp(*data,fp,res);
		case 3: //compile wave
			return mFormatCompileWav(*data,fp,res);
		case 4: //compile midi
			return mFormatCompileMid(*data,fp,res);
		case 6: //compile palette
			//TODO: make mFormatCompilePal and stop using char** for data
			if (!mImportPalette(data,&((*res).size))) {
				return 0;
			}
		case 1:
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

int compile(char* vFiledat, char* vDirExt, tResource* r[], char opt) {
	/*
		Return values:
			-1 File couldn't be open for writing
			00 File succesfully compiled
			positive number: number of missing files
	*/

	FILE* fp;
	char vFileext[200];
//	char vUpperFile[200];
	int ok=0;
	unsigned char* data;
	unsigned short int i=0;

	if (!mBeginDatFile(&fp,vFiledat)) {
		return -1; //File couldn't be open
	}

	//getUpperFolder(vUpperFile,vFiledat);

	for (;i!=65000;i++) {
		if (r[i]!=NULL) {
//			if (equals((*r[i]).file,vUpperFile)) {
				getFileName(vFileext,vDirExt,r[i],i,vFiledat);
				//the file is in the archive, so I'll add it to the main dat body
				if ((*r[i]).size=mLoadFileArray(vFileext,&data)) {
					(*r[i]).offset=(unsigned short)ftell(fp);
					if (!mAddCompiledFileToDatFile(fp,&data,r[i])) {
						ok++;
					}
					free(data);
				} else {
					ok++;
				}
//			}
		}
	}
	mSetEndFile(fp,mCreateIndexInDatFile(fp,r));
	return ok;
}
