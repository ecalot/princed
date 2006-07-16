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
plv.c: Princed Resources : PLV prince level files support
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 29 Nov 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2003-Nov-29)

 PLV file format:
  Defined: 28 Nov 2003

  Authors:
   Brendon James <roomshaker@princed.com.ar>
   Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2003-Nov-28)

 Note:
  DO NOT remove this copyright notice
*/

/* Includes */
#include "dat.h"
#include "disk.h"
#include "memory.h" /* freeBinary */
#include "plv.h"
#include <stdlib.h> /* malloc */
#include <string.h>
#include <time.h>

/* Private function to get the current date/time */
char* getDate() {
	/* Code taken from RDR 1.4.1a coded by Enrique Calot */

	/* Declare variables */
#ifdef PLV_FULL_FORMAT
	static char weeks   []   = DATE_WEEKDAYS;
	static char months  []   = DATE_MONTHS;
#endif
	static char formated [37];
	time_t      datet;
	struct tm*  date;

	/* get GMT time from the system clock */
	time(&datet);
	date=gmtime(&datet);

#ifdef PLV_FULL_FORMAT
	/* Format: "Tue, 26 Nov 2002 22:16:39" */
	sprintf(formated,"%s, %d %s %.4d %.2d:%.2d:%.2d",
		weeks+4*(date->tm_wday),
		date->tm_mday,
		months+4*(date->tm_mon),
		date->tm_year+1900,
		date->tm_hour,
		date->tm_min,
		date->tm_sec
	);
#else
	/* Format: "2002-11-26 22:16:39" */
	sprintf(formated,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
		date->tm_year+1900,
		date->tm_mon+1,
		date->tm_mday,
		date->tm_hour,
		date->tm_min,
		date->tm_sec
	);
#endif
	return formated;
}

int writePlv(const char* file, tBinary content, int popversion, const char* datfile, int level, const char* filename, const char* desc, const char* title, const char* vDatAuthor, int optionflag,const char* backupExtension) {
	/* PLV files are saved as raw except you must add the checksum and the plv constant file header and footer */

	/* Variables */
	char levelnum[10];
	char* now;
	const char* nullString="";
	const unsigned long int numberOfFieldPairs=9;
	FILE* target;
	int ok;
	static const char* author=PLV_DEFAULT_AUTHOR;
	unsigned char checksum;
	unsigned char sizeOfNow;
	unsigned char version=popversion;
	unsigned long int block2size;
	long contentSizeWithChecksum=content.size+1;

	/* Get current time */
	now=getDate();
	sizeOfNow=(unsigned char)(strlen(now)+1);

	/* Validate null strings when no description is set */
	if (desc==NULL) desc=nullString;
	if (title==NULL) title=nullString;
	if (vDatAuthor==NULL) vDatAuthor=author;
	sprintf(levelnum,"%d",level);

	/* Writing file */

	/* Safe open for writing mode */
	ok=writeOpen(file,&target,optionflag);

	/* Write headers */
	ok=ok&&fwrite(PLV_HEADER_MAGIC,PLV_HEADER_MAGIC_SIZE,1,target);
	ok=ok&&fwritechar(&version,target); /* POP version */
	version=1;
	ok=ok&&fwritechar(&version,target); /* PLV version */
	ok=ok&&fwritechar(&level,target);
	ok=ok&&fwritelong(&numberOfFieldPairs,target);
	ok=ok&&fwritelong(&contentSizeWithChecksum,target);

	/* Write block 1: checksum and raw data */
	checksum=getChecksum(content);
	ok=ok&&fwritechar(&checksum,target);
	ok=ok&&fwritebinary(content,target);

	/* Write footers */
	block2size=(
		sizeof(PLV_FOOT_EDITOR)+	     strlen(vDatAuthor)+1+
		sizeof(PLV_FOOT_TITLE)+	       strlen(title)+1+
		sizeof(PLV_FOOT_DESC)+	       strlen(desc)+1+
		sizeof(PLV_FOOT_TCREAT)+	     sizeOfNow+
		sizeof(PLV_FOOT_TMODIF)+	     sizeOfNow+
		sizeof(PLV_FOOT_ORIG_FILE)+	   strlen(filename)+1+
		sizeof(PLV_FOOT_LEV_NUM_ORIG)+ strlen(levelnum)+1
	);

	ok=ok&&fwritelong(&block2size,target);

	/* Write block 2 */
	ok=ok&&fwrite(PLV_FOOT_EDITOR,sizeof(PLV_FOOT_EDITOR),1,target);
	ok=ok&&fwrite(vDatAuthor,strlen(vDatAuthor)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_TITLE,sizeof(PLV_FOOT_TITLE),1,target);
	ok=ok&&fwrite(title,strlen(title)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_DESC,sizeof(PLV_FOOT_DESC),1,target);
	ok=ok&&fwrite(desc,strlen(desc)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_ORIG_FILE,sizeof(PLV_FOOT_ORIG_FILE),1,target);
	ok=ok&&fwrite(filename,strlen(filename)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_TCREAT,sizeof(PLV_FOOT_TCREAT),1,target);
	ok=ok&&fwrite(now,sizeOfNow,1,target);
	ok=ok&&fwrite(PLV_FOOT_TMODIF,sizeof(PLV_FOOT_TMODIF),1,target);
	ok=ok&&fwrite(now,sizeOfNow,1,target);
	ok=ok&&fwrite(PLV_FOOT_LEV_NUM_ORIG,sizeof(PLV_FOOT_LEV_NUM_ORIG),1,target);
	ok=ok&&fwrite(levelnum,strlen(levelnum)+1,1,target);

	/* Close file and return */
	ok=ok&&(!writeCloseOk(target,optionflag,backupExtension));
	return ok?PR_RESULT_SUCCESS:PR_RESULT_ERR_FILE_NOT_WRITE_ACCESS;
}

extern FILE* outputStream;

int readPlv(const char* file, tBinary* content, int *number, char** datfile, char** name, char** desc, char** datAuthor) {

/*
 * PLV 1 SPECS:
 * bytes offset description                     content
 *     7      0 HAS FILE TAG OF 8 LETTERS       "POP_LVL"
 *     1      7 POP VERS                        0x01
 *     1      8 PLV VERS                        0x01
 *     1      9 LEV NUM
 *     4     10 FIELD-PAIR ( NAME / CONTENT ) COUNT
 *     4     14 BLOCK 1: LEVEL SIZE (B1)        2306 (including the checksum)
 *    B1     18 BLOCK 1: LEVEL CODE
 *     4  18+B1 BLOCK 2: USER DATA SIZE VALUE IN BYTES (B2)
 *    B2  22+B1 BLOCK 2: LEVEL CODE NEXT, REST OF FILE
 *
 * Total size of file B1+B2+22.
 * All values are unsigned and in the Intel x86 architecture
 */
	
	char magic[PLV_HEADER_MAGIC_SIZE];
	unsigned char popVersion, plvVersion, levelNumber,checksum;
	long fieldPair,block1Size,block2Size;
	FILE* fd;
	int ok;

	content->isCopy=1;
	content->data=NULL;
	content->size=0;
	
	fd=fopen(file,"rb");
	if (!fd) return PR_RESULT_ERR_FILE_NOT_READ_ACCESS;

	/* Read headers */
	ok=fread(magic,PLV_HEADER_MAGIC_SIZE,1,fd);
	ok=ok&&!strncmp(magic,PLV_HEADER_MAGIC,PLV_HEADER_MAGIC_SIZE);
	ok=ok&&freadchar(&popVersion,fd);
	ok=ok&&freadchar(&plvVersion,fd);
	ok=ok&&freadchar(&levelNumber,fd);
	ok=ok&&freadlong(&fieldPair,fd);
	ok=ok&&freadlong(&block1Size,fd);
	ok=ok&&freadchar(&checksum,fd);
	block1Size--; /* drop the ckecksum */

	/* TODO: validate checksum */

	/* Check data */
	if (!ok) {
		fclose(fd);
		return PR_RESULT_ERR_FILE_NOT_READ_ACCESS; /* TODO: use a bad format code */
	}

	/* Read data */
	content->isCopy=0;
	content->size=block1Size;
	ok=(content->data=malloc(block1Size))?1:0;
	ok=ok&&fread(content->data,1,block1Size,fd);
	ok=ok&&freadlong(&block2Size,fd);
	/* TODO: finish reading this block */
	/* check: Total size of file B1+B2+22. */

	*number=levelNumber;
	*datfile=NULL;
	*name=NULL;
	*desc=NULL;
	*datAuthor=NULL;
	
	fclose(fd);

	/* Check data */
	if (!ok) {
		freeBinary(*content);
		content->isCopy=1;
		content->data=NULL;
		content->size=0;
		return PR_RESULT_ERR_FILE_NOT_READ_ACCESS; /* TODO: use a bad format code */
	}


#if 0
	/* declare variables */
	unsigned char* pos;
	unsigned char* posAux;
	unsigned long int oldSize=res->content.size;

	/* integrity check 1 */
	if (oldSize<=PLV_HEADER_MAGIC_SIZE+1+PLV_HEADER_B_SIZE) return 0; /* false */
	if (memcmp(res->content.data,PLV_HEADER_MAGIC,PLV_HEADER_MAGIC_SIZE)) return 0; /* false */

	/* jump to size */
	pos=res->content.data+PLV_HEADER_MAGIC_SIZE+7; /* TODO: check this */

	/* read size and jump to data */
	res->content.size=array2long(pos);pos+=4;

	/* integrity check 2 */
	if (oldSize<=PLV_HEADER_MAGIC_SIZE+1+PLV_HEADER_B_SIZE+res->content.size) return 0; /* false */

	/* validate checksum */
	if (!checkSum(pos,res->content.size))
		fprintf(outputStream,PR_TEXT_IMPORT_PLV_WARN);

	/* save data */
	posAux=res->content.data;
	res->content.data=pos;
	mWriteFileInDatFileIgnoreChecksum(res);
	res->content.data=posAux;
#endif

	return PR_RESULT_SUCCESS;
}

