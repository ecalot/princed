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
export.c: Princed Resources : DAT Extractor
¯¯¯¯¯¯¯¯
 Copyright 2003-2005 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)
  Version: 1.20 (2004-Mar-07)
  Version: 1.30 (2004-Mar-15)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <string.h>
#include "common.h"

#include "export.h"

#include "autodetect.h"
#include "dat.h"
#include "disk.h"
#include "idlist.h"
#include "memory.h"
#include "object.h"
#include "palette.h"
#include "pallist.h"
#include "unknown.h"

extern FILE* outputStream;

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/***************************************************************\
|                    M A I N   E X T R A C T                    |
\***************************************************************/

/*
	Extracts a DAT file
	For parameter documentation, see pr.c
*/
void showobj(tObject o); /* TODO: only for debug purposes, delete this line */

int extract(const char* vFiledat,const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName, const char* vDatAuthor,const char* backupExtension,const char* format) {
	char               file[MAX_FILENAME_SIZE];
	int                indexNumber;
	int                ok;
/*	tImage             image; * this is used to make a persistent palette */
	tObject            currentPalette=objectCreate(NULL,&ok); /*TODO: move to pallist */
	unsigned short int numberOfItems;
	tResourceId        nullPalette={0,"",0};
/*	tPaletteList       paletteBuffer;*/
	tResource          res;
	tPL                palettes=pl_create();
	int                count=0;

	/* Initialize abstract variables to read this new DAT file */
	if ((ok=mReadBeginDatFile(&numberOfItems,vFiledat))) return ok;

	/* initialize palette buffer */
	/*paletteBuffer=paletteListCreate();*/
	/* initialize the default palette */
	pl_add(&palettes,currentPalette,nullPalette,lowPriority); /* The null object will be used until a palette is set */
	ok=1;

	/* main loop */
	for (indexNumber=0;ok&&(indexNumber<numberOfItems);indexNumber++) {
		int ok2=mReadFileInDatFile(&res,indexNumber);
		if (ok2==PR_RESULT_INDEX_NOT_FOUND) return PR_RESULT_ERR_INVALID_DAT; /* Read error */
		if (ok2==PR_RESULT_CHECKSUM_ERROR) fprintf(outputStream,"Warning: Checksum error\n"); /* Warning */
		if (res.id.value==0xFFFF) continue; /* Tammo Jan Bug fix */
		/* add to res more information from the resource list */
		resourceListAddInfo(r,&res);

		if (isInTheItemMatchingList(res.path,res.id)) { /* If the resource was specified do the tasks */
			if ((!res.type)&&(!hasFlag(raw_flag))) res.type=verifyHeader(res.content);
			if (!(hasFlag(unknown_flag))) { /* If unknown flag is set do nothing but generate the unknown.xml file */
				tObject o;
				if (hasFlag(raw_flag)) res.type=0; /* If "extract as raw" is set, type is 0 */

				/* get save file name (if unknown document is in the XML) */
				getFileName(file,vDirExt,&res,vFiledat,vDatFileName,optionflag,backupExtension,format);

				res.datfile=vFiledat;
				res.datAuthor=vDatAuthor;

				/* handle palette linking */
				switch (res.type) {
					case eResTypePalettePop2_NColors:
					case eResTypePalettePop1_16: { /* save and remember palette file */
						o=objectCreate(&res,&ok);
printf("new palette object: o=%p type=%d\n",o.obj,o.type);
						if (!ok) { /* if SUCCESS remember the palette, otherwise keep using the default one */
							pl_add(&palettes,o,res.id,lowPriority);
						}
					}	break;
					case eResTypeImage16: /* save image */
					case eResTypeImage256: { /* save image */
						/* Palette handling */
						tResource otherPalette;
						int priorityRight;
						tObject pal;
						otherPalette.id=res.paletteId;
						/* Read the palette and load it into memory */
						if (resourceListCompareId(res.paletteId,nullPalette)) { /* The palette isn't in the buffer */
							if (mReadFileInDatFileId(&otherPalette)==PR_RESULT_SUCCESS) {
								o=objectCreate(&otherPalette,&ok);
printf("adding ");
showobj(o);
								pl_add(&palettes,o,res.paletteId,highPriority);
							} else { /*, that's bad, I'll have to use the previous palette, even if it is the default */
								printf("Warning: the selected palette doesn't exist in the file, the extracted image could result in a wrong palette\n");
							}
						}
						o=objectCreate(&res,&ok);
printf("getting the palette for the %d colours object ",objectGetColorCount(o));
showobj(o);
						pal=pl_get(&palettes,&priorityRight,objectGetColorCount(o));
printf("palette ");
showobj(pal);
						applyPalette(o,pal);
						if (!priorityRight) {
							printf("Warning: the selected palette can't be applied to the image\n");
						}
						break;
					}
					default:
						o=objectCreate(&res,&ok);
						break;
				}
/* TODO: warning counting here */
/*				if (!fatal(ok)) */
				if (ok==PR_RESULT_SUCCESS)
					ok=objectWrite(o,file,optionflag,backupExtension);
				else
				/*	printf("not ok. result=%d for %s\n",ok,file);*/

				/* Verbose information */
				if (hasFlag(verbose_flag)) /*{
					if (ok) {
						fprintf(outputStream,PR_TEXT_EXPORT_WORKING,getFileNameFromPath(file));
					} else {
*/						fprintf(outputStream,"Error in file %s (code %d)\n"/*PR_TEXT_EXPORT_ERROR*/,getFileNameFromPath(file),ok);/*
					}
				} TODO: add warning counter */
				ok=1; /* efit the for and add !fatal(ok)*/
				if (ok) count++;
				printf("done with %s\n",file);
			} else {
				/* If the DAT file is unknown, add it in the XML */
				getFileName(file,vDirExt,&res,vFiledat,vDatFileName,optionflag,backupExtension,format);
			}
		}
	}

	/* Free allocated resources, dynamic strings and the index */
	resourceListDrop(r);
	mReadCloseDatFile();
	/*pl_free(&paletteBuffer);*/

	/* Close unknownXML */
	return ok?count:PR_RESULT_ERR_EXTRACTION;
}

