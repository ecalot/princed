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
 * Source Author: Enrique Calot
 * Research: Tammo Jan Dijkemma, Anke Balderer, Enrique Calot
 */

#include <stdio.h>

/* BEGIN of page pasted part */

/* A big number (the output must be less that that) */
#define LZG_MAX_MEMSIZE    32001

/* modulus to be used in the 10 bits of the algorithm */
#define LZG_WINDOW_SIZE    0x400 /* =1024=1<<10 */

/* LZG expansion algorithm sub function */
unsigned char popBit(unsigned char *byte) {
	register unsigned char bit=(*byte)&1;
	(*byte)>>=1;
	return bit;
}

/* Expands LZ Groody algorithm. This is the core of PR */
void expandLzg(const unsigned char* input, int inputSize, 
								unsigned char* output, int *outputSize) {
	int           loc, oCursor=0, iCursor=0;
	unsigned char maskbyte, rep, k;

	/* clean output garbage */
	for(loc=LZG_MAX_MEMSIZE;loc--;output[loc]=0);

	/* main loop */
	while (iCursor<inputSize) {
		maskbyte=input[iCursor++];
		for (k=8;k&&(iCursor<inputSize);k--) {
			if (popBit(&maskbyte)) {
				output[oCursor++]=input[iCursor++]; /* copy input to output */
			} else {
				/*
				 * loc:
				 *  10 bits for the slide iCursorition (S). Add 66 to this number.
				 * rep:
				 *  6 bits for the repetition number (R). Add 3 to this number.
				 */
				loc= 66 + ((input[iCursor] & 0x03 /*00000011*/) <<8) + input[iCursor+1];
				rep= 3  + ((input[iCursor] & 0xfc /*11111100*/) >>2);
				
				iCursor+=2;
				
				while (rep--) { /* repeat pattern in output */
					loc=loc%LZG_WINDOW_SIZE; /* loc is in range 0-1023 */

					/*
					 * delta is ((loc-oCursor)%LZG_WINDOW_SIZE)
					 * this is the offset where the bytes will be looked for
					 * in the simple algorithm it is allways negative
					 * in bigger images it can be iCursoritive
					 * 
					 * this value is inside the range -1023 to 1023.
					 * if loc>oCursor the result is iCursoritive
					 * if loc<oCursor the result is negative
					 */
					
					output[oCursor]=output[oCursor+((loc-oCursor)%LZG_WINDOW_SIZE)];

					oCursor++;
					loc++;
				}
			}
		}
	}

	*outputSize=oCursor;
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

