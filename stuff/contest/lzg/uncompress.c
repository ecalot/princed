/*
 * This sample code is to test the LZG algorithm
 *
 * Compile with:
 *  gcc -o uncompress uncompress.c
 *
 * Usage:
 *  ./uncompress (input file) (output file)
 *
 * note that input file must be a valid LZG compressed file with
 * permission to be read and output file should be a nonexistent file
 * or it will be overwritten.
 *
 * This document is GPL
 */

#include <stdio.h>

/* BEGIN of page pasted part */

/* A big number */
#define MAX_MOD_SIZE_IN_LZG    32001

/* modulus to be used in the 10 bits of the algorithm */
#define MAX_MXD_SIZE_IN_LZG    0x400 /* 1024 */

/* LZG expansion algorithm sub function */
unsigned char popBit(unsigned char *byte) {
	unsigned char bit=(unsigned char)((*byte)&1);
	(*byte)>>=1;
	return bit;
}

/* Expands LZ Groody algorithm. This is the core of PR */
void expandLzg(const unsigned char* array, int arraySize, 
								unsigned char* img, int *imageSize) {
	char k;
	int location,h,cursor=0,pos=0;
	unsigned char maskbyte,rep;

	/* img is an unsigned char[] sized 0x400 */
	/* clean output garbage */
	for(location=0;location<MAX_MOD_SIZE_IN_LZG;img[location]=0,location++);

	/* main loop */
	while (pos<arraySize) {
		maskbyte=array[pos++];
		for (k=8;k&&(pos<arraySize);k--) {
			if (popBit(&maskbyte)) {
				img[cursor++]=array[pos++];
			} else {
				/*
				 * location:
				 *  10 bits for the slide position (S). Add 66 to this number.
				 * rep:
				 *  6 bits for the repetition number (R). Add 3 to this number.
				 */
				rep=      3  + ((array[pos] & 0xfc /*11111100*/) >>2);
				location= 66 + ((array[pos] & 0x03 /*00000011*/) <<8) + array[pos+1];
				
				pos+=2;
				
				/* Here is the difference between big and small images */
				while (rep--) {
					h=(cursor-(location%MAX_MXD_SIZE_IN_LZG))/MAX_MXD_SIZE_IN_LZG;
					/*
					 * if the image is stored in an array of 1024 x n bytes
					 * "h" is the height and "location" is the width
					 */
					img[cursor++]=img[
						((h<0)?0:h)*MAX_MXD_SIZE_IN_LZG+
						(location++)%MAX_MXD_SIZE_IN_LZG
					];
				}
			}
		}
	}

	*imageSize=cursor;
}

/* END of page pasted part */

int main(int argc, char** argv) {
	/* declare variables */
	unsigned char uncompressed[MAX_MOD_SIZE_IN_LZG];
	unsigned char compressed[MAX_MOD_SIZE_IN_LZG];
	int uncompressedSize;
	int compressedSize;
	FILE* fp;
				
	/* validate input */
	if (argc!=3) {
		fprintf(stderr,"Syntax: %s [cmp file] [target raw file]\n",*argv);
		return 1;
	}

	/* open file */
	fp=fopen(argv[1],"rb");
	if (!fp) {
		fprintf(stderr,"Error opening '%s'. Does it exist?\n",argv[1]);
		return 2;
	}

	/* read file to array */
	uncompressedSize=fread(uncompressed,1,sizeof(uncompressed),fp);
	
	/* close file */
	fclose(fp);

	/* compress file */
	expandLzg(uncompressed,uncompressedSize,compressed,&compressedSize);

	/* save results */
	fp=fopen(argv[2],"wb");	
	if (!fp) {
		fprintf(stderr,"Error opening '%s' for writting. Do you have permissions?\n",argv[2]);
		return 4;
	}
	fwrite(compressed,compressedSize,1,fp);
	fclose(fp);

	/* show results on screen */
	printf ("Results:\n Input file: %s (%d)\n Output file: %s (%d)\n",
		argv[1],uncompressedSize,argv[2],compressedSize);

	/* end */
	return 0;
}

