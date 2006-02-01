#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include "levelFormat.h"
#include "guard.h"
#include "tile.h"

class Level {
public:
	Level(const char* file); //open
	Level(int popVersion,int LevelNumber); // new

	void save();
	void save(const char* file);

	~Level();

	/*
	plvInfo getInfo();
	setInfo(plvInfo i);
	*/

	int getHeight();
	int getWidth();
	int countRooms();

	bool addGuard(int floor,int col,Guard g);
	bool delGuard(int floor,int col);
	bool moveGuard(int floor,int col,int nfloor,int ncol);
	bool getGuard(int floor,int col,Guard &g);
	/*vector <floor,col> getGuards()*/

	void setTile(int floor,int col,Tile* tile);
	Tile* getTile(int floor,int col);
	void copyTiles(int sfloor,int scol,int efloor,int ecol);


	bool addTrigger(int triggerfloor,int triggercol,int targetfloor,int targetcol);
	bool delTrigger(int triggerfloor,int triggercol,int targetfloor,int targetcol);
	/*vector <floor,col> getTargets(int triggerfloor,int triggercol);
	vector <floor,col> getTriggers(int targetfloor,int targetcol);
	*/

	void getStartPosition(int &floor,int &col,Direction &direction);
	void setStartPosition(int floor,int col,Direction direction);
	bool getDebugPosition(int &floor,int &col,Direction &direction);
	void setDebugPosition(int floor,int col,Direction direction);
	void clearDebugPosition();
	void switchPositions();

	/*
	bool undo()
	bool redo()
	*/

private:
	string* fileName;
#define MATRIX_HEIGHT (31+1+31)
#define MATRIX_WIDTH (31+1+31)

#define SIZE_OF_MATRIX (MATRIX_HEIGHT*MATRIX_WIDTH)
#define matrix(x,y) this->screenMatrix[MATRIX_WIDTH*y+x]

#define MATRIX_CENTER_X 32
#define MATRIX_CENTER_Y 32

	char screenMatrix[SIZE_OF_MATRIX];
	int rs;
	int cs;
	int re;
	int ce;

	LevelFormat* level;

	int firstFreeTarget;

	int* screens;

	//Functions
	void abstractToFormat(int floor,int col, int &screen, int &location);
	void formatToAbstract(int &floor,int &col, int screen, int location);

	void floorColToXY(int floor,int col, int &x, int &y);


	void arrangeRooms(); /* throws: roomsNotAdjacent */
	void linkRecurse(int x, int y, int room);

	int addScreen(int x, int y);

};

#endif
