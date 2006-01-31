#include "pop2tile.h"

Pop2Tile::Pop2Tile(unsigned char code, unsigned char back[5]) {
 this->code=code;
 this->attr=back[0];
 this->door=back[1];
 this->img=back[2]+back[3]>>8;
}

int Pop2Tile::getDoorEvent() {
	return this->door;
}

int Pop2Tile::getAttr() {
	return this->attr;
}

int Pop2Tile::getImg() {
	return this->img;
}
