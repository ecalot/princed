
#ifndef _DISK_H_
#define _DISK_H_

#include <stdio.h>

/* 64 Kb */
#define SIZE_OF_FILE    64*1024

char mDiskVealidateFileHeader(unsigned char* text, int size, FILE* fp);
char mSaveRaw      (const char* vFile,const unsigned char* output, int size);
char writeData     (const unsigned char* data, int ignoreChars, char* vFileext, int size);
char writeOpen     (const char* vFileext, FILE* *fp);
int  mLoadFileArray(const char* vFile,unsigned char** array);
int  makebase      (const char* p);
const char *repairFolders(const char* a);
const char* getFileNameFromPath(const char* path);
#endif
