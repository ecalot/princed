#ifndef _POP1LEVELFORMAT_H_
#define _POP1LEVELFORMAT_H_

#include <iostream>
#include "guard.h"
#include "tile.h"

using namespace std;

class Pop1LevelFormat: public LevelFormat {

public:

 Pop1LevelFormat::Pop1LevelFormat(iesstream& level, int blockSize); //open
 Pop1LevelFormat(int levelNumber); // new

 void save(ostream* level);

 ~Pop1LevelFormat();


 void setTile(int screen, int location, Tile* t);
 Tile* getTile(int screen, int location);

 bool addGuard(int screen, int location, Guard* g);
 bool delGuard(int screen, int location);
 Guard* getGuards(int screen, int number);


 void setRoomLink(int screen,int left,int right,int up,int down);
 void getRoomLink(int screen,int &left,int &right,int &up,int &down);

 void getStartPosition(int &screen, int &location,Direction &direction);
 void setStartPosition(int screen, int location,Direction direction);
 bool getDebugPosition(int &screen, int &location,Direction &direction);
 bool setDebugPosition(int screen, int location,Direction direction);
 void clearDebugPosition();

	void setDoorEvent(int event, int screen,int location, int next);
	void getDoorEvent(int event, int &screen,int &location, int &next);

private:
	unsigned char* walls;
	unsigned char* backs;
	unsigned char* doorI;
	unsigned char* doorII;
	unsigned char* links;
	unsigned char* uI;
	unsigned char* start_position;
	unsigned char* uII;
	unsigned char* uIII;
	unsigned char* guard_location;
	unsigned char* guard_direction;
	unsigned char* iIVa;
	unsigned char* uIVb;
	unsigned char* guard_skill;
	unsigned char* uIVc;
	unsigned char* guard_colour;
	unsigned char* uIVd;
	unsigned char* uV;
};

#endif
