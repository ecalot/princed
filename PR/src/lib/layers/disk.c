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
#include "pr.h"
#include "disk.h"
#include "xmlparse.h" /* equalsIgnoreCase */

#include <sys/types.h>
#include <sys/stat.h>

#ifdef UNIX
	#define defmkdir(a) mkdir (a,(mode_t)0755)
	#include <dirent.h>
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
	#define osIndepGetCharacter() getchar()
#else
	#include <direct.h>
	#include "direntwin.h"
	#define defmkdir(a) mkdir (a)
	#include <conio.h>
	#define osIndepGetCharacter() getche()
#endif

extern FILE* outputStream;

/***************************************************************\
|              Disk Access & File handling functions            |
\***************************************************************/

/* Repair folders */
const char *repairFolders(const char* a) {
	int i,k;
	static char result[MAX_FILENAME_SIZE];


	for (i=0,k=0;a[i]&&(k<MAX_FILENAME_SIZE);) {
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

static tOpenFiles* openFilesList=NULL;

void addFileToOpenFilesList(const char* fileName,int hasBackup) {
	/*
		Add a recently safe open file to the file pointer dynamic table
		using the LIFO philosophy.
	*/

	tOpenFiles* newNode;


	/* Create the new node and fill in the fields */
	newNode=(tOpenFiles*)malloc(sizeof(tOpenFiles));
	newNode->next=openFilesList;
	newNode->name=strallocandcopy(fileName);

	if (hasBackup) {

		newNode->size=mLoadFileArray(fileName,&(newNode->content));

	} else {

		newNode->size=0;

	}
	openFilesList=newNode;
}

void addPointerToOpenFilesList(FILE* fp) { /* TODO: use a define */
	openFilesList->file=fp;
}

int getFromOpenFilesList(FILE* fp, char** fileName, unsigned char** content, unsigned long int *size) {
	tOpenFiles* currentNode;
	tOpenFiles* priorNode=NULL;

	/* Search using FILE* file as key */
	if (openFilesList==NULL) return 0; /* Empty list */
	currentNode=openFilesList;
	while ((currentNode->file!=fp)&&(currentNode->next!=NULL)) {
		priorNode=currentNode;
		currentNode=currentNode->next;
	}
	if (currentNode->file!=fp) return 0; /* Not found */

	/* Return results */
	*fileName=currentNode->name;
	*content=currentNode->content;
	*size=currentNode->size;

	/* free node and set prior pointer to the next */
	if (priorNode==NULL) {
		openFilesList=currentNode->next;
	} else {
		priorNode->next=currentNode->next;
	}
	free(currentNode);

	return 1;
}

int writeClose(FILE* fp,int dontSave,int optionflag,const char* backupExtension) {
	unsigned char* content;
	char* fileName;
	unsigned long int size;

	if (getFromOpenFilesList(fp,&fileName,&content,&size)) {
		if (dontSave) {
			fclose(fp);
			if (size) {
				fp=fopen(fileName,"wb");
				if (fp==NULL) return -1;
				fwrite(content,1,size,fp);
			} else {
				remove(fileName);
			}
		} else {
			/* File Existed before and we need to back it up */
			if (hasFlag(backup_flag)) {
				char aux[MAX_FILENAME_SIZE];
				static const char defaultbackupExtension[]=DEFAULT_BACKUP_EXTENSION;
				/* Set default values if there isn't */
				if (backupExtension==NULL) backupExtension=defaultbackupExtension;
				/* generate the file name */
				sprintf(aux,"%s.%s",fileName, backupExtension);
				fclose(fp);
				fp=fopen(aux,"wb");
				if (fp==NULL) return -2;
				fwrite(content,1,size,fp);
			}
		}

		free(fileName);
		if (size) free(content);
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
	static int all=0;
	int result;

#ifdef UNIX
#ifndef IGNORE_TERM_CHANGE
	/* This will eliminate the enter after the input */
	struct termios term;
	struct termios termOld;

	tcgetattr (STDIN_FILENO, &term);
	tcgetattr (STDIN_FILENO, &termOld); /* save original proprieties */
	term.c_lflag &= ~(ICANON);
	tcsetattr (STDIN_FILENO, TCSANOW, &term);
#endif
#endif


	/* Create base directory and save file */
	file=repairFolders(vFileext);

	/* Verify if file already exists. */
	fileType=isDir(vFileext);
	if (fileType==eDirectory) return 0;


	if (fileType==eFile) {
		/* File exists. We need to ask */
		if ((!(hasFlag(force_flag)))&&(!all)) {
			char answer;
			printf(PR_TEXT_DISK_REPLACE,getFileNameFromPath(file));
			answer=osIndepGetCharacter();
			printf("\n");
			if (charToUpper(answer)==PR_DISK_REPLACE_NO)  return 0;
			if (charToUpper(answer)==PR_DISK_REPLACE_ALL) all=1;
		}
	} else {
		makebase(file);
	}


#ifdef UNIX
#ifndef IGNORE_TERM_CHANGE
	/* restoring previous terminal options */
	term=termOld;
	tcsetattr (STDIN_FILENO, TCSANOW, &termOld);
#endif
#endif
	/*
		If the file exists, we need to remember the old content in memory
		if not, we need to know the name in case we need to delete it
	*/


	addFileToOpenFilesList(file,hasFlag(backup_flag));

	if ((result=((*fp=fopen(file,"wb"))!=NULL))) addPointerToOpenFilesList(*fp);

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
		It will alloc an extra NULL byte at the end of the array so
		the file may be treated as a string too.
	*/

	/* declare variables */
	FILE *fp;
	int  aux;

	/* Open the file */
	if ((fp=fopen(repairFolders(vFile),"rb"))==NULL) {
		return 0;
	} else {
		/* get file size */
		fseek(fp,0,SEEK_END);
		aux=ftell(fp);
		if ( !aux || (aux>SIZE_OF_FILE) || ( ((*array=(unsigned char*)malloc(aux+1))==NULL) ) ) {
			/* if the file was null or bigger than the max size or couldn't allocate the file in memory */
			fclose(fp);
			return 0;
		} else {
			/* if the file was successfully open */
			fseek(fp,0,SEEK_SET);
			(*array)[aux]=0;
			aux=fread (*array,1,aux,fp);
			fclose(fp);
			return aux;
		}
	}
}

#ifdef PR_FUTURE_CODE
int mDiskVealidateFileHeader(unsigned char* text, int size, FILE* fp) {
	/*
		Validates if the file contains the following text in the stream.
		1 if it does
		0 if error or doesn't

		Moves the file pointer to the next position
	*/

	/* Declare variables */
	int i;
	unsigned char* readText;

	/* Reserves memory to allocate the read bytes */
	readText=getMemory(size);
	if (readText==NULL) return 0; /* memory error, abort */

	/* Read the file and move the file pointer */
	if (!fread(readText,size,1,fp)) {
		free(readText);
		return 0;
	}

	/* Make the binary compare */
	for (i=0;(i<size)&&(readText[i]==text[i]);i++);

	/* Frees memory and returns the result */
	free(readText);
	return (i==size); /* 0 if the compare for was stopped before end reached */
}
#endif

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
	return (S_IFDIR&buf.st_mode)?eDirectory:eFile;
}

#ifndef IGNORERECURSIVEFUNCTIONS

int recurseDirectory(const char* path,int optionflag, const char* extension,const char* dirName,const char* resFile, const char* datfilename,const char* datAuthor,FILE* output) {
	/*
		Search for all .dat files in the directory
		if recursive flag is set search over the subdirectories
		if verbose flag is set shows some messages in the screen
		when .dat files are found it runs prMain form each of them
	*/

	/* Declare variables */
	char*          recursive;
	struct dirent* directoryStructure;
	DIR*           dir;

	/* Opens directory */
	if ((dir = opendir(path))==NULL) {
		return 0;
	}

	/* Shows some messages */
	if ((hasFlag(recursive_flag))&&(hasFlag(verbose_flag))) { /* Only recourse if recursive and verbose flags are set */
		fprintf(outputStream,PR_TEXT_DISK_PROCESSING,path);
	}

	/* Main loop: while there are still more files left */
	while ((directoryStructure = readdir(dir))!=NULL) {
		if /* Don't look over the system directories */
			(
				strcmp(directoryStructure->d_name,".")&&
				strcmp(directoryStructure->d_name,"..")
		) {
			/* Declare variables */
			int sizeOfPath=strlen(path);
			int sizeOfFile=strlen(directoryStructure->d_name);

			/* Generate recursive path */
			recursive=(char*)malloc(sizeOfPath+2+sizeOfFile);
			memcpy(recursive,path,sizeOfPath);
			recursive[sizeOfPath]=DIR_SEPARATOR;
			memcpy(recursive+sizeOfPath+1,directoryStructure->d_name,sizeOfFile+1);

			/*
				If recursive path is a directory and recursive flag is set recourse into it
				if recursive path is a directory and recursive flag wasn't set, just ignore
				if recursive path is not a directory and is a dat file, do prMain
				if recursive path is not a directory and is not a dat file, ignore
			*/
			if (isDir(recursive)==eDirectory) {
				if (hasFlag(recursive_flag)) { /* Only recurse if recursive flag is set */
					recurseDirectory(recursive,optionflag,extension,dirName,resFile,datfilename,datAuthor,output);
				}
			} else {
				if (sizeOfFile>4) {
					if (equalsIgnoreCase(".dat",directoryStructure->d_name+sizeOfFile-4)) {
						prMain(optionflag,extension,dirName,resFile,recursive,directoryStructure->d_name,datAuthor,output);
					}
				}
			}
			/* Free all allocated memory */
			free(recursive);
		}
	}
	closedir(dir);
	return 1;
}

#endif

