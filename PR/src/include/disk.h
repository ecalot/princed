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
disk.h: Princed Resources : Disk Access & File handling functions headers
������
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

#ifndef _DISK_H_
#define _DISK_H_

#include <stdio.h>

/* 64 Kb */
#define SIZE_OF_FILE     256*1024

typedef enum {eFile,eDirectory,eNotFound}whatIs;

#define charToUpper(a) ((a)&0xDF)
#define isDirSep(a,i) ((a[i]=='\\')||(a[i]=='/'))

typedef struct tOpenFiles {
	struct tOpenFiles* next;
	FILE* file;
	char* name;
	unsigned char* content;
	unsigned long int size;
}tOpenFiles;

char mDiskVealidateFileHeader(unsigned char* text, int size, FILE* fp);
/* char mSaveRaw      (const char* vFile,const unsigned char* output, int size); */
#define mSaveRaw(f,d,s,optionflag,backup) writeData(d,0,f,s,optionflag,backup)
#define writeCloseOk(a,b,c) writeClose(a,0,b,c)
#define writeCloseDelete(a,b,c) writeClose(a,1,b,c)
int  writeData     (const unsigned char* data, int ignoreChars, char* vFileext, int size, int optionflag,const char* backupExtension);
int  writeOpen     (const char* vFileext, FILE* *fp, int optionflag);
int  writeClose    (FILE* fp,int dontSave,int optionflag,const char* backupExtension);
int  mLoadFileArray(const char* vFile,unsigned char** array);
int  makebase      (const char* p);
const char* repairFolders(const char* a);
const char* getFileNameFromPath(const char* path);
int recurseDirectory(const char* path,int recursive, void* pass, void (*function)(const char*,void*));
whatIs isDir(const char *nombre);
int mCopy(const char* strSource, const char* strTarget);
#define mRemoveFile(a) remove(repairFolders(a))

/* array2vars*/

#define array2short(a) ((*(a)))+((*((a)+1))<<8)
#define array2long(a)  ((*(a)))+((*((a)+1))<<8)+((*((a)+2))<<16)+((*((a)+3))<<24)

#ifdef MACOS
#define freadshort(var,file)  macfreads ((var),file)
#define freadlong(var,file)   macfreadl ((var),file)
#define fwriteshort(var,file) macfwrites((var),file)
#define fwritelong(var,file)  macfwritel((var),file)
#define fwritechar(var,file)  fwrite((var),1,1,file)
int macfreads (void* bigEndian,FILE* file);
int macfreadl (void* var,FILE* file);
int macfwrites(const void* var,FILE* file);
int macfwritel(const void* var,FILE* file);
#else
#define freadshort(var,file)  fread ((var),2,1,file)
#define freadlong(var,file)   fread ((var),4,1,file)
#define fwriteshort(var,file) fwrite((var),2,1,file)
#define fwritelong(var,file)  fwrite((var),4,1,file)
#define fwritechar(var,file)  fwrite((var),1,1,file)
#endif

#endif

