
#ifndef _DISK_H_
#define _DISK_H_

#define SIZE_OF_FILE    64*1024

void repairFolders(char* a);
char writeData(const unsigned char* data, int ignoreChars, char* vFileext, int size);
int makebase(const char* p);
int mLoadFileArray(const char* vFile,unsigned char** array);
char mSaveRaw(const char* vFile,const unsigned char* output, int size);

#endif
