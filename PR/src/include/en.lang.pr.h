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
pr.h: Princed Resources : English language strings
¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 18 Feb 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2004-Feb-18)

  Language: English

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                           Text Defines                        |
\***************************************************************/

#define PR_ABOUT                  "Princed resources (PR) "PR_VERSION" "OS"\n"PR_COPY"\n"PR_URL"\n\n"

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nRunning as a cgi\n"
#define PR_CGI_TEXT2              "Result: %02d type\n"

#define PR_TEXT_RESULT            "Result: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Result: %d files with errors\n"

#define PR_TEXT_FILE_NOT_FOUND    "Result: File or directory '%s' not found\n"

#define PR_TEXT_SCANNING_CURRENT  "Scanning dat files in current directory\n"
#define PR_TEXT_SCANNING_GIVEN    "Scanning dat files in given directory\n"

#define PR_TEXT_IMPORTING_CURRENT "Importing all valid dat files from the current directory\n"
#define PR_TEXT_IMPORTING_GIVEN   "Importing all valid files from given directory\n"

#define PR_TEXT_TASK_COMPILE      "Importing '%s' from '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "Classifying '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Exporting '%s' to '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "Processing '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "File %s already exists. Replace? [Yes/no/all]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' not open, skipped\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' successfully imported\n"
#define PR_TEXT_IMPORT_ERRORS     "'%s' has errors, skipped\n"
#define PR_TEXT_IMPORT_DONE       "Importing done, %d files imported ok, %d files with error\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' successfully exported\n"
#define PR_TEXT_EXPORT_ERROR      "'%s' has errors, aborting...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Warning: Extracted file may be corrupt\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Ok",\
"Error accessing a file",\
"Memory error in extraction",\
"Invalid DAT file",\
"XML Parsing error",\
"Memory error in parsing",\
"XML Attribute not recognized",\
"XML File not found"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Memory error",\
"File not found or no access error",\
"Not a valid DAT file",\
"Levels file",\
"Graphic DAT file without palette",\
"Waves/Digital sound file",\
"Midis file",\
"Valid DAT file with undefined content",\
"Graphic DAT file",\
"Internal PC Speaker dat file",\
"","","",\
"Pop2 dat file"}

#define PR_TEXT_IMPORT_ARRAY {\
"File successfully imported",\
"DAT file not found or invalid in partial importation",\
"DAT File couldn't be open",\
"XML Parseing error",\
"No memory",\
"XML Attribute not recognized",\
"XML File not found"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  uses raw format\n"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Usage: \n\
  pr [-x[EXPORTDIR]|-i[IMPORTDIR]|-d] [DATFILEPATH]\n\
  pr [OPTIONS] [DATFILEPATH]\n\
\n\
  Mandatory arguments to long options are mandatory for short options too.\n\
\n"
#define PARSING_HELP_PART1 "\
   -i, -c, --import[=DIRNAME] imports from DIRNAME into given dat file\n\
   -d, --classify             returns the dat file type\n\
   -x, -e, --export[=DIRNAME] exports given dat file into DIRNAME\n\
\n"
#define PARSING_HELP_PART2 "\
   -a, --setauthor=NAME       sets your name in extracted plv files\n\
   -b, --backup[=EXTENSION]   backup your files\n\
   -f, --force                rewrites your files without prompting\n\
   -g, --cgi                  run as CGI and output mime headers\n\
   -h, -?, --help             display this help and exit\n\
   -s, --resource=RESFILE     uses an user-specific resource xml file\n\
"PARSING_RAW

#define PARSING_HELP_PART3 "\
   -R, --recursive            searches for all dat files (only if DATFILEPATH\n\
                              is not a dat file)\n\
   -t, --datfile=DATFILE      specifies a dat file to read resources\n\
                              different than the original file\n\
       --unknown              generate the unknown file without performing\n\
                              any extraction\n\
   -v, --verbose              explain what is being done\n\
       --version              output version information and exit\n\
\n"

#define PR_TXT_AUTHORS           "Authors"
#define PR_TXT_CODER             "Coding & main routines"
#define PR_TXT_COD_ASSIST        "Coding assistant"
#define PR_TXT_GFX_COMP          "Graphic compression algorithms"
#define PR_TXT_GFX_DEV           "Graphic format development"
#define PR_TXT_MID               "MID Sound format development"
#define PR_TXT_XML               "Resources.xml edition"
#define PR_TXT_TRANSLATION       "Translation"
#define PR_TXT_ABOUT_TRANSLATOR  "<transwi text='Please add your name here for the credits, use different lines to add more names.'>\n"

