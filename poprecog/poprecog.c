/*  Poprecog - Prince Of Persia Screenshots Recognizer
		Copyright (C) 2005 Princed Development Team

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
poprecog.c: Prince of Persia Screenshots Recognizer
¯¯¯¯¯¯¯¯¯¯

 Author: peter_k <peter@princed.com.ar>

*/

/*
	Changelog:
	- 2005.06.15 - I've started coding this pretty tool ;)
	- 2005.06.16 - Second day of coding (few hours)...
	- 2005.06.17 - Third day of coding (few hours)...  Some problems with POP1 compability.
	- ...
	- 2005.07.01 - :)
*/

/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
/* #include <qsort.h> */
#include <allegro.h>

/* Defines */
#ifndef WIN32
#define UNIX
#endif

#include <sys/types.h>
#include <sys/stat.h>

#ifndef WIN32
	#define defmkdir(a) mkdir (a,(mode_t)0755)
	#define osIndepGetCharacter() getchar()
	#define SEPS "/"
	#define SEPC '/'
#else
	#include <direct.h> /* mkdir */ 
	#define defmkdir(a) mkdir (a)
	#define osIndepGetCharacter() getche()
	#define SEPS "\\"
	#define SEPC '\\'
#endif

/* Text Defines */
#define MAX_DIRS 9999
#define MAX_SCREENSHOTS 99999
#define MAX_IMAGES 999999
#define MAX_RECOGNIZED_ITEMS 999999
#define NUMBER_OF_CONTROL_PIXELS 5

/* about */
#define POPRECOG_URL "http://www.princed.com.ar"
#define POPRECOG_ABOUT "\
Prince of Persia Screenshots Recognizer\n\
(c) Copyright 2005 Princed Development Team\n\
Programmed by peter_k\n" POPRECOG_URL "\n\n"

/* steps */
#define POPRECOG_STEP1 "\
Step 1. Type dir where screenshots are stored.\nThis should be 320x200 bmp files with 256 colour palette.\n"

#define POPRECOG_STEP2 "\n\
Step 2. Type dirs where are stored bitmaps to recognize.\n\
Please type in this format: [dirname] [max images on screenshot][ENTER].\n\
When you'll finish type END[ENTER].\n"

#define POPRECOG_STEP3 "\n\
Step 3. Type number of maximum layers\n\
HINT:\n\
If you'll type 0, poprecog will automatically detect the number of layers,\n\
but it is not recommended for tiles, walls, cinematic etc.\n"

#define POPRECOG_STEP4 "\n\
Step 4. Type the number of bitmap debug method\n\
 0 - don't create bitmap debug files\n\
 1 - colour recognized things (expext not important) + additional info\n\
 2 - show recognized things (expect not important) + additional info\n\
 4 - pink EVERY recognized thing (320x200)\n\
 8 - only show recognized (expect not important; 320x200)\n\
15 - create every bitmap file from top list\n"

#define POPRECOG_STEP5 "\n\
Step 5. Type output dir, in which you'll see recognized things\n"

/* other texts */
#define POPRECOG_RECOGNIZING         "\nRecognizing %s (shot %d of %d)\n"
#define POPRECOG_RECOGNIZING2        "Recognizing %s\n"
#define POPRECOG_CHECKING_LAYER      "Checking layer nr. %d"
#define POPRECOG_DONE                "Done"
#define POPRECOG_FOUND               "Found %s d %c l %d x %d y %d w %d h %d fpx %d fpy %d\n"
#define POPRECOG_SUMMARY             "%d recognized images in about %d seconds\n"
#define POPRECOG_ERROR_CANNOT_OPEN   "Cannot open %s for append\n"
#define POPRECOG_LOADING             "Loading bitmaps from dir %s\n"
#define POPRECOG_END_SUMMARY         "\nRecognized %d frames in about %ld seconds\n"	
#define POPRECOG_RELEASING_MEMORY    "Releasing memory\n"
#define POPRECOG_TYPE_STH            "Type something and press ENTER key...\n"
#define POPRECOG_DEBUG_HEADER        "%s (%d/%d)"
#define POPRECOG_DEBUG_RECOGNIZED_NR "Recognized number: %d"
#define POPRECOG_DEBUG_FIRST         "P(%d;%d) D%c L%d"
#define POPRECOG_DEBUG_SECOND        "D(%d;%d) Per:%d%%"

/* DONE: move the strings as text defines */
/* DONE: textprintf is deprecated, use its replacement */
/* DONE: separate structs variables and type definitions using typedef */

/* Types */
typedef struct sImage {
	BITMAP *bitmap;
	char *filePath;
	char direction;  
	int pixelsNumber;
	int dirID;  
} tImage;

typedef struct sRecognized {
	int imageID;
	int posX, posY;
	int goodPixels;
	int layer;
	int pixelsNumber;  
	int upperLayers;  
	int goodPixelsPercent;
	int ownedPixels; /* for recognizeMap */
} tRecognized;

typedef struct sDirInfo {
	char dirName[16];
	int recognizedNumber;
	int optTwoDirections;
	int optGoodNumber;
	int optMinImagePercent;
	int optAllowTransparency;
	/* int optVolatile; */
} tDirInfo;

typedef struct sSharedPixels {
	int recognizedID;
	int number;
} tSharedPixels;

typedef struct sCp
{
	int x, y;
	short c;
} tCp;

/* Global variables TODO: try to delete as much global variables as possible */ 
char *screenShotList[MAX_SCREENSHOTS];

int screenShotsNumber;
char screenShotsDir[100];

tImage image[MAX_IMAGES];
int imagesNumber;

BITMAP *screenShot, *transparentScreenShot;

tDirInfo dirInfo[MAX_DIRS];
int dirsNumber;

tRecognized recognized[MAX_RECOGNIZED_ITEMS];
int recognizedNumber;
tRecognized recognized2[MAX_RECOGNIZED_ITEMS];
int recognized2Number;
int totalNumberOfRecognizedImages;
int actualLayer;
int recognizeMap[320][200]; /* here are stored information which 'recognize result' have this pixel */

tSharedPixels sharedPixels[MAX_RECOGNIZED_ITEMS];

FILE *outputFile, *outputSmallFile;
char output[2000]; /* for outputFile */

char optResultsDir[100];
int optMaxLayers;
unsigned int optDebugMethod;
/* TODO: create a .h file and send all defines there */
#define colour_flag 0x0001
#define show_flag   0x0002
#define trans_flag  0x0004
#define movie_flag  0x0008
#define hasFlag(a) (optDebugMethod&(a))
#define setFlag(a) optDebugMethod|=(a)

/* Functions */

#define equalsIgnoreCase(a,b) (((a)==(b))||((('A'<=((a)&0xdf)&&((a)&0xdf)<='Z'))&& !((a)^(b))&0xdf))

int match(char *pat, char *str) {
	switch(*pat)
	{
		case '\0': return !*str;
		case '*': return match(pat + 1, str) || (*str && match(pat, str + 1));
		case '?': return *str && match(pat + 1, str + 1);
		default: return (equalsIgnoreCase(*pat,*str)) && match(pat + 1, str + 1);
	}
}

int pstrcmp(const void *p1, const void *p2) {
	return strcmp(*(char * const *)p1, *(char * const *)p2);
}

int cmptImage(const void *a, const void *b) {
	register int pxa = ((tImage*)(a))->pixelsNumber;
	register int pxb = ((tImage*)(b))->pixelsNumber;
	if (pxa > pxb)
		return -1;
	if (pxa < pxb)
		return 1;
	return 0;
}

int cmptRecognized(const void *a, const void *b)
{
	register int laa = ((tRecognized*)(a))->layer;
	register int lab = ((tRecognized*)(b))->layer; 
	register int pxa = ((tRecognized*)(a))->goodPixels;
	register int pxb = ((tRecognized*)(b))->goodPixels;
	if (laa > lab)
		return -1;
	if (laa < lab)
		return 1;
	if (pxa > pxb)
		return -1;
	if (pxa < pxb)
		return 1;
	return 0;
}

/*inline*/ int findImageOnScreenShotInPosition(int imageID, int posX, int posY) {
	register int i, x, y;
	register short transparentPixel = makecol16(0, 0, 0);
	register short screenShotTransparentPixel = makecol16(255, 0, 255);  
	register int goodPixels;
	BITMAP *bitmap = image[imageID].bitmap;  
	
	sharedPixels[0].recognizedID = -1;
	
	goodPixels = 0;
	for (x = 0; x < bitmap->w; x++)
		for (y = 0; y < bitmap->h; y++) {
			if ((((short *)bitmap->line[y])[x] == transparentPixel) && (dirInfo[image[imageID].dirID].optAllowTransparency))
				continue;
			if (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]) {
				if (recognizeMap[posX+x][posY+y] != -1) {
					for (i=0; sharedPixels[i].recognizedID != -1; i++) {
						if (sharedPixels[i].recognizedID == recognizeMap[posX+x][posY+y]) {
							if (sharedPixels[i].number >= 0) sharedPixels[i].number++;
							break;
						}
					}
					if (sharedPixels[i].recognizedID == -1) {
						sharedPixels[i].recognizedID = recognizeMap[posX+x][posY+y];
						sharedPixels[i].number = 1;
						sharedPixels[i+1].recognizedID = -1;            
					}
				} else {
					goodPixels++;
				}
				continue;
			}        
			if (((short *)transparentScreenShot->line[posY+y])[posX+x] == screenShotTransparentPixel)	{
				if (recognizeMap[posX+x][posY+y] != -1)	{
					for (i=0; sharedPixels[i].recognizedID != -1; i++) {
						if (sharedPixels[i].recognizedID == recognizeMap[posX+x][posY+y]) {
							if (sharedPixels[i].number >= 0) sharedPixels[i].number = -1;
							break;
						}
					}
					if (sharedPixels[i].recognizedID == -1)	{
						sharedPixels[i].recognizedID = recognizeMap[posX+x][posY+y];
						sharedPixels[i].number = -1;
						sharedPixels[i+1].recognizedID = -1;
					}
				} 
				continue;
			}  
			return 0;
		}
		
	for (i=0; sharedPixels[i].recognizedID != -1; i++) {
		if (sharedPixels[i].number > 0) goodPixels += sharedPixels[i].number;
	}
	
	if (((goodPixels*100)/image[imageID].pixelsNumber) < dirInfo[image[imageID].dirID].optMinImagePercent)
		return 0;

	return goodPixels;
}

/*inline*/ int putImageOnRecognizeMap(BITMAP *bitmap, int posX, int posY, int recognizedID) {
	register int x, y, value;
	register short transparentPixel = makecol16(0, 0, 0);
	
	for (x = 0; x < bitmap->w; x++)
		for (y = 0; y < bitmap->h; y++) {
			value = recognizeMap[posX+x][posY+y];
			if (((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) &&
					(((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x])) {
				if (value == -1) {
					recognizeMap[posX+x][posY+y] = recognizedID;
					recognized[recognizedID].ownedPixels++;
				}	else if (recognized[value].goodPixels < recognized[recognizedID].goodPixels) {
					recognized[value].ownedPixels--;
					recognizeMap[posX+x][posY+y] = recognizedID;
					recognized[recognizedID].ownedPixels++;
				}
			}
		}
	return recognized[recognizedID].ownedPixels;  
}

void cutImageFromScreenShot(BITMAP *screenShot2, int recognizedID) {
	register int x, y;
	register int transparentPixel = makecol16(0, 0, 0);
	register int screenShotTransparentPixel = makecol16(255, 0, 255);  
	BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;      
		
	for (x = 0; x < bitmap->w; x++)
		for (y = 0; y < bitmap->h; y++) {
			if (((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) && 
					(((short *)bitmap->line[y])[x] == ((short *)screenShot->line[recognized[recognizedID].posY+y])[recognized[recognizedID].posX+x]))
				((short *)screenShot2->line[recognized[recognizedID].posY+y])[recognized[recognizedID].posX+x] = screenShotTransparentPixel;
		}
}

void debugScreenShot0(BITMAP *debugScreenShot, int *debugScreenShotY, int recognizedID) {
	register int x, y;
	register int transparentPixel = makecol16(0, 0, 0);
	register int randomColour = makecol16(128+rand()%128, 128+rand()%128, 128+rand()%128);
	BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;
	
	if (*debugScreenShotY < 300)	{
		textprintf_ex(debugScreenShot, font, 320+2, 30+*debugScreenShotY, makecol(255, 255, 255), makecol(50, 50, 50), "%s", image[recognized[recognizedID].imageID].filePath);
		textprintf_ex(debugScreenShot, font, 320+2+MIN(bitmap->w, 50)+1, 30+9+*debugScreenShotY, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_FIRST, recognized[recognizedID].posX, recognized[recognizedID].posY, image[recognized[recognizedID].imageID].direction, recognized[recognizedID].layer);
		textprintf_ex(debugScreenShot, font, 320+2+MIN(bitmap->w, 50)+1, 30+18+*debugScreenShotY, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_SECOND, bitmap->w, bitmap->h, (recognized[recognizedID].goodPixels*100)/recognized[recognizedID].pixelsNumber);
	}  
		
	for (x = 0; x < bitmap->w; x++)
		for (y = 0; y < bitmap->h; y++) {
			if ((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) {
				((short *)debugScreenShot->line[recognized[recognizedID].posY+y])[recognized[recognizedID].posX+x] = randomColour;
			}
			if ((((short *)bitmap->line[y])[x] != transparentPixel) && (*debugScreenShotY < 300) && (x < 50))
				((short *)debugScreenShot->line[40+*debugScreenShotY+y])[320+2+x] = randomColour;
		}
	*debugScreenShotY += MAX(bitmap->h+20, 40);
}

void debugScreenShot1(BITMAP *debugScreenShot, int *debugScreenShotY, int recognizedID) {
	register int x, y;
	register int transparentPixel = makecol16(0, 0, 0);
	BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;
	
	if (*debugScreenShotY < 300)	{
		textprintf_ex(debugScreenShot, font, 320+2, 30+*debugScreenShotY, makecol(255, 255, 255), makecol(50, 50, 50), "%s", image[recognized[recognizedID].imageID].filePath);
		textprintf_ex(debugScreenShot, font, 320+2+MIN(bitmap->w, 50)+1, 30+9+*debugScreenShotY, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_FIRST, recognized[recognizedID].posX, recognized[recognizedID].posY, image[recognized[recognizedID].imageID].direction, recognized[recognizedID].layer);
		textprintf_ex(debugScreenShot, font, 320+2+MIN(bitmap->w, 50)+1, 30+18+*debugScreenShotY, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_SECOND, bitmap->w, bitmap->h, (recognized[recognizedID].goodPixels*100)/recognized[recognizedID].pixelsNumber);
	}  
		
	for (x = 0; x < bitmap->w; x++)
		for (y = 0; y < bitmap->h; y++) {
			if ((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) {
				((short *)debugScreenShot->line[recognized[recognizedID].posY+y])[recognized[recognizedID].posX+x] = ((short *)bitmap->line[y])[x];
			}
			if ((((short *)bitmap->line[y])[x] != transparentPixel) && (*debugScreenShotY < 300) && (x < 50))
				((short *)debugScreenShot->line[40+*debugScreenShotY+y])[320+2+x] = ((short *)bitmap->line[y])[x];
		}
	*debugScreenShotY += MAX(bitmap->h+20, 40);
}

void blit2(BITMAP *screenShot, int recognizedID) {
	register int x, y;
	register int transparentPixel = makecol16(0, 0, 0);
	BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;
	
	for (x = 0; x < bitmap->w; x++)
		for (y = 0; y < bitmap->h; y++) {
			if ((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) {
				((short *)screenShot->line[recognized[recognizedID].posY+y])[recognized[recognizedID].posX+x] = ((short *)bitmap->line[y])[x];
			}
		}
}

int findImageOnScreenShot(int imageID) {
	int numberOfRecognizedImages = 0;
	register int x, y;
	int i, alreadyExist;
	int posX, posY;
	int tmp;
	short transparentPixel = makecol(0, 0, 0);
	register short screenShotTransparentPixel = makecol(255, 0, 255);
	BITMAP *bitmapToFind = image[imageID].bitmap;
	int newRecognized;
	int continueLoop;
	/* DONE: use struct cp[] .x, .y and .c and a for */
	tCp cp[NUMBER_OF_CONTROL_PIXELS];
	
	for (i = 0; i < NUMBER_OF_CONTROL_PIXELS; i++)
	{
		do
		{
			cp[i].x = rand()%(bitmapToFind->w);
			cp[i].y = rand()%(bitmapToFind->h);
			cp[i].c = ((short *)bitmapToFind->line[cp[i].y])[cp[i].x];
		}
		while (cp[i].c == transparentPixel);
	}

	for (x = 0; x < 321-(bitmapToFind->w); x++)
		for (y = 0; y < 201-(bitmapToFind->h); y++)	{
			continueLoop = 0;
			for (i = 0; i < NUMBER_OF_CONTROL_PIXELS; i++)
			{
				if ((((short *)screenShot->line[y+cp[i].y])[x+cp[i].x] != cp[i].c) &&
				(((short *)transparentScreenShot->line[y+cp[i].y])[x+cp[i].x] != screenShotTransparentPixel)) {
					continueLoop = 1;
					break;
				}
			}
			if (continueLoop) continue;

			posX = x;
			posY = y;      
			
			tmp = findImageOnScreenShotInPosition(imageID, posX, posY);
			
			if (tmp) {
				newRecognized = recognizedNumber;
				alreadyExist = 0;
				for (i = 0; i < recognizedNumber; i++) {
					if ((recognized[i].imageID == imageID) &&
							(recognized[i].posX == posX) &&
							(recognized[i].posY == posY)) {
						alreadyExist = 1;
						newRecognized = i;
						break;
					}  
				}
				if ((!alreadyExist) && (dirInfo[image[imageID].dirID].optGoodNumber) && (dirInfo[image[imageID].dirID].recognizedNumber >= dirInfo[image[imageID].dirID].optGoodNumber))
					return 0;
				recognized[newRecognized].imageID = imageID;
				recognized[newRecognized].posX = posX;  
				recognized[newRecognized].posY = posY;
				recognized[newRecognized].goodPixels = tmp;
				recognized[newRecognized].goodPixelsPercent = (recognized[newRecognized].goodPixels*65536)/image[recognized[newRecognized].imageID].pixelsNumber;
				recognized[newRecognized].ownedPixels = 0;
				recognized[newRecognized].pixelsNumber = image[imageID].pixelsNumber;
				if (!alreadyExist) {
					recognized[newRecognized].layer = actualLayer;         
					recognizedNumber++;          
					dirInfo[image[imageID].dirID].recognizedNumber++;
				}  
			}  
		}
	return numberOfRecognizedImages;
}

void recognizeScreenShot(int screenShotID) {
	char buf[100];
	int x;  
	int i, j, tmp;
	int maxPixelsNumber, maxPixelsID, maxTotalPixelsNumber;
	int recognizedNow, recognizedBefore;
	int timeBefore, timeAfter;
	int everythingRecognized;
	/*int transparentPixel = makecol16(0, 0, 0);*/
	BITMAP *debugScreenShot[4];
	int debugScreenShotY[2];
 
	timeBefore = time(0);
	printf(POPRECOG_RECOGNIZING, screenShotList[screenShotID], screenShotID+1, screenShotsNumber);
	fprintf(outputFile, POPRECOG_RECOGNIZING, screenShotList[screenShotID], screenShotID+1, screenShotsNumber);
	fprintf(outputSmallFile, POPRECOG_RECOGNIZING2, screenShotList[screenShotID]);  
	sprintf(buf, "%s" SEPS "%s", screenShotsDir, screenShotList[screenShotID]);
	
	screenShot = load_bmp(buf, 0);
printf("poirot: debug %s\n",buf);
	if (!screenShot) return; /* bugfix: do exit in case of file not open */
	transparentScreenShot = load_bmp(buf, 0);  
	
	debugScreenShot[0] = create_bitmap(500, 400);
	debugScreenShotY[0] = 0;
	clear_to_color(debugScreenShot[0], makecol(50, 50, 50));
	blit(screenShot, debugScreenShot[0], 0, 0, 0, 200, 320, 200);
	line(debugScreenShot[0], 320, 0, 320, 399, makecol(255, 255, 255));	
	
	debugScreenShot[1] = create_bitmap(500, 400);
	debugScreenShotY[1] = 0;
	clear_to_color(debugScreenShot[1], makecol(50, 50, 50));
	blit(screenShot, debugScreenShot[1], 0, 0, 0, 200, 320, 200);
	line(debugScreenShot[1], 320, 0, 320, 399, makecol(255, 255, 255));		
  
  debugScreenShot[2] = load_bmp(buf, 0);

	debugScreenShot[3] = create_bitmap(320, 200);
	clear(debugScreenShot[3]);
  
	totalNumberOfRecognizedImages = 0;  
	recognizedNumber = 0;
	actualLayer = 0;
	for (i = 0; i < 320; i++)
		for (j = 0; j < 200; j++)
			recognizeMap[i][j] = -1;
	
	for (i = 0; i < dirsNumber; i++)
		dirInfo[i].recognizedNumber = 0;
		
	recognizedNow = 0;
	do {
		recognizedBefore = recognizedNow;
		recognizedNow = 0;
		if ((optMaxLayers != 0) && (actualLayer+1 > optMaxLayers)) break;   
	 
		recognizedBefore = recognizedNumber;   
		printf("    " POPRECOG_CHECKING_LAYER " ...     ", actualLayer);
		fprintf(outputFile, "    " POPRECOG_CHECKING_LAYER " ... " POPRECOG_DONE "\n", actualLayer);

		for (x = 0; x < imagesNumber; x++) {
			if ((!dirInfo[image[x].dirID].optGoodNumber) || (dirInfo[image[x].dirID].recognizedNumber < dirInfo[image[x].dirID].optGoodNumber)) {
				printf("\b\b\b\b% 3d%%", (x*100)/imagesNumber);
				findImageOnScreenShot(x);
			}
		}
		printf("\b\b\b\b" POPRECOG_DONE "\n");
		
		for (i = 0; i < 320; i++)
			for (j = 0; j < 200; j++)
				recognizeMap[i][j] = -1;
		
		for (i = 0; i < recognizedNumber; i++) {
			cutImageFromScreenShot(transparentScreenShot, i);
			putImageOnRecognizeMap(image[recognized[i].imageID].bitmap, recognized[i].posX, recognized[i].posY, i);
		}
		
		recognizedNow = recognizedNumber;
		
		everythingRecognized = 1;
		for (i = 0; i < dirsNumber; i++)
			if (dirInfo[i].recognizedNumber < dirInfo[i].optGoodNumber)
				everythingRecognized = 0;
		if (everythingRecognized)	break;
		
		actualLayer++;
	}
	while (recognizedNow - recognizedBefore != 0);
	
	for (i = 0; i < recognizedNumber; i++)
		recognized[i].ownedPixels = 0;  
		
	recognized2Number = 0;
	while (1)	{ /* TODO: use a structured loop here */
		maxPixelsNumber = 0;
		maxTotalPixelsNumber = 0;    
		maxPixelsID = -1;
		for (i = 0; i < recognizedNumber; i++) {
			if (recognized[i].goodPixels > maxPixelsNumber)	{
				maxTotalPixelsNumber = 0;
				maxPixelsNumber = recognized[i].goodPixels;
				maxPixelsID = i;
			}	else if ((recognized[i].goodPixels == maxPixelsNumber) && (recognized[i].pixelsNumber > maxTotalPixelsNumber)) {
				maxTotalPixelsNumber = recognized[i].pixelsNumber;
				maxPixelsID = i;
			}
		}
		if (maxPixelsNumber != 0)	{
			tmp = findImageOnScreenShotInPosition(recognized[maxPixelsID].imageID, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY);
			if (recognized[maxPixelsID].goodPixels != tmp) {
				recognized[maxPixelsID].goodPixels = tmp;
				continue;
			}
			
			if (dirInfo[image[recognized[maxPixelsID].imageID].dirID].optGoodNumber)
			{
				debugScreenShot0(debugScreenShot[0], &debugScreenShotY[0], maxPixelsID);
				debugScreenShot1(debugScreenShot[1], &debugScreenShotY[1], maxPixelsID);				
				blit2(debugScreenShot[3], maxPixelsID);
				totalNumberOfRecognizedImages++;
			}
			cutImageFromScreenShot(debugScreenShot[2], maxPixelsID);			
			cutImageFromScreenShot(screenShot, maxPixelsID);
			putImageOnRecognizeMap(image[recognized[maxPixelsID].imageID].bitmap, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY, maxPixelsID);
			
			recognized2[recognized2Number].imageID =  recognized[maxPixelsID].imageID;
			recognized2[recognized2Number].posX =  recognized[maxPixelsID].posX;
			recognized2[recognized2Number].posY =  recognized[maxPixelsID].posY;
			recognized2[recognized2Number].goodPixels =  recognized[maxPixelsID].goodPixels;
			recognized2[recognized2Number].layer =  recognized[maxPixelsID].layer;
			recognized2[recognized2Number].pixelsNumber =  recognized[maxPixelsID].pixelsNumber;
			recognized2[recognized2Number].upperLayers =  recognized[maxPixelsID].upperLayers;
			recognized2[recognized2Number].goodPixelsPercent =  recognized[maxPixelsID].goodPixelsPercent;
			recognized2[recognized2Number].ownedPixels =  recognized[maxPixelsID].ownedPixels;
			recognized2Number++;

			recognized[maxPixelsID].goodPixels = 0;
		}
		else
			break;
	}
	
	qsort(recognized2, recognized2Number, sizeof(tRecognized), cmptRecognized);
	
	/* Print results */
	
	for (i = 0; i < recognized2Number; i++) {
		printf("    " POPRECOG_FOUND,
			image[recognized2[i].imageID].filePath,
			image[recognized2[i].imageID].direction,
			recognized2[i].layer,
			recognized2[i].posX,
			recognized2[i].posY,
			image[recognized2[i].imageID].bitmap->w,
			image[recognized2[i].imageID].bitmap->h,
			(image[recognized2[i].imageID].direction=='-') ?
				(recognized2[i].posX) :
				(recognized2[i].posX+image[recognized2[i].imageID].bitmap->w
			),
			recognized2[i].posY+image[recognized2[i].imageID].bitmap->h
		);
		fprintf(outputFile, "    " POPRECOG_FOUND,
			image[recognized2[i].imageID].filePath,
			image[recognized2[i].imageID].direction,
			recognized2[i].layer,
			recognized2[i].posX,
			recognized2[i].posY,
			image[recognized2[i].imageID].bitmap->w,
			image[recognized2[i].imageID].bitmap->h,
			(image[recognized2[i].imageID].direction=='-') ?
				(recognized2[i].posX) :
				(recognized2[i].posX+image[recognized2[i].imageID].bitmap->w
			),
			recognized2[i].posY+image[recognized2[i].imageID].bitmap->h
		);
		fprintf(outputSmallFile, POPRECOG_FOUND,
			image[recognized2[i].imageID].filePath,
			image[recognized2[i].imageID].direction,
			recognized2[i].layer,
			recognized2[i].posX,
			recognized2[i].posY,
			image[recognized2[i].imageID].bitmap->w,
			image[recognized2[i].imageID].bitmap->h,
			(image[recognized2[i].imageID].direction=='-') ?
				(recognized2[i].posX) :
				(recognized2[i].posX+image[recognized2[i].imageID].bitmap->w
			),
			recognized2[i].posY+image[recognized2[i].imageID].bitmap->h
		);				
	}
	
	if (hasFlag(colour_flag)) {
		textprintf_ex(debugScreenShot[0], font, 320+2, 1, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_HEADER, screenShotList[screenShotID], screenShotID+1, screenShotsNumber);
		textprintf_ex(debugScreenShot[0], font, 320+2, 10, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_RECOGNIZED_NR, totalNumberOfRecognizedImages);
		sprintf(buf, "%s/colour_%s.bmp", optResultsDir, screenShotList[screenShotID]);
		save_bitmap(buf, debugScreenShot[0], 0);
	}
	if (hasFlag(show_flag)) {
		textprintf_ex(debugScreenShot[1], font, 320+2, 1, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_HEADER, screenShotList[screenShotID], screenShotID+1, screenShotsNumber);
		textprintf_ex(debugScreenShot[1], font, 320+2, 10, makecol(255, 255, 255), makecol(50, 50, 50), POPRECOG_DEBUG_RECOGNIZED_NR, totalNumberOfRecognizedImages);				
		sprintf(buf, "%s/show_%s.bmp", optResultsDir, screenShotList[screenShotID]);
		save_bitmap(buf, debugScreenShot[1], 0);
	}
	if (hasFlag(trans_flag)) {
		sprintf(buf, "%s/trans_%s.bmp", optResultsDir, screenShotList[screenShotID]);
		save_bitmap(buf, debugScreenShot[2], 0);
	}
	if (hasFlag(movie_flag)) {
		sprintf(buf, "%s/movie_%s.bmp", optResultsDir, screenShotList[screenShotID]);
		save_bitmap(buf, debugScreenShot[3], 0);
	}
	timeAfter = time(0);

	printf("    " POPRECOG_SUMMARY, totalNumberOfRecognizedImages, timeAfter - timeBefore);
	fprintf(outputFile, POPRECOG_SUMMARY, totalNumberOfRecognizedImages, timeAfter - timeBefore);
	destroy_bitmap(screenShot);
	destroy_bitmap(transparentScreenShot);  
	for (i = 0; i < 4; i++)
		destroy_bitmap(debugScreenShot[i]);
}

void sortListOfScreenShots() {
	char *tmpScreenShotsList[MAX_SCREENSHOTS];
	int tmpScreenShotsNumber;  
	int i;
	DIR *dir = opendir(screenShotsDir);
	struct dirent *file;
	
	tmpScreenShotsNumber = 0;
	while ((file = readdir(dir))) {
		if (match("*.bmp", file->d_name))	{
			tmpScreenShotsList[tmpScreenShotsNumber] = (char *) malloc(strlen(file->d_name)+1);
			strcpy(tmpScreenShotsList[tmpScreenShotsNumber], file->d_name);
			tmpScreenShotsNumber++;
		}  
	}  
	closedir(dir);
	
	qsort(tmpScreenShotsList, tmpScreenShotsNumber, sizeof(char *), pstrcmp);
	
	for (i = 0; i < tmpScreenShotsNumber; i++) /* TODO: why are you using a tmp structure where you can use the output structure? */ 
		screenShotList[i] = tmpScreenShotsList[i];
	screenShotsNumber = tmpScreenShotsNumber;
}

void freeListOfScreenShots() {
	int i;
	 
	for (i = 0; i < screenShotsNumber; i++)
		free(screenShotList[i]);
	screenShotsNumber = 0;
}

int countPixels(BITMAP *bitmap) {
	int x, y, number;
	int transparentPixel = makecol16(0, 0, 0);

	number = 0;    
	for (x = 0; x < bitmap->w; x++)
		for (y = 0; y < bitmap->h; y++)
			if (((short *)bitmap->line[y])[x] != transparentPixel)
				number++;
	return number;  
}

void readDir(int dirID) {
	char buf[100];
	/*int i;*/
	DIR *dir = opendir(dirInfo[dirID].dirName);
	struct dirent *file;
	FILE *optFile;
	
	sprintf(buf, "%s" SEPS "bitmaps.cfg", dirInfo[dirID].dirName);
	if ((optFile = fopen(buf, "r"))) {
		fscanf(optFile, "%d", &dirInfo[dirID].optTwoDirections);    
		fscanf(optFile, "%d", &dirInfo[dirID].optGoodNumber);
		fscanf(optFile, "%d", &dirInfo[dirID].optMinImagePercent);
		fscanf(optFile, "%d", &dirInfo[dirID].optAllowTransparency);
		fclose(optFile);
	}	else {
		dirInfo[dirID].optTwoDirections = 1;
		dirInfo[dirID].optGoodNumber = 99999;
		dirInfo[dirID].optMinImagePercent = 30;
		dirInfo[dirID].optAllowTransparency = 1;
	}  

	while ((file = readdir(dir)))	{
		if (match("*.bmp", file->d_name))	{
			strcpy(buf, dirInfo[dirID].dirName);
			strcat(buf, SEPS); 
			strcat(buf, file->d_name);
			image[imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
			strcpy(image[imagesNumber].filePath, buf);
			image[imagesNumber].bitmap = load_bmp(image[imagesNumber].filePath, 0);
printf("poirot: debug %s\n",image[imagesNumber].filePath);
			if (!image[imagesNumber].bitmap) continue; /* bugfix in case the image couldn't be loaded */
			image[imagesNumber].direction = '-';      
			image[imagesNumber].pixelsNumber = countPixels(image[imagesNumber].bitmap);
			image[imagesNumber].dirID = dirID;
			imagesNumber++;
			
			if (dirInfo[dirID].optTwoDirections) {      
				image[imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
				strcpy(image[imagesNumber].filePath, buf);
				image[imagesNumber].bitmap = create_bitmap(image[imagesNumber-1].bitmap->w, image[imagesNumber-1].bitmap->h);
				draw_sprite_h_flip(image[imagesNumber].bitmap, image[imagesNumber-1].bitmap, 0, 0);
				image[imagesNumber].direction = 'M';
				image[imagesNumber].pixelsNumber = image[imagesNumber-1].pixelsNumber;
				image[imagesNumber].dirID = dirID;        
				imagesNumber++;
			}
		}  
	}  
	closedir(dir);
}

void sortListOfImages() {
/*	int i;*/
 
	qsort(image, imagesNumber, sizeof(tImage), cmptImage);
}

void freeListOfImages() {
	int i;
	 
	for (i = 0; i < imagesNumber; i++) {
		free(image[i].filePath);
		destroy_bitmap(image[i].bitmap);
	}
	imagesNumber = 0;
}

void readParameters() {
	/*int i;*/

	strcpy(output, POPRECOG_ABOUT);
	printf(output);
	
	printf(POPRECOG_STEP1);
	strcat(output, POPRECOG_STEP1);
	scanf("%s", screenShotsDir);
	sprintf(output, "%s%s\n", output, screenShotsDir);
	
	printf(POPRECOG_STEP2);
	strcat(output, POPRECOG_STEP2);
	do {
		scanf("%s", dirInfo[dirsNumber].dirName);
		sprintf(output, "%s%s\n", output, dirInfo[dirsNumber].dirName);
		dirsNumber++;
		/* DONE: use a do/while with strcmp as condition */
	} while(strcmp(dirInfo[dirsNumber-1].dirName, "END"));
	dirsNumber--;
	
	printf(POPRECOG_STEP3);
	strcat(output, POPRECOG_STEP3);
	scanf("%d", &optMaxLayers);  
	sprintf(output, "%s%d\n", output, optMaxLayers);
	
	printf(POPRECOG_STEP4);
	strcat(output, POPRECOG_STEP4);
	scanf("%ud", &optDebugMethod);  
	sprintf(output, "%s%d\n", output, optDebugMethod);
	
	printf(POPRECOG_STEP5);
	strcat(output, POPRECOG_STEP5);
	scanf("%s", optResultsDir);
	sprintf(output, "%s%s\n", output, optResultsDir);
	
	printf("\n");
	strcat(output, "\n");  
}

int main(int argc, char* argv[]) {
	int i;
	long timeBefore, timeAfter;
	char buf[100];
	
	srand((unsigned int)time((time_t *)NULL));  
	
	allegro_init();
	set_color_depth(16);
	set_color_conversion(COLORCONV_TOTAL);
	
	readParameters();

	defmkdir(optResultsDir);
	
	sprintf(buf, "%s/%s", optResultsDir, "all_results.txt");
	outputFile = fopen(buf, "a");
 	if (!outputFile) {
			printf(POPRECOG_ERROR_CANNOT_OPEN,buf);
			return -1;
	}
	sprintf(buf, "%s/%s", optResultsDir, "small_results.txt");  
	outputSmallFile = fopen(buf, "a");
 	if (!outputFile) {
			printf(POPRECOG_ERROR_CANNOT_OPEN,buf);
			return -1;
	}
	fprintf(outputFile, output);

	for (i = 0; i < dirsNumber; i++) {
		printf(POPRECOG_LOADING, dirInfo[i].dirName);
		fprintf(outputFile, POPRECOG_LOADING, dirInfo[i].dirName);    
		readDir(i);
	}
	sortListOfImages();  
	sortListOfScreenShots();
	
	timeBefore = time(0);
	
	for (i = 0; i < screenShotsNumber; i++)  
		recognizeScreenShot(i);

	timeAfter = time(0);
	printf(POPRECOG_END_SUMMARY, screenShotsNumber, timeAfter - timeBefore);
	fprintf(outputFile, POPRECOG_END_SUMMARY, screenShotsNumber, timeAfter - timeBefore);
	
	printf(POPRECOG_RELEASING_MEMORY);
	fprintf(outputFile, POPRECOG_RELEASING_MEMORY);
	freeListOfScreenShots();
	freeListOfImages();  
	
	fclose(outputFile);
	fclose(outputSmallFile);

	printf(POPRECOG_TYPE_STH);
	scanf("\n");
	
	return 0;
}
END_OF_MAIN()
