#ifndef _POP2TILE_H_
#define _POP2TILE_H_

#include "tile.h"

class Pop2Tile: Tile { // abstract class
public:
	Pop2Tile(unsigned char code, unsigned char back[5]);
/*      string serializeCode();
	string serializeBack();*/
	int getDoorEvent();
	int getAttr();
	int getImg();

private:
	int attr;
	int door;
	int img;
};

#endif
