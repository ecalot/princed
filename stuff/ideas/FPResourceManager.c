#include "FPResourceManager.h"

FPResourceManager this;

int mgrInit(FPConfig *_config) {
  this.config=_config;
  return(1);
}
