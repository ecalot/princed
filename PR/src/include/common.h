#ifndef _PR_H_
#define _PR_H_

//princed resource library

//User configuration defines
#ifndef WIN32
 #define UNIX
#endif

/* #define DLL --> use -DDLL as a precompiler option */

//Main program defines
#define PR_ABOUT             "Princed resources (PR) V0.9\r\n(c) Copyright 2003 - Princed Development Team\r\n"\
                             "http://www.princed.com.ar\r\n\r\n"

#define PR_CGI_TEXT1         "Content-Type:text/html\n\nRunning as a cgi\n"
#define PR_CGI_TEXT2         "Result: %02d type\r\n"

#define PR_TEXT_SKIPING      "Found invalid option '%c', skiping . . .\r\n"
#define PR_TEXT_RESULT       "Result: %s (%d)\r\n"
#define PR_TEXT_RESULT_ERR   "Result: %d files with errors\r\n"

#define PR_TEXT_SYNTAX       "Syntax:\r\n pr datfile option [extract dir]\r\n\r\nValid options:\r\n -x[rn] for extract\r\n  r: raw extraction\r\n  n: don't extract\r\n -c[r] for compile\r\n  r: raw compiling\r\n -d for type\r\n"

//Default Files
#define RES_XML_UNKNOWN_XML  "unknown.xml"
#define RES_XML_RESOURC_XML  "resources.xml"

//Difine max & min's
#define MAX_RES_COUNT 65000

//Path defines
#ifdef UNIX
 #define DIR_SEPARATOR       '/'
#else
 #define DIR_SEPARATOR       '\\'
#endif

//Main functions
int prExportDat(char* vDatFile, char* vDirName, char* vResFile);
int prImportDat(char* vDatFile, char* vDirName, char* vResFile);
int prVerifyDatType(char* vFiledat);

//Extra featured functions
int prExportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt);
int prImportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt);

#endif
