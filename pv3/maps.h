/***************************************************************\
|                  Format Block Specification                   |
\************************************************************** /

Len	Offs	BlockName & Description
1 	0   	validador (0-1953)
720	1   	wall
720	721 	back
18	1441	doors1
238	1459	unknown1 (22/215?) (por convencion: empty para guardar info)
18	1697	doors2
238	1715	unknown2 (22/215?) (por convencion: empty para guardar info)
96	1953	links
64	2049	unknown3 (peligroso)
3 	2113	startPosition (1: screen; 2: module; 3: direction)
3 	2116	unknownSP (Dafault: triple 0) (por convencion: 1: comienzo de string info; 2 y 3: Debug Position)
1 	2117	startPositionUnknown (direction?)
24	2120	guardPosition
24	2144	guardDirection (00: right; FF: left)
1 	2168	unknown4/1 (por convencion: validator 1953-2306)
23	2169	unknown4/23 (por convencion: empty para guardar info)
24	2192	unknown5 (por convencion: empty para guardar info)
24	2216	guardSkill
24	2240	unknown6 (por convencion: empty para guardar info)
24	2264	guardColor
16	2288	unknown7 (por convencion: empty para guardar info)
2 	2304	0F 09
  	2306	New Level

/ **************************************************************\
|                          Inclusiones                          |
\***************************************************************/

#ifndef INCLUDED_MAPS_H
#define INCLUDED_MAPS_H

#include <stdio.h>

/***************************************************************\
|                  Definiciones del Precompilador               |
\***************************************************************/

#define MAPS_MAX_SCREENS    24
#define MAPS_MAX_LENGTH     49
//MAPS_MAX_SCREENS*2+1
#define MAPS_LINKMAP_CENTER 24
//(char)(MAPS_MAX_SCREENS*2+1)/2

#define MAPS_DIRECTION_RIGHT 0x00
#define MAPS_DIRECTION_LEFT  0xFF

#define MAPS_BLOCK_SIZEOF_LEVEL    					2306
#define MAPS_BLOCK_OFFSET_LEVELS   					6

#define MAPS_BLOCK_OFFSET_VALIDATOR_WALL		0
#define MAPS_BLOCK_SIZEOF_VALIDATOR_WALL		1
#define MAPS_BLOCK_OFFSET_WALL							1
#define MAPS_BLOCK_SIZEOF_WALL							720
#define MAPS_BLOCK_SIZEOF_BACK							720
#define MAPS_BLOCK_OFFSET_BACK							721
#define MAPS_BLOCK_OFFSET_LINK							1953
#define MAPS_BLOCK_OFFSET_VALIDATOR_LINK		2168
#define MAPS_BLOCK_OFFSET_GUARD_DIRECTION 	2144
#define MAPS_BLOCK_OFFSET_GUARD_POSITION  	2120
#define MAPS_BLOCK_OFFSET_START_POSITION    2113
#define MAPS_BLOCK_OFFSET_GUARD_SKILL     	2216
#define MAPS_BLOCK_OFFSET_GUARD_COLOR     	2264
#define MAPS_BLOCK_OFFSET_UNKNOWN_1         1459
#define MAPS_BLOCK_SIZEOF_UNKNOWN_1         238
#define MAPS_BLOCK_OFFSET_UNKNOWN_2         1715
#define MAPS_BLOCK_SIZEOF_UNKNOWN_2         238
#define MAPS_BLOCK_OFFSET_UNKNOWN_4         2169
#define MAPS_BLOCK_SIZEOF_UNKNOWN_4         23
#define MAPS_BLOCK_SIZEOF_UNKNOWN_5         24
#define MAPS_BLOCK_OFFSET_UNKNOWN_5         2192
#define MAPS_BLOCK_SIZEOF_UNKNOWN_6         24
#define MAPS_BLOCK_OFFSET_UNKNOWN_6         2240
#define MAPS_BLOCK_SIZEOF_UNKNOWN_7         16
#define MAPS_BLOCK_OFFSET_UNKNOWN_7         2288
#define MAPS_BLOCK_SIZEOF_GATE_1            18
#define MAPS_BLOCK_SIZEOF_GATE              18
//256
#define MAPS_BLOCK_OFFSET_GATE_1            1441
#define MAPS_BLOCK_SIZEOF_GATE_2            18
#define MAPS_BLOCK_OFFSET_GATE_2            1697
#define MAPS_BLOCK_SIZEOF_FILE              37031

#define MAPS_OBJ_BAL_D 0x26
#define MAPS_OBJ_BAL_U 0x2f

//Constantes copadas
#define MAPS_sUp			2
#define MAPS_sDown		3
#define MAPS_sLeft		0
#define MAPS_sRight		1


#include "structures.h"

/*
char pantalla --> the screen number
char p --> floor (0 to 2)
char b --> square (0 to 9)

idem setWall but it sets the background property
char valor --> value (it has to be a MAP_BCK_* define)
vidas is something like lives but it depends on the level you are
sentido is where is the guard looking at
erase has to be 0, if you use a non 0 value you set no guard on this screen
*/

class cMaps {
public:
	cMaps(char* fileName);
	bool exists();
	~cMaps();
	tLevel* lev;

private:


/***************************************************************\
|                           Prototipos                          |
\***************************************************************/

public: //prototipos publicos

	//File management procedures
	char mLoadPED(const char* vFile,tLevel* lev);
	char mLoadDAT(const char* vFile,tLevel* lev,char levelNumber); //Tested 5
	char mSavePED(const char* vFile,tLevel* lev); //Tested 8
	char mSaveDAT(const char* vFile,tLevel* lev); //Tested 3

	//char mChangeLevel(tLevel* lev,char newLevel); //0 si no hay cambios; si hay cambios en paredes devuelve la camtidad de paredes cambiadas
	//char mAllowedOn(char levelnumber,char wall); //1 si wall es permitido en levelnumber; 0 si no

	//Information methods
	void mSetText (tLevel* lev,const char* text);
	void mGetText (tLevel* lev,char* text);

	//tLevel Set methods
	void mSetWall (tLevel* lev,char pantalla,char p,char b,char valor);
	void mSetLink (tLevel* lev,char pantalla,char tipo,char valor);
	void mSetGuard(tLevel* lev,char pantalla,char p,char b,char vidas,char color,char sentido,char erase);
	void mSetBack (tLevel* lev,char pantalla,char p,char b,char valor);
	//tLevel Get Methods
	char mGetWall (tLevel* lev,char pantalla,char p,char b) const;
	char mGetLink (tLevel* lev,char pantalla,char tipo);
	void mGetGuard(tLevel* lev,char pantalla,char* p,char* b,char* vidas,char* color,char* sentido,char* erase) const;
	char mGetBack (tLevel* lev,char pantalla,char p,char b) const;

	//Gate handling Methods
	/*
	Public:
	mCreateEventList
	mAddToEventList
	mGetEventList
	mRemFromEventList
	mRemoveEventList

	Private:

	mSaveEventList
	mLoadEventList
	*/


	//Screen Links Handling Methods
	char mGetScreen    (tLevel* lev, char i,char j, char* error) const;
	void mGetMainScreen(tLevel* lev, char* i,char* j) const;
	void mRemScreen    (tLevel* lev, char i,char j, char* error);
	void mAddScreen    (tLevel* lev, char i,char j, char* error);
	void mGetSize      (tLevel* lev, char* i,char* j) const;
	char mGetScrCount  (tLevel* lev);
		//Nota: si mGetScrCount(lev) es 24, entonces el nivel esta lleno
	void mCpyScreen    (tLevel* lev, char fromi,char fromj,char toi,char toj, char* error);

	//Start position handling
	void mGetStartPosition(tLevel* lev,char* pantalla, char* p, char *b,char *sentido,char *sentido2);
	void mSetStartPosition(tLevel* lev,char pantalla, char p, char b,char sentido,char sentido2);
	void mGetDebugPosition(tLevel* lev,char* pantalla, char* p, char *b,char *sentido);
	void mSetDebugPosition(tLevel* lev,char pantalla, char p, char b,char sentido);
		//Nota: DebugPosition puede no existir en lev, en ese caso la convension es no existe si pantalla igual a 0, de lo contrario existe y la pantalla inicial es pantalla

private: //Prototipos privados
	void mSetArray(tLevel* lev, const char *pos,int from,unsigned int length,int validator);
	void mGetArray(tLevel* lev, char* pos,int from,int length);
	void getRawEvent(tLevel* lev,char id,tGateEvent* event);
	void setRawEvent(tLevel* lev,char id,tGateEvent* event);
	void mLoadEventList(tLevel* lev);
	void anularEvento(tGateEvent *e);
	char obtenerFila(tGateRow *fila,unsigned char c,int k,tLevel* lev);
	char sameEvent(tGateEvent event1,tGateEvent event2);
	tGateEvent setFlag(tGateEvent evento,char flag);
	char belongsToDiskList(tLevel* lev, tGateRow *fila,int i);
	void apuntar(tLevel* lev,unsigned char i);
	char intertLeft(tLevel* lev,tGateRow fila,int inicioFila,int numeroFila,char mode);
	char mSaveEventList(tLevel* lev);
	void mRecLink(char i, char j, tLevel* lev); //recursive prototype
	void mRecIndirect(char i, char j, tLevel* lev, char scr, char pos);
	void mCalcularLimUp (tLevel* lev);
	void mCalcularLimDown (tLevel* lev);
	void mCalcularLimLeft (tLevel* lev);
	void mCalcularLimRight (tLevel* lev);
	void mCreateLinkMap(tLevel* lev);
	void cMaps::getGateAsociation(tLevel* lev,char id,char* scr,char* p,char* b, char *termino);
	void cMaps::setGateAsociation(tLevel* lev,char id,char scr,char p,char b, char termino);
	void cMaps::shiftEventId(tLevel* lev,char from,char delta);
	void cMaps::insertEvent(tLevel* lev,char id,char scr,char p,char b);



}
;

#endif
