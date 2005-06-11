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
import.c: Princed Resources : DAT Compiler
¯¯¯¯¯¯¯¯
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
#include "common.h"
#include "import.h"

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
int mAddCompiledFileToDatFile(tResource* res,const char* vFile) {
	/* return 1 	if ok, 0 	if error */
	switch (res->type) {
		case RES_TYPE_LEVEL:
			return mFormatImportPlv(res);
		case RES_TYPE_IMAGE:
			return mFormatImportBmp(res);
		case RES_TYPE_WAVE:
			return mFormatImportWav(res);
		case RES_TYPE_MIDI:
		case RES_TYPE_PCSPEAKER:
			return mFormatImportMid(res);
		case RES_TYPE_PALETTE:
			return mFormatImportPal(res,vFile);
		case RES_TYPE_BINARY:
		default:
			mWriteFileInDatFile(res);
			break;
	}
	return 1;
}

/***************************************************************\
|                   M A I N   F U N C T I O N                   |
\***************************************************************/

int fullCompile(const char* vFiledat, const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName,const char* backupExtension) {
	/*
		Return values:
			-1 File couldn't be open for writing
			00 File successfully compiled
			positive number: number of missing files
	*/

	char vFileext[MAX_FILENAME_SIZE];
	int error=0;
	int ok=0;
	const tResource* res;
	tResource newRes;

	if (mWriteBeginDatFile(vFiledat,optionflag)) return PR_RESULT_ERR_FILE_DAT_NOT_WRITE_ACCESS; /* File couldn't be open */
	
	list_firstCursor(r);
	while ((res=list_getCursor(r))) {
		/* remember only id and type */
		newRes.id=res->id;
		newRes.type=res->type;
	
		if (hasFlag(raw_flag)) newRes.type=0; /* compile from raw */
		getFileName(vFileext,vDirExt,res,vFiledat,vDatFileName,optionflag,backupExtension);
		/* the file is in the archive, so i'll add it to the main dat body */
		if ((newRes.size=((unsigned short)mLoadFileArray(vFileext,&newRes.data))<=0)) {
			if (!mAddCompiledFileToDatFile(&newRes,vFileext)) {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_ERRORS,getFileNameFromPath(vFileext));
				error++;
			} else {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_SUCCESS,getFileNameFromPath(vFileext));
				ok++;
			}
			free(newRes.data);
		} else {
			if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_NOT_OPEN,getFileNameFromPath(vFileext));
			error++;
		}

		list_nextCursor(r);
	}

	/* Close file. If empty, don't save */
	mWriteCloseDatFile(!ok,optionflag,backupExtension);

	/* Free allocated resources and dynamic strings */
	/*freeResources;*/

	if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_DONE,ok,error);
	return error;
}

#define RW_ERROR {mRWCloseDatFile(1);return error;}
int partialCompile(const char* vFiledat, const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName,const char* backupExtension) {
	/*
		Return values:
			-2 Previous DAT file was invalid
			-1 File couldn't be open for writing
			00 File successfully compiled
			positive number: number of missing files
	*/

	int                error,errors=0,ok=0;
	int                indexNumber;
	char vFileext[MAX_FILENAME_SIZE];
	unsigned short int numberOfItems;
	tResource          res;
	tResource          newRes;

	/* Initialize abstract variables to read this new DAT file */
	if ((error=mRWBeginDatFile(vFiledat,&numberOfItems,optionflag))) return error;

	/* main loop */
	for (indexNumber=0;(indexNumber<numberOfItems);indexNumber++) {
		if ((error=mReadFileInDatFile(&res,indexNumber))) return error; /* Read error */

		if (res.id.value==0xFFFF) continue; /* Tammo Jan Bug fix * TODO: move to the dat layer? */

		/* add to res more information from the resource list */
		resourceListAddInfo(r,&res);

		if (isInThePartialList(res.path,res.id)) { /* If the resource was specified */
			if ((!res.type)&&(!hasFlag(raw_flag))) res.type=verifyHeader(res.data,res.size); 
			if (hasFlag(raw_flag)) res.type=0; /* If "extract as raw" is set, type is 0 */

			/* get save file name (if unknown document is in the xml) */
			getFileName(vFileext,vDirExt,&res,vFiledat,vDatFileName,optionflag,backupExtension);

			/* the file is in the partial list, so i'll import */
			if ((newRes.size=((unsigned short)mLoadFileArray(vFileext,&newRes.data)))) {
				newRes.id=res.id;
				newRes.type=res.type;
				if (!mAddCompiledFileToDatFile(&newRes,vFileext)) {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_ERRORS,getFileNameFromPath(vFileext));
					errors++;
				} else {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_SUCCESS,getFileNameFromPath(vFileext));
					ok++;
				}
				free(newRes.data);
			} else {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_NOT_OPEN,getFileNameFromPath(vFileext));
				errors++;
			}
		} else {
			/* the file wasn't in the partial list, so I'll re-copy it from the open dat file */
			res.size--;
			mWriteFileInDatFileIgnoreChecksum(&res);
		}
	}

	/* Close dat file */
	mRWCloseDatFile(0);

	if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_DONE,ok,errors);
	return errors;
}

int compile(const char* vFiledat, const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName,const char* backupExtension) {

	if (partialListActive()) {
		return partialCompile(vFiledat,vDirExt,r,optionflag,vDatFileName,backupExtension);
	} else {
		return fullCompile(vFiledat,vDirExt,r,optionflag,vDatFileName,backupExtension);
	}
}

