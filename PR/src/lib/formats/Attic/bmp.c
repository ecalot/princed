#if DIR_SEPARATOR=='/'
#include "bmp.h"
#else
#include "formats/bmp.h"
#endif

char mFormatCompileBmp(unsigned char* data, FILE* fp, tResource *res) {
	int size;
	tImage img;
	if (!mReadBitMap(&img,data,(*res).size)) return 0;
	free(data);
	mCompressGraphic(data,img,&size);
	free(img.pix);
	mAddFileToDatFile(fp,data,size);
	return 1;
}

char mWriteBitMap(tImage img,char* vFile) {
	//declare variables
	unsigned char i=0;
	unsigned char j=0;
	FILE* bitmap;
	if ((bitmap = fopen (vFile,"wb"))==NULL) return 0;

	unsigned long int filesize=((img.size+1)/2+118);
	unsigned long int width=img.width;
	unsigned long int height=img.height;

	unsigned char header[]={
		'B','M',
		0x22,0x22,0x22,0x22,
		0,0,0,0,
		0x76,0,0,0,

		40,0,0,0,
		0x22,0x22,0,0,
		0x22,0x22,0,0,
		1,0,
		4,0,
		0,0,0,0,
		0,0,0,0,
		0xE8,0x03,0,0,
		0xE8,0x03,0,0,
		0x10,0,0,0,
		0,0,0,0,

		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0,
		0x22,0x22,0x22,0
	};

	//TODO: avoid using header array andwrite everithing in one run

	header[2]=filesize;
	header[3]=filesize>>8;
	header[4]=filesize>>16;
	header[5]=filesize>>24;

	header[18]=width;
	header[19]=width>>8;

	header[22]=height;
	header[23]=height>>8;

//BEGIN of format writing
	//Write ColorTable
	char b;
	char c;
	for (char a=0;a<16;a++) {
		b=a*3;
		c=a<<2;
		header[54+c]=(img.pal[b+2])*4;  //Red
		header[55+c]=(img.pal[b+1])*4;  //Green
		header[56+c]=(img.pal[b])*4;    //Blue
	}
	//Write header
	fwrite(header,118,1,bitmap);
	//Write data
	char junk[3]={0,0,0};
	img.width=(img.width+1)>>1;
	while (img.height--) {
		fwrite(img.pix+img.height*img.width,img.width,1,bitmap);
		fwrite(junk,(-img.width)&3,1,bitmap);
	}
//END of format writing

	fclose(bitmap);
	return 1;
}


char mReadBitMap(tImage* img,char* data, int size) {
	//add validations
	//printf("tllego 1\r\n");

	char ok;
	unsigned short int width;
	unsigned short int height;

	ok    = size>118;
	ok=ok&& data[0]=='B' && data[1]=='M';

	width=data[18];
	width+=data[19]<<8;

	height=data[22];
	height+=data[23]<<8;

	(*img).width=width;
	(*img).height=height;
	(*img).size=height*width;
	ok=ok&& (((((*img).size))/2)==(size-118));
	ok=ok&& (
		(
			(*img).pix=getMemory(
				(((*img).size))/2
			)
		)!=NULL
	);

	if (!ok) {
		if ((*img).pix!=NULL) free((*img).pix);
		return 0; //this is not a valid bmp file format
	}

	width=(width+1)>>1;
	int width2=width+((-width)&3);

/*	while (img.height--) {
		for (int x=0;x<width;x++) {
			*(6+img.pix+height*img.width)=data+118+height*width2
		fwrite(img.pix+img.height*img.width,img.width,1,bitmap);
		fwrite(junk,(-img.width)&3,1,bitmap);
	}
*/
	int x=0;
	while (height--) {
		memcpy(6+(*img).pix+(x++)*(*img).width,data+118+height*width2,width);
	}



//	printf("tllego 4 %d %d %d\r\n",((*img).size)/2,size-118,(*img).pix);
//	memcpy(data+118,(*img).pix,(((*img).size)/2));
/*
img.width=img.width+(img.width&1);
for (int h=img.height-1;h>=0;--h) {
	for (int w=0;w<img.width;w++) {
		mBmpWritePixel(bitmap,mBmpGetPixel(img,w,h),&i,&j);
	}
	//align by 4
	for (int w=img.width;w&7;w++) {
		mBmpWritePixel(bitmap,5,&i,&j);
	}

}

*/


//	printf("tllego 5\r\n");


	return 1;
}
