#ifndef _TILE_H_
#define _TILE_H_

#include <string>
using namespace std;

class Tile { // abstract class
public:
      int getDoorEvent();
      int getAttr();
      int getImg();

      /* used to serialize self created tiles */
      int getWalls();
      int getBacks();

protected:
	int level; /* this is very important to know what tiles are allowed */
	int code;

};

#endif
