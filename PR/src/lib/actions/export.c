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
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include "extract.h"
#include "memory.h"
#include "bmp.h"
#include "wav.h"
#include "pal.h"
#include "mid.h"

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

char writeData(unsigned char* data, int ignoreChars, char* vFileext, int size) {
	FILE*              target;
	char ok;

	ok=((target=fopen(vFileext,"wb"))!=NULL);
	ok=ok&&fwrite(data+ignoreChars,size-ignoreChars,1,target);
	ok=ok&&(!fclose(target));
	return ok;
}

/***************************************************************\
|                     M A I N   E X T R A C T                   |
\***************************************************************/

/*
	Tasks:
		0 - none
		1 - extract
		2 - update resources (rebuild resources list)
		3 - 1&2
		4 - use raw file if extraction
		5 - 1&4
*/

int extract(char* vFiledat,char* vDirExt, tResource* r[], char task) {

	//Variables
	char          vFileext[260];
	FILE*         fp;
  char          ok,pop1;

	if (ok=((fp=fopen(vFiledat,"rb"))!=NULL)) {
		//loop variables
		unsigned long int indexOffset;
		unsigned short int indexSize,numberOfItems;
		unsigned char* index;
		int                ofk=0;
		int k;

		//if header ok, new variables
		unsigned short int id;
		unsigned long int  size,offset;
		unsigned char*     data;
//		FILE*              target;
		char               type=0;
		char               recordSize;
		char               aux[260];
		tImage             image; //this is used to make a persistent palette
		char               isntImageSet=1;
	////printf("kkkkkkkkkkk: %x\n",task);

		//verify dat format
		ok    = fread(&indexOffset,4,1,fp);
		ok=ok&& fread(&indexSize,2,1,fp);
		ok=ok&& !fseek(fp,indexOffset,SEEK_SET);
		ok=ok&& fread(&numberOfItems,2,1,fp);
		pop1=((numberOfItems*8+2)==indexSize);

		if (!pop1) { //verify if pop2
			ofk=numberOfItems*6+2+(numberOfItems-2)*13;
			numberOfItems=((indexSize-6-(numberOfItems*6)-((numberOfItems-2)*13))/11);
////printf("verificando pop2: numberOfItems=%d, indexSize=%d\r\n",numberOfItems,indexSize);
		}
		recordSize=pop1?8:11;
		if (!ok) {
			fclose(fp);
			return -3; //this is not a valid prince dat file
		}
		if ((index=getMemory(indexSize-2))==NULL) {
			fclose(fp);
			return -2; //no memory
		}
		ok=fread(index,indexSize-2,1,fp);

		//parse folder and take dat filename in caps
		getUpperFolder(aux,vFiledat);

		//main loop
		for (k=0;ok&&(k<numberOfItems);k++) {
			//for each archived file
			id=index[ofk+k*recordSize]+256*index[ofk+k*recordSize+1];
			offset=index[ofk+k*recordSize+2]+256*index[ofk+k*recordSize+3]+256*256*index[ofk+k*recordSize+4]+256*256*256*index[ofk+k*recordSize+5];
			size=index[ofk+k*recordSize+6]+256*index[ofk+k*recordSize+7]+1;
			if (!pop1) {
				//printf("jajaA %d\r\n",ok);
				ok=ok&&(index[ofk+k*recordSize+8]==0x40)&&(!index[ofk+k*recordSize+9])&&(!index[ofk+k*recordSize+10]);
				//printf("jajaB %d\r\n",ok);
			}
			ok=ok&&((data=getMemory(size))!=NULL);
				//printf("jajaC %d offset=%d\r\n",ok,offset);
			ok=ok&&(!fseek(fp,offset,SEEK_SET));
				//printf("jajaD %d\r\n",ok);
			ok=ok&&fread(data,size,1,fp);
				//printf("jajaEd %d\r\n",ok);
			if (!ok) return -3;

			//For the moment rebuilt option will be mandatory:
			task|=2;

			//If rebuild option has been chosen, destroy previous declaration
			if ((r[id]!=NULL)&&(task&2)) {
				if ((*(r[id])).coms!=NULL) free ((*(r[id])).coms);
				if ((*(r[id])).desc!=NULL) free ((*(r[id])).desc);
				free(r[id]);
			}

			//set resource information on this index entry
			if (r[id]==NULL) {
				r[id]=(tResource*)malloc(sizeof(tResource));
				(*(r[id])).size=(unsigned short int)size;
				(*(r[id])).offset=(unsigned short)offset;
				sprintf((*(r[id])).file,aux);
				(*(r[id])).desc=NULL;
				(*(r[id])).coms=NULL;
				(*(r[id])).type=verifyHeader(data,(unsigned short int)size);
			}

			if (task&1) {
				//select type
				if (task&4) (*(r[id])).type=0;
				//save file
				getFileName(vFileext,vDirExt,(*(r[id])).type,id);
				switch ((*(r[id])).type) {
					case 1:
					case 5:
					case 0: //Raw files
						ok=ok&&writeData(data,1,vFileext,size); //Ignore checksum
						break;
					case 6: //save and remember palette file
						//This will remember the palette for the next extacted images
						if (1/*isntImageSet*/) {
							mLoadPalette(data,&image);
							isntImageSet=0;
						}
						ok=ok&&mFormatExtractPal(&data,vFileext,size);
						break;
					case 4:	//save midi file
						ok=ok&&mFormatExtractMid(data,vFileext,size);
						break;
					case 3: //save wav file
						ok=ok&&mFormatExtractWav(data,vFileext,size);
						break;
					case 2: //save image
						ok=ok&&mFormatExtractBmp(data,vFileext,size,image);
						//printf("sali\n");
						break;
				}
			}
			if (data!=NULL) free(data);
			//printf("libere data que no era nula\n");
		}
		fclose(fp);
		//printf("termine A %d\n",ok);
		return ok;
	} else {
		return -1; //file could not be open
	}
}
