
#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#define HEADER      "Princed V3.0 alpha game resources list\n(c) Copyright 2003 - Enrique P. Calot\n(c) Copyright 2003 - Princed Development Team\n\nSyntax:\nId    Size  Offset    File    Type Description       Comments\nXXXXX YYYYY ZZZZZ SSSSSSSS.DAT TT Ddddddddddddd...  #cccccccc....\n\nComments are optional\n\nNote:\n You can edit description and commets here in the file and\n they will be saved in the dat files.\n\nResource Types:\n 01 Levels\n 02 Bitmaps\n 03 Waves\n 04 Midis\n"
#define BEGIN_TABLE "BEGIN resources table\n"
#define END_TABLE   "END resources table\n"

#define MAX_LINE_SIZE 300

//types
typedef struct {
	unsigned short int size;
	unsigned short int offset;
	char      file[13];
	char      type;
	char*     desc;
	char*     coms;
}tResource;

//Verify  header
char verifyHeader(char* array, int size);

/*
	Headers may be:
	 01 Levels
	 02 Bitmaps
	 03 Waves
	 04 Midis
	 05 Undefined
	 06 Palette
*/

/***************************************************************\
|                       File format handling                    |
\***************************************************************/

//Empty resource table
void emptyTable(tResource* r[]);

//parse file
char parseFile(char* vFile,tResource* r[]);

//generate file
char generateFile(char* vFile,tResource* r[]);

//Resources extras

void getFileName(char* vFileext,char* vDirExt,char type, unsigned short int id);
void getUpperFolder(char* aux, char* vFiledat);

//File handrling
int mLoadFileArray(char* vFile,unsigned char** array);
char mSaveRaw(char* vFile,unsigned char* output, int size);

#endif
