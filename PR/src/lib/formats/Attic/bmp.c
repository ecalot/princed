/*  Princed V3 - Prince of Persia Level Editor for PC Version
    Copyright (C) 2003 Princed Development Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    The authors of this program may be contacted at http://forum.princed.com.ar
*/

/*
bmp.c: Princed Resources : BMP file support
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "disk.h"
#include "memory.h"
#include "compile.h"

char mFormatExtractBmp(unsigned char* data, const char *vFileext,unsigned long int size,tImage image) {
	/*
		This function will expand the data into an image structure,
		then the bitmap structure will be saved to disk

		Note: The old structure is passed by paremeters in order to
		      keep the right palette.
	*/

	if ((mExpandGraphic(data,&image,size))>0) {
		//Write bitmap
		mWriteBitMap(image,vFileext);
		//free bitmap
		free(image.pix);
		return 1;
	} else {
		return 0;
	}
}


char mFormatCompileBmp(unsigned char* data, FILE* fp, tResource *res) {
	int size;
	tImage img;
	unsigned char aux[32700];

	if (!mReadBitMap(&img,data,res->size)) return 0;
	mCompressGraphic(aux,&img,&size);

	free(img.pix);

	mAddFileToDatFile(fp,aux,size);
	res->size=(unsigned short)size; //this was a bug (added to debug ;) ironic, don't you think?
	/* Note: after the debugging we realized this line was missing so this is not a bug anymore*/
	return 1;
}

char mWriteBitMap(tImage img,const char* vFile) {

	//declare variables
	char b;
	char c;
	char a;
	FILE* bitmap;
	unsigned long int filesize;
	unsigned long int width;
	unsigned long int height;
	char junk[3]={0,0,0};

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

	if (!writeOpen(vFile, &bitmap)) return 0;

	filesize=((img.size+1)/2+118);
	width=img.width;
	height=img.height;


	//TODO: avoid using header array andwrite everithing in one run

	header[2]=(unsigned char)(filesize);
	header[3]=(unsigned char)(filesize>>8);
	header[4]=(unsigned char)(filesize>>16);
	header[5]=(unsigned char)(filesize>>24);

	header[18]=(unsigned char)(width);
	header[19]=(unsigned char)(width>>8);

	header[22]=(unsigned char)(height);
	header[23]=(unsigned char)(height>>8);

	//BEGIN of format writing
	//Write ColorTable
	for (a=0;a<16;a++) {
		b=(char)(a*3);
		c=(char)(a<<2);
		header[54+c]=(unsigned char)(img.pal[b+2]*4);  //Red
		header[55+c]=(unsigned char)(img.pal[b+1]*4);  //Green
		header[56+c]=(unsigned char)(img.pal[b]*4);    //Blue
	}
	//Write header
	fwrite(header,118,1,bitmap);
	//Write data
	img.width=(img.width+1)>>1;
	while (img.height--) {
		fwrite(img.pix+img.height*img.width,img.width,1,bitmap);
		fwrite(junk,(-img.width)&3,1,bitmap);
	}
	//END of format writing

	fclose(bitmap);
	return 1;
}


char mReadBitMap(tImage* img,unsigned char* data, int size) {
	int ok;
	unsigned short int width;
	unsigned short int height;
	int width2;
	int x=0;

	//Validate if there is header and if it starts in BM
	ok    = size>118;
	ok=ok&& data[0]=='B' && data[1]=='M';

	//Read sizes from header
	width=(unsigned short)(data[18]+(data[19]<<8));
	height=(unsigned short)(data[22]+(data[23]<<8));

	//Save sizes into image
	(*img).width=width;            //width in pixels
	(*img).height=height;
	(*img).size=height*width;      //total of pixels

	//Calculate serialized widths
	width=(unsigned short)((width+1)>>1); //raw serialized width
	width2=width+((-width)&3);            //bmp serialized width

	//Validate image and file size; get memory to allocate the image
	ok=ok&& ((img->height*width2)==(size-118));
	ok=ok&& (	((*img).pix=getMemory((*img).size/2)) != NULL	);

	//if validations==wrong
	if (!ok) {
		if ((*img).pix!=NULL) free((*img).pix);
		return 0; //this is not a valid bmp file format or memory too low
	}

	//Serialize bitmap-->raw array
	while (height--) memcpy((*img).pix+(x++)*width,data+118+height*width2,width);

	return 1;
}
