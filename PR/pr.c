/*
	Princed Resources library V1 beta
	(c) Copyright 2003, Princed Development Team

	Authors
	 Main routines
	  Enrique Calot

	 Graphic compression algorithm
    Tammo Jan Dijkema
    Enrique Calot

   Graphic format development
    Tammo Jan Dijkema
    Anke Balderer

   MID Sound format development
    Christian Lundheim

*/

//Compilation options

//#define UNIX
//#define DLL
#define PR_ABOUT "Princed resources (PR) V0.5 beta.\r\n(c) Copyright 2003 - Princed Development Team\r\nhttp://www.princed.com.ar\r\n\r\n"

//Headers

#include "pr.h"

//Inclusion of implementations

#include "compile.c"
#include "extract.c"
#include "memory.c"
#include "parser.c"
#include "pg.c"
#include "resources.c"
#include "tasks.c"

//format support
#include "formats/wav.c"
#include "formats/bmp.c"
#include "formats/pal.c"

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
			-1 Error accesing the file
			-2 Memory error
	*/
	tResource* r[65536];
	parseFile (vResFile,r);
	int a=extract(vDatFile, vDirName,r,opt);
	if (!(opt&8)) generateFile(vResFile,r);
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
			-1 File couldn't be open for writing
			00 File succesfully compiled
			positive number: number of missing files
	*/
	tResource*    r[65536];
	parseFile     (vResFile,r);
	int a=compile (vDatFile, vDirName,r,opt);
	generateFile  (vResFile,r);
	return a;
}

int prClearRes(char* vResFile) {
	/*
		Return values:
			01 Ok
			00 Error
	*/
	tResource* r[65536];
	emptyTable(r);
	return generateFile(vResFile,r);
}



//Main program
#ifndef DLL
void syntax() {
	printf("Syntax:\r\n pr datfile option [extract dir]\r\n\r\nValid options:\r\n -x[rnus] for extract\r\n  r: raw extraction\r\n  n: don't extract\r\n  u: update res file in case there were records\r\n  s: don't save res file\r\n -c for compile\r\n  r: raw compiling\r\n -d for type\r\n -t to clear the resource file.\r\n");
}

int main(int argc, char* argv[]) {

#ifdef UNIX
	if (argc==2) {
		printf("Content-Type:text/html\n\nRunning as a cgi\n");
		printf("Result: %d type\r\n",prVerifyDatType(argv[1]));
		return 1;
	}
#endif

	//declare variables
	char dir[260]=".";
	int returnValue=1;
	int option;

	//TODO: send to defines
	printf(PR_ABOUT);

	//Verify syntax
	if ((argc<3)||(argc>4)||(argv[2][0])!='-') {
		syntax();
		return -1;
	}
	if (argc==4) {
		sprintf(dir,argv[3]);
	}

	//do selected taskbars
	switch (argv[2][1]) {
		case 'e':
		case 'x': // file.dat --> files.ext + resource.txt
			option=1;
			for (int i=2;argv[2][i];i++) {
				switch (argv[2][i]) {
					case 'n':option&=0xFE;break;
					case 'r':option|=0x04;break;
					case 'u':option|=0x02;break;
					case 's':option|=0x08;break;
					default:printf("Found invalid option '%c', skiping . . .\r\n",argv[2][i]);break;
				}
			}
			printf("Extracting '%s' to '%s' with %d\r\n",argv[1],dir,option);
			printf("Result: %d\r\n",returnValue=prExportDatOpt(argv[1],dir,"resources.txt",option));
			break;
		case 'd': // get type of file.dat
			printf("Classifing '%s'\r\n",argv[1]);
			printf("Result: %d type\r\n",returnValue=prVerifyDatType(argv[1]));
			break;
		case 'i':
		case 'c': // files.ext + resource.txt --> files.dat
			option=1;
				for (int i=2;argv[2][i];i++) {
					switch (argv[2][i]) {
						case 'r':option&=0xFE;break;
						default:printf("Found invalid option '%c', skiping . . .\r\n",argv[2][i]);break;
					}
				}
			printf("Compiling '%s' from '%s' with %d\r\n",argv[1],dir,option);
			printf("Result: %d\r\n",returnValue=prImportDatOpt(argv[1],dir,"resources.txt",option));
			break;
		case 't': // none --> resource.txt (destroy resource table)
			printf("Clearing 'resources.txt'\r\n");
			printf("Result: %d\r\n",returnValue=prClearRes("resources.txt"));
			break;
		case 'b': // img.bmp --> img.ext
			{
			char vFileraw[100];
			char vFilebmp[100];
			unsigned char* data;
			tImage img;

			sprintf(vFileraw,"%s%cres%s.raw",dir,DIR_SEPARATOR,argv[1]);
			sprintf(vFilebmp,"%s%cres%s.bmp",dir,DIR_SEPARATOR,argv[1]);

			printf("Converting '%s' into '%s'\r\n",vFilebmp,vFileraw);
			int size=mLoadFileArray(vFilebmp,&data);
			if (size && mReadBitMap(&img,data,size)) {
				free(data);
				mCompressGraphic(data,img,&size);
				free(img.pix);
				mSaveRaw(vFileraw,data,size);
				free(data);

			} else {
				printf("No access to the file\r\n");
				break;
			}

			}
			break;
		default:
			syntax();
			return -1;
	}
	return returnValue;
}

#endif
