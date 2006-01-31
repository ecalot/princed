#include <iostream>
#include "levelformat.h"
#include "guard.h"
#include "tile.h"

using namespace std;

LevelFormat::LevelFormat(iesstream& level, int blockSize){


}

LevelFormat::LevelFormat(){}

LevelFormat::LevelFormat(int levelNumber) {




}
/*
void LevelFormat::save(ostream* level) {
}
*/
LevelFormat::~LevelFormat() {
}

int LevelFormat::countMax(){
	return this->countRooms;
}
/*

setTile(int screen, int location, tile t);
getTile(int screen, int location, tile &t);

bool addGuard(int screen, int location, guard g);
bool delGuard(int screen, int location);
bool getGuards(int screen, int number, guard &g);



void LevelFormat::setRoomLink(int screen,int left,int right,int up,int down){}
void LevelFormat::getRoomLink(int screen,int &left,int &right,int &up,int &down){}


getStartPosition(int &screen, int &location,&direction);
setStartPosition(int screen, int location,direction);
bool getDebugPosition(int &screen, int &location,&direction);
bool setDebugPosition(int screen, int location,direction);
clearDebugPosition();


private:
int countRooms;
*/

