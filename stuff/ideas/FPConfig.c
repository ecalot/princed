#include <string.h>
#include "FPConfig.h"

int cfgInit(FPConfig *_this,char *_pop1_path) {
  _this->pop1_path=(char *)malloc(strlen(_pop1_path)+1);
  if (_this->pop1_path==NULL) {
    return(0);
  }
  strcpy(_this->pop1_path,_pop1_path);
  return(1);
}

void cfgDstr(FPConfig *_this) {
  free(_this->pop1_path);
  _this->pop1_path=NULL;
}
