#include "outertile.h"

OuterTile::OuterTile(int code) {
 this->code=code;
}

int OuterTile::getDoorEvent() {
	return 0;
}

int OuterTile::getAttr() {
	return 0;
}

int OuterTile::getImg() {
	return 0;
}

int OuterTile::getCode() {
	return this->code; //TODO: calculate the special codes
}
