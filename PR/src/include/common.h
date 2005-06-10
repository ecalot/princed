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
common.h: Princed Resources : Defines and prototypes common to all PR code
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

#ifndef _COMMON_H_
#define _COMMON_H_

/***************************************************************\
|                Princed Resource Library Functions             |
\***************************************************************/

#include <stdio.h>

/* User configuration defines */
#ifndef WIN32
 #define UNIX
#endif

#ifdef WIN32
#ifndef OS
#define OS "Win32"
#endif
#endif

/***************************************************************\
|             M A I N   P R O G R A M   D E F I N E S           |
\***************************************************************/

/***************************************************************\
|                     PR Specific Defines                       |
\***************************************************************/

#define PR_URL                    "http://www.princed.com.ar"
#define PR_VERSION                "v1.1-rc2"
#define PR_COPY                   "(c) Copyright 2003 - 2005 Princed Development Team"

/***************************************************************\
|                         Other defines                         |
\***************************************************************/

/* Default Files */
#define RES_XML_UNKNOWN_XML      "unknown.xml"
#define RES_XML_RESOURC_XML      "resources.xml"

#define DEFAULT_BACKUP_EXTENSION "bak"

/***************************************************************\
|                        L A N G U A G E                        |
\***************************************************************/

#include "en.lang.pr.h"

/* Credits */
#define PARSING_ABOUT PR_TXT_AUTHORS": \n\
   "PR_TXT_CODER"\n\
    Enrique Calot\n\
\n\
   "PR_TXT_COD_ASSIST"\n\
    Santiago Zamora\n\
\n\
   "PR_TXT_GFX_COMP"\n\
    Tammo Jan Dijkema\n\
    Enrique Calot\n\
    Diego Essaya\n\
\n\
   "PR_TXT_GFX_DEV"\n\
    Tammo Jan Dijkema\n\
    Anke Balderer\n\
\n\
   "PR_TXT_MID"\n\
    Christian Lundheim\n\
\n\
   "PR_TXT_XML"\n\
    Steven Fayers\n\
\n\
   "PR_TXT_TRANSLATION"\n\
    "PR_TXT_ABOUT_TRANSLATOR"\n\
\n"

/***************************************************************\
|                           Prototypes                          |
\***************************************************************/

#include "pr.h"

/***************************************************************\
|                   Command Line specific options               |
\***************************************************************/

#define PR_IGNORE_RAW_OPTION

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_OPTRAW ,{"raw",         no_argument,       0,'r'},
#define PARSING_CHRRAW "r"
#define PARSING_SHOWRAW(a) a
#else
#define PARSING_OPTRAW ,
#define PARSING_CHRRAW ""
#define PARSING_SHOWRAW(a) "" 
#endif
#define PARSING_OPTIONS {\
{"import",      optional_argument, 0,'m'},\
{"classify",    no_argument,       0,'c'},\
{"export",      optional_argument, 0,'x'},\
\
{"setauthor",   required_argument, 0,'a'},\
{"backup",      optional_argument, 0,'b'},\
{"force",       no_argument,       0,'f'},\
{"cgi",         no_argument,       0,'g'},\
{"help",        no_argument,       0,'?'},\
{"resource",    required_argument, 0,'s'},\
{"compression-level",    required_argument, 0,'z'}\
PARSING_OPTRAW\
{"recursive",   no_argument,       0,'R'},\
{"datfile",     required_argument, 0,'t'},\
{"unknown",     no_argument,       0,2},\
{"verbose",     no_argument,       0,'v'},\
{"version",     no_argument,       0,1},\
{0, 0, 0, 0}\
}

#define PARSING_CHARS    "z::i::m::cx::e::b::a::fgs::t::Rvh?"PARSING_CHRRAW

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

#define PR_RESULT_SUCCESS 0
#define PR_RESULT_ERR_EXTRACTION -1
#define PR_RESULT_ERR_FILE_DAT_NOTFOUND -2
#define PR_RESULT_ERR_FILE_DAT_NOTOPEN -3
#define PR_RESULT_ERR_FILE_DAT_NOTOPEN_W -3
#define PR_RESULT_ERR_FILE_NOTFOUND -4
#define PR_RESULT_ERR_FILE_NOTOPEN -5
#define PR_RESULT_ERR_INVALID_DAT -6
#define PR_RESULT_ERR_MEMORY -7
#define PR_RESULT_ERR_XML_ATTR -8
#define PR_RESULT_ERR_XML_FILE -9
#define PR_RESULT_ERR_XML_PARSING -10
#define PR_RESULT_TYPE_1BINARY 1
#define PR_RESULT_TYPE_1GRAPHIC_BAD 2
#define PR_RESULT_TYPE_1GRAPHIC_PAL 3
#define PR_RESULT_TYPE_1LEVEL 4
#define PR_RESULT_TYPE_1PCS 5
#define PR_RESULT_TYPE_1WAVE 6
#define PR_RESULT_TYPE_2BINARY 11
#define PR_RESULT_TYPE_2GRAPHIC_BAD 12
#define PR_RESULT_TYPE_2GRAPHIC_PAL 13
#define PR_RESULT_TYPE_2LEVEL 14
#define PR_RESULT_TYPE_2PCS 15
#define PR_RESULT_TYPE_2WAVE 16

#endif

