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
*/

/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <dirent.h>
//#include <qsort.h>
#include <allegro.h>

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

BITMAP *screenShot, *transparentScreenShot;

struct sDirInfo
{
  char dirName[16];
  int maxImagesOnScreenShot;
} dirInfo[MAX_DIRS];
int dirsNumber;

struct sRecognized
{
  int imageID;
  int posX, posY;
  int goodPixels;
} recognized[MAX_RECOGNIZED_ITEMS];
int recognizedNumber;

/*This will be soonint recognizeTable[320][200]; // here are stored information which 'recognize result' have this pixel*/

FILE *outputFile;
char outputFileName[20];

/* Functions */
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
  if (pxa>pxb) return -1;
  if (pxa<pxb) return 1;
  return 0;
}

int findImageOnScreenShotInPosition(BITMAP *bitmap, int posX, int posY)
{
  register int i, j, x, y, c;
  register short transparentPixel = makecol16(0, 0, 0);
  register short screenShotTransparentPixel = makecol16(255, 0, 255);  
  register int goodPixels;
  register int wPer2 = bitmap->w / 2;
  register int hPer2 = bitmap->h / 2;  

  goodPixels = 0;  
  for (i = 0; i < bitmap->w; i++)
    for (j = 0; j < bitmap->h; j++)  
    {
      x = (i+wPer2)%bitmap->w;
      y = (j+hPer2)%bitmap->h;
      if (((short *)bitmap->line[y])[x] == transparentPixel)
        continue;
      if (((short *)bitmap->line[y])[x] == ((short *)transparentScreenShot->line[posY+y])[posX+x])
      {
        goodPixels++;
        continue;
      }        
      if (((short *)transparentScreenShot->line[posY+y])[posX+x] == screenShotTransparentPixel)
        continue;        
      return 0;
    }
  if (goodPixels < 0) return 0;
  return goodPixels;
}

int cutImageFromScreenShot(BITMAP *bitmap, int posX, int posY)
{
  int x, y, c;
  int transparentPixel = makecol16(0, 0, 0);
  int screenShotTransparentPixel = makecol16(255, 0, 255);  
  char buf[100];
    
  for (x = 0; x < bitmap->w; x++)
    for (y = 0; y < bitmap->h; y++)  
    {
      if (((short *)bitmap->line[y])[x] != transparentPixel)
        ((short *)transparentScreenShot->line[posY+y])[posX+x] = screenShotTransparentPixel;
    }
  //sprintf(buf, "ss_%05d.bmp", rand()%9999);
  //save_bitmap(buf, actualScreenShot, 0);    
  //rest(500);
}

int findImageOnScreenShot(int imageID, int maxImages)
{
  /*int i;*/
  int numberOfRecognizedImages = 0;
  register int x, y;
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
       if ((((short *)transparentScreenShot->line[y+cp1y])[x+cp1x] != cp1c) &&
           (((short *)transparentScreenShot->line[y+cp1y])[x+cp1x] != screenShotTransparentPixel))
        continue;                          
       if ((((short *)transparentScreenShot->line[y+cp2y])[x+cp2x] != cp2c) &&
           (((short *)transparentScreenShot->line[y+cp2y])[x+cp2x] != screenShotTransparentPixel))
        continue;                          
       if ((((short *)transparentScreenShot->line[y+cp3y])[x+cp3x] != cp3c) &&
           (((short *)transparentScreenShot->line[y+cp3y])[x+cp3x] != screenShotTransparentPixel))
        continue;                                          
       if ((((short *)transparentScreenShot->line[y+cp4y])[x+cp4x] != cp4c) &&
           (((short *)transparentScreenShot->line[y+cp4y])[x+cp4x] != screenShotTransparentPixel))
        continue;                                                  
       if ((((short *)transparentScreenShot->line[y+cp5y])[x+cp5x] != cp5c) &&
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
                  
      if (tmp = findImageOnScreenShotInPosition(image[imageID].bitmap, posX, posY))
      {
        recognized[recognizedNumber].imageID = imageID;
        recognized[recognizedNumber].posX = posX;  
        recognized[recognizedNumber].posY = posY;
        recognized[recognizedNumber].goodPixels = tmp;
        recognizedNumber++;
        
        cutImageFromScreenShot(image[imageID].bitmap, posX, posY);
        numberOfRecognizedImages++;
        printf("    %s %c %d %d\n", image[imageID].filePath, image[imageID].direction, posX, posY);
        fprintf(outputFile, "    %s %c %d %d\n", image[imageID].filePath, image[imageID].direction, posX, posY);
        //if (numberOfRecognizedImages >= maxImages) return numberOfRecognizedImages;        
        /*char buf[100];
        sprintf(buf, "%d_%d_%d.bmp", x, y, rand()%100);
        save_bitmap(buf, bitmapToFind, 0);*/
      }
    }
  return numberOfRecognizedImages;
}

void recognizeScreenShot(int screenShotID)
{
  char buf[100];
  int x, y;  
/*This will be soon  int i, j;*/
  //int maxImages, tmp, nrOfRecognizedImages, nrOfRecognizedImagesThisSession;
  
  printf("Recognizing %s\n", screenShotList[screenShotID].fileName);
  fprintf(outputFile, "Recognizing %s\n", screenShotList[screenShotID].fileName);
  sprintf(buf, "%s\\%s", screenShotsDir, screenShotList[screenShotID].fileName);
  screenShot = load_bmp(buf, 0);
  transparentScreenShot = load_bmp(buf, 0);  
  recognizedNumber = 0;
/*This will be soon  for (i = 0; i < 320; i++)
    for (j = 0; j < 200; j++)
    {
      recognizeTable[i][j] = -1;
    }*/
    
  for (x = 0; x < 2; x++)
  {
    for (y = 0; y < imagesNumber; y++)
    {
      //maxImages = dirContents[x].maxImagesOnScreenShot - nrOfRecognizedImages;
      //if (maxImages <= 0) break;        
      /*tmp = */findImageOnScreenShot(y, 999/*maxImages*/);
      //nrOfRecognizedImagesThisSession += tmp;
      //nrOfRecognizedImages += tmp;        
    }
    //if ((nrOfRecognizedImagesThisSession == 0) || (maxImages <= 0) || (dirContents[x].maxImagesOnScreenShot == 1)) break;
    //printf("WARNING!!! SECOND LOOP!!! WARNING!!! SECOND LOOP!!!\n");
    printf("Session %d, recognized %d images\n", x, recognizedNumber);
    fprintf(outputFile, "Session %d, recognized %d images\n", x, recognizedNumber);
  }
    sprintf(buf, "ss_%03d.bmp", screenShotID);
    save_bitmap(buf, transparentScreenShot, 0);
  destroy_bitmap(screenShot);
  destroy_bitmap(transparentScreenShot);  
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
      tmpScreenShotsList[tmpScreenShotsNumber] = (char *) malloc(file->d_namlen+1);
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

  while (file = readdir(dir))
  {
    if (match("*.bmp", file->d_name) || match("*.BMP", file->d_name))
    {
      strcpy(buf, dirInfo[dirID].dirName);
      strcat(buf, "\\");      
      strcat(buf, file->d_name);
      image[imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
      strcpy(image[imagesNumber].filePath, buf);
      image[imagesNumber].bitmap = load_bmp(image[imagesNumber].filePath, 0);
      image[imagesNumber].direction = 'L';      
      image[imagesNumber].pixelsNumber = countPixels(image[imagesNumber].bitmap);
      imagesNumber++;
      
      image[imagesNumber].filePath = (char *) malloc(strlen(buf)+1);
      strcpy(image[imagesNumber].filePath, buf);
      image[imagesNumber].bitmap = create_bitmap(image[imagesNumber-1].bitmap->w, image[imagesNumber-1].bitmap->h);
      draw_sprite_h_flip(image[imagesNumber].bitmap, image[imagesNumber-1].bitmap, 0, 0);
      image[imagesNumber].direction = 'R';
      image[imagesNumber].pixelsNumber = image[imagesNumber-1].pixelsNumber;
      imagesNumber++;
 
      
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
  for (i = 0; i < imagesNumber; i++)
  {
    printf("#### %s -> %d pixels\n", image[i].filePath, image[i].pixelsNumber);
    //rest(100);
  }
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
      scanf("%d", &dirInfo[dirsNumber].maxImagesOnScreenShot);     
      dirsNumber++;
    }  
    else
      break; 
  }
  
  printf("\nStep 3. Type output filename\nIf you will type file which exist it will be appended.\n");
  scanf("%s", outputFileName);  
  
  printf("\n");
}

int main(int argc, char *argv[])
{
  int i;
  long timeBefore, timeAfter;
  
  system("del ss_*.bmp");
  
  allegro_init();
  set_color_depth(16);
  set_color_conversion(COLORCONV_TOTAL);
  
  readParameters();
  
  outputFile = fopen(outputFileName, "a");
  fprintf(outputFile, "\n");

  for (i = 0; i < dirsNumber; i++)
  {
    printf("Loading bitmaps from dir %s (%d)\n", dirInfo[i].dirName, dirInfo[i].maxImagesOnScreenShot);      
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
  printf("Info: Recognizing %d frames last about %d seconds\n", screenShotsNumber, timeAfter - timeBefore);    
    
  printf("Freeing memory\n");
//  for (i = 0; i < dirsNumber; i++)
//    freeDir(i);  
  freeListOfScreenShots();
  freeListOfImages();  
  
  fclose(outputFile);
    
  printf("Press any key...\n");
  
  while (!kbhit()) {};
  return 0;
}
END_OF_MAIN();