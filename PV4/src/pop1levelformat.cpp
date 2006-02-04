#include <iostream>
#include "levelformat.h"
#include "pop1levelformat.h"
#include "guard.h"
#include "tile.h"
#include "pop1tile.h"
#include "endiansafestream.h"

using namespace std;

Pop1LevelFormat::Pop1LevelFormat(iesstream& stream, int blockSize){
//open
/*
                   Table 4.1: DAT 1.0 Level blocks
                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Length Offset  Block Name
  ~~~~~~ ~~~~~~  ~~~~~~~~~~
  720    0       wall
  720    720     pop1_background
  256    1440    door I
  256    1696    door II
  96     1952    links
  64     2048    unknown I
  3      2112    start_position
  3      2115    unknown II
  1      2116    unknown III
  24     2119    guard_location
  24     2143    guard_direction
  24     2167    unknown VI (a)
  24     2191    unknown VI (b)
  24     2215    guard_skill
  24     2239    unknown VI (c)
  24     2263    guard_colour
  16     2287    unknown VI (d)
  2      2303    0F 09 (2319)

*/

	this->countRooms=24;

	//Read walls
	this->walls=new unsigned char[720];
	stream.read(this->walls,720);

	//Read backs
	this->backs=new unsigned char[720];
	stream.read(this->backs,720);

	//Read door I
	this->doorI=new unsigned char[256];
	stream.read(this->doorI,256);

	//Read door II
	this->doorII=new unsigned char[256];
	stream.read(this->doorII,256);

	//Read links
	this->links=new unsigned char[96];
	stream.read(this->links,96);

	//Read unknown I
	this->uI=new unsigned char[64];
	stream.read(this->uI,64);

	//Read start_position
	this->start_position=new unsigned char[3];
	stream.read(this->start_position,3);

	//Read unknown II
	this->uII=new unsigned char[3];
	stream.read(this->uII,3);

	//Read unknown III
	this->uIII=new unsigned char[1];
	stream.read(this->uIII,1);

	//Read guard_location
	this->guard_location=new unsigned char[24];
	stream.read(this->guard_location,24);

	//Read guard_direction
	this->guard_direction=new unsigned char[24];
	stream.read(this->guard_direction,24);

	//Read unknown IV (a)
	this->iIVa=new unsigned char[24];
	stream.read(this->iIVa,24);

	//Read unknown IV (b)
	this->uIVb=new unsigned char[24];
	stream.read(this->uIVb,24);

	//Read guard_skill
	this->guard_skill=new unsigned char[24];
	stream.read(this->guard_skill,24);

	//Read unknown IV (c)
	this->uIVc=new unsigned char[24];
	stream.read(this->uIVc,24);

	//Read guard_colour
	this->guard_colour=new unsigned char[24];
	stream.read(this->guard_colour,24);

	//Read unknown IV (d)
	this->uIVd=new unsigned char[16];
	stream.read(this->uIVd,16);

	//Read 0F 09 (2319)
	this->uV=new unsigned char[2];
	stream.read(this->uV,2);

}

Pop1LevelFormat::Pop1LevelFormat(int levelNumber) {

}

void Pop1LevelFormat::save(ostream* level) {
}

Pop1LevelFormat::~Pop1LevelFormat() {
}

/* Map */

void Pop1LevelFormat::setTile(int screen, int location, Tile* t) {
	if (screen<1||screen>24||location<0||location>29) throw -10;
	this->walls[(screen-1)*30+location]=t->getCode();
	this->backs[(screen-1)*30+location]=t->getAttr();
}

Tile* Pop1LevelFormat::getTile(int screen, int location) {
//cout << "s="<<screen<<" l="<<location<<endl;
	if (screen<1||screen>24||location<0||location>29) throw -10;
	return (Tile*)new Pop1Tile(
		this->walls[(screen-1)*30+location],
		this->backs[(screen-1)*30+location]
	);
}

/* Guards */

bool Pop1LevelFormat::addGuard(int screen, int location, Guard* g) {
	if (this->guard_location[screen-1]>29) return false;
	this->guard_location[screen-1]=location;
	this->guard_direction[screen-1]=g->getDirection();
	this->guard_skill[screen-1]=g->getSkill();
	this->guard_colour[screen-1]=g->getColour();

	return true;
}

bool Pop1LevelFormat::delGuard(int screen, int location) {
	if (!(this->guard_location[screen-1]>29)) return false;
	this->guard_location[screen-1]=30;
	this->guard_direction[screen-1]=0;
	this->guard_skill[screen-1]=0;
	this->guard_colour[screen-1]=0;
	return true;
}

Guard* Pop1LevelFormat::getGuards(int screen, int number){
	if (number!=0) return NULL;
	if (this->guard_location[screen-1]>29) return NULL;
	return new Guard(
		(Direction)this->guard_direction[screen-1],
		this->guard_skill[screen-1],
		this->guard_colour[screen-1]
	);
}

/* Links */

void Pop1LevelFormat::setRoomLink(int screen,int left,int right,int up,int down){
	this->links[(screen-1)*4]=left;
	this->links[(screen-1)*4+1]=right;
	this->links[(screen-1)*4+2]=up;
	this->links[(screen-1)*4+3]=down;
}

void Pop1LevelFormat::getRoomLink(int screen,int &left,int &right,int &up,int &down){
	left=this->links[(screen-1)*4];
	right=this->links[(screen-1)*4+1];
	up=this->links[(screen-1)*4+2];
	down=this->links[(screen-1)*4+3];
}

/* Start Positions */

void Pop1LevelFormat::getStartPosition(int &screen, int &location,Direction &direction) {
	screen=this->start_position[0];
	if (screen<0||screen>24) throw 23; //bad file format
	location=this->start_position[1];
	if (location>29) throw 23; //bad file format
	direction=(Direction)this->start_position[2];
}


void Pop1LevelFormat::setStartPosition(int screen, int location,Direction direction) {
	if (screen<0||screen>24) throw 24;
	this->start_position[0]=screen;
	if (location>29) throw 24;
	this->start_position[1]=location;
	this->start_position[2]=(unsigned char)direction;
}

bool Pop1LevelFormat::getDebugPosition(int &screen, int &location,Direction &direction) {}
bool Pop1LevelFormat::setDebugPosition(int screen, int location,Direction direction) {}
void Pop1LevelFormat::clearDebugPosition() {}

/* Doors */

void Pop1LevelFormat::setDoorEvent(int event, int S,int L, int T) {
	unsigned char* b1=&this->doorI[event];
	unsigned char* b2=&this->doorII[event];

	/*
	 Let's define:
		Screen as S and it is a number from 1 to 24 (5 bits)
		 S = s1 s2 s3 s4 s5
			where sn is the bit n of the binary representation of S
		Location as L and is a number from 0 to 29 (5 bits)
		 L = l1 l2 l3 l4 l5
			where ln is the bit n of the binary representation of L
		 This number is according to the location format specifications.
		Trigger-next as T and is a 1 for "on" or a 0 for "off" (1 bit)
		 T = t1

	 Byte I  has the form: t1 s4 s5 l1 l2 l3 l4 l5
	 Byte II has the form: s1 s2 s3  0  0  0  0  0
	*/

	*b1=(T?1:0)<<7|((S&3)<<5)|L;
	*b2=(S&0x1C)<<5;

}

void Pop1LevelFormat::getDoorEvent(int event, int &S,int &L, int &T) {
	unsigned char b1=this->doorI[event];
	unsigned char b2=this->doorII[event];

	T=b1>>7;
	L=b1&0x1f;
	S=((b1>>5)&3)|b2>>3;

}

