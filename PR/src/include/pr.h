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

  Language: Abstract

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_H_
#define _PR_H_

/***************************************************************\
|                Princed Resource Library Functions             |
\***************************************************************/

#include <stdio.h>

/* User configuration defines */
#ifndef WIN32
 #define UNIX
#endif

/* do not #define DLL, use -DDLL as a precompiler option instead */
#ifdef DLL
void prSetOutput(FILE* output);
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

/* Debug options */
/* #define DEB_FLAG */
/* #define MEM_CHECK */

#ifdef MEM_CHECK

#ifdef malloc
#undef malloc
#endif

#include "memory.h"
 #define malloc(a) mymalloc(a,__FILE__,__LINE__)
 #define free(a) myfree(a,__FILE__,__LINE__)

#endif

#ifdef DEB_FLAG
 #define fld(a) printf(a "\n")
#else
 #define fld(a)
#endif

/***************************************************************\
|             M A I N   P R O G R A M   D E F I N E S           |
\***************************************************************/

/***************************************************************\
|                     PR Specific Defines                       |
\***************************************************************/

#define PR_URL                    "http://www.princed.com.ar"
#define PR_VERSION                "v1.0b-dev1"
#define PR_COPY                   "(c) Copyright 2003, 2004 - Princed Development Team"
/***************************************************************\
|                         Other defines                         |
\***************************************************************/

/* Default Files */
#define RES_XML_UNKNOWN_XML      "unknown.xml"
#define RES_XML_RESOURC_XML      "resources.xml"

#define DEFAULT_BACKUP_EXTENSION "bak"

/* Define max & min's */
#define MAX_RES_COUNT            25000
#define MAX_FILENAME_SIZE        260

/* Path defines */
#ifdef UNIX
 #define DIR_SEPARATOR           '/'
#else
 #define DIR_SEPARATOR           '\\'
#endif

/***************************************************************\
|                        L A N G U A G E                        |
\***************************************************************/

#include "english.h"

/***************************************************************\
|                           Prototypes                          |
\***************************************************************/

/* Main functions */
int prExportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int prImportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int prVerifyDatType(const char* vFiledat);

/* Extra featured functions */
int prExportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char * vDatFileName,const char* datAuthor, const char* backupExtension);
int prImportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName, const char* backupExtension);

/* Recursive function */
int prMain(int option, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor,FILE* output);

/***************************************************************\
|                   Command Line specific options               |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_OPTRAW ,{"raw",         no_argument,       0,'r'},
#define PARSING_CHRRAW "r"
#else
#define PARSING_OPTRAW ,
#define PARSING_CHRRAW ""
#endif
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
{"resource",    required_argument, 0,'s'}\
PARSING_OPTRAW\
{"recursive",   no_argument,       0,'R'},\
{"datfile",     required_argument, 0,'t'},\
{"unknown",     no_argument,       0,2},\
{"verbose",     no_argument,       0,'v'},\
{"version",     no_argument,       0,1},\
{0, 0, 0, 0}\
}

#define PARSING_CHARS    "i::c::dx::e::b::a::fgs::t::Rvh?"PARSING_CHRRAW

/* Flags */
#define import_flag      0x0001
#define classify_flag    0x0002
#define export_flag      0x0004
#define backup_flag      0x0008
#define force_flag       0x0010
#define cgi_flag         0x0020
#define help_flag        0x0040
#define raw_flag         0x0080
#define recursive_flag   0x0100
#define verbose_flag     0x0200
#define version_flag     0x0400
#define unknown_flag     0x0800
#define undef1_flag      0x1000
#define undef2_flag      0x2000
#define undef3_flag      0x4000
#define undef4_flag      0x8000

#define hasFlag(a) (optionflag&(a))
#define setFlag(a) optionflag|=(a)

#endif
