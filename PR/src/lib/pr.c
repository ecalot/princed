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

//Headers
#include <stdio.h>
#include <string.h>

#include "pr.h"

#include "compress.h"

#include "extract.h"
#include "compile.h"
#include "tasks.h"

#include "memory.h" /* getMemory, free */
#include "disk.h"      /* getFileNameFromPath */

#ifndef DLL
 #ifdef UNIX
  #include <stduni.h>
 #else
  #include "getopt.h"
 #endif
#endif


//functions

int prExportDat(const char* vDatFile, const char* vDirName, const char* vResFile) {
	return prExportDatOpt(vDatFile,vDirName,vResFile,0,NULL,NULL,"");
}

int prExportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName,const char* datAuthor, const char* backupExtension) {
	/*
		Arguments:
			char* vDatFile        - full Path to the dat file;
			                        if file use it, if dir, perform it for
			                        all files
			char* vDirName        - full Path to the extracting folder
			                        (doesn't need to exist)
			char* vResFile        - full Path to the resources XML file
			                        (resources.xml by default)
			                        No NULL
			char opt              - program options, see below
			char * vDatFileName   - name of the file to be extracted
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
x			recursive_flag - searches for all dat files (only if vDatFile
			                 is not a dat file and vDatFileName is NULL)
x			force_flag     - default option, you cannot disable it,
			                 so please make a backup of your files
x			backup_flag    - backup your files


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

	//Declare variables
	tResource* r[MAX_RES_COUNT];
	int a;
	const char defaultXmlFile[]=RES_XML_RESOURC_XML;

	//Set default values
	if (vResFile==NULL) vResFile=defaultXmlFile;
	if (vDatFileName==NULL) vDatFileName=getFileNameFromPath(vDatFile);

	//Parse XML and extract the file
	a=parseFile     (vResFile,vDatFileName,r);
	if (a<0) return a-3; //parsing errors
	a=extract(vDatFile, vDirName,r,opt,vDatFileName);
	printf("-3 es error: %d\n",a);
	return a; //extracting errors/no errors
}

int prImportDat(const char* vDatFile, const char* vDirName, const char* vResFile) {
	return prImportDatOpt(vDatFile,vDirName,vResFile,0,NULL,"");
}

int prImportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName, const char* backupExtension) {
	/*
		Arguments:
			char* vDatFile        - full Path to the dat file;
			                        if file use it, if dir, perform it for
			                        all files
			char* vDirName        - full Path to the extracting folder
			                        (doesn't need to exist)
			char* vResFile        - full Path to the resources XML file
			                        (resources.xml by default)
			                        No NULL
			char opt              - program options, see below
			char * vDatFileName   - name of the file to be extracted
			                        NULL means predict it from vDatFile
			const char* backupExtension
			                      - If backup_flag is set, the string to attach
			                        to the backup files

		Options:
			raw_flag       - uses raw format
			verbose_flag   - explain what is being done
x			recursive_flag - searches for all dat files (only if vDatFile
			                 is not a dat file and vDatFileName is NULL)
x			force_flag     - default option, you cannot disable it,
			                 so please make a backup of your files
x			backup_flag    - backup your files


		Return values:
			-1 DAT File couldn't be open for writing
			-2 XML Parse error
			-3 No memory
			-4 XML Attribute not recognized
			-5 XML File not found
			00 File succesfully compiled
			positive number: number of missing files
	*/

	//Declare variables
	tResource* r[MAX_RES_COUNT];
	int a;
	const char defaultXmlFile[]=RES_XML_RESOURC_XML;

	//Set default values
	if (vResFile==NULL) vResFile=defaultXmlFile;
	if (vDatFileName==NULL) vDatFileName=getFileNameFromPath(vDatFile);

	//Parser XML and compile files
	a=parseFile     (vResFile,vDatFileName,r);
	if (a<0) return a-1;
	a=compile (vDatFile, vDirName,r,opt,vDatFileName);
	return a;
}


/*******************************************************
                    M A I N   P R O G R A M
       *******************************************************/



//Main program
#ifndef DLL
void syntax() {
	printf(PARSING_HELP);
}

int prMain(int option, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor) {

	//declare variables
	int returnValue=1;

	//Do CGI tasks
	if (option&cgi_flag) {
		option&=(~classify_flag);
		printf(PR_CGI_TEXT1);
		if (option==cgi_flag) {
			printf(PR_CGI_TEXT2,prVerifyDatType(datfile));
			return 1;
		} else {
			option=help_flag;
		}
	}

	printf(PR_ABOUT);

	//If bad syntax or help screen requested
	if (option&help_flag) {
		syntax();
		return -1;
	}

	if ((datfile==NULL)||option&version_flag) { //TODO use the recursive option and/or generic datfile
		return -1;
	}

	//do selected tasks
	if (option&export_flag) {
		char* array[]={
			"Ok",
			"Error accessing the file DAT", /* DAT or extracted */
			"Memory error in extraction",
			"Invalid DAT file",
			"XML Parse error",
			"Memory error in parsing",
			"XML Attribute not recognized",
			"XML File not found"};
		printf("Extracting '%s' to '%s' with %04x\r\n",datfile,dirName,option);
		returnValue=prExportDatOpt(datfile,dirName,resFile,option,NULL,datAuthor,extension);
		printf(PR_TEXT_RESULT,array[-returnValue],returnValue);
	}	else if (option&classify_flag) {
		char* array[]={
			"Memory error",
			"File not found or no access error",
			"Not a valid POP1 DAT file",
			"Levels file",
			"Graphic file with an image in the first valid entry (not common)",
			"Waves/Digital sound file",
			"Midis file",
			"Valid DAT file with Undefined content",
			"Graphic file with a palette in the first valid entry (common)",
			"PC Speaker dat file",
			"\0","\0","\0",
			"Pop2 dat files"};
		printf("Classifing '%s'\r\n",datfile);
		returnValue=prVerifyDatType(datfile);
		printf(PR_TEXT_RESULT,array[2+returnValue],returnValue);
	}	else if (option&import_flag) {
		char* array[]={
			"File succesfully compiled",
			"DAT File couldn't be open for writing",
			"XML Parse error",
			"No memory",
			"XML Attribute not recognized",
			"XML File not found"};
		printf("Compiling '%s' from '%s' with %04x\r\n",datfile,dirName,option);
		returnValue=prImportDatOpt(datfile,dirName,resFile,option,NULL,extension);
		if (returnValue<=0) {
			printf(PR_TEXT_RESULT,array[-returnValue],returnValue);
		} else {
			printf(PR_TEXT_RESULT_ERR,returnValue);
		}
	} else {
		syntax();
		return -1;
	}
	return returnValue;
}

int main (int argc, char **argv) {
	//declare variables
	char  datFileName[MAX_FILENAME_SIZE]; //TODO add
	char  dirName[MAX_FILENAME_SIZE]=".";
	char  extension[MAX_EXTENSION_SIZE]=DEFAULT_BACKUP_EXTENSION;
	char  resFile[MAX_FILENAME_SIZE]=RES_XML_RESOURC_XML;
	char* datFilePath;
	char* datAuthor=NULL;
	int   c;
	int   flag=0;

	//Parse options
	while (1) {

		static struct option long_options[] = PARSING_OPTIONS;

		/* getopt_long stores the option index here. */
		int option_index = 0;
		c = getopt_long (argc, argv, PARSING_CHARS,long_options,&option_index);

		/* Detect the end of the options. */
		if (c == -1) break;

		switch (c) {
				case 'c':
					flag|=import_flag;
					if (optarg) strncpy(dirName,optarg,MAX_FILENAME_SIZE);
					break;
				case 'd':
					flag|=force_flag;
					break;
				case 'x':
				case 'e':
					flag|=export_flag;
					if (optarg) strncpy(dirName,optarg,MAX_FILENAME_SIZE);
					break;
				case 'b':
					flag|=backup_flag;
					if (optarg) strncpy(extension,optarg,MAX_FILENAME_SIZE);
					break;
				case 'f':
					flag|=force_flag;
					break;
				case 'g':
					flag|=cgi_flag;
					break;
				case 'm':
					strncpy(resFile,optarg,MAX_FILENAME_SIZE);
					break;
				case 'r':
					flag|=raw_flag;
					break;
				case 'R':
					flag|=force_flag;
					break;
				case 't': {
					int size;
					size=strlen(optarg)+1;
					datAuthor=getMemory(size);
					memcpy(datAuthor,optarg,size);
				}	break;
				case 'v':
					flag|=verbose_flag;
					break;
				case 1:
					flag|=version_flag;
					break;
				case 2:
					flag|=unknown_flag;
					break;
				default:
					flag|=help_flag;
		}
	}

	if (optind < argc) {
		datFilePath=argv[optind];
	} else {
		datFilePath=NULL;
	}

	if (!flag) flag=help_flag;

	//Run main program
	prMain(flag,extension,dirName,resFile,datFilePath,datFileName,datAuthor);

	//Free memory and exit
	if (datAuthor!=NULL) free(datAuthor);
  return 0;
}

#endif

#ifdef SO
//When compiling in Unix SO libraries
void start() {}
#endif
