#include <iostream>
#include "endiansafestream.h"

#include "levelformat.h"
#include "pop1levelformat.h"
#include "level.h"
#include "guard.h"
#include "tile.h"
#include "outertile.h"

using namespace std;
/*
                   Table 6.1: PLV blocks
                   ~~~~~~~~~~~~~~~~~~~~~

   Size Offset Description                  Type   Content
   ~~~~ ~~~~~~ ~~~~~~~~~~~                  ~~~~   ~~~~~~~
      7      0 Magic identifier             text   "POP_LVL"
      1      7 POP version                  UC     0x01
      1      8 PLV version                  UC     0x01
      1      9 Level Number                 UC
      4     10 Number of fields             UL
      4     14 Block 1: Level size (B1)     UL     2306/2305
     B1     18 Block 1: Level code          -
      4  18+B1 Block 2: User data size (B2) UL
     B2  22+B1 Block 2: User data           -
*/
typedef char* fieldPointer;

Level::Level(const char* file) { //open
	//open the file
  iesstream stream(file);
//cout<<"Opening level "<< file <<endl;

	//check the magic
	char magic[7];
	stream.read(magic,7);
	if (strncmp(magic,"POP_LVL",7)) throw -3;

	//read pop version
	unsigned char popVersion;
	stream.read(popVersion);

	//read plv version
	unsigned char plvVersion;
	stream.read(plvVersion);

	//level number
	unsigned char level;
	stream.read(level);

	//number of fields
	unsigned long nf;
	stream.read(nf);

	//level size
	unsigned long b1;
	stream.read(b1);

	//level code
	unsigned char checksum;
	stream.read(checksum);

	switch (popVersion) {
		case 1:
//cout<<"is pop1"<<endl;
			this->level=new Pop1LevelFormat(stream,b1);
			break;
		case 2:
//			this->level=new Pop2LevelFormat(stream,b1);
		default:
			throw -3;
	}

	//user data size
	unsigned long b2;
	stream.read(b2);
//cout<<"b2 "<<b2<<endl;

	//alloc user data (TODO: use integrity checks here before doing the allocation)
	char* ud=new char[b2];
	stream.read(ud,b2);

	//process user data
	fieldPointer* fields=new fieldPointer[nf*2];
	unsigned int currentField=1;

	fields[0]=ud;
	for (unsigned int i=0;i<b2&&currentField<nf*2;i++) {
		if (!ud[i]) fields[currentField++]=ud+i+1;
	}

	for (unsigned int i=0;i<nf;i++) {
		cout<<"f['"<<fields[i*2]<<"']='"<<fields[i*2+1]<<"'"<<endl;
		info[fields[i*2]]=fields[i*2+1];
	}

	if (currentField!=nf*2 || ud[b2-1]!=0) throw -4;
//cout<<"cf "<<currentField<<" nf "<<nf<<" b2 "<<b2<<endl;

	//TODO: generate a hash table with this values

	//remember the file name
	this->fileName=new string(file);
//cout<<"ok"<<endl;

	//Finally arrange the rooms
	this->arrangeRooms();

	//Free memory
	//delete[] fields;
	delete[] ud;
}

Level::Level(int popVersion,int LevelNumber){} // new

void Level::save(){}
void Level::save(const char* file){}

Level::~Level(){}

/*map<const char*,const char*>* Level::getInfo() { //TODO: decide if let map as public and forget about this
	return &(this->info);
}*/

void Level::linkRecurse(int x, int y, int room) {
 if (matrix(x,y)==-1) {
  matrix(x,y)=room;
  if (room) {
   int up,down,left,right;
   this->level->getRoomLink(room,left,right,up,down);
   linkRecurse(x+1,y,right);
   linkRecurse(x-1,y,left);
   linkRecurse(x,y+1,down);
   linkRecurse(x,y-1,up);
  }
 } else {
  if (matrix(x,y)!=room) throw -1;
 }
}

void Level::arrangeRooms() {
 //initialize matrix
 for (int i=0;i<SIZE_OF_MATRIX;i++)
  this->screenMatrix[i]=-1;

 int startScreen,junk;
 Direction junk2;

 //Fill in the matrix with the rooms
 this->level->getStartPosition(startScreen,junk,junk2);
 this->linkRecurse(MATRIX_CENTER_X,MATRIX_CENTER_Y,startScreen);

 //Get the matrix limits
 for (int i=0;i<MATRIX_HEIGHT;i++) {
  for (int j=0;j<MATRIX_WIDTH;j++) {
   if (!matrix(i,j)) {
    this->cs=i;
    i=j=MATRIX_WIDTH;
   }
  }
 }

 for (int i=MATRIX_HEIGHT;i--;) {
  for (int j=0;j<MATRIX_WIDTH;j++) {
   if (!matrix(i,j)) {
    this->ce=i;
    i=0;
    j=MATRIX_WIDTH;
   }
  }
 }

 for (int i=0;i<MATRIX_WIDTH;i++) {
  for (int j=0;j<MATRIX_HEIGHT;j++) {
   if (!matrix(j,i)) {
    this->rs=i;
    i=j=MATRIX_WIDTH;
   }
  }
 }

 for (int i=MATRIX_WIDTH;i--;) {
  for (int j=0;j<MATRIX_HEIGHT;j++) {
   if (!matrix(j,i)) {
    this->re=i;
    i=0;
    j=MATRIX_WIDTH;
   }
  }
 }

cout<<"level=("<<this->rs<<","<<this->cs<<"),("<<this->re<<","<<this->ce<<")"<<endl;
	for (int j=this->rs;j<=this->re;j++) {
		for (int i=this->cs;i<=this->ce;i++) {
			cout<<(int)(matrix(i,j))<<" ";
		}
		cout<<endl;
	}


	//Now it's time to add all screens in the screens array
	this->screens=new int[this->level->countMax()];
	for (int i=0;i<this->level->countMax();i++)
		this->screens[i]=0;

	for (int i=this->rs;i<this->re;i++) {
		for (int j=this->cs;i<this->ce;i++) {
			if (matrix(i,j)>0) {
				if (matrix(i,j)>this->level->countMax()) throw -13;
				this->screens[matrix(i,j)]=MATRIX_WIDTH*j+i;
			}
		}
	}
}

int Level::getHeight(){
 return (this->re-this->rs-1)*3+2;
}

int Level::getWidth(){
 return (this->ce-this->cs-1)*10+2;
}

bool Level::addGuard(tPos pos,Guard g){return false;}
bool Level::delGuard(tPos pos){return false;}
bool Level::moveGuard(tPos spos,tPos tpos){return false;}
bool Level::getGuard(tPos pos,Guard &g){return false;}
/*vector <floor,col> getGuards()*/


void Level::setTile(tPos pos,Tile* tile) {
	int screen;
	int location;

	this->abstractToFormat(pos,screen,location);

	//check if the screen exists
	if (screen<1) {
		if (screen==-1) throw -80; //room is too far and cannot be displayed in the level

		//the screen doesn't exist, but it's near, we'll try to create it
		int x,y;
		this->floorColToXY(pos,x,y);

		screen=this->addScreen(x,y);
	}

	//and now set the tile

}

int Level::addScreen(int x, int y) {
	int result=-1;
	for (int i=0;i<this->level->countMax() && result==-1;i++) {
		if (!this->screens[i]) result=i;
	}
	if (result==-1) throw -87; //level full

	//Link the new screen
	int left,right,up,down;
	int screen;

	screen=matrix(x-1,y); //left
	if (screen>0) {
		this->level->getRoomLink(screen,left,right,up,down);
		this->level->setRoomLink(screen,left,result,up,down);
	}

	screen=matrix(x+1,y); //right
	if (screen>0) {
		this->level->getRoomLink(screen,left,right,up,down);
		this->level->setRoomLink(screen,result,right,up,down);
	}

	screen=matrix(x,y+1); //down
	if (screen>0) {
		this->level->getRoomLink(screen,left,right,up,down);
		this->level->setRoomLink(screen,left,right,result,down);
	}

	screen=matrix(x,y-1); //up
	if (screen>0) {
		this->level->getRoomLink(screen,left,right,up,down);
		this->level->setRoomLink(screen,left,right,up,result);
	}

	this->level->setRoomLink(result,matrix(x-1,y),matrix(x+1,y),matrix(x,y-1),matrix(x,y+1));

	//recalculate
	this->arrangeRooms();
	return 0; //TODO: return true if the level must be redrawn!
}

//TODO: delScreen

Tile* Level::getTile(tPos pos) {
	int screen;
	int location;

	this->abstractToFormat(pos,screen,location);
//cout<<"f("<<floor<<","<<col<<")=("<<screen<<","<<location<<")"<<endl;
	if (screen<1) {
		int r=0;
		if (!screen) { //only screens 0 (means near to a level)
			//calculate x and y of the screen
			int x,y;
			this->floorColToXY(pos,x,y);
			if (pos.col%10==0  && matrix(x-1,y)>0) r|=DL;
			if (pos.col%10==9  && matrix(x+1,y)>0) r|=DR;
			if (pos.floor%3==0 && matrix(x,y-1)>0) r|=DU;
			if (pos.floor%3==2 && matrix(x,y+1)>0) r|=DD;
		}
		return (Tile*)new OuterTile(r);
	} else {
		return this->level->getTile(screen,location);
	}
}

void Level::copyTiles(tPos spos, tPos epos, tPos tpos) {
	
	//equal col and floor are done spos==epos
	if (spos.floor==epos.floor) return;
	if (spos.col==epos.col) return;
	
	//order col and floor in case the selection is in other order
	if (spos.floor<epos.floor) {int aux=epos.floor;epos.floor=spos.floor;spos.floor=aux;}
	if (spos.col<epos.col)     {int aux=epos.col;epos.col=spos.col;spos.col=aux;}
	
	Tile* t;
	
	for (int i=0;i<epos.floor-spos.floor;i++) { //TODO: run over eux.floor and col
		for (int j=0;j<epos.col-spos.col;j++) {
			tPos aux;
			aux.floor=spos.floor+i;
			aux.col=spos.col+j;
			t=this->getTile(aux);
			aux.floor=tpos.floor+i;
			aux.col=tpos.col+j;
			this->setTile(aux,t); //TODO: handle sollapations
		}
	}
}

/*
bool addTrigger(int triggerfloor,int triggercol,int targetfloor,int targetcol)
bool delTrigger(int triggerfloor,int triggercol,int targetfloor,int targetcol)
vector <floor,col> getTargets(int triggerfloor,int triggercol)
vector <floor,col> getTriggers(int targetfloor,int targetcol)
*/
void Level::getStartPosition(tPos &pos,Direction &direction){
	int screen;
	int location;
	this->level->getStartPosition(screen,location,direction);
	this->formatToAbstract(pos,screen,location);
}

void Level::setStartPosition(tPos pos,Direction direction){
	int screen;
	int location;
	this->abstractToFormat(pos,screen,location);
	if (screen<1) throw -90;
	this->level->setStartPosition(screen,location,direction);
}

bool Level::getDebugPosition(tPos &pos,Direction &direction){
	int screen;
	int location;
	this->level->getDebugPosition(screen,location,direction);
	this->formatToAbstract(pos,screen,location);
	return screen!=0;
}

void Level::setDebugPosition(tPos pos,Direction direction){
	int screen;
	int location;
	this->abstractToFormat(pos,screen,location);
	if (screen<1) throw -90;
	this->level->setDebugPosition(screen,location,direction);
}

void Level::clearDebugPosition(){
	this->level->clearDebugPosition();
}

void Level::switchPositions(){
	int dscreen;
	int dlocation;
	Direction ddirection;

	this->level->getDebugPosition(dscreen,dlocation,ddirection);

	int sscreen;
	int slocation;
	Direction sdirection;

	this->level->getStartPosition(sscreen,slocation,sdirection);

	if (!dscreen) throw -91; //no debug position set

	this->level->setStartPosition(dscreen,dlocation,ddirection);
	this->level->setDebugPosition(sscreen,slocation,sdirection);
}

/*
bool undo();
bool redo();
*/

//Functions
void Level::floorColToXY(tPos pos, int &x, int &y){
	x=this->rs+(pos.floor+2)/3;
	y=this->cs+(pos.col+9)/10;
}

void Level::abstractToFormat(tPos pos, int &screen, int &location){
	//calculate x and y of the screen
	int x,y;
	floorColToXY(pos,x,y);

	//return values
	screen=matrix(y,x);
	location=(pos.col-1)%10+((pos.floor-1)%3)*10;
}

void Level::formatToAbstract(tPos &pos, int screen, int location){
	if (screen<1||screen>this->level->countMax()) throw -14;

	int posaux=this->screens[screen];

	int y=posaux/MATRIX_WIDTH;
	int x=posaux%MATRIX_WIDTH;

	y-=this->rs;
	x-=this->cs;

	pos.col=x*10+1;
	pos.floor=y*3+1;
}
