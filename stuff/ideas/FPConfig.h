#ifndef _FP_CONFIG_H
#define _FP_CONFIG_H

typedef struct FPConfig FPConfig;

struct FPConfig {
  char *pop1_path;
};

int cfgInit(FPConfig *_this,char *_pop1_path);
void cfgDstr(FPConfig *_this);

#endif
