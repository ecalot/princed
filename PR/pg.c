/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

//reserved memory for the Lzx algorithm
#define MAX_MOD_SIZE_IN_LZX 32001
//38401
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
	int pos,h;
	unsigned char maskbyte,rep;
	for(pos=0;pos<MAX_MXD_SIZE_IN_LZX;(*img).pix[pos++]=0); //clean output garbage
	while (cursor<virtualSize) {
		maskbyte=array[(*i)++];
		for (char k=8;k&&(cursor<virtualSize);k--) {
			if (popBit(&maskbyte)) {
				(*img).pix[cursor++]=array[(*i)++];
			} else {
				pos=66+((0x100)*((rep=array[(*i)++])&3))+(unsigned char)array[(*i)++];
				rep=(rep>>2)+3;
				while (rep--) { //Be careful in big images
					h=cursor/MAX_MXD_SIZE_IN_LZX-(pos%MAX_MXD_SIZE_IN_LZX>cursor%MAX_MXD_SIZE_IN_LZX);
					(*img).pix[cursor++]=(*img).pix[((h<0)?0:h)*MAX_MXD_SIZE_IN_LZX+pos++%MAX_MXD_SIZE_IN_LZX];
				}
			}
		}
	}
}

//Compress B1/B2 algorithm
void compressRle(unsigned char* data,tImage* img,int *dataSize) {
  int cursorPix=0;
  int cursorData=0;

  int imgSize=((*img).size>>1)-1;
  char enthropyBlock=1;
  int  count=0;

  while (cursorPix<imgSize) {
		//Count different pixels
		if (data[cursorPix++]==data[cursorPix]) {
			if (enthropyBlock) { //TODO: add ||count>127
				enthropyBlock=0;
				//Print count . enthropy block (data[cursorPix-count..cursorPix])

				count=0;
			} else {
				count++;
			}
		} else {
			if (enthropyBlock) {  //TODO: add ||count>128
				count++;
			} else {
				enthropyBlock=1;
				//Print -count . unenthropy byte (data[cursorPix-1])

				count=0;
			}
		}
	}


/*

		if ((signed char)array[i]<0) {
					//negative
					while (array[i]++) (*image).pix[(cursor++)%((*image).size)]=array[i+1];
					i+=2;
				} else {
					//Positive
					char cx=array[i++]+1;
					while (cx--) (*image).pix[(cursor++)%((*image).size)]=array[i++];
				}
			}
			*/
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
	(*image).height=(unsigned char)array[i++]+256*(unsigned char)array[i++];
	(*image).width =(unsigned char)array[i++]+256*(unsigned char)array[i++];
	(*image).size  =(*image).height*(*image).width;
	virtualSize=((*image).height*((*image).width+((*image).width&1)))>>1;
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
int mCompressGraphic(unsigned char* a,tImage i, int* size) {
	*size=(i.size/2)+6;
	a=getMemory(*size);
	//height - 00 - width - 00 - 00 - compression type
	a[2]=i.width;
	a[3]=i.width>>8;

	a[0]=i.height;
	a[1]=i.height>>8;

	a[4]=0;
	a[5]=0xB0; // how q&d I am :)

	memcpy(i.pix,a+6,*size);
	return 1;
}

/***************************************************************\
|                         File handling                         |
\***************************************************************/

//Deprecated
/*
char mLoadFilePalette(char* vFile,int address, char* pal) {
	FILE *fp;
	char aux;

	if ((fp=fopen(vFile,"rb"))==NULL) {
		return 0;
	} else {
		fseek (fp, address, SEEK_SET);
		aux=fread (pal,SIZE_OF_PALETTE,1,fp);
		fclose(fp);
		return aux;
	}
}
*/
