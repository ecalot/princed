#ifndef _FP_RESOURCE_H
#define _FP_RESOURCE_H

typedef struct FPResource FPResource;

struct FPResource {
  int id;
};

int resGetId(FPResource *_this);

#endif
