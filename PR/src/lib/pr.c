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
pr.c: Main source file for Princed Resources
¯¯¯¯
	Princed Resources editor
	(c) Copyright 2003, Princed Development Team

	Authors
	 Coding & main routines
	  Enrique Calot
	  Santiago Zamora

	 Graphic compression algorithms
	  Tammo Jan Dijkema
	  Enrique Calot

	 Graphic format development
	  Tammo Jan Dijkema
	  Anke Balderer

	 MID Sound format development
	  Christian Lundheim

	 Resources.xml edition
	  Steven Fayers

 Note:
  DO NOT remove this copyright notice
*/

/* Headers */
#include <stdio.h>
#include <string.h>

#include "pr.h"

#include "compress.h"

#include "extract.h"
#include "compile.h"
#include "tasks.h"

#include "memory.h"    /* getMemory, free */
#include "disk.h"      /* getFileNameFromPath */

#ifndef DLL
 #ifdef UNIX
  #include <unistd.h>
  #ifndef LINUX
   #include "getopt.h"
  #endif
 #else
  #include "getopt.h"
 #endif
#endif

/***************************************************************\
|                      Main working functions                   |
\***************************************************************/

FILE* outputStream=NULL;

#ifdef DLL
void prSetOutput(FILE* output) {
	outputStream=output;
}
#endif

#ifdef DLL
int prExportDat(const char* vDatFile, const char* vDirName, const char* vResFile) {
	outputStream=stdout;
	return prExportDatOpt(vDatFile,vDirName,vResFile,export_flag,NULL,NULL,NULL);
}
#endif

int prExportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName,const char* datAuthor, const char* backupExtension) {
	/*
		Arguments:
			char* vDatFile        - full Path to the dat file;
			                        if file use it, if directory, perform it for
			                        all files
			char* vDirName        - full Path to the extracting folder
			                        (doesn't need to exist)
			char* vResFile        - full Path to the resources XML file
			                        NULL is the default file resources.xml
			char opt              - program options, see below
			char* vDatFileName    - name of the file to be extracted
			                        NULL means predict it from vDatFile
			const char* datAuthor - Author's name when extracting PLV's,
			                        NULL is default
			const char* backupExtension
			                      - If backup_flag is set, the string to attach
			                        to the backup files

		Options:
			unknown_flag   - generate the unknown file without performing
                       any extraction
			raw_flag       - uses raw format
			verbose_flag   - explain what is being done
			recursive_flag - searches for all dat files (only if vDatFile
			                 is not a dat file and vDatFileName is NULL)
			force_flag     - default option, you cannot disable it,
			                 so please make a backup of your files
			backup_flag    - backup your files


		Return values:
			00 Ok
			-1 Error accessing the file DAT
			-2 Memory error in extraction
			-3 Invalid DAT file
			-4 XML Parse error
			-5 Memory error in parsing
			-6 XML Attribute not recognized
			-7 XML File not found
	*/

	/* Declare variables */
	tResource* r[MAX_RES_COUNT];
	int a;
	const char* aux;
	char* currentDatFileName;
	char* currentDatFile;

	currentDatFile=strallocandcopy(vDatFileName);

	parseGivenPath(currentDatFile);

	if (vDatFileName==NULL) { /* if no special dat file was specified, a path parsed will be used */
		aux=getFileNameFromPath(vDatFile);
	} else {
		aux=currentDatFile;
	}
	currentDatFileName=strallocandcopy(aux);

	/* Parse XML and export the file */
	a=parseFile(vResFile,currentDatFileName,r);
	if (a<0) {
		/* parsing errors */
		a-=3;
	} else {
		/* exporting errors/no errors */
		a=extract(vDatFile,vDirName,r,opt,currentDatFileName,datAuthor,backupExtension);
	}
	free(currentDatFileName);
	free(currentDatFile);
	freePartialList();

	return a;
}

#ifdef DLL
int prImportDat(const char* vDatFile, const char* vDirName, const char* vResFile) {
	outputStream=stdout;
	return prImportDatOpt(vDatFile,vDirName,vResFile,0,NULL,NULL);
}
#endif

int prImportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName, const char* backupExtension) {
	/*
		Arguments:
			char* vDatFile        - full Path to the dat file;
			                        if file use it, if directory, perform it for
			                        all files
			char* vDirName        - full Path to the extracting folder
			                        (doesn't need to exist)
			char* vResFile        - full Path to the resources XML file
			                        (resources.xml by default)
			char opt              - program options, see below
			char *vDatFileName    - name of the file to be extracted
			                        NULL means predict it from vDatFile
			const char* backupExtension
			                      - If backup_flag is set, the string to attach
			                        to the backup files

		Options:
			raw_flag       - uses raw format
			verbose_flag   - explain what is being done
			recursive_flag - searches for all dat files (only if vDatFile
			                 is not a dat file and vDatFileName is NULL)
			force_flag     - If not set and the file exists it will prompt
			                 for action
			backup_flag    - backup your files


		Return values:
			-1 DAT File couldn't be open for writing
			-2 XML Parse error
			-3 No memory
			-4 XML Attribute not recognized
			-5 XML File not found
			00 File successfully imported
			positive number: number of missing files
	*/

	/* Declare variables */
	tResource* r[MAX_RES_COUNT];
	int a;
	const char* aux;
	char* currentDatFileName;

	if (vDatFileName==NULL) { /* if no special dat file was specified, a path parsed will be used */
		aux=getFileNameFromPath(vDatFile);
	} else {
		aux=vDatFileName;
	}
	currentDatFileName=strallocandcopy(aux);

	/* Parse XML and import files */
	a=parseFile(vResFile,currentDatFileName,r);
	if (a<0) {
		/* parsing errors */
		a-=1;
	} else {
		/* importing errors/no errors */
		a=compile (vDatFile, vDirName,r,opt,currentDatFileName,backupExtension);
	}
	free(currentDatFileName);
	return a;
}

/***************************************************************\
|                     M A I N   P R O G R A M                   |
\***************************************************************/

void syntax() {
	fprintf(outputStream,PARSING_HELP);
}

int prMain(int optionflag, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor,FILE* output) {

	/* declare variables */
	int returnValue;

	outputStream=output;

	/* do selected tasks */
	if (hasFlag(export_flag)) { /* Export file */
		char* array[]=PR_TEXT_EXPORT_ARRAY;
		fprintf(output,PR_TEXT_TASK_EXTRACT,datfile,dirName);
		returnValue=prExportDatOpt(datfile,dirName,resFile,optionflag,datfilename,datAuthor,extension);
		fprintf(output,PR_TEXT_RESULT,array[-returnValue],returnValue);
	}	else if (hasFlag(classify_flag)) { /* Classify file */
		char* array[]=PR_TEXT_CLASSIFY_ARRAY;
		fprintf(output,PR_TEXT_TASK_CLASSIFY,datfile);
		returnValue=prVerifyDatType(datfile);
		if (hasFlag(cgi_flag)) {
			fprintf(output,PR_CGI_TEXT2,returnValue);
		} else {
			fprintf(output,PR_TEXT_RESULT,array[2+returnValue],returnValue);
		}
	}	else if (hasFlag(import_flag)) { /* Import file */
		char* array[]=PR_TEXT_IMPORT_ARRAY;
		fprintf(output,PR_TEXT_TASK_COMPILE,datfile,dirName);
		returnValue=prImportDatOpt(datfile,dirName,resFile,optionflag,datfilename,extension);
		if (returnValue<=0) {
			fprintf(output,PR_TEXT_RESULT,array[-returnValue],returnValue);
		} else {
			fprintf(output,PR_TEXT_RESULT_ERR,returnValue);
		}
	} else {
		syntax();
		returnValue=-1;
	}

	return returnValue;
}

#ifndef DLL

/***************************************************************\
|             Standard executable specific functions            |
\***************************************************************/

int prStart(int optionflag, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor,FILE* output) {
	int result=1;
	whatIs selectedFile;
	outputStream=output;

	/* Show about or cgi stuff */
	if (hasFlag(cgi_flag)) {
		fprintf(output,PR_CGI_TEXT1);
	} else {
		fprintf(output,PR_ABOUT);
	}

	/* Show version screen if requested */
	if (hasFlag(version_flag)) {
		fprintf(output,PARSING_ABOUT);
		return -1;
	}

	/* If bad syntax or help screen requested */
	if (hasFlag(help_flag)) {
		syntax();
		return -1;
	}

	/* Perform tasks depending on the argument */
	if (hasFlag(import_flag)) {
		/* We have to import something, let's see what the parameters are */
		if (datfile==NULL) {
			/* no files? let's use the whole current directory */
			fprintf(output,PR_TEXT_IMPORTING_CURRENT);
			importDir(dirName,resFile,optionflag,extension,".",output);
		} else if ((selectedFile=isDir(datfile))!=eFile) {
			fprintf(output,PR_TEXT_IMPORTING_GIVEN);
			if ((selectedFile==eDirectory)||(isntADatFile(getFileNameFromPath(datfile),resFile))) {
				/* it's a directory or doesn't exist (but it's not a dat file) */
				importDir(dirName,resFile,optionflag,extension,datfile,output);
			} else {
				/* it doesn't exist but it's a dat file */
				result=prMain(optionflag,extension,dirName,resFile,datfile,datfilename,datAuthor,output);
			}
		} else { /* it's only one existent file */
			result=prMain(optionflag,extension,dirName,resFile,datfile,datfilename,datAuthor,output);
		}
	} else {
		/* We have to export/classify something, perhaps we'll need to use somerecursive functions */
		if (datfile==NULL) {
			/* If nothing, let's use the current dir and check for all the files there */
			fprintf(output,PR_TEXT_SCANNING_CURRENT);
			recurseDirectory(".",optionflag,extension,dirName,resFile,datfilename,datAuthor,output);
		} else if ((selectedFile=isDir(datfile))==eDirectory) {
			/* If it's a directory, let's check for all the files there */
			fprintf(output,PR_TEXT_SCANNING_GIVEN);
			recurseDirectory(datfile,optionflag,extension,dirName,resFile,datfilename,datAuthor,output);
		} else if (selectedFile==eNotFound) {
			/* If the file doesn't exist, print an error and exit */
			fprintf(output,PR_TEXT_FILE_NOT_FOUND,datfile);
			result=0;
		} else {
			/* If it is a file, just do the tasks for it */
			result=prMain(optionflag,extension,dirName,resFile,datfile,datfilename,datAuthor,output);
		}
	}

	freeParsedStructure();
	return result;
}

/***************************************************************\
|      Standard executable command line parsing function        |
\***************************************************************/

int main (int argc, char **argv) {
	/* declare variables */
	char  dirName[MAX_FILENAME_SIZE]=".";
	char* datAuthor  =NULL;
	char* datFileName=NULL;
	char* datFilePath=NULL;
	char* extension  =NULL;
	char* resFile    =NULL;
	int   c;
	int   optionflag=0;

	/* Parse command line options */
	do {
		static struct option long_options[] = PARSING_OPTIONS;
		int junk = 0;

		c = getopt_long(argc,argv,PARSING_CHARS,long_options,&junk);
		switch (c) {
				case 'c':
				case 'i':
					if (hasFlag(classify_flag|export_flag)) setFlag(help_flag);
					setFlag(import_flag);
					if (optarg) strncpy(dirName,optarg,MAX_FILENAME_SIZE);
					break;
				case 'g':
					setFlag(cgi_flag); /* if cgi, a classify must be performed */
				case 'd':
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
				case 'f':
					setFlag(force_flag);
					break;
				case 's':
					freeAllocation(resFile);
					resFile=strallocandcopy(optarg);
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

	if (optind < argc) {
		int size;
		datFilePath=strallocandcopy(argv[optind]);
		/* Erase the last "/" if exists. TODO: send to parseGivenPath  */
		size=strlen(datFilePath);
		if (size>0) {
			size--;
			if (isDirSep(datFilePath,size)) datFilePath[size]=0;
		}
	}

	/* At least one of these options must be selected, if not, the user needs help! */
	if (!(hasFlag(import_flag|export_flag|classify_flag))) setFlag(help_flag);

	parseGivenPath(datFilePath);

	/* Run main program */
	prStart(optionflag,extension,dirName,resFile,datFilePath,datFileName,datAuthor,stdout);

	/* Free memory and exit */
	freePartialList();
	freeAllocation(datAuthor);
	freeAllocation(datFileName);
	freeAllocation(datFilePath);
	freeAllocation(extension);
	freeAllocation(resFile);

	return 0;
}

#endif

/***************************************************************\
|              Main Library start dummy function                |
\***************************************************************/

#ifdef SO
/* When compiling in Unix SO libraries */
void start() {}
#endif



