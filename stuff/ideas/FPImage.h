#ifndef _FP_IMAGE_H
#define _FP_IMAGE_H

typedef struct FPImage FPImage;

/* FPImage is a FPResource for static images
 */
struct FPImage {
  int id;
  int height;
  int width;
  char *image;
};

#endif
