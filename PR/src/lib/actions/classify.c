
#include <stdio.h>
#include "tasks.h"
#include "memory.h"

int prVerifyDatType(char* vFiledat) {

	//Variables
	FILE*         fp;
  char          ok;
  int 					k;

	//if header ok, new variables
	unsigned short int offset;
	unsigned long int  size;
	unsigned char*     data;
	unsigned char      type=5;

	//loop variables
	unsigned long int indexOffset;
	unsigned short int indexSize,val,numberOfItems;
	unsigned char* index;


	if (ok=((fp=fopen(vFiledat,"rb"))!=NULL)) {
		//verify dat format
		ok    = fread(&indexOffset,4,1,fp);
		ok=ok&& fread(&indexSize,2,1,fp);
		ok=ok&& !fseek(fp,indexOffset,SEEK_SET);
		ok=ok&& fread(&numberOfItems,2,1,fp);
		printf("jaaaj %d",ok);
		if (!ok) {
			fclose(fp);
			return 0; //this is not a valid prince dat file
		}
		if ((numberOfItems*8+2)!=indexSize) {
			printf("jajaj %d %d\r\n",indexOffset,indexSize);
			indexOffset+=indexSize;
			fseek(fp,0,SEEK_END);
			printf("jaaaj %d %d\r\n",indexOffset,ftell(fp));
			ok=(ftell(fp)==indexOffset)?11:0; //see if it is a pop2 file
			fclose(fp);
			return ok; //this is a pop2 dat file or invalid
		}
		if ((index=getMemory(indexSize-2))==NULL) {
			fclose(fp);
			return -2; //no memory
		}
		ok=fread(index,indexSize-2,1,fp);

		//main loop
		for (k=0;ok&&(k<numberOfItems)&&(type==5);k++) {
			//for each archived file
			ok=ok&&!(index[k*8+4]||index[k*8+5]);
			offset=index[k*8+2]+256*index[k*8+3];
			size=index[k*8+6]+256*index[k*8+7]+1;
			ok=ok&&((data=getMemory(size))!=NULL);
			ok=ok&&(!fseek(fp,offset,SEEK_SET));
			ok=ok&&fread(data,size,1,fp);

			//set resource information on this index entry
			type=verifyHeader(data,(unsigned short int)size);

		}
		fclose(fp);
		return ok?type:-1;
	} else {
		return -1; //file could not be open
	}
}

