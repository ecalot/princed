#ifndef _LEVELFORMAT_H_
#define _LEVELFORMAT_H_

#include <iostream>
#include "guard.h"
#include "tile.h"
#include "endiansafestream.h"

using namespace std;

class LevelFormat { //abstract class

public:

	LevelFormat::LevelFormat(iesstream& level, int blockSize); //open
	LevelFormat(int levelNumber); // new
	LevelFormat(); // dummy constructor (?)

	~LevelFormat();


	virtual void save(ostream* level)=0;
	virtual void setTile(int screen, int location, Tile* t)=0;
	virtual Tile* getTile(int screen, int location)=0;

	virtual bool addGuard(int screen, int location, Guard* g)=0;
	virtual bool delGuard(int screen, int location)=0;
virtual Guard* getGuards(int screen, int number)=0;
	virtual void setRoomLink(int screen,int left,int right,int up,int down)=0;
	virtual void getRoomLink(int screen,int &left,int &right,int &up,int &down)=0;

	virtual void getStartPosition(int &screen, int &location,Direction &direction)=0;
	virtual void setStartPosition(int screen, int location,Direction direction)=0;
	virtual bool getDebugPosition(int &screen, int &location,Direction &direction)=0;
	virtual bool setDebugPosition(int screen, int location,Direction direction)=0;
	virtual void clearDebugPosition()=0;

	virtual void setDoorEvent(int event, int screen,int location, int next)=0;
	virtual void getDoorEvent(int event, int &screen,int &location, int &next)=0;

	int countMax();

protected:
	int countRooms;

};

#endif
