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
disk.c: Princed Resources : Disk Access & File handling functions
¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 29 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2003-Oct-29)

  Modified by: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.10 (2003-Dec-03)

 Note:
  DO NOT remove this copyright notice
*/

//Defines
#include "memory.h"
#include <string.h>
#include "pr.h"
#include "disk.h"
#ifdef UNIX
	#include <sys/types.h>
	#include <sys/stat.h>
	#define defmkdir(a) mkdir (a,(mode_t)0755)
#else
	#include <direct.h>
	#define defmkdir(a) mkdir (a)
#endif

#define isDirSep(a,i) ((a[i]=='\\')||(a[i]=='/'))

/***************************************************************\
|              Disk Access & File handling functions            |
\***************************************************************/

//Repair folders
void repairFolders(char* a) {
	int i,k;

	for (i=0,k=0;a[i];) {
		if (isDirSep(a,i)) {
			a[k]=DIR_SEPARATOR;
			i++;
			while (isDirSep(a,i)) i++;
		} else {
			a[k]=a[i];
			i++;
		}
		k++;
	}
	a[k]=0;
}


//Create base directory of a file
int makebase(const char* p) {
	/*
		Creates the base directory of the given file "p"
		Returns 0 if created
		Returns -1 if an error occurred

		Note: if the base dir already exists it will return -1!
	*/

	//Declare variables
	int i,a;
	int size;
	char* aux;

	//Initialize variables
	aux=(char*)malloc(size=(strlen(p)+1));
	memset(aux,0,size);

	//Make directories
	for (i=0;i<size;i++) {
		if (p[i]==DIR_SEPARATOR) a=defmkdir(aux);
		aux[i]=p[i];
	}
	free(aux);

	return a;
}

char writeOpen(char* vFileext, FILE* *fp) {
	/*
		Opens vFileext for write access
		 if the path does't exist it is created
		 if the file doesn't exist it is created
		 if the file does exist it is overwriten

		Sets the file pointer and returns 1 if Ok or 0 if error

		Returns
		 0 if error
		 1 if ok
	*/

	//Create base dir and save file
	repairFolders(vFileext);
	if (makebase(vFileext)) return 0;

	return ((*fp=fopen(vFileext,"wb"))!=NULL);
}


char writeData(const unsigned char* data, int ignoreChars, char* vFileext, int size) {
	/*
		Creates vFileext and saves data in it. In case the directory doesn't
		exist it will be created.

		Data is read from ignoreChars to size.
		Example:
			if data="123456789", ignoreChars=3, size=8
			saved file will contain "45678"

		Returns
		 0 if error
		 1 if ok
	*/

	//Declare variables
	FILE* target;
	char  ok;

	//Verify parameters
	size-=ignoreChars;
	if (size<=0) return 0;

	//Save file
	ok=writeOpen(vFileext,&target);
	ok=ok&&fwrite(data+ignoreChars,size,1,target);
	ok=ok&&(!fclose(target));
	return ok;
}

int mLoadFileArray(const char* vFile,unsigned char** array) {
	/*
		Using the string in vFile, it opens the file and returns the
		number of bytes	in it and the content of the file in array.
		In case the file couldn't be open or memory allocated returns 0.
	*/

	//declare variables
	FILE *fp;
	int  aux;

	//Open the file
	if ((fp=fopen(vFile,"rb"))==NULL) {
		return 0;
	} else {
		//get file size
		fseek(fp,0,SEEK_END);
		aux=ftell(fp);
		if ( !aux || (aux>SIZE_OF_FILE) || ( ((*array=(char*)malloc(sizeof(char)*aux))==NULL) ) ) {
			//if the file was null or bigger than the max size or couldn't allocate the file in memory
			fclose(fp);
			return 0;
		} else {
			//if the filewas succesfully open
			fseek(fp,0,SEEK_SET);
			aux=fread (*array,1,aux,fp);
			fclose(fp);
			return aux;
		}
	}
}

#define mSaveRaw(f,d,s) writeData(d,0,f,s)
/*
char mSaveRaw(const char* vFile,const unsigned char* output, int size) {
	/*
		Using the given string in vFile, it opens the file and saves the
		first "size" bytes from the "output" in it.
		In case the file couldn't be open or there was no size returns 0,
		otherways returns 1.
	/

  FILE * pFile;

	if (!size) return 0;
  if ((pFile=fopen(vFile,"wb"))==NULL) return 0;

  fwrite (output, 1, size, pFile);
  fclose (pFile);
  return 1;
}
*/

char mDiskVealidateFileHeader(unsigned char* text, int size, FILE* fp) {
	/*
		Validates if the file contains the following text in the stream.
		1 if it does
		0 if error or doesn't

		Moves the file pointer to the next position
	*/

	//Declare vars
	int i;
	unsigned char* readText;

	//Reserves memory to allocate the read bytes
	readText=getMemory(size);
	if (readText==NULL) return 0; //memory error, abort

	//Read the file and move the file pointer
	if (!fread(readText,size,1,fp)) {
		free(readText);
		return 0;
	}

	//Make the binary compare
	for (i=0;(i<size)&&(readText[i]==text[i]);i++);

	//Frees memory and returns the result
	free(readText);
	return (i==size); //0 if the compare for was stopped before end reached
}



