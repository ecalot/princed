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
 Copyright 2003, 2004 Princed Development Team
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
#include "pr.h"

#include "extract.h"
#include "dat.h"
#include "disk.h"
#include "memory.h"

#include "bmp.h"
#include "wav.h"
#include "pal.h"
#include "plv.h"
#include "mid.h"

extern FILE* outputStream;

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#define initializePaletteList \
for (id=0;id<MAX_RES_COUNT;id++) {\
	if (r[id]!=NULL) {\
		r[id]->palAux=NULL;\
	}\
}

/***************************************************************\
|                    M A I N   E X T R A C T                    |
\***************************************************************/


/*
	Extracts a dat file
	For parameter documentation, see pr.c
*/

int extract(const char* vFiledat,const char* vDirExt, tResource* r[], int optionflag, const char* vDatFileName, const char* vDatAuthor,const char* backupExtension) {
	char               vFileext[MAX_FILENAME_SIZE];
	int                indexNumber;
	int                ok=1;
	long int           id;
	tImage             image; /* this is used to make a persistent palette */
	unsigned char*     data;
	unsigned long  int size;
	unsigned short int numberOfItems;
	signed long    int bufferedPalette=0;

	/* Initialize abstract variables to read this new DAT file */
	if (!mReadBeginDatFile(&numberOfItems,vFiledat)) return -1;
fld("a");
	/* Initializes the palette list */
	initializePaletteList;
	
fld("b");

	/* main loop */
	for (indexNumber=0;ok&&(indexNumber<numberOfItems);indexNumber++) {
		id=mReadFileInDatFile(indexNumber,&data,&size);
fld("c");
/* printf("*K) id=%d size=%d %d:%d:%d:%d:%d:%d\n",id,size,data[0],data[1],data[2],data[3],data[4],data[5]); */

		if (id<0) return -3; /* Read error */
		if (id==0xFFFF) continue; /* Tammo Jan Bug fix */
		if (id>=MAX_RES_COUNT) return -3; /* A file with an ID out of range will be treated as invalid */
fld("d");

		/* set resource information on this index entry */
		if (mReadInitResource(r+id,data,size)) return -2;
fld("e");
/* printf("z->%d\n",r[id]->type); */
		if ((r[id]->type==RES_TYPE_PALETTE)||isInThePartialList(r[id]->path,id)) { /* If the resource was specified or is a palette, do the tasks */
			if (!(hasFlag(unknown_flag))) { /* If unknown flag is set do nothing but generate the unknown.xml file */
				if (hasFlag(raw_flag)) r[id]->type=0; /* If "extract as raw" is set, type is 0 */

				/* get save file name (if unknown document it in the xml) */
				getFileName(vFileext,vDirExt,r[id],(unsigned short)id,vFiledat,vDatFileName,optionflag,backupExtension);
fld("f");
				switch (r[id]->type) {
					case RES_TYPE_LEVEL:
						ok=ok&&mFormatExportPlv(data,vFileext,size,r[id]->number,vDatFileName,r[id]->name,r[id]->desc,vDatAuthor,optionflag,backupExtension);
						break;
					case RES_TYPE_BINARY: /* Binary files */
					case RES_TYPE_RAW: /* Raw files */
						ok=ok&&writeData(data,1,vFileext,size,optionflag,backupExtension); /* Ignore checksum */
						break;
					case RES_TYPE_PALETTE: /* save and remember palette file */
						/* This will remember the palette for the next images */
						r[id]->palAux=getMemory(size);
						memcpy(r[id]->palAux,data,size);
						if (!bufferedPalette) { /* In case the palette buffer is empty, load immediately the first found palette to clear garbage */
							mLoadPalette(data,image);
							bufferedPalette=id;
						}
						/* This will export the palette */
						if (isInThePartialList(r[id]->path,id))  /* If the palette was specified extract it */
							ok=ok&&mFormatExportPal(data,vFileext,size,optionflag,backupExtension);
						break;
					case RES_TYPE_PCSPEAKER: /* save pcs file */
					case RES_TYPE_MIDI:	/* save midi file */
/* printf("a->%d\n",ok); */
						ok=ok&&mFormatExportMid(data,vFileext,size,optionflag,backupExtension);
/* printf("b->%d\n",ok); */
						break;
					case RES_TYPE_WAVE: /* save wav file */
						ok=ok&&mFormatExportWav(data,vFileext,size,optionflag,backupExtension);
						break;
					case RES_TYPE_IMAGE: /* save image */
						/* Palette handling */
fld("Z1");
						if (r[id]->palette!=bufferedPalette) { /* The palette isn't in the buffer */
							if (r[id]->palette) { /* We need a palette */
								/*
									We need a palette and it is not the palette we have loaded in buffer
									So a new palette is going to be loaded.
								*/
								if ((r[r[id]->palette]->palAux)!=NULL) { /* If this palette wasn't loaded, it becomes loaded */
									mLoadPalette(r[r[id]->palette]->palAux,image);
									bufferedPalette=r[id]->palette; /* sets the new palette loaded */
								}
							}
						} else {
							/* It could be that r[id]->palette==bufferedPalette because both are 0,
							 * in that case there is no buffered palette, but I don't know what palette
							 * I need (cga case), so I'll use a default hard-coded palette */
							if (!bufferedPalette) {
								memcpy(image.pal,DEFAULT_PALETTE,SIZE_OF_PALETTE);
								bufferedPalette=-1; /* -1 is the id of this default palette */
							}
						}
						/* Export bitmap */
fld("Z2");
						ok=ok&&mFormatExportBmp(data,vFileext,size,image,optionflag,backupExtension);
fld("Z3");
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
				getFileName(vFileext,vDirExt,r[id],(unsigned short)id,vFiledat,vDatFileName,optionflag,backupExtension);
			}
			/*freeAllocation(data);*/
		}
	}

	/* Free allocated resources, dynamic strings and the index */
	for (id=0;id<MAX_RES_COUNT;id++) {
		if (r[id]!=NULL) {
			freeAllocation(r[id]->palAux);
			freeAllocation(r[id]->desc);
			freeAllocation(r[id]->name);
			freeAllocation(r[id]->path);
			free(r[id]);
		}
	}
	mReadCloseDatFile();

	/* Close unknownXML */
	endUnknownXml(optionflag,backupExtension);
	return ok-1;
}

