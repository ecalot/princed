/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include <string.h>
#include "compile.h"
#include "memory.h"
#include "bmp.h"
#include "wav.h"
#include "pal.h"
#include "parser.h"

/***************************************************************\
|                  Dat compiling primitives                     |
\***************************************************************/

char mBeginDatFile(FILE* *fp,char* vFile) {
	//opens a file and reserves space for the headers
	*fp=fopen(vFile,"wb");
	if (*fp!=NULL) {
		fseek(*fp,6,SEEK_SET);
		return 1;
	} else {
		return 0;
	}
}

void mAddFileToDatFile(FILE* fp, char* data, int size) {
	//calculates the checksum of a file
	//unsigned char sndHeader[]={0,2};
	unsigned char checksum=0;
	int k;
	for (k=0;k<size;k++) checksum+=data[k];
	checksum=~checksum;
printf("mAddFileToDatFile: llega X1 size=%d\n",size);
	//writes the header and the midi sound
	fwrite(&checksum,1,1,fp);
	fwrite(data,size,1,fp);
printf("mAddFileToDatFile: llega X2\n");

}

void mSetEndFile(FILE* fp,int sizeOfIndex) {
	//sets the headers
	short int size1,zero,size2;
	fseek(fp,0,SEEK_END);
	size1=ftell(fp)-(size2=sizeOfIndex);
	zero=0;
	fseek(fp,0,SEEK_SET);
	fwrite(&size1,2,1,fp);
	fwrite(&zero,2,1,fp);
	fwrite(&size2,2,1,fp);
	fclose(fp);
}

int mCreateIndexInDatFile(FILE* fp, tResource* r[], char* vUpperFile) {
	//Add extra text at the end of the file
	int k=2;
	unsigned short int tot=0;
	unsigned short int junk=0;
	unsigned short int i=0;
	int pos=ftell(fp);

	fwrite(&tot,2,1,fp);
	for (;i!=65535;i++) {
		if (r[i]!=NULL) {
			if (equals((*r[i]).file,vUpperFile)) {
				//the file is in the archive, so I'll add it to the index
				k+=8;
				tot++;
				fwrite(&i,2,1,fp);
				fwrite(&((*r[i]).offset),2,1,fp);
				fwrite(&junk,2,1,fp);
				fwrite(&((*r[i]).size),2,1,fp);
			}
		}
	}
	fseek(fp,pos,SEEK_SET);
	fwrite(&tot,2,1,fp);
	return k;
}

//Format detection function
void mAddCompiledFileToDatFile(FILE* fp,unsigned char** data, tResource *res) {
			printf("llega G size=%d\n",res->size);

	switch ((*res).type) {
		case 2: //compile bitmap
			printf("llega 1\n");
			printf("bitmap size in=%d\n",res->size);
			if (!mFormatCompileBmp(*data,fp,res)) {
				printf("Error!!\n");
			}
			printf("bitmap size out=%d\n",res->size);
			break;
		case 3: //compile wave
			mFormatCompileWav(*data,fp,res);
			break;
		case 4: //compile midi
			//send to mid
			{
			unsigned char* file;
			file=getMemory((*res).size);
			file[0]=(*res).type-2;
			memcpy(file+1,*data,(*res).size);
			(*res).size++;
			mAddFileToDatFile(fp,file,(*res).size);
			free(file);
			}
			break;
		case 6:
			mImportPalette(data,&((*res).size));
		case 1:
		case 5:
		default:
			mAddFileToDatFile(fp,*data,(*res).size);
			break;
	}
}

/***************************************************************\
|                    M A I N   F U N C T I O N                  |
\***************************************************************/
//TODO: delete this function
char mSaveFile(char* vFile,unsigned char *d, int s) {
	FILE *fp;

	if ((fp=fopen(vFile,"wb"))==NULL) {
		return 0;
	} else {
		fwrite (d,s,1,fp);
		fclose(fp);
		return 1;
	}
}

/*
	Return values:
		-1 File couldn't be open for writing
		00 File succesfully compiled
		positive number: number of missing files


*/
int compile(char* vFiledat, char* vDirExt, tResource* r[], char opt) {
	FILE* fp;
	char vUpperFile[200];
	char vFileext[200];
	unsigned char* data;
	int ok=0;
	unsigned short int i=0;

	if (!mBeginDatFile(&fp,vFiledat)) {
		printf("Error opening the file.\r\n");
		return -1;
	}

	getUpperFolder(vUpperFile,vFiledat);

	for (;i!=65535;i++) {
		if (r[i]!=NULL) {
			if (equals((*r[i]).file,vUpperFile)) {
				getFileName(vFileext,vDirExt,(char)((opt&1)?((*(r[i])).type):0),i);
				//the file is in the archive, so I'll add it to the main dat body
				if ((*r[i]).size=mLoadFileArray(vFileext,&data)) {
					(*r[i]).offset=(unsigned short)ftell(fp);
					mAddCompiledFileToDatFile(fp,&data,r[i]);

					{
						char sss[300];
						sprintf(sss,"r1\\bmp%05d.raw",i);
printf("size: id=%d size=%d %02x %02x %02x %02x %02x %02x %02x\n",i,r[i]->size,data[0],data[1],data[2],data[3],data[4],data[5],data[6]);

					mSaveFile(sss,data,r[i]->size);
					}

					free(data);
					printf("libere ok\n");
				} else {
					ok++;
				}
			}
		}
	}
	mSetEndFile(fp,mCreateIndexInDatFile(fp,r,vUpperFile));
	return ok;
}
