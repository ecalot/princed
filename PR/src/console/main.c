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
main.c: PR console program parsing routine
¯¯¯¯

 Note:
  DO NOT remove this copyright notice
*/

#ifdef UNIX
 #include <unistd.h>
 #ifndef LINUX
  #include "getopt.h"
 #endif
#else
 #include "getopt.h"
#endif

#include "common.h"
#include <string.h>
#include "memory.h"
#include "filedir.h"
#include "disk.h" /* MAX_FILENAME_SIZE */
#include "compress.h" /* setCompressionLevel */
#include "parse.h" /* free parsed cache */

FILE* outputStream;

void syntax() {
	fprintf(outputStream,PARSING_HELP_BEGIN);
	fprintf(outputStream,PARSING_HELP_PART1);
	fprintf(outputStream,PARSING_HELP_PART2);
	fprintf(outputStream,PARSING_HELP_PART3);
	fprintf(outputStream,PARSING_HELP_PART4);
}

/***************************************************************\
|      Standard executable command line parsing function        |
\***************************************************************/

int main (int argc, char **argv) {
	/* declare variables */
	char  dirName[MAX_FILENAME_SIZE]=".";
	char* datAuthor        =NULL;
	char* datFileName      =NULL;
	char* datfile          =NULL;
	char* extension        =NULL;
	char* format           =NULL;
	char* resFile          =NULL;
	char* file;
	char* errors[]         =PR_TEXT_ERRORS;
	char* types[]          =PR_TEXT_TYPES;
	int   c,result;
	int   optionflag=0;

	/* Parse command line options */
	do {
		static struct option long_options[] = PARSING_OPTIONS;
		int junk = 0;

		c = getopt_long(argc,argv,PARSING_CHARS,long_options,&junk);
		switch (c) {
				case 'm':
				case 'i':
					if (hasFlag(classify_flag|export_flag)) setFlag(help_flag);
					setFlag(import_flag);
					if (optarg) strncpy(dirName,optarg,MAX_FILENAME_SIZE);
					break;
				case 'g':
					setFlag(cgi_flag); /* if cgi, a classify must be performed */
				case 'c':
					if (hasFlag(import_flag|export_flag)) setFlag(help_flag);
					setFlag(classify_flag);
					break;
				case 'x':
				case 'e':
					if (hasFlag(classify_flag|import_flag)) setFlag(help_flag);
					setFlag(export_flag);
					if (optarg) strncpy(dirName,optarg,MAX_FILENAME_SIZE);
					break;
				case 'b':
					setFlag(backup_flag);
					freeAllocation(extension);
					extension=strallocandcopy(optarg);
					break;
				case 'F':
					freeAllocation(format);
					format=strallocandcopy(optarg);
					break;
				case 'f':
					setFlag(force_flag);
					break;
				case 's':
					freeAllocation(resFile);
					resFile=strallocandcopy(optarg);
					break;
				case 'z':
					if (optarg) setCompressionLevel(atoi(optarg));
					break;
				case 'r':
#ifndef PR_IGNORE_RAW_OPTION
					setFlag(raw_flag);
					break;
#endif
				case 'R':
					setFlag(recursive_flag);
					break;
				case 't':
					freeAllocation(datFileName);
					datFileName=strallocandcopy(optarg);
					break;
				case 'a':
					freeAllocation(datAuthor);
					datAuthor=strallocandcopy(optarg);
					break;
				case 'v':
					setFlag(verbose_flag);
					break;
				case 2:
					setFlag(unknown_flag);
				case -1:
					break;
				case 1:
					setFlag(version_flag);
				default:
					setFlag(help_flag);
		}
	} while (c!=-1);

	outputStream=stdout;
	c=0;

	/* At least one of these options must be selected, if not, the user needs help! */
	if (!(hasFlag(import_flag|export_flag|classify_flag))) setFlag(help_flag);

	/* Show cgi and about stuff */
	if (hasFlag(cgi_flag)) fprintf(outputStream,PR_CGI_TEXT1);
	fprintf(outputStream,PR_ABOUT);

	/* Show version screen if requested */
	if (hasFlag(version_flag)) {
		fprintf(outputStream,PARSING_ABOUT);
		return -1;
	}

	if (hasFlag(help_flag)) {
		syntax();
	} else {
		tFileDir2 files;
		tFileDir2 input;

		fileDirClearOptions(&input);
		if (optind < argc) {
			while (optind < argc)
				fileDirAddOption(&input,argv[optind++]);
    } else {
				fileDirAddOption(&input,".");
		}
		c=fileDirGetFiles(&input,&files,!hasFlag(import_flag),!hasFlag(recursive_flag),resFile,datFileName!=NULL);

		switch (c) {
		case -20: /* import from more than one directory */
			fprintf(stderr,"You may select one directory to export all dat files or specifiy dat files.\nBoth actions are not allowed.\n");
			break;
		case -21: /* import with recursive flag */
			fprintf(stderr,"Recursive flag must not be set when you import files\n");
			break;
		case -22: /* no files selected */
			fprintf(stderr,"No files selected\n");
			break;
		case 0:
			while ((file=fileDirGetFile(&files,&datfile))) {
				const char* dat;
				if (datFileName)
					dat=datFileName;
				else
					dat=datfile;
				/* Call PR */
				if (hasFlag(import_flag)) {
					/* import */
					fprintf(outputStream,PR_TEXT_TASK_COMPILE,file,dirName);
					result=prImportDatOpt(file,dirName,resFile,optionflag,dat,extension);
					if (result>0) {
						fprintf(outputStream,PR_TEXT_RESULT_ERR,result,result);
					} else {
						fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
					}
				} else if (hasFlag(export_flag)) {
					/* export */
					fprintf(outputStream,PR_TEXT_TASK_EXTRACT,file,dirName);
					result=prExportDatOpt(file,dirName,resFile,optionflag,dat,datAuthor,extension,format);
					if (result>0) {
						fprintf(outputStream,PR_TEXT_EXPORT_OK,result,result);
					} else {
						fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
					}
				} else {
					/* classify */
					fprintf(outputStream,PR_TEXT_TASK_CLASSIFY,file);
					result=prClassifyDat(file);
					if (result>0) {
						fprintf(outputStream,"Result: Detected as %s (%d)\n",types[result],result);
					} else {
						fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
					}
				}
				free(file);
				free(datfile);
			}
			fprintf(outputStream,"\n");
			break;
		default:
			fprintf(stderr,"Error, check the xml file\n");
			break;
		}
	}
	freeParsingCache();
	freeAllocation(datAuthor);
	freeAllocation(datFileName);
	freeAllocation(extension);
	freeAllocation(resFile);

	return c;
}

