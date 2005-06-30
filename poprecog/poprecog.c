/*
**  PRINCE OF PERSIA SCREENSHOTS RECOGNIZER
**  (c) Copyright 2005 Princed Development Team
**  Programmed by peter_k
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
//#include <qsort.h>
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
	#define SEPS "\\"
	#define SEPC '\\'
#else
	#include <direct.h> /* mkdir */ 
  #define defmkdir(a) mkdir (a)
	#define osIndepGetCharacter() getche()
	#define SEPS "/"
	#define SEPC '/'
#endif

/* Compile options */
#define MAX_DIRS 9999
#define MAX_SCREENSHOTS 99999
#define MAX_IMAGES 999999
#define MAX_RECOGNIZED_ITEMS 999999
#define NUMBER_OF_CONTROL_PIXELS 5 // do not edit this value now

/* Types and global variables */
struct sScreenShotList
{
  char *fileName;
} screenShotList[MAX_SCREENSHOTS];
int screenShotsNumber;
char screenShotsDir[100];

typedef struct sImage
{
  BITMAP *bitmap;
  char *filePath;
  char direction;  
  int pixelsNumber;
  int dirID;  
} tImage;

tImage image[MAX_IMAGES];
int imagesNumber;
//tImage tmpImage[MAX_IMAGES];
//int tmpImagesNumber;

BITMAP *screenShot, *transparentScreenShot, *DEBUGScreenShot, *DEBUGScreenShot2;

int DEBUGY = 0;

struct sDirInfo
{
  char dirName[16];
  int recognizedNumber;
  int optTwoDirections;
  int optMaxRecognizedNumber;
  int optMinImagePercent;
  int optAllowTransparency;
  //int optVolatile;
} dirInfo[MAX_DIRS];
int dirsNumber;

typedef struct sRecognized
{
  int imageID;
  int posX, posY;
  int goodPixels;
  int goodPixels2;  
  int layer;
  int pixelsNumber;  
  int upperLayers;  
  int goodPixelsPercent;
  int ownedPixels; // for recognizeMap
} tRecognized;
tRecognized recognized[MAX_RECOGNIZED_ITEMS];
int recognizedNumber;
int totalNumberOfRecognizedImages;

int actualLayer;
int recognizeMap[320][200]; // here are stored information which 'recognize result' have this pixel
struct sSharedPixels
{
  int recognizedID;
  int number;
} sharedPixels[MAX_RECOGNIZED_ITEMS];

FILE *outputFile, *outputSmallFile;

char optResultsDir[100];
int optMaxLayers;

/* Functions */

//#define MAX(A, B) (A > B) ? (A) : (B)

int match(char *pat, char *str)
{
  switch(*pat)
  {
    case '\0': return !*str;
    case '*': return match(pat + 1, str) || *str && match(pat, str + 1);
    case '?': return *str && match(pat + 1, str + 1);
    default: return *pat == *str && match(pat + 1, str + 1);
  }
}

int pstrcmp(const void *p1, const void *p2)
{
  return strcmp(*(char * const *)p1, *(char * const *)p2);
}

int cmptImage(void *a, void *b)
{
  register int pxa = ((tImage*)(a))->pixelsNumber;
  register int pxb = ((tImage*)(b))->pixelsNumber;
  if (pxa > pxb)
    return -1;
  if (pxa < pxb)
    return 1;
  return 0;
}

/*int cmptRecognized(void *a, void *b)
{
  register int pxa = ((tRecognized*)(a))->goodPixelsPercent;
  register int pxb = ((tRecognized*)(b))->goodPixelsPercent;
  if (pxa>pxb)
    return -1;
  if (pxa<pxb)
    return 1;
  return 0;
}*/

int cmptRecognized(void *a, void *b)
{
  register int laa = ((tRecognized*)(a))->layer;
  register int lab = ((tRecognized*)(b))->layer; 
  register int pxa = ((tRecognized*)(a))->goodPixels;
  register int pxb = ((tRecognized*)(b))->goodPixels;
  register int ula = ((tRecognized*)(a))->upperLayers;
  register int ulb = ((tRecognized*)(b))->upperLayers;  
  register int pna = ((tRecognized*)(a))->pixelsNumber;
  register int pnb = ((tRecognized*)(b))->pixelsNumber;  
  /*if (laa>lab)
    return -1;
  if (laa<lab)
    return 1;*/
  if (pxa>pxb)
    return -1;
  if (pxa<pxb)
    return 1;
  /*if (ula>ulb)
    return 1;
  if (ula<ulb)
    return -1;*/
  if (pna>pnb)
    return -1;
  if (pna<pnb)
    return 1;    
  return 0;
}

/*inline int findImageOnScreenShotInPosition(int imageID, int posX, int posY)
{
  register int i, j, x, y, value;
  register short transparentPixel = makecol16(0, 0, 0);
  register short screenShotTransparentPixel = makecol16(255, 0, 255);  
  register int goodPixels;
  BITMAP *bitmap = image[imageID].bitmap;  
  register int wPer2 = bitmap->w / 2;
  register int hPer2 = bitmap->h / 2;
  int alreadyExist, newRecognized = 0;
  
  for (i = 0; i < recognizedNumber; i++)
    layerOrder[i] = 0;
  
  goodPixels = 0;
  for (i = 0; i < bitmap->w; i++)
    for (j = 0; j < bitmap->h; j++)  
    {
      x = (i+wPer2)%bitmap->w;
      y = (j+hPer2)%bitmap->h;
      if (((short *)bitmap->line[y])[x] == transparentPixel)
        continue;
      if (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x])
      {
        if (recognizeMap[posX+x][posY+y] != -1)
        {
          if (layerOrder[recognizeMap[posX+x][posY+y]] >= 0) layerOrder[recognizeMap[posX+x][posY+y]]++;
        }  
        else
        {
          goodPixels++;
        }
        continue;
      }        
      if (((short *)transparentScreenShot->line[posY+y])[posX+x] == screenShotTransparentPixel)
      {
        if (recognizeMap[posX+x][posY+y] != -1) layerOrder[recognizeMap[posX+x][posY+y]] = -1;
        continue;
      }  
      return 0;
    }  

  newRecognized = recognizedNumber;
  alreadyExist = 0;  
  for (i = 0; i < recognizedNumber; i++)
  {
    if ((recognized[i].imageID == imageID) &&
        (recognized[i].posX == posX) &&
        (recognized[i].posY == posY))
      {  
        newRecognized = i;
        alreadyExist = 1;        
        break;
      }  
  }

  recognized[newRecognized].upperLayers = 0;    
  for (i = 0; i < newRecognized; i++)
    //if ((layerOrder[i] > 0) && (image[recognized[i].imageID].pixelsNumber > image[imageID].pixelsNumber))
    //if ((layerOrder[i] > 0) && ((recognized[i].layer > actualLayer) || (layerOrder[i] == image[recognized[i].imageID].pixelsNumber)))
    if (layerOrder[i] > 0)
      goodPixels += layerOrder[i];
    else
    if (layerOrder[i] < 0)
      recognized[newRecognized].upperLayers++;
      
  if (((goodPixels*100)/image[imageID].pixelsNumber) < dirInfo[image[imageID].dirID].optMinImagePercent) return 0;
  
  if ((!strcmp(image[imageID].filePath, "pop1swords\\image00714.bmp")) && (posY == 32))
  {
    printf("%03d Maybe %s (%d,%d) -> goodPixels = %d (max %d)\n", goodPixels, image[imageID].filePath, posX, posY, goodPixels, image[imageID].pixelsNumber);
    rest(200);
  }  
//   dir[image[imageID].dirID].newRecognized++;  
  ///printf("Added!\n");
  //return goodPixels;
  
  recognized[newRecognized].imageID = imageID;
  recognized[newRecognized].posX = posX;  
  recognized[newRecognized].posY = posY;
  recognized[newRecognized].goodPixels = goodPixels;
  recognized[newRecognized].pixelsNumber = image[imageID].pixelsNumber;
  recognized[newRecognized].goodPixelsPercent = (recognized[newRecognized].goodPixels*100)/image[imageID].pixelsNumber;
  recognized[newRecognized].ownedPixels = 0;
  if (!alreadyExist) recognized[newRecognized].layer = actualLayer;
  
    if (!alreadyExist)
    {
      recognizedNumber++;
      dirInfo[image[imageID].dirID].recognizedNumber++; 
    }
    return recognized[newRecognized].ownedPixels;
}*/

inline int findImageOnScreenShotInPosition(int imageID, int posX, int posY)
{
  register int i, j, x, y;
  register short transparentPixel = makecol16(0, 0, 0);
  register short screenShotTransparentPixel = makecol16(255, 0, 255);  
  register int goodPixels;
  BITMAP *bitmap = image[imageID].bitmap;  
  register int wPer2 = bitmap->w / 2;
  register int hPer2 = bitmap->h / 2;
  
  sharedPixels[0].recognizedID = -1;
  
  goodPixels = 0;
  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      if ((((short *)bitmap->line[y])[x] == transparentPixel) && (dirInfo[image[imageID].dirID].optAllowTransparency))
        continue;
      if (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x])
      {
        if (recognizeMap[posX+x][posY+y] != -1)
        {
          for (i=0; sharedPixels[i].recognizedID != -1; i++)
          {
            if (sharedPixels[i].recognizedID == recognizeMap[posX+x][posY+y])
            {
              if (sharedPixels[i].number >= 0) sharedPixels[i].number++;
              break;
            }
          }
          if (sharedPixels[i].recognizedID == -1)
          {
            sharedPixels[i].recognizedID = recognizeMap[posX+x][posY+y];
            sharedPixels[i].number = 1;
            sharedPixels[i+1].recognizedID = -1;            
          }
        }  
        else
        {
          goodPixels++;
        }
        continue;
      }        
      if (((short *)transparentScreenShot->line[posY+y])[posX+x] == screenShotTransparentPixel)
      {
        if (recognizeMap[posX+x][posY+y] != -1)
        {
          for (i=0; sharedPixels[i].recognizedID != -1; i++)
          {
            if (sharedPixels[i].recognizedID == recognizeMap[posX+x][posY+y])
            {
              if (sharedPixels[i].number >= 0) sharedPixels[i].number = -1;
              break;
            }
          }
          if (sharedPixels[i].recognizedID == -1)
          {
            sharedPixels[i].recognizedID = recognizeMap[posX+x][posY+y];
            sharedPixels[i].number = -1;
            sharedPixels[i+1].recognizedID = -1;
          }
        } 
        continue;
      }  
      return 0;
    }
    
  for (i=0; sharedPixels[i].recognizedID != -1; i++)
  {
    if (sharedPixels[i].number > 0) goodPixels += sharedPixels[i].number;
  }
  
  if (((goodPixels*100)/image[imageID].pixelsNumber) < dirInfo[image[imageID].dirID].optMinImagePercent) return 0;

  return goodPixels;
}

inline int putImageOnRecognizeMap(BITMAP *bitmap, int posX, int posY, int recognizedID)
{
  register int x, y, value;
  register short transparentPixel = makecol16(0, 0, 0);
  register short screenShotTransparentPixel = makecol16(255, 0, 255);
  
  //return 13;
  
/*  int lastRecognizedID = -1, OK = 0;
  
  for (x = 0; x < bitmap->w; x++)
  {
    for (y = 0; y < bitmap->h; y++)  
    {
      value = recognizeMap[posX+x][posY+y];
      if ((((short *)bitmap->line[y])[x] != transparentPixel) &&       
          (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]))
      {
        if (value == -1)
        {
        }
        else
        if (recognized[value].goodPixelsPercent < recognized[recognizedID].goodPixelsPercent)
        {
          if (lastRecognizedID == 0)
            lastRecognizedID = value;
          else
          if (lastRecognizedID != value)
          {
            OK = 1;
            break;
          }
        }
      }
    }
    if (OK) break;
  }
  if (lastRecognizedID == -1) OK = 1;
    
  if (!OK) return 0;*/

  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      value = recognizeMap[posX+x][posY+y];
      if (((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) &&
          (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]))
      {
        if (value == -1)
        {
          recognizeMap[posX+x][posY+y] = recognizedID;
          recognized[recognizedID].ownedPixels++;
        }
        else
        if (recognized[value].goodPixels < recognized[recognizedID].goodPixels)
        {
          recognized[value].ownedPixels--;
          recognizeMap[posX+x][posY+y] = recognizedID;
          recognized[recognizedID].ownedPixels++;
        }  
      }
    }
 ///printf("Recognize map result-> %d\n", recognized[recognizedID].ownedPixels);
  return recognized[recognizedID].ownedPixels;  
}

inline int putAndCheck(int recognizedID, int posX, int posY)
{
  register int x, y, value;
  register short transparentPixel = makecol16(0, 0, 0);
  register short screenShotTransparentPixel = makecol16(255, 0, 255);
  BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;
  int newPixels = 0;
  
  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      value = recognizeMap[posX+x][posY+y];
      if ((((short *)bitmap->line[y])[x] != transparentPixel) &&       
          (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]))
      {
        if (recognized[value].layer == recognized[recognizedID].layer) return 0;
        if ((value == -1) || (value == recognizedID))
        {
          recognizeMap[posX+x][posY+y] = recognizedID;
          recognized[recognizedID].ownedPixels++;
          newPixels++;          
        }
      }
    }    
   
  ///printf("Recognize map result-> %d\n", recognized[recognizedID].ownedPixels);
  if (newPixels > 0)
    return 1;
  else
    return 0;
  //return recognized[recognizedID].ownedPixels;  
}

int removeImageFromRecognizeMap(int recognizedID, int posX, int posY)
{
  register int x, y, value;
  register short transparentPixel = makecol16(0, 0, 0);
  register short screenShotTransparentPixel = makecol16(255, 0, 255);
  BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;  

  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      if ((((short *)bitmap->line[y])[x] != transparentPixel) &&       
          (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]))
        recognizeMap[posX+x][posY+y] = recognizedID;
    }  
}

int cutImageFromTransparentScreenShot(int recognizedID, int posX, int posY)
{
  register int x, y, c;
  register int transparentPixel = makecol16(0, 0, 0);
  register int screenShotTransparentPixel = makecol16(255, 0, 255);  
  char buf[100];
  BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;    
    
  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      if (((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) && 
          (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]))
        ((short *)transparentScreenShot->line[posY+y])[posX+x] = screenShotTransparentPixel;
    }
  //sprintf(buf, "ss_%05d.bmp", rand()%9999);
  //save_bitmap(buf, actualScreenShot, 0);    
  //rest(500);
}

int cutImageFromScreenShot(int recognizedID, int posX, int posY)
{
  register int x, y, c;
  register int transparentPixel = makecol16(0, 0, 0);
  register int screenShotTransparentPixel = makecol16(255, 0, 255);  
  char buf[100];
  BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;      
    
  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      if (((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) && 
          (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]))
        ((short *)screenShot->line[posY+y])[posX+x] = screenShotTransparentPixel;
    }
  //sprintf(buf, "ss_%05d.bmp", rand()%9999);
  //save_bitmap(buf, actualScreenShot, 0);    
  //rest(500);
}

int cutImageFromDEBUGScreenShot(int recognizedID, int posX, int posY)
{
  register int x, y, c, i;
  register int transparentPixel = makecol16(0, 0, 0);
  register int randomColour = makecol16(128+rand()%128, 128+rand()%128, 128+rand()%128);
  char buf[100];
  BITMAP *bitmap = image[recognized[recognizedID].imageID].bitmap;
  
  if (DEBUGY < 300)
  {
    textprintf(DEBUGScreenShot, font, 320+2, 30+DEBUGY, makecol(255, 255, 255), "%s", image[recognized[recognizedID].imageID].filePath);
    textprintf(DEBUGScreenShot, font, 320+2+MIN(bitmap->w, 50)+1, 30+9+DEBUGY, makecol(255, 255, 255), "P(%d;%d) D%c L%d", recognized[recognizedID].posX, recognized[recognizedID].posY, image[recognized[recognizedID].imageID].direction, recognized[recognizedID].layer);
    textprintf(DEBUGScreenShot, font, 320+2+MIN(bitmap->w, 50)+1, 30+18+DEBUGY, makecol(255, 255, 255), "D(%d;%d) Per:%d%%", bitmap->w, bitmap->h, (recognized[recognizedID].goodPixels*100)/recognized[recognizedID].pixelsNumber);
  }  
    
  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      if (((((short *)bitmap->line[y])[x] != transparentPixel) || (!dirInfo[image[recognized[recognizedID].imageID].dirID].optAllowTransparency)) && 
          (((short *)bitmap->line[y])[x] == ((short *)screenShot->line[posY+y])[posX+x]))
      {  
        ((short *)DEBUGScreenShot->line[posY+y])[posX+x] = randomColour;
      }
      if ((((short *)bitmap->line[y])[x] != transparentPixel) && (DEBUGY < 300) && (x < 50))
        ((short *)DEBUGScreenShot->line[40+DEBUGY+y])[320+2+x] = randomColour;
    }
  DEBUGY += MAX(bitmap->h+20, 40);
  //sprintf(buf, "ss_%05d.bmp", rand()%9999);
  //save_bitmap(buf, actualScreenShot, 0);    
  //rest(500);
}

int findImageOnScreenShot(int imageID)
{
  int numberOfRecognizedImages = 0;
  register int x, y;
  int i, alreadyExist;
  int posX, posY;
  int tmp;
  /*struct sControlPixel
  {
    int posX, posY, color;
  } controlPixel[NUMBER_OF_CONTROL_PIXELS];*/
  short transparentPixel = makecol(0, 0, 0);
  register short screenShotTransparentPixel = makecol(255, 0, 255);
  BITMAP *bitmapToFind = image[imageID].bitmap;
  int cp1x; int cp1y; short cp1c;
  int cp2x; int cp2y; short cp2c;
  int cp3x; int cp3y; short cp3c;
  int cp4x; int cp4y; short cp4c;
  int cp5x; int cp5y; short cp5c;
  int newRecognized;

/*  if (((short *)actualScreenShot->line[0])[0] == screenShotTransparentPixel)
    printf(".");
  else
    printf("#");  */
    
  do {
    cp1x = rand()%(bitmapToFind->w);
    cp1y = rand()%(bitmapToFind->h);
    cp1c = ((short *)bitmapToFind->line[cp1y])[cp1x];   
  } while (cp1c == transparentPixel);
  do {
    cp2x = rand()%(bitmapToFind->w);
    cp2y = rand()%(bitmapToFind->h);
    cp2c = ((short *)bitmapToFind->line[cp2y])[cp2x];   
  } while (cp2c == transparentPixel);
  do {
    cp3x = rand()%(bitmapToFind->w);
    cp3y = rand()%(bitmapToFind->h);
    cp3c = ((short *)bitmapToFind->line[cp3y])[cp3x];   
  } while (cp3c == transparentPixel);    
  do {
    cp4x = rand()%(bitmapToFind->w);
    cp4y = rand()%(bitmapToFind->h);
    cp4c = ((short *)bitmapToFind->line[cp4y])[cp4x];   
  } while (cp4c == transparentPixel);    
  do {
    cp5x = rand()%(bitmapToFind->w);
    cp5y = rand()%(bitmapToFind->h);
    cp5c = ((short *)bitmapToFind->line[cp5y])[cp5x];   
  } while (cp5c == transparentPixel);        
 
/*  for (i = 0; i < NUMBER_OF_CONTROL_PIXELS; i++)
  {
    do
    {
      controlPixel[i].posX = rand()%(bitmapToFind->w);
      controlPixel[i].posY = rand()%(bitmapToFind->h);
      controlPixel[i].color = ((short *)bitmapToFind->line[controlPixel[i].posY])[controlPixel[i].posX];
    }
    while (controlPixel[i].color == transparentPixel);
  }*/
  
  for (x = 0; x < 321-(bitmapToFind->w); x++)
    for (y = 0; y < 201-(bitmapToFind->h); y++)
    {
       if ((((short *)screenShot->line[y+cp1y])[x+cp1x] != cp1c) &&
           (((short *)transparentScreenShot->line[y+cp1y])[x+cp1x] != screenShotTransparentPixel))
        continue;                          
       if ((((short *)screenShot->line[y+cp2y])[x+cp2x] != cp2c) &&
           (((short *)transparentScreenShot->line[y+cp2y])[x+cp2x] != screenShotTransparentPixel))
        continue;                          
       if ((((short *)screenShot->line[y+cp3y])[x+cp3x] != cp3c) &&
           (((short *)transparentScreenShot->line[y+cp3y])[x+cp3x] != screenShotTransparentPixel))
        continue;                                          
       if ((((short *)screenShot->line[y+cp4y])[x+cp4x] != cp4c) &&
           (((short *)transparentScreenShot->line[y+cp4y])[x+cp4x] != screenShotTransparentPixel))
        continue;                                                  
       if ((((short *)screenShot->line[y+cp5y])[x+cp5x] != cp5c) &&
           (((short *)transparentScreenShot->line[y+cp5y])[x+cp5x] != screenShotTransparentPixel))
        continue;                                                  
/*       if ((((short *)actualScreenShot->line[y + controlPixel[0].posY])[x + controlPixel[0].posX] != controlPixel[0].color) &&
           (((short *)actualScreenShot->line[y + controlPixel[0].posY])[x + controlPixel[0].posX] != screenShotTransparentPixel))
        continue;                     
      if ((((short *)actualScreenShot->line[y + controlPixel[1].posY])[x + controlPixel[1].posX] != controlPixel[1].color) &&
          (((short *)actualScreenShot->line[y + controlPixel[1].posY])[x + controlPixel[1].posX] != screenShotTransparentPixel))
        continue;
      if ((((short *)actualScreenShot->line[y + controlPixel[2].posY])[x + controlPixel[2].posX] != controlPixel[2].color) &&
          (((short *)actualScreenShot->line[y + controlPixel[2].posY])[x + controlPixel[2].posX] != screenShotTransparentPixel))
        continue;
      if ((((short *)actualScreenShot->line[y + controlPixel[3].posY])[x + controlPixel[3].posX] != controlPixel[3].color) &&
          (((short *)actualScreenShot->line[y + controlPixel[3].posY])[x + controlPixel[3].posX] != screenShotTransparentPixel))
        continue;
      if ((((short *)actualScreenShot->line[y + controlPixel[4].posY])[x + controlPixel[4].posX] != controlPixel[4].color) &&
          (((short *)actualScreenShot->line[y + controlPixel[4].posY])[x + controlPixel[4].posX] != screenShotTransparentPixel))
        continue;                */
/*      for (i = 0; i < NUMBER_OF_CONTROL_PIXELS; i++)
      {
        if ((((short *)actualScreenShot->line[y + controlPixel[i].posY])[x + controlPixel[i].posX] == controlPixel[i].color) ||
            (((short *)actualScreenShot->line[y + controlPixel[i].posY])[x + controlPixel[i].posX] == screenShotTransparentPixel))
      }*/
      posX = x;
      posY = y;      
      //printf("    Probably object %s found at position %d:%d\n", image[imageID].filePath, posX, posY);
      //fprintf(outputFile, "    Probably object %s found at position %d:%d\n", image[imageID].filePath, posX, posY);
      
      tmp = findImageOnScreenShotInPosition(imageID, posX, posY);
      
      //continue;
      
      if (tmp)
      {
        ///printf("Step1. Adding\n");
        newRecognized = recognizedNumber;
        alreadyExist = 0;
        for (i = 0; i < recognizedNumber; i++)
        {
          if ((recognized[i].imageID == imageID) &&
              (recognized[i].posX == posX) &&
              (recognized[i].posY == posY))
            {  
              alreadyExist = 1;
              newRecognized = i;
              break;
            }  
        }
/*        if (alreadyExist == 0)*/
        if (0 == 0)
        {
          ///printf("Step2. alreadyExist = 0 ;)\n");         
          recognized[newRecognized].imageID = imageID;
          if (!alreadyExist) recognized[newRecognized].layer = actualLayer;
          recognized[newRecognized].posX = posX;  
          recognized[newRecognized].posY = posY;
          recognized[newRecognized].goodPixels = tmp;
          recognized[newRecognized].goodPixels2 = tmp;          
          recognized[newRecognized].goodPixelsPercent = (recognized[newRecognized].goodPixels*65536)/image[recognized[newRecognized].imageID].pixelsNumber;
          recognized[newRecognized].ownedPixels = 0;
          recognized[newRecognized].pixelsNumber = image[imageID].pixelsNumber;
          if (!alreadyExist)
          {
            recognizedNumber++;          
            dirInfo[image[imageID].dirID].recognizedNumber++;
          }  
          //printf("tmp1 - %d ||| tmp2 - %d\n", dirInfo[image[imageID].dirID].recognizedNumber, dirInfo[image[imageID].dirID].optMaxRecognizedNumber);
          if (dirInfo[image[imageID].dirID].recognizedNumber > dirInfo[image[imageID].dirID].optMaxRecognizedNumber)
            return 0;
          if (recognizedNumber > 99900)
          {
            printf("[ERROR] %s\n", "Too many recognized items!");
            while (1) {};
          }
          ///printf("Step3. Just a while to add to recognize map\n");          
          //cutImageFromScreenShot(image[imageID].bitmap, posX, posY);        
        //char buf[100];
        //sprintf(buf, "rec_%d.bmp", x, y, rand()%100);
        //save_bitmap(buf, transparentScreenShot, 0);        
        //if (recognizedNumber%100 == 0) printf("# Already recognized %d images\n", recognizedNumber);
//          printf("Found %s %c %d %d\n", image[imageID].filePath, image[imageID].direction, posX, posY);        
        // int putImageOnRecognizeMap(BITMAP *bitmap, int posX, int posY, int recognizedID)
/*          if (putImageOnRecognizeMap(image[imageID].bitmap, posX, posY, recognizedNumber-1))
          {
            //cutImageFromTransparentScreenShot(image[imageID].bitmap, posX, posY);
            
            //numberOfRecognizedImages++;
            //if (numberOfRecognizedImages >= maxImages) return numberOfRecognizedImages;        
            //char buf[100];
            //sprintf(buf, "%d_%d_%d.bmp", x, y, rand()%100);
            //save_bitmap(buf, bitmapToFind, 0);
          }
          else
            recognizedNumber--;*/
        }  
      }  
    }
  return numberOfRecognizedImages;
}

void recognizeScreenShot(int screenShotID)
{
  char buf[100];
  int x, y;  
  int i, j, k, l, tmp;
  short transparentPixel = makecol(0, 0, 0);
  register short screenShotTransparentPixel = makecol(255, 0, 255);  
  int maxPixelsNumber, maxPixelsID, maxTotalPixelsNumber;
  int recognizedNow, recognizedBefore;
  //int maxImages, tmp, nrOfRecognizedImages, nrOfRecognizedImagesThisSession;
  
  printf("\nRecognizing %s\n", screenShotList[screenShotID].fileName);
  fprintf(outputFile, "\nRecognizing %s\n", screenShotList[screenShotID].fileName);
  fprintf(outputSmallFile, "Recognizing %s\n", screenShotList[screenShotID].fileName);  
  sprintf(buf, "%s" SEPS "%s", screenShotsDir, screenShotList[screenShotID].fileName);
  screenShot = load_bmp(buf, 0);
  transparentScreenShot = load_bmp(buf, 0);  
  DEBUGScreenShot2 = load_bmp(buf, 0);  
  DEBUGScreenShot = create_bitmap(500, 400);
  clear_to_color(DEBUGScreenShot, makecol(50, 50, 50));
  blit(screenShot, DEBUGScreenShot, 0, 0, 0, 200, 320, 200);  
  line(DEBUGScreenShot, 320, 0, 320, 399, makecol(255, 255, 255));  
  totalNumberOfRecognizedImages = 0;  
  recognizedNumber = 0;
  actualLayer = 0;
  DEBUGY = 0;
  for (i = 0; i < 320; i++)
    for (j = 0; j < 200; j++)
    {
      recognizeMap[i][j] = -1;
    }
  for (i = 0; i < dirsNumber; i++)
    dirInfo[i].recognizedNumber = 0;
    
  recognizedNow = 0;
  do
  {
    recognizedBefore = recognizedNow;
    recognizedNow = 0;
    if ((optMaxLayers != 0) && (actualLayer+1 > optMaxLayers)) break;   
   
    recognizedBefore = recognizedNumber;   
    printf("    Checking layer nr. %d ...     ", actualLayer);
    //i = 0;
    //for (y = 0; y < recognizedNumber; y++)
    //if (recognized[y].ownedPixels != 0) i++;
    
    //recognizedNumber = 0;
    for (x = 0; x < imagesNumber; x++)
    {
      /* TODO (#1#): delete // below */
      //if (dirInfo[image[x].dirID].recognizedNumber < dirInfo[image[x].dirID].optMaxRecognizedNumber)
      //if (x%20 == 0)
      printf("\b\b\b\b% 3d%%", (x*100)/imagesNumber);
      findImageOnScreenShot(x);
    }
    printf("\b\b\b\bDone\n");
    
    for (i = 0; i < 320; i++)
      for (j = 0; j < 200; j++)
      {
        recognizeMap[i][j] = -1;
      }        
    
    for (i = 0; i < recognizedNumber; i++)
    {
      cutImageFromTransparentScreenShot(i, recognized[i].posX, recognized[i].posY);
      putImageOnRecognizeMap(image[recognized[i].imageID].bitmap, recognized[i].posX, recognized[i].posY, i);
    }    
    
    /*qsort(recognized, recognizedNumber, sizeof(tRecognized), cmptRecognized);*/
    
    /*for (i = 0; i < 320; i++)
      for (j = 0; j < 200; j++)
      {
        recognizeMap[i][j] = -1;
      }   
    recognizedNumber = 0;*/
    /*for (i = 0; i < 320; i++)
      for (j = 0; j < 200; j++)
      {
        recognizeMap[i][j] = -1;
      }    
    totalNumberOfRecognizedImages = 0;
    for (i = 0; i < recognizedNumber; i++)
      recognized[i].ownedPixels = 0;
    for (i = 0; i < recognizedNumber; i++)
    {
      if (putAndCheck(i, recognized[i].posX, recognized[i].posY))
      {
        totalNumberOfRecognizedImages++;       
        cutImageFromTransparentScreenShot(image[recognized[i].imageID].bitmap, recognized[i].posX, recognized[i].posY);
        removeImageFromRecognizeMap(i, recognized[i].posX, recognized[i].posY);
        printf("Hmm %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d / UL:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].layer, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber, recognized[i].upperLayers);
      }
    }*/ 

    //j = 0;
    //for (y = 0; y < recognizedNumber; y++)
    //if (recognized[y].ownedPixels != 0) j++;    
    recognizedNow = recognizedNumber;
//    sprintf(buf, "ss_db_%02d_s_%d.bmp", screenShotID, actualLayer);
//    save_bitmap(buf, transparentScreenShot, 0);      
    
    actualLayer++;    
        
    /* TODO (#1#): delete this two lines below */
  }
  while (recognizedNow - recognizedBefore != 0);
  
  //return 13+13;
  
//  for (i = 0; i < 320; i++)
//    for (j = 0; j < 200; j++)
//    {
//      recognizeMap[i][j] = -1;
//    }
    
  /*totalNumberOfRecognizedImages = 0;  
  for (i = 0; i < recognizedNumber; i++)
  {
    if (recognized[i].ownedPixels)
    {
      totalNumberOfRecognizedImages++;
      printf("Found %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].layer, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber);
      fprintf(outputFile, "Found %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].layer, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber);
      
      /*blit(transparentScreenShot, DEBUGScreenShot, 0, 0, 0, 0, 320, 200);
      blit(image[recognized[i].imageID].bitmap, DEBUGScreenShot, 0, 0, recognized[i].posX, recognized[i].posY, image[recognized[i].imageID].bitmap->w, image[recognized[i].imageID].bitmap->h);
      sprintf(buf, "ss_ds_%02d_%03d.bmp", screenShotID, totalNumberOfRecognizedImages);
      save_bitmap(buf, DEBUGScreenShot, 0);*/
      
      /*cutImageFromDEBUGScreenShot(image[recognized[i].imageID].bitmap, recognized[i].posX, recognized[i].posY);
    }
  }
  sprintf(buf, "ss_debug_%s.bmp", screenShotList[screenShotID]);
  save_bitmap(buf, DEBUGScreenShot, 0);  
  
  return 13;*/
  
  for (i = 0; i < recognizedNumber; i++)
    recognized[i].ownedPixels = 0;  

  for(;;)
  {
    maxPixelsNumber = 0;
    maxTotalPixelsNumber = 0;    
    maxPixelsID = -1;
    for (i = 0; i < recognizedNumber; i++)
    {
      if (recognized[i].goodPixels > maxPixelsNumber)
      {
        maxTotalPixelsNumber = 0;
        maxPixelsNumber = recognized[i].goodPixels;
        maxPixelsID = i;
      }
      else
      if ((recognized[i].goodPixels == maxPixelsNumber) && (recognized[i].pixelsNumber > maxTotalPixelsNumber))
      {
        maxTotalPixelsNumber = recognized[i].pixelsNumber;
        maxPixelsID = i;
      }
    }
    if (maxPixelsNumber != 0)
    {
      tmp = findImageOnScreenShotInPosition(recognized[maxPixelsID].imageID, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY);
      if (recognized[maxPixelsID].goodPixels != tmp)
      {
        recognized[maxPixelsID].goodPixels = tmp;
        continue;
      }
      totalNumberOfRecognizedImages++;     
      cutImageFromDEBUGScreenShot(maxPixelsID, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY);
      cutImageFromScreenShot(maxPixelsID, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY);
      putImageOnRecognizeMap(image[recognized[maxPixelsID].imageID].bitmap, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY, maxPixelsID);
      printf("    Found %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d)\n", image[recognized[maxPixelsID].imageID].filePath, image[recognized[maxPixelsID].imageID].direction, recognized[maxPixelsID].layer, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY, recognized[maxPixelsID].ownedPixels, recognized[maxPixelsID].goodPixels, image[recognized[maxPixelsID].imageID].pixelsNumber);
      fprintf(outputFile, "    Found %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d)\n", image[recognized[maxPixelsID].imageID].filePath, image[recognized[maxPixelsID].imageID].direction, recognized[maxPixelsID].layer, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY, recognized[maxPixelsID].ownedPixels, recognized[maxPixelsID].goodPixels, image[recognized[maxPixelsID].imageID].pixelsNumber);
      fprintf(outputSmallFile, "Found %s d%c l%d x%d y%d w%d h%d\n", image[recognized[maxPixelsID].imageID].filePath, image[recognized[maxPixelsID].imageID].direction, recognized[maxPixelsID].layer, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY, image[recognized[maxPixelsID].imageID].bitmap->w, image[recognized[maxPixelsID].imageID].bitmap->h);
      recognized[maxPixelsID].goodPixels = 0;
/*      for (k = 0; k < 320; k++)
        for (l = 0; l < 200; l++)
        {
          recognizeMap[k][l] = -1;
        }      
      for (j = 0; j < recognizedNumber; j++)
      {
        if (recognized[j].goodPixels != 0)
        {
          recognized[j].ownedPixels = 0;
          recognized[j].goodPixels = findImageOnScreenShotInPosition(recognized[j].imageID, recognized[j].posX, recognized[j].posY);
          //putImageOnRecognizeMap(image[recognized[j].imageID].bitmap, recognized[j].posX, recognized[j].posY, j);
        }
      } */
      /* debug screenshots */
      //blit(transparentScreenShot, DEBUGScreenShot2, 0, 0, 0, 0, 320, 200);
      //blit(image[recognized[maxPixelsID].imageID].bitmap, DEBUGScreenShot2, 0, 0, recognized[maxPixelsID].posX, recognized[maxPixelsID].posY, image[recognized[maxPixelsID].imageID].bitmap->w, image[recognized[maxPixelsID].imageID].bitmap->h);
      //sprintf(buf, "ss_ds_%02d_%03d.bmp", screenShotID, totalNumberOfRecognizedImages);
      //save_bitmap(buf, DEBUGScreenShot2, 0);
    }
    else
      break;
  }  
  
  /*totalNumberOfRecognizedImages = 0;  
  for (i = 0; i < recognizedNumber; i++)
  {
    if (recognized[i].ownedPixels)
    {
      totalNumberOfRecognizedImages++;
      printf("    Found %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].layer, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber);
      fprintf(outputFile, "    Found %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].layer, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber);
      fprintf(outputSmallFile, "    Found %s d%c l%d x%d y%d (OW:%d / GO:%d / TO:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].layer, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber);
    }
  }*/
  
/*  for (i = 0; i < recognizedNumber; i++)
  {
    if (recognized[i].ownedPixels != 0)
    {
      printf("Found %s %c %d %d (OW:%d / GO:%d / TO:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber);
      fprintf(outputFile, "Found %s %c %d %d (OW:%d / GO:%d / TO:%d)\n", image[recognized[i].imageID].filePath, image[recognized[i].imageID].direction, recognized[i].posX, recognized[i].posY, recognized[i].ownedPixels, recognized[i].goodPixels, image[recognized[i].imageID].pixelsNumber);
      totalNumberOfRecognizedImages++;
      
      blit(transparentScreenShot, DEBUGScreenShot, 0, 0, 0, 0, 320, 200);
      blit(image[recognized[i].imageID].bitmap, DEBUGScreenShot, 0, 0, recognized[i].posX, recognized[i].posY, image[recognized[i].imageID].bitmap->w, image[recognized[i].imageID].bitmap->h);
      for (x = 0; x < image[recognized[i].imageID].bitmap->w; x++)
        for (y = 0; y < image[recognized[i].imageID].bitmap->h; y++)  
        {
          if ((((short *)image[recognized[i].imageID].bitmap->line[y])[x] != transparentPixel) && 
              (((short *)image[recognized[i].imageID].bitmap->line[y])[x] == ((short *)screenShot->line[recognized[i].posY+y])[recognized[i].posX+x]))
            putpixel(DEBUGScreenShot, recognized[i].posX+x, recognized[i].posY+y, makecol16(255, 0, 0));
        }         
      for (x = 0; x < 320; x++)
        for (y = 0; y < 200; y++)
        {
          if (recognizeMap[x][y] == i)
            putpixel(DEBUGScreenShot, x, y, makecol16(0, 255, 0));
        }      
      sprintf(buf, "ss_%02d_%03d.bmp", screenShotID, totalNumberOfRecognizedImages);
      save_bitmap(buf, DEBUGScreenShot, 0);
    }  
  }*/
  textprintf(DEBUGScreenShot, font, 320+2, 1, makecol(255, 255, 255), "%s (%d/%d)", screenShotList[screenShotID], screenShotID+1, screenShotsNumber);
  textprintf(DEBUGScreenShot, font, 320+2, 10, makecol(255, 255, 255), "Recognized number: %d", totalNumberOfRecognizedImages);
  
  printf("    Total number of recognized images %d\n", totalNumberOfRecognizedImages);
  fprintf(outputFile, "    Total number of recognized images %d\n", totalNumberOfRecognizedImages);
  sprintf(buf, "%s/trans_%s.bmp", optResultsDir, screenShotList[screenShotID]);
  save_bitmap(buf, transparentScreenShot, 0);
  sprintf(buf, "%s/rec_%s.bmp", optResultsDir, screenShotList[screenShotID]);
  save_bitmap(buf, DEBUGScreenShot, 0);
  destroy_bitmap(screenShot);
  destroy_bitmap(transparentScreenShot);  
  destroy_bitmap(DEBUGScreenShot);
  destroy_bitmap(DEBUGScreenShot2);
}

void sortListOfScreenShots()
{
  char *tmpScreenShotsList[MAX_SCREENSHOTS];
  int tmpScreenShotsNumber;  
  int i;
  DIR *dir = opendir(screenShotsDir);
  struct dirent *file;
  
  tmpScreenShotsNumber = 0;
  while (file = readdir(dir))
  {
    if (match("*.bmp", file->d_name) || match("*.BMP", file->d_name))
    {
      tmpScreenShotsList[tmpScreenShotsNumber] = (char *) malloc(strlen(file->d_name)+1);
      strcpy(tmpScreenShotsList[tmpScreenShotsNumber], file->d_name);
      tmpScreenShotsNumber++;
    }  
  }  
  closedir(dir);
  
  qsort(tmpScreenShotsList, tmpScreenShotsNumber, sizeof(char *), pstrcmp);
  
  for (i = 0; i < tmpScreenShotsNumber; i++)
  {
    screenShotList[i].fileName = tmpScreenShotsList[i];
  }
  screenShotsNumber = tmpScreenShotsNumber;
}

void freeListOfScreenShots()
{
  int i;
   
  for (i = 0; i < screenShotsNumber; i++)
  {
    //printf("%s\n", screenShotList[i].fileName);
    free(screenShotList[i].fileName);
  }
  screenShotsNumber = 0;
}

int countPixels(BITMAP *bitmap)
{
  int x, y, number;
  int transparentPixel = makecol16(0, 0, 0);

  number = 0;    
  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      if (((short *)bitmap->line[y])[x] != transparentPixel)
        number++;
    }
  return number;  
}

void readDir(int dirID)
{
  char buf[100];
  int i;
  DIR *dir = opendir(dirInfo[dirID].dirName);
  struct dirent *file;
  FILE *optFile;
  
  sprintf(buf, "%s" SEPS "bitmaps.cfg", dirInfo[dirID].dirName);
  if (optFile = fopen(buf, "r"))
  {
    //printf("Reading data from bitmaps.cfg\n");
    fscanf(optFile, "%d", &dirInfo[dirID].optTwoDirections);    
    fscanf(optFile, "%d", &dirInfo[dirID].optMaxRecognizedNumber);
    fscanf(optFile, "%d", &dirInfo[dirID].optMinImagePercent);
    fscanf(optFile, "%d", &dirInfo[dirID].optAllowTransparency);    
    fclose(optFile);
  }
  else
  {
    //printf("Cannot find bitmaps.cfg in dat file\n");
    dirInfo[dirID].optTwoDirections = 1;    
    dirInfo[dirID].optMaxRecognizedNumber = 99999;
    dirInfo[dirID].optMinImagePercent = 30;
    dirInfo[dirID].optAllowTransparency = 1;    
    //dirInfo[dirID].optVolatile = 0;
  }  

  while (file = readdir(dir))
  {
    if (match("*.bmp", file->d_name) || match("*.BMP", file->d_name))
    {
      strcpy(buf, dirInfo[dirID].dirName);
      strcat(buf, SEPS); 
      strcat(buf, file->d_name);
      image[imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
      strcpy(image[imagesNumber].filePath, buf);
      image[imagesNumber].bitmap = load_bmp(image[imagesNumber].filePath, 0);
      image[imagesNumber].direction = '-';      
      image[imagesNumber].pixelsNumber = countPixels(image[imagesNumber].bitmap);
      image[imagesNumber].dirID = dirID;
      imagesNumber++;
      
      if (dirInfo[dirID].optTwoDirections)
      {      
        image[imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
        strcpy(image[imagesNumber].filePath, buf);
        image[imagesNumber].bitmap = create_bitmap(image[imagesNumber-1].bitmap->w, image[imagesNumber-1].bitmap->h);
        draw_sprite_h_flip(image[imagesNumber].bitmap, image[imagesNumber-1].bitmap, 0, 0);
        image[imagesNumber].direction = 'M';
        image[imagesNumber].pixelsNumber = image[imagesNumber-1].pixelsNumber;
        image[imagesNumber].dirID = dirID;        
        imagesNumber++;
      }
      
/*      dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
      strcpy(dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, buf);
      dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap = load_bmp(dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, 0);
      dirContents[dirID].image[dirContents[dirID].imagesNumber].direction = 'L';
      printf("%s -> %d pixels\n", dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, countPixels(dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap));
      dirContents[dirID].imagesNumber++;
      
      dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
      strcpy(dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, buf);
      dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap = create_bitmap(dirContents[dirID].image[dirContents[dirID].imagesNumber - 1].bitmap->w, dirContents[dirID].image[dirContents[dirID].imagesNumber - 1].bitmap->h);
      draw_sprite_h_flip(dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap, dirContents[dirID].image[dirContents[dirID].imagesNumber - 1].bitmap, 0, 0);      
      dirContents[dirID].image[dirContents[dirID].imagesNumber].direction = 'R';      
      dirContents[dirID].imagesNumber++;      */
    }  
  }  
  closedir(dir);
}

void sortListOfImages()
{
  int i;
 
  qsort(image, imagesNumber, sizeof(tImage), cmptImage);
//  for (i = 0; i < imagesNumber; i++)
//  {
//    printf("#### %s -> %d pixels\n", image[i].filePath, image[i].pixelsNumber);
//    //rest(100);
//  }
}

/*void readDir(int dirID)
{
  char buf[100];
  int i;
  DIR *dir = opendir(dirContents[dirID].dirName);
  struct dirent *file;
  struct sTmpFilesList
  {
    char *fileName;
    int pixelsNumber;
  } tmpFilesList[MAX_FILES_IN_DIR];
  int tmpFilesNumber;
  
  while (file = readdir(dir))
  {
    if (match("*.bmp", file->d_name) || match("*.BMP", file->d_name))
    {
      strcpy(buf, dirContents[dirID].dirName);
      strcat(buf, "\\");      
      strcat(buf, file->d_name);
      
      dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
      strcpy(dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, buf);
      dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap = load_bmp(dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, 0);
      dirContents[dirID].image[dirContents[dirID].imagesNumber].direction = 'L';
      printf("%s -> %d pixels\n", dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, countPixels(dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap));
      dirContents[dirID].imagesNumber++;
      
      dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
      strcpy(dirContents[dirID].image[dirContents[dirID].imagesNumber].filePath, buf);
      dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap = create_bitmap(dirContents[dirID].image[dirContents[dirID].imagesNumber - 1].bitmap->w, dirContents[dirID].image[dirContents[dirID].imagesNumber - 1].bitmap->h);
      draw_sprite_h_flip(dirContents[dirID].image[dirContents[dirID].imagesNumber].bitmap, dirContents[dirID].image[dirContents[dirID].imagesNumber - 1].bitmap, 0, 0);      
      dirContents[dirID].image[dirContents[dirID].imagesNumber].direction = 'R';      
      dirContents[dirID].imagesNumber++;      
    }  
  }  
  closedir(dir);
}*/

void freeListOfImages()
{
  int i;
   
  for (i = 0; i < imagesNumber; i++)
  {
    free(image[i].filePath);
    destroy_bitmap(image[i].bitmap);
  }
  imagesNumber = 0;
}

void readParameters()
{
  int i;
  
  printf("Prince of Persia Screenshots Recognizer\n(c) Copyright 2005 Princed Development Team\nProgrammed by peter_k\nhttp://www.princed.com.ar\n\n");
  
  printf("Step 1. Type dir where screenshots are stored.\nThis should be 320x200 bmp files with 256 colour palette.\n");
  scanf("%s", screenShotsDir);
  
  printf("\nStep 2. Type dirs where are stored bitmaps to recognize.\nPlease type in this format: [dirname] [max images on screenshot][ENTER].\nWhen you'll finish type END[ENTER].\n");
  for (;;)
  {
    scanf("%s", dirInfo[dirsNumber].dirName);
    if (strcmp(dirInfo[dirsNumber].dirName, "END"))
    {
/*      scanf("%d", &dirInfo[dirsNumber].optMaxImagesOnScreenShot);     */
      dirsNumber++;
    }  
    else
      break; 
  }
  
  printf("\nStep 3. Type number of maximum layers\nHINT:\nIf you'll type 0, poprecog will automatically detect the number of layers,\nbut it is not recommended for tiles, walls, cinematic etc.\n");
  scanf("%d", &optMaxLayers);  
  
  printf("\nStep 4. Type output dir, in which you'll see recognized things\n");
  scanf("%s", optResultsDir);
  
  printf("\n");
}

int main(int argc, char *argv[])
{
  int i;
  long timeBefore, timeAfter;
  char buf[100];
  
  srand((unsigned int)time((time_t *)NULL));  
  
  allegro_init();
  set_color_depth(16);
  set_color_conversion(COLORCONV_TOTAL);
  
  readParameters();
  /*system("del ss_*.bmp");*/

  defmkdir(optResultsDir);
  
  sprintf(buf, "%s/%s", optResultsDir, "all_results.txt");
  outputFile = fopen(buf, "a");
	if (!outputFile) {
		printf("Cannot open %s for append\n",buf);
		return -1;
	}
  sprintf(buf, "%s/%s", optResultsDir, "small_results.txt");  
  outputSmallFile = fopen(buf, "a");
	if (!outputFile) {
		printf("Cannot open %s for append\n",buf);
		return -1;
	}
  fprintf(outputFile, "File generated by Prince Of Persia Screenshots Recognizer\n(c) Copyright 2005 Princed Development Team\nProgrammed by peter_k\nhttp://www.princed.com.ar\n\n");

  for (i = 0; i < dirsNumber; i++)
  {
    printf("Loading bitmaps from dir %s\n", dirInfo[i].dirName);
    fprintf(outputFile, "Loading bitmaps from dir %s\n", dirInfo[i].dirName);    
    //rest(500);
    readDir(i);
  }
  sortListOfImages();  
  sortListOfScreenShots();
  
//  for (i = 0; i < dirContents[0].imagesNumber; i++)
//    recognizeScreenShot(i);

//  for (i = 0; i < dirContents[1].imagesNumber; i++)
//    findImageOnImage(dirContents[0].image[0].bitmap, dirContents[1].image[i].bitmap);

  timeBefore = time(0);
    
  for (i = 0; i < screenShotsNumber; i++)  
    recognizeScreenShot(i);

  timeAfter = time(0);
  printf("\nRecognizing %d frames last about %d seconds\n", screenShotsNumber, timeAfter - timeBefore);    
  fprintf(outputFile, "\nRecognizing %d frames last about %d seconds\n", screenShotsNumber, timeAfter - timeBefore);       
    
  printf("Freeing memory\n");
  fprintf(outputFile, "Freeing memory\n");  
//  for (i = 0; i < dirsNumber; i++)
//    freeDir(i);  
  freeListOfScreenShots();
  freeListOfImages();  
  
  fclose(outputFile);
  fclose(outputSmallFile);
    
  printf("Press ENTER key...\n");
  
  scanf("\n");
  return 0;
}
END_OF_MAIN();
