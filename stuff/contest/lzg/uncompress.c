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

/* A big number (the output must be less that that) */
#define LZG_MAX_MEMSIZE    32001

/* modulus to be used in the 10 bits of the algorithm */
#define LZG_WINDOW_SIZE    0x400 /* =1024=1<<10 */

/* LZG expansion algorithm sub function */
unsigned char popBit(unsigned char *byte) {
	unsigned char bit=(unsigned char)((*byte)&1);
	(*byte)>>=1;
	return bit;
}

/* Expands LZ Groody algorithm. This is the core of PR */
void expandLzg(const unsigned char* input, int inputSize, 
								unsigned char* output, int *outputSize) {
	char k;
	int location,cursor=0,pos=0;
	unsigned char maskbyte,rep;

	/* output is an unsigned char[] sized 0x400 */
	/* clean output garbage */
	for(location=LZG_MAX_MEMSIZE;location--;output[location]=0);

	/* main loop */
	while (pos<inputSize) {
		maskbyte=input[pos++];
		for (k=8;k&&(pos<inputSize);k--) {
			if (popBit(&maskbyte)) {
				output[cursor++]=input[pos++]; /* copy input to output */
			} else {
				/*
				 * location:
				 *  10 bits for the slide position (S). Add 66 to this number.
				 * rep:
				 *  6 bits for the repetition number (R). Add 3 to this number.
				 */
				location= 66 + ((input[pos] & 0x03 /*00000011*/) <<8) + input[pos+1];
				rep=      3  + ((input[pos] & 0xfc /*11111100*/) >>2);
				
				pos+=2;
				
				while (rep--) { /* repeat pattern in output */
					location=location%LZG_WINDOW_SIZE; /* location is in range 0-1023 */

					/*
					 * delta is ((location-cursor)%LZG_WINDOW_SIZE)
					 * this is the correction factor for the complex algorithm
					 * this value is inside the range -1023 to 1023.
					 * if location>cursor the result is positive
					 * if location<cursor the result is negative
					 */
					
					output[cursor]=output[cursor+((location-cursor)%LZG_WINDOW_SIZE)];

					/* Increase both variables */
					cursor++;
					location++;
				}
			}
		}
	}

	*outputSize=cursor;
}

/* END of page pasted part */

int main(int argc, char** argv) {
	/* declare variables */
	unsigned char uncompressed[LZG_MAX_MEMSIZE];
	unsigned char compressed[LZG_MAX_MEMSIZE];
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

