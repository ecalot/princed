#ifndef _POP1TILE_H_
#define _POP1TILE_H_

#include "tile.h"

class Pop1Tile: Tile { // abstract class
public:
	Pop1Tile(unsigned char code, unsigned char back);
/*      string serializeCode();
	string serializeBack();*/
	int getDoorEvent();
	int getAttr();
	int getImg();

private:
	int back;

};

#endif
