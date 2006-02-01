#ifndef _TILE_H_
#define _TILE_H_

#include <string>
using namespace std;

class Tile { // abstract class
public:
	virtual int getCode()=0;
	virtual int getDoorEvent()=0;
	virtual int getAttr()=0;
	virtual int getImg()=0;

protected:
	int level; /* this is very important to know what tiles are allowed */
	int code;

};

#endif
