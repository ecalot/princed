#ifndef _PR_H_
#define _PR_H_

//princed resource library

#include <stdio.h>

//User configuration defines
#ifndef WIN32
 #define UNIX
#endif

/* #define DLL --> use -DDLL as a precompiler option */
#ifdef DLL
 #ifdef UNIX
  #define SO
 #endif
#endif

#ifndef OS
 #ifndef UNIX
  #define OS "Win32"
 #else
  #define OS ""
 #endif
#endif

//Debug options
#define DEB_FLAG

#ifdef DEB_FLAG
 #define fld(a) printf(a);
#else
 #define fld(a)
#endif

//Main program defines
#define PR_ABOUT             "Princed resources (PR) V0.9 "OS"\r\n(c) Copyright 2003 - Princed Development Team\r\n"\
                             "http://www.princed.com.ar\r\n\r\n"

#define PR_CGI_TEXT1         "Content-Type:text/html\n\nRunning as a cgi\n"
#define PR_CGI_TEXT2         "Result: %02d type\r\n"

#define PR_TEXT_SKIPING      "Found invalid option '%c', skiping . . .\r\n"
#define PR_TEXT_RESULT       "Result: %s (%d)\r\n"
#define PR_TEXT_RESULT_ERR   "Result: %d files with errors\r\n"

#define PR_TEXT_SYNTAX       "Usage: pr [datFilePath] [-x [extractDir]|-c [compileDir]|-d] [-n filename]\r\n\r\nValid options:\r\n -x[rn] for extract\r\n  r: raw extraction\r\n  n: don't extract\r\n -c[r] for compile\r\n  r: raw compiling\r\n -d for type\r\n"

//Default Files
#define RES_XML_UNKNOWN_XML  "unknown.xml"
#define RES_XML_RESOURC_XML  "resources.xml"

//Define max & min's
#define MAX_RES_COUNT 65000

//Define max & min's
#define MAX_FILENAME_SIZE  260
#define MAX_EXTENSION_SIZE 10

#define DEFAULT_BACKUP_EXTENSION "bak"

//Path defines
#ifdef UNIX
 #define DIR_SEPARATOR       '/'
#else
 #define DIR_SEPARATOR       '\\'
#endif

//Main functions
int prExportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int prImportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int prVerifyDatType(const char* vFiledat);

//Extra featured functions
int prExportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char * vDatFileName,const char* datAuthor, const char* backupExtension);
int prImportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName, const char* backupExtension);

int prMain(int* option, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor,FILE* output);

//Command Line specific options

#define PARSING_HELP "Usage: \n"\
"  pr [-x [EXTRACTDIR]|-c [COMPILEDIR]|-d] [DATFILEPATH]\n"\
"  pr [OPTIONS] [DATFILEPATH]\n"\
"\n"\
"  Mandatory arguments to long options are mandatory for short options too.\n"\
"\n"\
"   -c, --import[=DIRNAME]     imports from DIRNAME into given dat file\n"\
"   -d, --classify             returns the DAT file type\n"\
"   -x, -e, --export[=DIRNAME] extracts given dat file into DIRNAME\n"\
"\n"\
"   -b, --backup[=EXTENSION]   backup your files\n"\
"   -f, --force                default option, you cannot disable it,\n"\
"                              so please make a backup of your files\n"\
"   -g, --cgi                  run as CGI and output mime headers\n"\
"   -h, --help                 display this help and exit\n"\
"   -m, --resource=RESFILE     uses an user-specific resource xml file\n"\
"   -r, --raw                  uses raw format\n"\
"   -R, --recursive            searches for all dat files (only if DATFILEPATH\n"\
"                              is not a dat file)\n"\
"   -t, --datfile=DATFILE      specifies a dat file to read resources\n"\
"                              different that the original file\n"\
"       --unknown              generate the unknown file without performing\n"\
"                              any extraction\n"\
"   -v, --verbose              explain what is being done\n"\
"       --version              output version information and exit\n"\
"\n"

//Add -a, --author for PLV format

#define PARSING_OPTIONS {\
			{"import",optional_argument,0,'c'},\
			{"classify",no_argument,0,'d'},\
			{"export",optional_argument,0,'x'},\
\
			{"backup",optional_argument,0,'b'},\
			{"force",no_argument,0,'f'},\
			{"cgi",no_argument,0,'g'},\
			{"help",no_argument,0,'?'},\
			{"resource",required_argument,0,'m'},\
			{"raw",no_argument,0,'r'},\
			{"recursive",no_argument,0,'R'},\
			{"datfile",required_argument,0,'t'},\
			{"unknown",no_argument,0,2},\
			{"verbose",no_argument,0,'v'},\
			{"version",no_argument,0,1},\
			{0, 0, 0, 0}\
		}

#define PARSING_CHARS "c::dx::e::b::fgm:t:rRv"

/* Flags */
#define import_flag      0x0001
#define classify_flag    0x0002
#define export_flag      0x0004
#define backup_flag      0x0008
#define force_flag       0x0010
#define cgi_flag         0x0020
#define help_flag        0x0040
#define first_flag       0x0080
#define raw_flag         0x0100
#define recursive_flag   0x0200
#define verbose_flag     0x0400
#define version_flag     0x0800
#define unknown_flag     0x1000
#define undef1_flag      0x2000
#define undef2_flag      0x4000
#define undef3_flag      0x8000

#define optionflag (*pOption)

#endif
