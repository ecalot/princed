#ifndef _PR_H_
#define _PR_H_

//princed resource library

//User configuration defines
#ifndef WIN32
 #define UNIX
#endif
//#define DLL
#define PR_ABOUT "Princed resources (PR) V0.8 beta\r\n(c) Copyright 2003 - Princed Development Team\r\nhttp://www.princed.com.ar\r\n\r\n"

//Path defines
#ifdef UNIX
 #define DIR_SEPARATOR '/'
#else
 #define DIR_SEPARATOR '\\'
#endif

//Main functions
int prExportDat(char* vDatFile, char* vDirName, char* vResFile);
int prImportDat(char* vDatFile, char* vDirName, char* vResFile);
int prClearRes(char* vResFile);
int prVerifyDatType(char* vFiledat);

//Extra featured functions
int prExportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt);
int prImportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt);

#endif
