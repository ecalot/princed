#ifndef _FP_RESOURCE_H
#define _FP_RESOURCE_H

typedef enum FPResourceType FPResourceType;

enum FPResourceType {
  FP_RES_PALETTE=1,
  FP_RES_IMAGE=2
};

typedef struct FPResource FPResource;

struct FPResource {
  FPResourceType type;
  int id;
};


FPResourceType resGetType(FPResource *_this);
int resGetId(FPResource *_this);

#endif
