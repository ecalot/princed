#ifndef _PR_H_
#define _PR_H_

//princed resource library

#ifdef UNIX
 #define DIR_SEPARATOR '/'
#else
 #define DIR_SEPARATOR '\\'
#endif

#include <string.h>
#include "extract.h"
#include "compile.h"
#include "tasks.h"

//Main functions
int prExportDat(char* vDatFile, char* vDirName, char* vResFile);
int prImportDat(char* vDatFile, char* vDirName, char* vResFile);
int prClearRes(char* vResFile);
int prVerifyDatType(char* vFiledat);

//Extra featured functions
int prExportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt);
int prImportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt);

#endif
