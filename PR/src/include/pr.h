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
pr.h: Princed Resources : Main header prototypes and definitions
¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2003-Aug-24)

  Modified by: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.10 (2003-Dec-03)
  
  Language: English

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_H_
#define _PR_H_

/***************************************************************\
|                Princed Resource Library Functions             |
\***************************************************************/

#include <stdio.h>

//User configuration defines
#ifndef WIN32
 #define UNIX
#endif

/* do not #define DLL, use -DDLL as a precompiler option instead */
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
 #define fld(a) printf(a "\n");
#else
 #define fld(a)
#endif

/***************************************************************\
|             M A I N   P R O G R A M   D E F I N E S           |
\***************************************************************/


/***************************************************************\
|                           Text Defines                        |
\***************************************************************/


#define PR_ABOUT                  "Princed resources (PR) V0.9 "OS"\n(c) Copyright 2003 - Princed Development Team\n\
http://www.princed.com.ar\n\n"

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nRunning as a cgi\n"
#define PR_CGI_TEXT2              "Result: %02d type\n"

#define PR_TEXT_SKIPING           "Found invalid option '%c', skiping . . .\n"
#define PR_TEXT_RESULT            "Result: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Result: %d files with errors\n"

#define PR_TEXT_FILE_NOT_FOUND    "Result: File or directory '%s' not found\n"

#define PR_TEXT_SCANNING_CURRENT  "Scanning dat files in current directory\n"
#define PR_TEXT_SCANNING_GIVEN    "Scanning dat files in given directory\n"

#define PR_TEXT_IMPORTING_CURRENT "Importing all valid dat files from the currect directory\n"
#define PR_TEXT_IMPORTING_GIVEN   "Importing all valid files from given directory\n"

#define PR_TEXT_TASK_COMPILE      "Compiling '%s' from '%s' with %04x\n"
#define PR_TEXT_TASK_CLASSIFY     "Classifing '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Extracting '%s' to '%s' with %04x\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Ok",\
"Error accessing the file DAT", /* DAT or extracted */\
"Memory error in extraction",\
"Invalid DAT file",\
"XML Parse error",\
"Memory error in parsing",\
"XML Attribute not recognized",\
"XML File not found"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Memory error",\
"File not found or no access error",\
"Not a valid POP1 DAT file",\
"Levels file",\
"Graphic file with an image in the first valid entry (not common)",\
"Waves/Digital sound file",\
"Midis file",\
"Valid DAT file with Undefined content",\
"Graphic file with a palette in the first valid entry (common)",\
"PC Speaker dat file",\
"","","",\
"Pop2 dat files"}

#define PR_TEXT_IMPORT_ARRAY {\
"File succesfully compiled",\
"DAT File couldn't be open for writing",\
"XML Parse error",\
"No memory",\
"XML Attribute not recognized",\
"XML File not found"}

/***************************************************************\
|                         Other defines                         |
\***************************************************************/


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

/***************************************************************\
|                            Prototypes                         |
\***************************************************************/

//Main functions
int prExportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int prImportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int prVerifyDatType(const char* vFiledat);

//Extra featured functions
int prExportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char * vDatFileName,const char* datAuthor, const char* backupExtension);
int prImportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName, const char* backupExtension);

//Recursive function
int prMain(int* option, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor,FILE* output);

/***************************************************************\
|                   Command Line specific options               |
\***************************************************************/

#define PARSING_HELP "Usage: \n\
  pr [-x[EXTRACTDIR]|-c[COMPILEDIR]|-d] [DATFILEPATH]\n\
  pr [OPTIONS] [DATFILEPATH]\n\
\n\
  Mandatory arguments to long options are mandatory for short options too.\n\
\n\
   -c, --import[=DIRNAME]     imports from DIRNAME into given dat file\n\
   -d, --classify             returns the DAT file type\n\
   -x, -e, --export[=DIRNAME] extracts given dat file into DIRNAME\n\
\n\
   -a, --setauthor=NAME       sets your name in extracted PLV files\n\
   -b, --backup[=EXTENSION]   backup your files\n\
   -f, --force                default option, you cannot disable it,\n\
                              so please make a backup of your files\n\
   -g, --cgi                  run as CGI and output mime headers\n\
   -h, -?, --help             display this help and exit\n\
   -m, --resource=RESFILE     uses an user-specific resource xml file\n\
   -r, --raw                  uses raw format\n\
   -R, --recursive            searches for all dat files (only if DATFILEPATH\n\
                              is not a dat file)\n\
   -t, --datfile=DATFILE      specifies a dat file to read resources\n\
                              different that the original file\n\
       --unknown              generate the unknown file without performing\n\
                              any extraction\n\
   -v, --verbose              explain what is being done\n\
       --version              output version information and exit\n\
\n"

#define PARSING_ABOUT "Authors: \n\
   Coding & main routines\n\
    Enrique Calot\n\
\n\
   Graphic compression algorithms\n\
    Tammo Jan Dijkema\n\
    Enrique Calot\n\
\n\
   Graphic format development\n\
    Tammo Jan Dijkema\n\
    Anke Balderer\n\
\n\
   MID Sound format development\n\
    Christian Lundheim\n\
\n\
   Resources.xml edition\n\
    Steven Fayers\n\
\n"

#define PARSING_OPTIONS {\
{"import",      optional_argument, 0,'c'},\
{"classify",    no_argument,       0,'d'},\
{"export",      optional_argument, 0,'x'},\
\
{"setauthor",   required_argument, 0,'a'},\
{"backup",      optional_argument, 0,'b'},\
{"force",       no_argument,       0,'f'},\
{"cgi",         no_argument,       0,'g'},\
{"help",        no_argument,       0,'?'},\
{"resource",    required_argument, 0,'m'},\
{"raw",         no_argument,       0,'r'},\
{"recursive",   no_argument,       0,'R'},\
{"datfile",     required_argument, 0,'t'},\
{"unknown",     no_argument,       0,2},\
{"verbose",     no_argument,       0,'v'},\
{"version",     no_argument,       0,1},\
{0, 0, 0, 0}\
}

#define PARSING_CHARS    "c::dx::e::b::a:fgm:t:rRv"

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
