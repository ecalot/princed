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
  Modified by: Santiago Zamora <drkzight@users.sourceforge.net>
  Version: 1.20 (2004-Jan-06)

 Note:
  DO NOT remove this copyright notice
*/

/* Defines */
#include "memory.h"
#include <string.h>
#include "disk.h"
#define IGNORERECURSIVEFUNCTIONS
#ifndef NOLINUX
#define UNIX
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include "resources.h"

#ifndef WIN32
  #define defmkdir(a) mkdir (a,(mode_t)0755)
#else
	#include <direct.h> 
  #define defmkdir(a) mkdir (a)
#endif

/***************************************************************\
|              Disk Access & File handling functions            |
\***************************************************************/

/* Repair folders */
const char *repairFolders(const char* a) {
	int i,k;
	static char result[MAX_FILENAME_SIZE];

	for (i=0,k=0;(k<MAX_FILENAME_SIZE)&&a[i];) {
		if (isDirSep(a,i)) {
			result[k]=DIR_SEPARATOR;
			i++;
			while (isDirSep(a,i)) i++;
		} else {
			result[k]=a[i];
			i++;
		}
		k++;
	}
	result[k]=0;
	return result;
}


/* Create base directory of a file */
int makebase(const char* p) {
	/*
		Creates the base directory of the given file "p"
		Returns 0 if created
		Returns -1 if an error occurred

		Note: if the base directory already exists it will return -1!
	*/

	/* Declare variables */
	static char old[MAX_FILENAME_SIZE];
	int i,a,equal=1;
	int size;
	char* aux;

	/* Initialize variables */
	size=(strlen(p)+1);
	aux=(char*)malloc(size);

	/* Make directories */
	for (i=0;i<size;i++) {
		aux[i]=0;
		equal=equal&&(old[i]==p[i]);
		if ((!equal)&&(p[i]==DIR_SEPARATOR)) a=defmkdir(aux);
		old[i]=p[i];
		aux[i]=p[i];
	}

	free(aux);
	return a;
}

int writeClose(FILE* fp,int dontSave,int optionflag,const char* backupExtension) {
	unsigned long int size=0;

		if (dontSave) {
			fclose(fp);
			if (size) {
				fp=fopen(/*fileName*/"/dev/null","wb");
				if (fp==NULL) return -1;
			}
	}

	return fclose(fp);
}

int writeOpen(const char* vFileext, FILE* *fp, int optionflag) {
	/*
		Opens vFileext for write access
		 if the path doesn't exist it is created
		 if the file doesn't exist it is created
		 if the file does exist it is overwritten

		Sets the file pointer and returns 1 if Ok or 0 if error

		Returns
		 0 if error
		 1 if ok
	*/
	const char* file;
	whatIs fileType;
	int result;

	/* Create base directory and save file */
	file=repairFolders(vFileext);

	/* Verify if file already exists. */
	fileType=isDir(vFileext);
	if (fileType==eDirectory) return 0;

	if (fileType==eFile) {
		/* File exists. We need to ask */
	} else {
		makebase(file);
	}

	/*
		If the file exists, we need to remember the old content in memory
		if not, we need to know the name in case we need to delete it
	*/

/*	addFileToOpenFilesList(file,hasFlag(backup_flag));*/
	result=((*fp=fopen(file,"wb"))!=NULL);/* addPointerToOpenFilesList(*fp);*/
	return result;
}


int writeData(const unsigned char* data, int ignoreChars, char* vFileext, int size, int optionflag,const char* backupExtension) {
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

	/* Declare variables */
	FILE* target;
	char  ok;

	/* Verify parameters */
	size-=ignoreChars;
	if (size<0) return 0;
	/*if (size==0) return 1; * Wrote 0 bytes */

	/* Save file */
	ok=writeOpen(vFileext,&target,optionflag);
	ok=ok&&((!size)||fwrite(data+ignoreChars,size,1,target));
	ok=ok&&(!writeCloseOk(target,optionflag,backupExtension));
	return ok;
}

int mLoadFileArray(const char* vFile,unsigned char** array) {
	/*
		Using the string in vFile, it opens the file and returns the
		number of bytes	in it and the content of the file in array.
		In case the file couldn't be open or memory allocated returns 0.
	*/

	/* declare variables */
	FILE *fp;
	int  aux;

	/* Open the file */
	if ((fp=fopen(repairFolders(vFile),"rb"))==NULL) {
		fprintf(stderr, "mLoadFileArray: Unable to open file\n");
		return 0;
	} else {
		/* get file size */
		fseek(fp,0,SEEK_END);
		aux=ftell(fp);
		if ( !aux || (aux>SIZE_OF_FILE) || ( ((*array=(unsigned char*)malloc(sizeof(char)*aux))==NULL) ) ) {
			/* if the file was null or bigger than the max size or couldn't allocate the file in memory */
			fclose(fp);
			fprintf(stderr, "mLoadFileArray: Wrong size\n");
			return 0;
		} else {
			/* if the file was successfully open */
			fseek(fp,0,SEEK_SET);
			aux=fread (*array,1,aux,fp);
			fclose(fp);
			return aux;
		}
	}
}

const char* getFileNameFromPath(const char* path) {
	/*
		If you give a path you get the filename,
		if you give a filename, you get the same filename
	*/
	int size;
	size=strlen(path);
	while (size) {
		if (isDirSep(path,size)) {
			return path+size+1;
		}
		size--;
	}
	return path;
}

whatIs isDir(const char *path) {
	/*
		eDirectory if path is a directory
		eNotFound if path isn't a directory or doesn't exist
		eFile if it is a file
	*/
	struct stat buf;

	if(stat(path,&buf)==-1) return eNotFound;
	return (S_ISDIR(buf.st_mode))?eDirectory:eFile;
}

