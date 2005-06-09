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
extract.c: Princed Resources : DAT Extractor
¯¯¯¯¯¯¯¯¯
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
#include "dat.h"
#include "disk.h"
#include "memory.h"
#include "reslist.h" /* resIdcmp, resourceList primitives for the palette */

#include "bmp.h"
#include "wav.h"
#include "pal.h"
#include "plv.h"
#include "mid.h"

extern FILE* outputStream;

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/***************************************************************\
|                    M A I N   E X T R A C T                    |
\***************************************************************/


/*
	Extracts a dat file
	For parameter documentation, see pr.c
*/

int extract(const char* vFiledat,const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName, const char* vDatAuthor,const char* backupExtension) {
	char               vFileext[MAX_FILENAME_SIZE];
	int                indexNumber;
	int                ok=1;
	tImage             image; /* this is used to make a persistent palette */
	unsigned short int numberOfItems;
	tResourceList      paletteBuffer;
	tResourceId        bufferedPalette={0,""};
	tResource          res;

	/* Initialize abstract variables to read this new DAT file */
	if (mReadBeginDatFile(&numberOfItems,vFiledat)) return -1;

	/* initialize palette buffer */
	paletteBuffer=resourceListCreate(1);
	/* initialize the default palette */
	memcpy(image.pal,DEFAULT_PALETTE,SIZE_OF_PALETTE);
	
	/* main loop */
	for (indexNumber=0;ok&&(indexNumber<numberOfItems);indexNumber++) {

		if (!mReadFileInDatFile(&res,indexNumber)) return -3; /* Read error */
		if (res.id.value==0xFFFF) continue; /* Tammo Jan Bug fix */

		/* add to res more information from the resource list */
		resourceListAddInfo(r,&res);
		
		if (isInThePartialList(res.path,res.id.value /*TODO: use res.id and code the index support to the partial list*/)) { /* If the resource was specified do the tasks */
			if ((!res.type)&&(!hasFlag(raw_flag))) res.type=verifyHeader(res.data,res.size); 
			if (!(hasFlag(unknown_flag))) { /* If unknown flag is set do nothing but generate the unknown.xml file */
				if (hasFlag(raw_flag)) res.type=0; /* If "extract as raw" is set, type is 0 */

				/* get save file name (if unknown document is in the xml) */
				getFileName(vFileext,vDirExt,&res,vFiledat,vDatFileName,optionflag,backupExtension);

				switch (res.type) {
						case RES_TYPE_LEVEL:
						ok=mFormatExportPlv(res.data,vFileext,res.size,res.number,vDatFileName,res.name,res.desc,vDatAuthor,optionflag,backupExtension);
						break;
					case RES_TYPE_BINARY: /* Binary files */
					case RES_TYPE_RAW: /* Raw files */
						ok=writeData(res.data,1,vFileext,res.size,optionflag,backupExtension); /* Ignore checksum */
						break;
					case RES_TYPE_PALETTE: /* save and remember palette file */
						/* This will remember the palette for the next images
						 * (because it's more probable to get all the images after its palette) */
						mLoadPalette(res.data,image,mReadGetVersion());
						bufferedPalette=res.id;
						resourceListAdd(&paletteBuffer,&res);
						/* This will export the palette */
						ok=mFormatExportPal(res.data,vFileext,res.size,optionflag,backupExtension);
						break;
					case RES_TYPE_PCSPEAKER: /* save pcs file */
					case RES_TYPE_MIDI:	/* save midi file */
						ok=mFormatExportMid(res.data,vFileext,res.size,optionflag,backupExtension);
						break;
					case RES_TYPE_WAVE: /* save wav file */
						ok=mFormatExportWav(res.data,vFileext,res.size,optionflag,backupExtension);
						break;
					case RES_TYPE_IMAGE: /* save image */
						/* Palette handling */
						if (resIdCmp(res.palette,bufferedPalette)) { /* The palette isn't in the buffer */
							tResource readPalette;
							readPalette.id=res.palette;
							/* read the palette and load it into memory */
							if (mReadFileInDatFileId(&readPalette)) {
								/* all right, it's not so bad, I can handle it! I'll buffer the new palette */
								bufferedPalette=readPalette.id;
								mLoadPalette(readPalette.data,image,mReadGetVersion());
								resourceListAdd(&paletteBuffer,&readPalette);
							} /* else, that's bad, I'll have to use the previous palette, even if it is the default */
						} /* else, good, the palette is buffered */
						/* Export bitmap */

						ok=mFormatExportBmp(res.data,vFileext,res.size,image,optionflag,backupExtension);

						break;
				}
				/* Verbose information */
				if (hasFlag(verbose_flag)) {
					if (ok) {
						fprintf(outputStream,PR_TEXT_EXPORT_WORKING,getFileNameFromPath(vFileext));
					} else {
						fprintf(outputStream,PR_TEXT_EXPORT_ERROR,getFileNameFromPath(vFileext));
					}
				}
			} else {
				/* if the dat file is unknown, add it in the xml */
				getFileName(vFileext,vDirExt,&res,vFiledat,vDatFileName,optionflag,backupExtension);
			}
		}
	}

	/* Free allocated resources, dynamic strings and the index */
	resourceListDrop(r);
	mReadCloseDatFile();
	resourceListDrop(&paletteBuffer);

	/* Close unknownXML */
	endUnknownXml(optionflag,backupExtension);
	return ok-1;
}

