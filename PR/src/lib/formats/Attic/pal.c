
#if DIR_SEPARATOR=='/'
#include "pal.h"
#else
#include "formats/pal.h"
#endif

/***************************************************************\
|                 Jasc Palette handling functions               |
\***************************************************************/

char mImportPalette(unsigned char** data, unsigned short *size) {
	//check size
	if (*size<130) return 0;

	//declare variables
	unsigned char palh[]=PAL_HEADER;
	unsigned char pals[]=PAL_SAMPLE;
	unsigned char* pal=getMemory(101);
	unsigned short int parsed;
	int i=0;

	//set palette with sample
	memcpy(pal,pals,101);

	//set current values
	while (palh[i]==(*data)[i++]);
	if (i!=sizeof(palh)) return 0;
	for (int k=0;k<16;k++) {
		getNumberToken((*data),&parsed,' ',&i,4);
		pal[(k*3)+5]=(parsed+2)>>2;
		getNumberToken((*data),&parsed,' ',&i,4);
		pal[(k*3)+6]=(parsed+2)>>2;
		getNumberToken((*data),&parsed,'\r',&i,4);
		pal[(k*3)+7]=(parsed+2)>>2;
		i++;
	}

	//free old data and set new
	free(*data);
	*data=pal;
	*size=101;
	return 1;
}

void mExportPalette(unsigned char** data, unsigned long int *size) {
	unsigned char* pal=getMemory(240);
	unsigned char* aux=getMemory(240);

	sprintf(pal,PAL_HEADER);

	unsigned char i;
	for (i=0;i<16;i++) {
		sprintf(aux,pal);
		sprintf(pal,"%s%d %d %d\r\n",aux,(*data)[(i*3)+5]<<2,(*data)[(i*3)+6]<<2,(*data)[(i*3)+7]<<2);
	}
	for (i=0;pal[i];i++);
	free(*data);
	free(aux);
	(*data)=pal;
	*size=i-1;
}

void mLoadPalette(char* array,tImage *image) {
	int k=0;
	for (int i=5;i<5+16*3;i++) {
		(*image).pal[k++]=array[i];
	}
}

