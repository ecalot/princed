

//Needed files
#include <stdio.h>
#include <stdlib.h>
#include "pr.h"
#include "parser.h"
#include "memory.h"
#include "resources.h"
#include "compress.h"

/***************************************************************\
|                       File format handling                    |
\***************************************************************/

char verifyLevelHeader(char* array, int size) {
	return (((size==2306)||(size==2305))&&!(array[1698]&0x0F)&&!(array[1700]&0x0F)&&!(array[1702]&0x0F));
}

char verifyImageHeader(char* array, int size) {
	//return (size>6) && (!(((!array[1])||(array[2])||(!array[3])||(array[4])||(array[5])||(((unsigned char)array[6]&0xF0)!=0xB0))));
	return (size>7) && (!array[5]) && (((unsigned char)array[6]&0xF0)==0xB0);
	//todo: fix the expression
}

char verifyPaletteHeader(char* array, int size) {
	return ((size==101)&&(!array[2])&&(!array[3])&&(array[4]==0x10));
}

char verifyMidiHeader(char* array, int size) {
	return
	  (size>6) &&
		(array[1]==0x02) &&
		(array[2]=='M') &&
		(array[3]=='T') &&
		(array[4]=='h') &&
		(array[5]=='d')
	;
}

char verifyWaveHeader(char* array, int size) {
	return
		(size>1)&&(array[1]==0x01)
	;
}

char verifyHeader(char* array, int size) {
	if (verifyLevelHeader(array,size)) return 1;
	if (verifyMidiHeader(array,size)) return 4;
	if (verifyImageHeader(array,size)) return 2;
	if (verifyPaletteHeader(array,size)) return 6;
	if (verifyWaveHeader(array,size)) return 3;
	return 05;
}


/***************************************************************\
|                      Parsing resource file                    |
\***************************************************************/


//Parse line
void parseResource(tResource* r[], char* line) {
	//declare variables
	int i=0;
	unsigned short int id,ty;
	int k=0;

	//Begin parsing

	for (;!(line[k]=='\r'||line[k]=='\n'||(!line[k]));k++);
	line[k]=0;

	if (getNumberToken(line,&id,' ',&i,6)) {
		r[id]=(tResource*)malloc(sizeof(tResource));
		getNumberToken(line,&(*(r[id])).size,' ',&i,6);
		getNumberToken(line,&(*(r[id])).offset,' ',&i,6);
		getUpperToken(line,(*(r[id])).file,' ',&i,12);
		if (getNumberToken(line,&ty,' ',&i,3)) {
			(*(r[id])).desc=(char*)malloc(255);
			if (getToken(line,(*(r[id])).desc,'#',&i,255)) {
				(*(r[id])).coms=(char*)malloc(1023);
				if (!getToken(line,(*(r[id])).coms,0,&i,1023)) {
					free ((*(r[id])).coms);
					(*(r[id])).coms=NULL;
				}
			}
		} else {
			(*(r[id])).desc=NULL;
		}
		(*(r[id])).type=ty;
	}
}

void emptyTable(tResource* r[]) {
	int i=0;
	for (;i<65536;i++) r[i]=NULL;
}

//parse file
char parseFile(char* vFile,tResource* r[]) {
	//declare variables
	char parsing=0;
	char line[MAX_LINE_SIZE];
	char token[MAX_LINE_SIZE];
	char B[]=BEGIN_TABLE;
	char E[]=END_TABLE;
	FILE* fp;

	//empty resource array
	emptyTable(r);

	//parse file
	if ((fp=fopen(vFile,"rt"))!=NULL) {
		while (fgets(line,MAX_LINE_SIZE,fp)!=NULL) {
			if (parsing) {
				if (equals(line,E)) {
					parsing=0;
				} else {
					parseResource(r,line);
				}
			} else {
				if (equals(line,B)) parsing=1;
			}
		}
		fclose(fp);
		return 1;
	} else {
		return 0;
	}
}

//generate file
char generateFile(char* vFile,tResource* r[]) {
	//declare variables
	char parsing=0;
	char line[MAX_LINE_SIZE];
	char coms[MAX_LINE_SIZE];
	char B[]=BEGIN_TABLE;
	char E[]=END_TABLE;
	char none[]="";
	FILE* fp;
	FILE* source;

	if ((fp=fopen("res.tmp","wt"))!=NULL) {
		//insert headers
		if ((source=fopen(vFile,"rt"))!=NULL) {
			while ((parsing!=2)&&(fgets(line,MAX_LINE_SIZE,source)!=NULL)) {
				if (parsing) {
					if (equals(line,E)) {
						parsing=2;
					}
				} else {
					if (equals(line,B)) {
						parsing=1;
					} else {
						fputs(line,fp);
					}
				}
			}
		}

		//insert main body file
		fputs(B,fp);
		fputs("\n",fp);
		int id=0;
		for (;id<65536;id++) {
			if (r[id]!=NULL) {
				if ((*(r[id])).desc==NULL) {
					if ((*(r[id])).coms==NULL) {
						sprintf(coms,none);
					} else {
						sprintf(coms," #",(*(r[id])).coms);
					}
				} else {
					if ((*(r[id])).coms==NULL) {
						sprintf(coms," %s",(*(r[id])).desc);
					} else {
						sprintf(coms," %s#%s",(*(r[id])).desc,(*(r[id])).coms);
					}
				}
				sprintf(line,"%05d %05d %05d %s %02d%s\n",
				id,
				(*(r[id])).size,
				(*(r[id])).offset,
				(*(r[id])).file,
				(*(r[id])).type,
				coms);
				fputs(line,fp);
			}
		}
		fputs("\n",fp);
		fputs(E,fp);

		//insert footers
		if (source!=NULL) {
			while ((parsing==2) && (fgets(line,MAX_LINE_SIZE,source)!=NULL)) {
				fputs(line,fp);
			}
			fclose(source);
		}
		fclose(fp);
		remove(vFile);
		rename("res.tmp",vFile);
		return 1;
	} else {
		return 0;
	}
}

//Resources extras

void getFileName(char* vFileext,char* vDirExt,char type, unsigned short int id) {
	char    extMidi[]   = "mid";
	char    extWave[]   = "wav";
	char    extBmp[]    = "bmp";
	char    extLevel[]  = "pet";
	char    extExtra[]  = "ext";
	char    extRaw[]    = "raw";
	char    extPal[]    = "pal";
	char* ext;

	switch (type) {
		case 0:
			ext=extRaw;
			break;
		case 1:
			ext=extLevel;
			break;
		case 2:
			ext=extBmp;
			break;
		case 4:
			ext=extMidi;
			break;
		case 3:
			ext=extWave;
			break;
		case 6:
			ext=extPal;
			break;
		default:
			ext=extExtra;
			break;
	}

	//set filename
	sprintf(vFileext,"%s%cres%05d.%s",vDirExt,DIR_SEPARATOR,id,ext);
}

void getUpperFolder(char* aux, char* vFiledat) {
	int i=0;
	while (getUpperToken(vFiledat,aux,DIR_SEPARATOR,&i,260));
	aux[12]=0;
}





//File handling functions




int mLoadFileArray(char* vFile,unsigned char** array) {
	//declare variables
	FILE *fp;
	int aux;

	//Open the file
	if ((fp=fopen(vFile,"rb"))==NULL) {
		return 0;
	} else {
		//get file size
		fseek(fp,0,SEEK_END);
		aux=ftell(fp);
		if ( !aux || (aux>SIZE_OF_FILE) || ( ((*array=(char*)malloc(sizeof(char)*aux))==NULL) ) ) {
			//if the file was null or bigger than the max size or couldn't allocate the file in memory
			fclose(fp);
			return 0;
		} else {
			//if the filewas succesfully open
			fseek(fp,0,SEEK_SET);
			aux=fread (*array,1,aux,fp);
			fclose(fp);
			return aux;
		}
	}
}

char mSaveRaw(char* vFile,unsigned char* output, int size) {
  FILE * pFile;
  if ((pFile = fopen (vFile , "wb"))==NULL) {
		return 0;
	}
	if (!size) {
		return 0;
	}
  fwrite (output, 1, size, pFile);
  fclose (pFile);
  return 1;
}
