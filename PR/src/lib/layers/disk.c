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
#include "xml.h" /* equalsIgnoreCase */

#include <sys/types.h>
#include <sys/stat.h>

#ifdef UNIX
	#define defmkdir(a) mkdir (a,(mode_t)0755)
	#include <dirent.h>
#else
	#include <direct.h>
	#include "direntwin.h"
	#define defmkdir(a) mkdir (a)
#endif

#define isDirSep(a,i) ((a[i]=='\\')||(a[i]=='/'))

/***************************************************************\
|              Disk Access & File handling functions            |
\***************************************************************/

//Repair folders
const char *repairFolders(const char* a) {
	int i,k;
	static char result[260];

	for (i=0,k=0;a[i]&&(k<260);) {
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

char writeOpen(const char* vFileext, FILE* *fp) {
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
	const char* file;

	//Create base dir and save file
	file=repairFolders(vFileext);
	makebase(file);

	return ((*fp=fopen(file,"wb"))!=NULL);
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
	if ((fp=fopen(repairFolders(vFile),"rb"))==NULL) {
		return 0;
	} else {
		//get file size
		fseek(fp,0,SEEK_END);
		aux=ftell(fp);
		if ( !aux || (aux>SIZE_OF_FILE) || ( ((*array=(unsigned char*)malloc(sizeof(char)*aux))==NULL) ) ) {
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
  if (!writeOpen(vFile,&pFile)) return 0;

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
		1 if nombre is a directory
		0 if nombre isn't a directory or doesn't exist
	*/
	struct stat buf;

	if(stat(path,&buf)==-1) return eNotFound;
	return (S_IFDIR&buf.st_mode)?eDirectory:eFile;
}

int recurseDirectory(const char* path,int* pOption, const char* extension,const char* dirName,const char* resFile, const char* datfilename,const char* datAuthor,FILE* output) {
	/*
		Searchs for all .dat files in the directory
		if recursive flag is set searchs over the dubdirectories
		if verbose flag is set shows some messages in the screen
		when .dat files are found it runs prMain form each of them
	*/

	//Declare variables
	char*          recursive;
	struct dirent* directoryStructure;
	DIR*           dir;

	//Opens directory
	if ((dir = opendir(path))==NULL) {
		return 0;
	}

	//Shows some messages
	if ((optionflag&recursive_flag)&&(optionflag&verbose_flag)) { //Only recurse if recursive and verbose flags are set
		printf("Processing '%s'...\n",path);
	}

	//Main cicle: while there are still more files left
	while ((directoryStructure = readdir(dir))!=NULL) {
		if /* Don't look over the system directories */
			(!(
				!strcmp(directoryStructure->d_name,".")||
				!strcmp(directoryStructure->d_name,"..")
		)) {
			//Declare variables
			int sizeOfPath=strlen(path);
			int sizeOfFile=strlen(directoryStructure->d_name);

			//Generate recursive path
			recursive=getMemory(sizeOfPath+1+sizeOfFile);
			memcpy(recursive,path,sizeOfPath);
			recursive[sizeOfPath]=DIR_SEPARATOR;
			memcpy(recursive+sizeOfPath+1,directoryStructure->d_name,sizeOfFile+1);

			/*
				If recursive path is a directory and recursive flag is set recurse into it
				if recursive path is a directory and recursive flag wasn't set, just ignore
				if recursive path is not a directory and is a dat file, do prMain
				if recursive path is not a directory and is not a dat file, ignore
			*/
			if (isDir(recursive)==eDirectory) {
				if (optionflag&recursive_flag) { //Only recurse if recursive flag is set
					recurseDirectory(recursive,pOption,extension,dirName,resFile,datfilename,datAuthor,output);
				}
			} else {
				char aux[]=".dat";
				if (sizeOfFile>4) {
					if (equalsIgnoreCase(aux,directoryStructure->d_name+sizeOfFile-4)) {
						prMain(pOption,extension,dirName,resFile,recursive,directoryStructure->d_name,datAuthor,output);
					}
				}
			}
			//Free all allocated mem
			free(recursive);
		}
	}
	return 1;
}
