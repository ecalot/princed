
#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#define MAX_LINE_SIZE 300

#define RES_XML_UNKNOWN_XML   "unknown.xml"
#define RES_XML_UNKNOWN_PATH  "%s/unknown/%s/"
#define RES_XML_UNKNOWN_START "<?xml version=\"1.0\" ?>\n<itemtypes version=\"0.6\">\n <folder name=\"unknown\" file=\"%s\" external=\"%s\" palette=\"%d\">\n"
#define RES_XML_UNKNOWN_ITEM  "  <item value=\"%d\" external=\"res%05d.%s\" itemtype=\"%d\">Unknown Resource number %d</item>\n"
#define RES_XML_UNKNOWN_END   " </folder>\n</itemtypes>\n"
#define RES_XML_UNKNOWN_FILES "%d.%s"

//types
typedef struct {
	unsigned short int palette;
	unsigned short int size;
	unsigned short int offset;
	char      type;
	char*     path;
	char*     palAux;
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
	 07 Speaker Sounds
*/

/***************************************************************\
|                       File format handling                    |
\***************************************************************/

//Empty resource table
void emptyTable(tResource* r[]);

//parse file
char parseFile(char* vFile, char* datFile, tResource* r[]);

//generate file
char generateFile(char* vFile,tResource* r[]);

//Resources extras

void getFileName(char* vFileext,char* vDirExt,tResource* r,short id,char* vFiledat);
void getUpperFolder(char* aux, char* vFiledat);

//In case there are unknown resources it closes the unknown XML output
void endUnknownXml();

#endif
