#include "pop1tile.h"

Pop1Tile::Pop1Tile(unsigned char code, unsigned char back) {
 this->code=code;
 this->back=back;
}

int Pop1Tile::getDoorEvent() {
	return this->back;
}

int Pop1Tile::getAttr() {
	return this->back;
}

int Pop1Tile::getImg() {
	return 0;
}
