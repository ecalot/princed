#ifndef _FP_RESOURCE_MANAGER_H
#define _FP_RESOURCE_MANAGER_H

#include "FPConfig.h"

typedef struct FPResourceManager FPResourceManager;

struct FPResourceManager {
  FPConfig *config;
};

int mgrInit(FPConfig *_config);

#endif
