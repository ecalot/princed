#ifndef _FP_IMAGE_H
#define _FP_IMAGE_H

#include "FPResource.h"

typedef struct FPImage FPImage;

/* FPImage is a FPResource for static images
 */
struct FPImage {
  FPResourceType type;
  int id;
  int height;
  int width;
  char *image;
};

#endif
