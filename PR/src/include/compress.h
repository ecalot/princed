
#ifndef _COMPRESS_H_
#define _COMPRESS_H_

#define SIZE_OF_ARRAY   1000
#define SIZE_OF_PALETTE 3*16

#define PG_COMP_RAW    0xB0
#define PG_COMP_RLE_LR 0xB1
#define PG_COMP_RLE_UD 0xB2
#define PG_COMP_LZX_LR 0xB3
#define PG_COMP_LZX_UD 0xB4

typedef struct {
	int width;
	int height;
	int size;
	unsigned char* pix;
	unsigned char pal[SIZE_OF_PALETTE];
}tImage;

//Prototypes:

int mCompressGraphic(unsigned char* a,tImage* i, int* size);
int mExpandGraphic(unsigned char* array,tImage *image, int size);

/*
	Class future definitions

public:
 char cPrincedGraph::mOpenGraphDat     (char* fileName);
 char cPrincedGraph::mSaveGraphDat     (char* fileName);
 char cPrincedGraph::mGetGraphFull     (int id, CBitmap &bitmap, CBitmap &mask);
 char cPrincedGraph::mGetGraphMap      (int id, CBitmap &bitmap);
 char cPrincedGraph::mGetGraphMask     (int id, CBitmap &bmask);
 char cPrincedGraph::mSetGraph         (int id, CBitmap &bitmap);
 int  cPrincedGraph::mGetGraphCount    ();
 char cPrincedGraph::mSaveImportBmp    (int id, char* fileName);
 char cPrincedGraph::mSaveExportBmp    (int id, char* fileName);

private:
 unsigned char* file;
 int            fileSize;
 tImage*        imageArray[256];
 int            imageArrayLength;

*/
#endif
