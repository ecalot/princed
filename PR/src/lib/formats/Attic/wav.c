#include "wav.h"
#include "compile.h"


char mFormatExtractWav(unsigned char* data, char *vFileext,unsigned long int size) {

	unsigned char wav[]=WAVE_HEADER;
	FILE*         target;
	char ok;

						size-=2;
						ok=((target=fopen(vFileext,"wb"))!=NULL);
						wav[4]=(unsigned char)((size+36)&0xFF);
						wav[5]=(unsigned char)(((size+36)>>8)&0xFF);
						wav[6]=(unsigned char)(((size+36)>>16)&0xFF);
						wav[7]=(unsigned char)(((size+36)>>24)&0xFF);

						wav[40]=(unsigned char)((size)&0xFF);
						wav[41]=(unsigned char)(((size)>>8)&0xFF);
						wav[42]=(unsigned char)(((size)>>16)&0xFF);
						wav[43]=(unsigned char)(((size)>>24)&0xFF);
						ok=ok&&fwrite(wav,sizeof(wav),1,target);
						ok=ok&&fwrite(data+2,size-2,1,target);
						ok=ok&&(!fclose(target));

	return ok;
}

char mFormatCompileWav(unsigned char* data, FILE* fp, tResource *res) {
	unsigned char wav[]=WAVE_HEADER;
	int i=sizeof(wav);
	if ((*res).size<=i) return 0;
	(*res).size-=(--i);
	while ((i==4||i==5||i==6||i==7||i==40||i==41||i==42||i==43||(data[i]==wav[i]))&&(i--));
	data[sizeof(wav)-1]=1;
	if (i==-1) mAddFileToDatFile(fp,data+sizeof(wav)-1,(*res).size);
	return 1;
}
