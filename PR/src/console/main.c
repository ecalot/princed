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

 Note:
  DO NOT remove this copyright notice
*/

//Headers
#include <string.h>

#include "pr.h"

#include "compress.h"

#include "extract.h"
#include "compile.h"
#include "tasks.h"

#include "bmp.h"
#include "memory.h"
#include "disk.h"

//functions

int prExportDat(char* vDatFile, char* vDirName, char* vResFile) {
	return prExportDatOpt(vDatFile,vDirName,vResFile,3);
}

int prExportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt) {
	/*
		Options:
			0 - none
			1 - extract
			2 - update resources (rebuild resources list)
			3 - 1&2
			4 - use raw file if extraction
			5 - 1&4
			8 - Don't save

		Return values:
			00 Ok
			-1 Error accesing the file DAT
			-2 Memory error in extraction
			-3 Invalid DAT file
			-4 XML Parse error
			-5 Memory error in parsing
			-6 XML Attribute not recognized
			-7 XML File not found
	*/
	tResource* r[MAX_RES_COUNT];
	int a;

	a=parseFile     (vResFile,vDatFile,r);
	if (a<0) return a-3;
	a=extract(vDatFile, vDirName,r,opt);
	return a;
}

int prImportDat(char* vDatFile, char* vDirName, char* vResFile) {
	return prImportDatOpt(vDatFile,vDirName,vResFile,1);
}

int prImportDatOpt(char* vDatFile, char* vDirName, char* vResFile,char opt) {
	/*
		Options:
			0 - use raw files
			1 - read data type

		Return values:
			-1 DAT File couldn't be open for writing
			-2 XML Parse error
			-3 No memory
			-4 XML Attribute not recognized
			-5 XML File not found
			00 File succesfully compiled
			positive number: number of missing files
	*/
	tResource*    r[MAX_RES_COUNT];
	int a;
	a=parseFile     (vResFile,vDatFile,r);
	if (a<0) return a-1;
	a=compile (vDatFile, vDirName,r,opt);
	return a;
}

//Main program
#ifndef DLL
void syntax() {
	printf(PR_TEXT_SYNTAX);
}

int main(int argc, char* argv[]) {

	//declare variables
	char dir[260]=".";
	int returnValue=1;
	int option;
	int i;

#ifdef UNIX
	if (argc==2) { //CGI support
		printf(PR_CGI_TEXT1);
		printf(PR_CGI_TEXT2,prVerifyDatType(argv[1]));
		return 1;
	}
#endif

	printf(PR_ABOUT);

	//Verify syntax
	if ((argc<3)||(argc>4)||(argv[2][0])!='-') {
		syntax();
		return -1;
	}
	if (argc==4 ) {
		if (strlen(argv[3])<(260-1)) sprintf(dir,argv[3]);
	}

	//do selected tasks
	switch (argv[2][1]) {
		case 'e':
		case 'x': {// file.dat --> extracted files + resource.xml
			char array[8][29]={
				"Ok",
				"Error accesing the file DAT", /* DAT or extracted */
				"Memory error in extraction",
				"Invalid DAT file",
				"XML Parse error",
				"Memory error in parsing",
				"XML Attribute not recognized",
				"XML File not found"};
			option=1;
			for (i=2;argv[2][i];i++) {
				switch (argv[2][i]) {
					case 'n':option&=0xFE;break;
					case 'r':option|=0x04;break;
					default:printf(PR_TEXT_SKIPING,argv[2][i]);break;
				}
			}
			printf("Extracting '%s' to '%s' with %d\r\n",argv[1],dir,option);
			returnValue=prExportDatOpt(argv[1],dir,RES_XML_RESOURC_XML,(char)option);
			printf(PR_TEXT_RESULT,array[-returnValue],returnValue);
		}	break;
		case 'd': {// get type of file.dat
			char array[14][65]={
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
			printf("Classifing '%s'\r\n",argv[1]);
			returnValue=prVerifyDatType(argv[1]);
			printf(PR_TEXT_RESULT,array[2+returnValue],returnValue);
		}	break;
		case 'c': { // extracted files + resource.xml --> files.dat
			char array[6][39]={
				"File succesfully compiled",
				"DAT File couldn't be open for writing",
				"XML Parse error",
				"No memory",
				"XML Attribute not recognized",
				"XML File not found"};
			option=1;
				for (i=2;argv[2][i];i++) {
					switch (argv[2][i]) {
						case 'r':option&=0xFE;break;
						default:printf(PR_TEXT_SKIPING,argv[2][i]);break;
					}
				}
			printf("Compiling '%s' from '%s' with %d\r\n",argv[1],dir,option);
			returnValue=prImportDatOpt(argv[1],dir,RES_XML_RESOURC_XML,(char)option);
			if (returnValue>=0) {
				printf(PR_TEXT_RESULT,array[-returnValue],returnValue);
			} else {
				printf(PR_TEXT_RESULT_ERR,returnValue);
			}
		} break;
		default:
			syntax();
			return -1;
	}
	return returnValue;
}

#endif
