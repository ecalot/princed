#include "FPConfig.h"
#include "FPResourceManager.h"

int main(int _argc,char *_argv[]) {
  FPConfig config;

  /* this should be loaded from a conf file on disk
   */
  if (!cfgInit(&config,"./")) {
    /* print error message */
    return(-1);
  }

  /* configure the resource manager */
  if (!mgrInit(&config)) {
    /* print error message */
    return(-1);
  }


  return(0);
}
