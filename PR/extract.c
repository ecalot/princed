/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include "extract.h"

/***************************************************************\
|                     M A I N   E X T R A C T                   |
\***************************************************************/

/*
	Tasks:
		0 - none
		1 - extract
		2 - update resources (rebuild resources list)
		3 - 1&2
		4 - use raw file if extraction
		5 - 1&4
*/

int extract(char* vFiledat,char* vDirExt, tResource* r[], char task) {

	//Variables
	char          vFileext[260];
	FILE*         fp;
  char          ok,pop1;

	if (ok=((fp=fopen(vFiledat,"rb"))!=NULL)) {
		//loop variables
		unsigned long int indexOffset;
		unsigned short int indexSize,numberOfItems;
		unsigned char* index;
		//verify dat format
		ok    = fread(&indexOffset,4,1,fp);
		//ok=ok&& fread(&val,2,1,fp);
		//ok=ok&& (!val);
		ok=ok&& fread(&indexSize,2,1,fp);
		ok=ok&& !fseek(fp,indexOffset,SEEK_SET);
		ok=ok&& fread(&numberOfItems,2,1,fp);
		pop1=((numberOfItems*8+2)==indexSize);
		int                ofk=0;

		if (!pop1) { //verify if pop2
			ofk=numberOfItems*6+2+(numberOfItems-2)*13;
			numberOfItems=((indexSize-6-(numberOfItems*6)-((numberOfItems-2)*13))/11);
			//printf("verificando pop2: numberOfItems=%d, indexSize=%d\r\n",numberOfItems,indexSize);
		}
		if (!ok) {
			fclose(fp);
			return -3; //this is not a valid prince dat file
		}
		if ((index=getMemory(indexSize-2))==NULL) {
			fclose(fp);
			return -2; //no memory
		}
		ok=fread(index,indexSize-2,1,fp);

		//if header ok, new variables
		unsigned short int id;
		unsigned long int  size,offset;
		unsigned char*     data;
		FILE*              target;
		char               type=0;
		char               recordSize=pop1?8:11;
		char               aux[260];
		tImage             image; //this is used to make a persistent palette
		char               isntImageSet=1;
		//parse folder and take dat filename in caps
		getUpperFolder(aux,vFiledat);

		//main loop
		for (int k=0;ok&&(k<numberOfItems);k++) {
			//for each archived file
			id=index[ofk+k*recordSize]+256*index[ofk+k*recordSize+1];
			offset=index[ofk+k*recordSize+2]+256*index[ofk+k*recordSize+3]+256*256*index[ofk+k*recordSize+4]+256*256*256*index[ofk+k*recordSize+5];
			size=index[ofk+k*recordSize+6]+256*index[ofk+k*recordSize+7]+1;
			if (!pop1) {
//				printf("jajaA %d\r\n",ok);
//printf("verificando pop2: %d, k=%d, record size=%d\r\n",size,k,recordSize);
				ok=ok&&(index[ofk+k*recordSize+8]==0x40)&&(!index[ofk+k*recordSize+9])&&(!index[ofk+k*recordSize+10]);
//				printf("jajaB %d\r\n",ok);
			}
			ok=ok&&((data=getMemory(size))!=NULL);
//				printf("jajaB %d\r\n",ok);
			ok=ok&&(!fseek(fp,offset,SEEK_SET));
//				printf("jajaB %d\r\n",ok);
			ok=ok&&fread(data,size,1,fp);
//				printf("jajaB %d\r\n",ok);
			if (!ok) return -3;

			//If rebuild option has been chosen, destroy previous declaration
			if ((r[id]!=NULL)&&(task&2)) {
				if ((*(r[id])).coms!=NULL) free ((*(r[id])).coms);
				if ((*(r[id])).desc!=NULL) free ((*(r[id])).desc);
				free(r[id]);
			}

			//set resource information on this index entry
			if (r[id]==NULL) {
				r[id]=(tResource*)malloc(sizeof(tResource));
				(*(r[id])).size=(unsigned short int)size;
				(*(r[id])).offset=offset;
				sprintf((*(r[id])).file,aux);
				(*(r[id])).desc=NULL;
				(*(r[id])).coms=NULL;
				(*(r[id])).type=verifyHeader(data,(unsigned short int)size);
			}

			if (task&1) {
				//select type
				if (task&4) (*(r[id])).type=0;
				//save file
				getFileName(vFileext,vDirExt,(*(r[id])).type,id);
				switch ((*(r[id])).type) {
					case 0:
						ok=ok&&((target=fopen(vFileext,"wb"))!=NULL);
						ok=ok&&fwrite(data+1,size-1,1,target); //Ignore checksum
						ok=ok&&(!fclose(target));
						break;
					case 6:

						if (1/*isntImageSet*/) {
							mLoadPalette(data,&image);
							isntImageSet=0;
						}
						//todo send to pal.c
						mExportPalette(&data,&size);
						//save palette
						ok=ok&&((target=fopen(vFileext,"wb"))!=NULL);
						ok=ok&&fwrite(data,size,1,target);
						ok=ok&&(!fclose(target));

						//until here
						break;
					case 1:
					case 5:
					case 4:
						//save file

						//TODO send to mid.c
						ok=ok&&((target=fopen(vFileext,"wb"))!=NULL);
						ok=ok&&fwrite(data+2,size-2,1,target); //Ignore checksum & sound type
						ok=ok&&(!fclose(target));

						//until here
						break;
					case 3: //save wav file
						ok=ok&&mFormatExtractWav(data,vFileext,size);
						break;
					case 2:
						//save image
						//Todo send to bmp.c
						if ((mExpandGraphic(data,&image,size))>0) {
							mWriteBitMap(image,vFileext);
							free(image.pix);
						}

						//until here
						break;
				}
			}
			if (data!=NULL) free(data);
		}
		fclose(fp);
		return ok;
	} else {
		return -1; //file could not be open
	}
}
