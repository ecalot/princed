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
int expandLzg(const unsigned char* input, int inputSize, 
               unsigned char* output, int *outputSize) {

	int           loc, oCursor=0, iCursor=0;
	unsigned char maskbyte=0, rep, k;

	/* initialize the first 1024 bytes of the window with zeros */
	for(oCursor=0;oCursor<LZG_WINDOW_SIZE;output[oCursor++]=0);

	/* main loop */
	while (iCursor<inputSize) {
		maskbyte=input[iCursor++];
		for (k=8;k&&(iCursor<inputSize);k--) {
			if (popBit(&maskbyte)) {
				output[oCursor++]=input[iCursor++]; /* copy input to output */
printf("print %02x@%d from input(%d)\n",output[oCursor-1],oCursor-1025,iCursor-1);
			} else {
				/*
				 * loc:
				 *  10 bits for the slide iCursorition (S). Add 66 to this number,
				 *  substract the result to the current oCursor and take the last 10 bits.
				 * rep:
				 *  6 bits for the repetition number (R). Add 3 to this number.
				 */
				loc= 66 + ((input[iCursor] & 0x03 /*00000011*/) <<8) + input[iCursor+1];
				rep= 3  + ((input[iCursor] & 0xfc /*11111100*/) >>2);
			
				iCursor+=2; /* move the cursor 2 bytes ahead */
				
				loc=(oCursor-loc)&0x3ff; /* this is the real loc number (allways positive!) */
				
				while (rep--) { /* repeat pattern in output */
					output[oCursor]=output[oCursor-loc];

					printf("print %02x@%d copied from %d (loc=%d)\n",output[oCursor],oCursor-LZG_WINDOW_SIZE,oCursor-loc-LZG_WINDOW_SIZE,loc);

					oCursor++;
				}
			}
		}
	}
	
	/* ignore the first 1024 bytes */
	for(iCursor=LZG_WINDOW_SIZE;iCursor<oCursor;iCursor++)
		output[iCursor-LZG_WINDOW_SIZE]=output[iCursor];

	*outputSize=oCursor-LZG_WINDOW_SIZE;
	return maskbyte;
}

/* END of page pasted part */

int main(int argc, char** argv) {
	/* declare variables */
	unsigned char uncompressed[LZG_MAX_MEMSIZE];
	unsigned char compressed[LZG_MAX_MEMSIZE];
	int           uncompressedSize, compressedSize;
	FILE*         fp;
	int           result;
				
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
	result=expandLzg(uncompressed,uncompressedSize,compressed,&compressedSize);

	if (result) fprintf(stderr,"Maskbyte not clean, possible data corruption\n");
	
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

