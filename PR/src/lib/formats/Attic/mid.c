#include "mid.h"
#include "extract.h"

char mFormatExtractMid(unsigned char* data, char *vFileext,unsigned long int size) {
	//Ignore checksum & sound type
	return writeData(data,2,vFileext,size);
}
