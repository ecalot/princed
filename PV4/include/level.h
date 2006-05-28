#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "levelformat.h"
#include "guard.h"
#include "tile.h"

#include <map>
#include <string>

typedef struct {
	int floor;
	int col;
} tPos;

class Level {
public:
	Level (const char *file);	//open
	Level (int popVersion, int LevelNumber);	// new

	void save ();
	void save (const char *file);

	~Level ();

	map < const char *, const char *>info;
	//map<const char*,const char*>* getInfo(); //warning: will be string in a future

	int getHeight ();
	int getWidth ();
	int countRooms ();

	bool addGuard (tPos pos, Guard g);
	bool delGuard (tPos pos);
	bool moveGuard (tPos spos, tPos tpos);
	bool getGuard (tPos pos, Guard & g);
	//vector<tPos> getGuardPositions();

	void setTile (tPos pos, Tile * tile);
	Tile *getTile (tPos pos);
	void copyTiles (tPos ssource, tPos esource, tPos target);


	bool addTrigger (int triggerfloor, int triggercol, int targetfloor,
			 int targetcol);
	bool delTrigger (int triggerfloor, int triggercol, int targetfloor,
			 int targetcol);
	//vector <tPos> getTargets(tPos trigger);
	//vector <tPos> getTriggers(tPos target);


	void getStartPosition (tPos & pos, Direction & direction);
	void setStartPosition (tPos pos, Direction direction);
	bool getDebugPosition (tPos & pos, Direction & direction);
	void setDebugPosition (tPos pos, Direction direction);
	void clearDebugPosition ();
	void switchPositions ();

	/*
	   bool undo()
	   bool redo()
	 */

private:
	string * fileName;

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

	LevelFormat *level;

	int firstFreeTarget;

	int *screens;

	//Functions
	void abstractToFormat (tPos pos, int &screen, int &location);
	void formatToAbstract (tPos & pos, int screen, int location);

	void floorColToXY (tPos pos, int &x, int &y);


	void arrangeRooms ();		/* throws: roomsNotAdjacent */
	void linkRecurse (int x, int y, int room);

	int addScreen (int x, int y);

	//Configuration
	bool checkIntegrity;
	bool genericLinks;

};

#endif

