#include "FPResource.h"

FPResourceType resGetType(FPResource *_this) {
  return(_this->type);
}

int resGetId(FPResource *_this) {
  return(_this->id);
}
