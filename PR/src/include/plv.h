
#ifndef _PLV_H_
#define _PLV_H_

#include <stdio.h>
#include "resources.h"

char mFormatCompilePlv(unsigned char* data, FILE* fp, tResource *res);
char mFormatExtractPlv(unsigned char* data, const char *vFileext,unsigned long int size,unsigned char level, const char* filename, const char* desc, const char* title);

//Dias de la semana y meses del anio
#define DATE_WEEKDAYS "Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat"
#define DATE_MONTHS   "Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec"

#define PLV_HEADER_A           "POP_LVL\x01\x01"
#define PLV_HEADER_A_SIZE      sizeof(PLV_HEADER_A)-1
#define PLV_HEADER_B           ":)"
#define PLV_HEADER_B_SIZE      sizeof(PLV_HEADER_B)-1
#define PLV_FOOT               "EDITORNAME\0PR\0EDITORVERS\0\x30.9\0AUTHOR\0Pr User\0TITLE"
#define PLV_FOOT_DESC          "DESCRIPTION"
#define PLV_FOOT_TCREAT        "TIMECREATED"
#define PLV_FOOT_TMODIF        "TIMELASTMODIF"
#define PLV_FOOT_ORIG_FILE     "ORIGINALFILENAME"
#define PLV_HEADER_SIZE_OFFSET 1


#endif
