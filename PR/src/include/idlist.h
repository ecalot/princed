
#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#define MAX_LINE_SIZE 300

#define RES_XML_UNKNOWN_PATH  "%s/unknown/%s/"
#define RES_XML_UNKNOWN_FILES "res%d.%s"
#define RES_XML_UNKNOWN_START "<?xml version=\"1.0\" ?>\n<itemtypes version=\"0.6\">\n <folder name=\"unknown\" file=\"%s\" external=\"%s\" palette=\"%d\">\n"
#define RES_XML_UNKNOWN_ITEM  "  <item value=\"%d\" external=\""RES_XML_UNKNOWN_FILES"\" itemtype=\"%s\">Unknown Resource number %d</item>\n"
#define RES_XML_UNKNOWN_END   " </folder>\n</itemtypes>\n"

#define RES_FILE_EXTENSIONS   {"raw","plv","bmp","wav","mid","ext","pal","pcs"}
#define RES_FILE_TYPES        {"raw","level","image","wave","midi","unknown","palette","pcspeaker"}
//types
typedef struct {
	unsigned short int palette;
	unsigned short int size;
	unsigned long int offset;
	unsigned char number; //Used for level number
	char      type;
	char*     path;
	char*     palAux;
	char*     title;
	char*     desc;
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
	 07 Internal Speaker Sounds
*/

/***************************************************************\
|                       File format handling                    |
\***************************************************************/

//Empty resource table
void emptyTable(tResource* r[]);

//parse file
char parseFile(const char* vFile, const char* datFile, tResource* r[]);

//Resources extras

void getFileName(char* vFileext,const char* vDirExt,tResource* r,short id,const char* vFiledat);
void getUpperFolder(char* aux, char* vFiledat);

const char* getExtDesc(int type);

//In case there are unknown resources it closes the unknown XML output
void endUnknownXml();

#endif
