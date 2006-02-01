#ifndef _OUTERTILE_H_
#define _OUTERTILE_H_

#include "tile.h"

#define DU 1
#define DD 2
#define DL 4
#define DR 8

class OuterTile: Tile {
public:
	OuterTile(int code);
/*      string serializeCode();
	string serializeBack();*/
	int getDoorEvent();
	int getAttr();
	int getImg();
	int getCode();

};

#endif
