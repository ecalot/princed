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
compress.c: Princed Resources : Image Compressor
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include <stdio.h>
#include "compress.h"
#include "memory.h"

//reserved memory for the Lzx algorithm
#define MAX_MOD_SIZE_IN_LZX 32001                     /* 38401 */
//modulus to be used in the 10 bits of the algorithm
#define MAX_MXD_SIZE_IN_LZX 0x400

/***************************************************************\
|                Compression algorithm handling                 |
\***************************************************************/

//Determines where the transposed byte must be saved
int transpose(int x,int n,int m) {
	return ((x%m)*((n+1)/2))+(int)(x/m);
}

//B3 and B4 expansion algorithm sub function
unsigned char popBit(unsigned char *byte) {
  unsigned char bit=(*byte)&1;
  (*byte)>>=1;
  return bit;
}

//Expands B3/B4 algorithm
void expandLzx(char* array,tImage* img, int *i,int cursor, int virtualSize) {
	char k;
	int pos,h;
	unsigned char maskbyte,rep;
//printf("expandLzx %d %d \n",virtualSize,cursor);

	for(pos=0;pos<MAX_MXD_SIZE_IN_LZX;(*img).pix[pos]=0,pos++); //clean output garbage
	while (cursor<virtualSize) {
		maskbyte=array[*i];(*i)++;
		for (k=8;k&&(cursor<virtualSize);k--) {
			if (popBit(&maskbyte)) {
				(*img).pix[cursor]=array[*i];(*i)++;cursor++;
			} else {
				pos=66+((0x100)*((rep=array[*i])&3))+(unsigned char)array[(*i)+1];(*i)+=2;
				rep=(rep>>2)+3;
				while (rep--) { //Be careful in big images
					h=cursor/MAX_MXD_SIZE_IN_LZX-(pos%MAX_MXD_SIZE_IN_LZX>cursor%MAX_MXD_SIZE_IN_LZX);
					(*img).pix[cursor]=(*img).pix[((h<0)?0:h)*MAX_MXD_SIZE_IN_LZX+pos%MAX_MXD_SIZE_IN_LZX];cursor++;pos++;
				}
			}
		}
	}
}

//Compress B1/B2 algorithm
void compressRle(unsigned char* data,tImage* img,int *dataSize) {
	//Declare pointers
  char* cursorData  = data;
  char* counter;
  char* cursorPix   = (*img).pix;
  char* imgEnd      = (*img).pix+((*img).size>>1)-1;

  while (cursorPix<imgEnd) {
		//Step 1: Create counter
		*(counter=(cursorData++))=-1;

		//Step 2: Look and copy the string until a repeated byte is found
		while ((cursorPix<imgEnd)&&(*cursorPix!=*(cursorPix+1))&&((*counter)!=127)) {
			cursorPix++;
			(*counter)++;
			*(cursorData)=*(cursorPix-1);
			cursorData++;
		}

		//Step 3: If there was a repeated string, let's ignore it and add the cursor with the repetitions
		if (*counter==-1) {
			while ((cursorPix<imgEnd)&&(*cursorPix==(*(cursorPix+1)))&&((*counter)!=-128)) {
				cursorPix++;
				(*counter)--;
			}
			cursorPix++; //TODO poner esta linea abajo
			*(cursorData)=*(cursorPix-1); //Print repeated char
			cursorData++;
		}
	}
	*(cursorData++)=0;
	*(cursorData++)=*(cursorPix);
	*dataSize=(int)cursorData-(int)data; //Note: data loss in 64 bits architectures
}

//Expands an array into an image
int mExpandGraphic(char* array,tImage *image, int virtualSize) {
	/*
		Reads array and extracts tImage
		returns the next image address or -1 in case of error

		Header info:
		 char checksum, short int height, short int width, (char)0, char compressionType
		 normaly: (* ignored types)
		 checksum* - height - 00 - width - 00 - 00* - compression type
	*/

  int cursor=0;
  int i=1;

  //Get memory for the image
	image->height=((unsigned char)array[(i)])+256*((unsigned char)array[(i+1)]) ;
	i=i+2;
	image->width =((unsigned char)array[(i)])+256*((unsigned char)array[(i+1)]);
	i=i+2;
	(*image).size  =(*image).height*(*image).width;
	virtualSize=(((*image).height*((*image).width+((*image).width&1)))>>1);
	i++;

  switch ((unsigned char)array[i++]) {
		case PG_COMP_RAW: //No Compression Algorithm
		  if (((*image).pix=getMemory(virtualSize))==NULL) return -1;
		  while (cursor<virtualSize) {
				(*image).pix[cursor++]=array[i++];
			}
			break;
		case PG_COMP_RLE_LR: //RLE Left to Right Compression Algorithm
		  if (((*image).pix=getMemory(virtualSize))==NULL) return -1;
		  while (cursor<virtualSize) {
				if ((signed char)array[i]<0) {
					//negative
					while (array[i]++) (*image).pix[(cursor++)]=array[i+1];
					i+=2;
				} else {
					//Positive
					char cx=array[i++]+1;
					while (cx--) (*image).pix[(cursor++)]=array[i++];
				}
			}
			break;
		case PG_COMP_RLE_UD: //RLE Up to Down Compression Algorithm
		  if (((*image).pix=getMemory(virtualSize))==NULL) return -1;
		  while (cursor<virtualSize) {
				if ((signed char)array[i]<0) {
					//negative
					while (array[i]++) (*image).pix[(transpose(cursor++,(*image).width,(*image).height))]=array[i+1];
					i+=2;
				} else {
					//Positive
					char cx=array[i++]+1;
					while (cx--) (*image).pix[transpose(cursor++,(*image).width,(*image).height)]=array[i++];
				}
			}
			break;
		case PG_COMP_LZX_LR: //LZ Groody Up to Down Version Compression Algorithm
		  if (((*image).pix=getMemory(MAX_MOD_SIZE_IN_LZX))==NULL) return -1;
			expandLzx(array,image,&i,cursor,virtualSize);
			break;
		case PG_COMP_LZX_UD: //LZ Groody Left to Right Version Compression Algorithm
		  if (((*image).pix=getMemory(MAX_MOD_SIZE_IN_LZX))==NULL) return -1;
			{
			unsigned char* outputaux=getMemory(virtualSize);
			expandLzx(array,image,&i,cursor,virtualSize);
			//Transpose
		  while (cursor<virtualSize) outputaux[transpose(cursor,(*image).width,(*image).height)]=(*image).pix[cursor++];
			free((*image).pix);
			(*image).pix=outputaux;
			}
			break;
		default:
		 return -1;
	}
	return i;
}

//Compress an image into an array in the most quick & dirty way
int mCompressGraphic(unsigned char* data,tImage i, int* size) {
	int dataSize;
	//unsigned char* a;
/*

	*size=(i.size/2)+6;
	a=getMemory(*size);
	//height - 00 - width - 00 - 00 - compression type
	a[2]=i.width;
	a[3]=i.width>>8;

	a[0]=i.height;
	a[1]=i.height>>8;

	a[4]=0;
	a[5]=0xB0; // how q&d I am :)

	memcpy(a+6,i.pix,*size); //TODO it should be a+6,i.pix
	*/
////printf("llega 3\n");
	//=getMemory(10*i.size+50); //This will reserve 10*2*(image size)+50 bytes, to allocate the compressed file
////printf("llega 3.3\n");
#if 1
	compressRle(data+6,&i,&dataSize);
//printf("mCompressGraphic: size=%d\n",dataSize);
	//a=getMemory(*size=(dataSize+6));
////printf("llega 4\n");
	/*
		Header Settings:
		height - 00 - width - 00 - 00 - compression type
	*/
	data[2]=i.width;
	data[3]=i.width>>8;

	data[0]=i.height;
	data[1]=i.height>>8;

	data[4]=0;
	data[5]=0xB1;

*size=(dataSize+6);
////printf("Tize: %d %02x %02x %02x %02x %02x %02x %02x\n",*size,a[0],a[1],a[2],a[3],a[4],a[5],a[6]);

#endif
	return 1;
}
