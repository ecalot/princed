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

//Includes
#include "plv.h"
#include "disk.h"
#include "compile.h"
#include "memory.h"
#include <string.h>
#include <time.h>

//Private function to get the currect date/time
char* getDate() {
	//Code taken from RDR 1.4.1a coded by Enrique Calot
	//TODO: define date string format for plv

	//Declare variables
	static char weeks   []   = DATE_WEEKDAYS;
	static char months  []   = DATE_MONTHS;
	static char formated [37];
	time_t      datet;
	struct tm*  date;

	time(&datet);
	date   = gmtime(&datet);

	//Format: "Tue, 26 Nov 2002 22:16:39 GMT"
	sprintf(formated,"%s, %d %s %.4d %.2d:%.2d:%.2d GMT",
		weeks+4*(date->tm_wday),
		date->tm_mday,
		months+4*(date->tm_mon),
		date->tm_year+1900,
		date->tm_hour,
		date->tm_min,
		date->tm_sec
	);
	return formated;
}


char mFormatExtractPlv(unsigned char* data, const char *vFileext,unsigned long int size,unsigned char level, const char* filename, const char* desc, const char* title, const char* vDatAuthor) {
	//Plv files are saved as raw except you must ignore the checksum and add the plv constant file header

	//Variables
	FILE* target;
	int ok;
	unsigned char sizeOfNow;
	char* now;
	const char* nullString="";
	static const char* author="PR user";

	//Get current time
	now=getDate();
	sizeOfNow=(unsigned char)(strlen(now)+1);

	//Ignore checksum
	size--;

	//Validate null strings when no description is set
	if (desc==NULL) desc=nullString;
	if (title==NULL) title=nullString;
	if (vDatAuthor==NULL) vDatAuthor=author;

	/* Writing file */

	//Safe open for writing mode
	ok=writeOpen(vFileext,&target);

	//Write headers
	ok=ok&&fwrite(PLV_HEADER_A,PLV_HEADER_A_SIZE,1,target);
	ok=ok&&fwrite(&level,1,1,target);
	ok=ok&&fwrite(PLV_HEADER_B,PLV_HEADER_B_SIZE,1,target);
	ok=ok&&fwrite(&size,1,sizeof(size),target);

	//Write raw data ignoring checksum
	ok=ok&&fwrite(data+1,size,1,target);

	//Write footers
	ok=ok&&fwrite(PLV_FOOT_EDITOR,sizeof(PLV_FOOT_EDITOR),1,target);
	ok=ok&&fwrite(vDatAuthor,strlen(vDatAuthor)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_TITLE,sizeof(PLV_FOOT_TITLE),1,target);
	ok=ok&&fwrite(title,strlen(title)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_DESC,sizeof(PLV_FOOT_DESC),1,target);
	ok=ok&&fwrite(desc,strlen(desc)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_TCREAT,sizeof(PLV_FOOT_TCREAT),1,target);
	ok=ok&&fwrite(now,sizeOfNow,1,target);
	ok=ok&&fwrite(PLV_FOOT_TMODIF,sizeof(PLV_FOOT_TMODIF),1,target);
	ok=ok&&fwrite(now,sizeOfNow,1,target);
	ok=ok&&fwrite(PLV_FOOT_ORIG_FILE,sizeof(PLV_FOOT_ORIG_FILE),1,target);
	ok=ok&&fwrite(filename,strlen(filename)+1,1,target);

	//Close file and return
	ok=ok&&(!fclose(target));
	return (char)ok;
}

char mFormatCompilePlv(unsigned char* data, FILE* fp, tResource *res) {

	unsigned long int size;
	//This is ignoring all kind of verifications and assuming (res->size)>PLV_HEADER_SIZE !!!
	//TODO: must verify the plv format version, if not 1, then PR is too old!

	//memcpy(&size,data+PLV_HEADER_SIZE_OFFSET,4); //Only when longs are 4 bytes
//	mAddFileToDatFile(fp,data+PLV_HEADER_A_SIZE+PLV_HEADER_B_SIZE+1+4,size);
//	free(file);
	return 1;
}
