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
#include "pr.h"
#include "compile.h"

#include "dat.h"
#include "memory.h"
#include "disk.h"

#include "bmp.h"
#include "mid.h"
#include "wav.h"
#include "pal.h"
#include "plv.h"

extern FILE* outputStream;

/***************************************************************\
|                    Dat compiling primitive                    |
\***************************************************************/

/* Format detection function (private function, not in header file) */
int mAddCompiledFileToDatFile(unsigned char* data, tResource* res,const char* vFile) {
	/* return 1 	if ok, 0 	if error */
	switch (res->type) {
		case RES_TYPE_LEVEL:
			return mFormatImportPlv(data,res);
		case RES_TYPE_IMAGE:
			return mFormatImportBmp(data,res);
		case RES_TYPE_WAVE:
			return mFormatImportWav(data,res);
		case RES_TYPE_MIDI:
		case RES_TYPE_PCSPEAKER:
			return mFormatImportMid(data,res);
		case RES_TYPE_PALETTE:
			return mFormatImportPal(data,res,vFile);
		case RES_TYPE_BINARY:
		default:
			mWriteSetFileInDatFile(data,res->size);
			break;
	}
	return 1;
}

#define freeResources \
for (id=0;id<MAX_RES_COUNT;id++) {\
	if (r[id]!=NULL) {\
		freeAllocation(r[id]->desc);\
		freeAllocation(r[id]->name);\
		freeAllocation(r[id]->path);\
		free(r[id]);\
	}\
}


/***************************************************************\
|                   M A I N   F U N C T I O N                   |
\***************************************************************/

int fullCompile(const char* vFiledat, const char* vDirExt, tResource* r[], int optionflag, const char* vDatFileName,const char* backupExtension) {
	/*
		Return values:
			-1 File couldn't be open for writing
			00 File successfully compiled
			positive number: number of missing files
	*/

	char vFileext[MAX_FILENAME_SIZE];
	int error=0;
	int ok=0;
	unsigned char* data;
	unsigned short int id=1;

	if (!mWriteBeginDatFile(vFiledat,optionflag)) return -1; /* File couldn't be open */

	for (;id!=MAX_RES_COUNT;id++) {
		if (r[id]!=NULL) {
			if (hasFlag(raw_flag)) r[id]->type=0; /* compile from raw */
			getFileName(vFileext,vDirExt,r[id],id,vFiledat,vDatFileName,optionflag,backupExtension);
			/* the file is in the archive, so i'll add it to the main dat body */
			if ((r[id]->size=((unsigned short)mLoadFileArray(vFileext,&data)))) {
				mWriteInitResource(r+id);
				if (!mAddCompiledFileToDatFile(data,r[id],vFileext)) {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_ERRORS,getFileNameFromPath(vFileext));
					error++;
				} else {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_SUCCESS,getFileNameFromPath(vFileext));
					ok++;
				}
				free(data);
			} else {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_NOT_OPEN,getFileNameFromPath(vFileext));
				error++;
			}
		}
	}
	/* Close file. 	if empty, don't save */
	mWriteCloseDatFile(r,!ok,optionflag,backupExtension);

	/* Free allocated resources and dynamic strings */
	freeResources;

	if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_DONE,ok,error);
	return error;
}

#define RW_ERROR {mRWCloseDatFile(1);return 0;}
int partialCompile(const char* vFiledat, const char* vDirExt, tResource* r[], int optionflag, const char* vDatFileName,const char* backupExtension) {
	/*
		Return values:
			-2 Previous DAT file was invalid
			-1 File couldn't be open for writing
			00 File successfully compiled
			positive number: number of missing files
	*/

	char vFileext[MAX_FILENAME_SIZE];
	int                error,ok=0;
	int                indexNumber;
	long int           id;
	unsigned char*     data;
	unsigned long  int size;
	unsigned short int numberOfItems;

	/* Initialize abstract variables to read this new DAT file */
	if ((error=mRWBeginDatFile(vFiledat,&numberOfItems,optionflag))) return error;

	/* main loop */
	for (indexNumber=0;(indexNumber<numberOfItems);indexNumber++) {
		id=mReadGetFileInDatFile(indexNumber,&data,&size);
		if (id<0) RW_ERROR; /* Read error */ /* TODO BUG: return doesn't close file */
		if (id==0xFFFF) continue; /* Tammo Jan Bug fix */
		if (id>=MAX_RES_COUNT) RW_ERROR; /* A file with an ID out of range will be treated as invalid */

		mWriteInitResource(r+id);
		if (r[id]&&isInThePartialList(r[id]->path,id)) { /* If the resource was specified */
			if (hasFlag(raw_flag)) r[id]->type=0; /* compile from raw */
			getFileName(vFileext,vDirExt,r[id],(unsigned short)id,vFiledat,vDatFileName,optionflag,backupExtension);
			/* the file is in the archive, so i'll add it to the main dat body */
			if ((r[id]->size=((unsigned long)mLoadFileArray(vFileext,&data)))) {
				if (!mAddCompiledFileToDatFile(data,r[id],vFileext)) {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_ERRORS,getFileNameFromPath(vFileext));
					error++;
				} else {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_SUCCESS,getFileNameFromPath(vFileext));
					ok++;
				}
				free(data);
			} else {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_NOT_OPEN,getFileNameFromPath(vFileext));
				error++;
			}
		} else {
			r[id]->size=size-1;
			mWriteSetFileInDatFileIgnoreChecksum(data,size);
		}
	}

	/* Close dat file */
	mRWCloseDatFile(0);

	/* Free allocated resources and dynamic strings */
	freeResources;

	if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_DONE,ok,error);
	return error;
}

int compile(const char* vFiledat, const char* vDirExt, tResource* r[], int optionflag, const char* vDatFileName,const char* backupExtension) {
	if (partialListActive()) {
		return partialCompile(vFiledat,vDirExt,r,optionflag,vDatFileName,backupExtension);
	} else {
		return fullCompile(vFiledat,vDirExt,r,optionflag,vDatFileName,backupExtension);
	}
}
