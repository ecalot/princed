#ifndef _LZG_UNCOMPRESS_H_
#define _LZG_UNCOMPRESS_H_

int expandLzg(const unsigned char* input, int inputSize, 
              unsigned char* output, int *outputSize);

#endif
