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

#include "memory.h"    /* getMemory, free */
#include "disk.h"      /* getFileNameFromPath */

#ifndef DLL
 #ifdef UNIX
  #include <unistd.h>
 #else
  #include "getopt.h"
 #endif
#endif

/***************************************************************\
|                      Main working functions                   |
\***************************************************************/

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
			                        NULL means predict it from vDatFile (DISABLED!!!)
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
	if (vResFile==NULL) vResFile=defaultXmlFile; //TODO: Send to prMain or main

	//Parse XML and extract the file //TODO: XML parser outside the function
	a=parseFile(vResFile,vDatFileName,r);
	if (a<0) return a-3; //parsing errors
	a=extract(vDatFile,vDirName,r,opt,vDatFileName,datAuthor);
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
			recursive_flag - searches for all dat files (only if vDatFile
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

	//Parse XML and compile files
	a=parseFile     (vResFile,vDatFileName,r);
	if (a<0) return a-1;
	a=compile (vDatFile, vDirName,r,opt,vDatFileName);
	return a;
}


/***************************************************************\
|                     M A I N   P R O G R A M                   |
\***************************************************************/

void syntax(FILE* output) {
	fprintf(output,PARSING_HELP);
}

int prMain(int* pOption, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor,FILE* output) {

	//declare variables
	int returnValue=1;
	char* currentDatFileName;
	const char* aux;

	if (datfilename==NULL) {
		aux=getFileNameFromPath(datfile);
	} else {
		aux=datfilename;
	}

	currentDatFileName=strallocandcopy(aux);

	//do selected tasks
	if (optionflag&export_flag) {
		char* array[]=PR_TEXT_EXPORT_ARRAY;
		fprintf(output,"Extracting '%s' to '%s' with %04x\r\n",datfile,dirName,optionflag);
		returnValue=prExportDatOpt(datfile,dirName,resFile,optionflag,currentDatFileName,datAuthor,extension);
		fprintf(output,PR_TEXT_RESULT,array[-returnValue],returnValue);
	}	else if (optionflag&classify_flag) {
		char* array[]=PR_TEXT_CLASSIFY_ARRAY;
		fprintf(output,"Classifing '%s'\r\n",datfile);
		returnValue=prVerifyDatType(datfile);
		fprintf(output,PR_TEXT_RESULT,array[2+returnValue],returnValue);
	}	else if (optionflag&import_flag) {
		char* array[]=PR_TEXT_IMPORT_ARRAY;
		fprintf(output,"Compiling '%s' from '%s' with %04x\r\n",datfile,dirName,optionflag);
		returnValue=prImportDatOpt(datfile,dirName,resFile,optionflag,currentDatFileName,extension);
		if (returnValue<=0) {
			fprintf(output,PR_TEXT_RESULT,array[-returnValue],returnValue);
		} else {
			fprintf(output,PR_TEXT_RESULT_ERR,returnValue);
		}
	} else {
		syntax(output);
		returnValue=-1;
	}
	if (currentDatFileName) free(currentDatFileName);
	return returnValue;
}

//Main program
#ifndef DLL

/***************************************************************\
|             Standard executable specific functions            |
\***************************************************************/

int prStart(int* pOption, const char* extension,const char* dirName,const char* resFile,const char* datfile, const char* datfilename,const char* datAuthor,FILE* output) {
	int result=1;

	//Do CGI tasks
	if (optionflag&cgi_flag) {
		optionflag&=(~classify_flag);
		if (!(optionflag&first_flag)) {
			fprintf(output,PR_CGI_TEXT1);
			optionflag|=first_flag;
		}
		if (optionflag==cgi_flag) {
			fprintf(output,PR_CGI_TEXT2,prVerifyDatType(datfile));
			return 1;
		} else {
			optionflag=help_flag;
		}
	}

	//Show about
	if (!(optionflag&first_flag)) {
		fprintf(output,PR_ABOUT);
		optionflag|=first_flag;
	}

	//If bad syntax or help screen requested
	if (optionflag&help_flag) {
		syntax(output);
		return -1;
	}

	//If version asked, stop
	if (optionflag&version_flag) {
		return -1;
	}

	//Perform tasks depending on the argument
	if (optionflag&import_flag) {
		//Check out the xml file to get the files to me compiled
		if (datfile==NULL) {
			fprintf(output,"Importing all valid dat files from the currect directory\n");
			importDir(dirName,resFile,pOption,extension,".",output);
		} else if (isDir(datfile)!=eFile) {
			fprintf(output,"Importing all valid files from given directory\n");
			importDir(dirName,resFile,pOption,extension,datfile,output);
		} else {
			result=prMain(pOption,extension,dirName,resFile,datfile,datfilename,datAuthor,output);
		}
	} else {

		//Recursive testing for export/classify
		if (datfile==NULL) {
			fprintf(output,PR_TEXT_SCANNING_CURRENT);
			recurseDirectory(".",pOption,extension,dirName,resFile,datfilename,datAuthor,output);
		} else if (isDir(datfile)==eDirectory) {
			fprintf(output,PR_TEXT_SCANNING_GIVEN);
			recurseDirectory(datfile,pOption,extension,dirName,resFile,datfilename,datAuthor,output);
		} else if (isDir(datfile)==eNotFound) {
			fprintf(output,PR_TEXT_FILE_NOT_FOUND,datfile);
			return 0;
		} else {

			result=prMain(pOption,extension,dirName,resFile,datfile,datfilename,datAuthor,output);
		}
	}

	freeParsedStructure();
	return result;
}

/***************************************************************\
|      Standard executable command line parsing function        |
\***************************************************************/

int main (int argc, char **argv) {
	//declare variables
	char* datFileName=NULL;
	char  dirName[MAX_FILENAME_SIZE]=".";
	char  extension[MAX_EXTENSION_SIZE]=DEFAULT_BACKUP_EXTENSION;
	char  resFile[MAX_FILENAME_SIZE]=RES_XML_RESOURC_XML;
	char* datFilePath=NULL;
	char* datAuthor=NULL;
	int   c;
	int   flag=0;

	//Parse options
	while (1) {
		static struct option long_options[] = PARSING_OPTIONS;

		/* getopt_long stores the option index here. */
		int junk = 0;
		c = getopt_long (argc, argv, PARSING_CHARS,long_options,&junk);

		/* Detect the end of the options. */
		if (c == -1) break;

		switch (c) {
				case 'c':
					if (flag&(classify_flag|export_flag)) flag|=help_flag;
					flag|=import_flag;
					if (optarg) strncpy(dirName,optarg,MAX_FILENAME_SIZE);
					break;
				case 'd':
					if (flag&(import_flag|export_flag)) flag|=help_flag;
					flag|=classify_flag;
					break;
				case 'x':
				case 'e':
					if (flag&(classify_flag|import_flag)) flag|=help_flag;
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
					flag|=recursive_flag;
					break;
				case 't':
					if (datFileName!=NULL) free(datFileName);
					datFileName=strallocandcopy(optarg);
					break;
				case 'a':
					if (datAuthor!=NULL) free(datAuthor);
					datAuthor=strallocandcopy(optarg);
					break;
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
	}

	if (!flag) flag=help_flag;

	//Run main program
fld("a");
	prStart(&flag,extension,dirName,resFile,datFilePath,datFileName,datAuthor,stdout);
fld("b");
	//Free memory and exit
	if (datAuthor!=NULL) free(datAuthor);
	if (datFileName!=NULL) free(datFileName);
  return 0;
}

#endif

/***************************************************************\
|              Main Library start dummy function                |
\***************************************************************/

#ifdef SO
//When compiling in Unix SO libraries
void start() {}
#endif


