/*  FreePrince - POP1 remake
    Copyright (C) 2003,2004 Princed Development Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    The authors of this program may be contacted at http://forum.princed.com.ar
*/

/*
maps.c: Freeprince : Map handling library
¯¯¯¯¯¯
 Copyright 2003,2004 Princed Development Team
  Created: 24 Mar 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include <string.h> /* mempcy */
#include <stdlib.h> /* malloc */
#include "maps.h"
#include "room.h"
#include "kid.h"
#include "resources.h"

#define slevel(field) (((tMap*)(((tData*)map)->pFrames))->field)

/* Privates 

void maps_getStartPosition(int* pantalla, int* p, int *b,tDirection *sentido,tDirection *sentido2) {
	int valor;

	*pantalla =slevel[MAPS_BLOCK_OFFSET_START_POSITION];
	valor     =slevel[MAPS_BLOCK_OFFSET_START_POSITION+1];
	*b        =(valor%10);
	*p        =(valor/10);
	*sentido  =(slevel[MAPS_BLOCK_OFFSET_START_POSITION+2])?eRight:eLeft;
	*sentido2 =(slevel[MAPS_BLOCK_OFFSET_START_POSITION+6])?eRight:eLeft;
}

void maps_getGuard(int pantalla,int *p,int *b,int *skill,int *color,tDirection *sentido,int *exists) {
	 Posicion 
	unsigned char valor=(slevel[(MAPS_BLOCK_OFFSET_GUARD_POSITION+sscreen-1)]);
	*exists = (valor<30);
	*b      = (valor%10);
	*p      = (valor/10);
	 sentido 
	*sentido=slevel[MAPS_BLOCK_OFFSET_GUARD_DIRECTION+pantalla-1]?eRight:eLeft;
	* skill *
	*skill  =slevel[MAPS_BLOCK_OFFSET_GUARD_SKILL+pantalla-1];
	* Color *
	*color  =slevel[MAPS_BLOCK_OFFSET_GUARD_COLOR+pantalla-1];
}
*/
/* Publics */
void* mapLoadLevel(tMemory level) {
	tMap* map=(tMap*)malloc(sizeof(tMap));
	memcpy(map->fore,level.array+MAPS_BLOCK_OFFSET_WALL,30*24);
	memcpy(map->back,level.array+MAPS_BLOCK_OFFSET_BACK,30*24);
	memcpy(map->start,level.array+MAPS_BLOCK_OFFSET_START_POSITION,3);
	memcpy(map->links,level.array+MAPS_BLOCK_OFFSET_LINK,4*24);
	return (void*)map;
}

tRoom mapGetRoom(tData* map, tRoomId roomAux) {
	tRoom result;

	/* SET room id*/
	result.id=roomAux;
	result.level=map->pFrames;
	
	/* SET room links */
	memcpy(result.links,slevel(links)+((roomAux-1)*4),4);
	/* up corners */
	roomAux=result.links[2];
	if (roomAux) {
		result.corners[0]=*(slevel(links)+((roomAux-1)*4)+0);
		result.corners[1]=*(slevel(links)+((roomAux-1)*4)+1);
	} else {
		result.corners[0]=0;
		result.corners[1]=0;
	}
	/* down corners */
	roomAux=result.links[3];
	if (roomAux) {
		result.corners[2]=*(slevel(links)+((roomAux-1)*4)+0);
		result.corners[3]=*(slevel(links)+((roomAux-1)*4)+1);
	} else {
		result.corners[2]=0;
		result.corners[3]=0;
	}
	
	/* SET corner bytes */
	/* left+up */
	if ((roomAux=result.corners[0])) {
		result.fore[0]=*(slevel(fore)+30*(roomAux-1)+29);
		result.back[0]=*(slevel(back)+30*(roomAux-1)+29);
	} else {
		result.fore[0]=MAP_F_WALL;
		result.back[0]=MAP_B_NONE;
	}
	/* right+up */
	if ((roomAux=result.corners[1])) {
		result.fore[11]=*(slevel(fore)+30*(roomAux-1)+20);
		result.back[11]=*(slevel(back)+30*(roomAux-1)+20);
	} else {
		result.fore[11]=MAP_F_FREE;
		result.back[11]=MAP_B_NONE;
	}
	/* left+down */
	if ((roomAux=result.corners[2])) {
		result.fore[48]=*(slevel(fore)+30*(roomAux-1)+9);
		result.back[48]=*(slevel(back)+30*(roomAux-1)+9);
	} else {
		result.fore[48]=MAP_F_WALL;
		result.back[48]=MAP_B_NONE;
	}
	/* right+down */
	if ((roomAux=result.corners[3])) {
		result.fore[59]=*(slevel(fore)+30*(roomAux-1)+0);
		result.back[59]=*(slevel(back)+30*(roomAux-1)+0);
	} else {
		result.fore[59]=MAP_F_WALL;
		result.back[59]=MAP_B_NONE;
	}

	/* Left room */
	if ((roomAux=result.links[0])) {
		result.fore[12]=*(slevel(fore)+30*(roomAux-1)+9);
		result.back[12]=*(slevel(back)+30*(roomAux-1)+9);
		result.fore[24]=*(slevel(fore)+30*(roomAux-1)+19);
		result.back[24]=*(slevel(back)+30*(roomAux-1)+19);
		result.fore[36]=*(slevel(fore)+30*(roomAux-1)+29);
		result.back[36]=*(slevel(back)+30*(roomAux-1)+29);
	} else {
		result.fore[12]=MAP_F_WALL;
		result.back[12]=MAP_B_NONE;
		result.fore[24]=MAP_F_WALL;
		result.back[24]=MAP_B_NONE;
		result.fore[36]=MAP_F_WALL;
		result.back[36]=MAP_B_NONE;	
	}

	/* Right room */
	if ((roomAux=result.links[1])) {
		result.fore[11]=*(slevel(fore)+30*(roomAux-1)+0);
		result.back[11]=*(slevel(back)+30*(roomAux-1)+0);
		result.fore[23]=*(slevel(fore)+30*(roomAux-1)+10);
		result.back[23]=*(slevel(back)+30*(roomAux-1)+10);
		result.fore[35]=*(slevel(fore)+30*(roomAux-1)+20);
		result.back[35]=*(slevel(back)+30*(roomAux-1)+20);
	} else {
		result.fore[11]=MAP_F_WALL;
		result.back[11]=MAP_B_NONE;
		result.fore[23]=MAP_F_WALL;
		result.back[23]=MAP_B_NONE;
		result.fore[35]=MAP_F_WALL;
		result.back[35]=MAP_B_NONE;	
	}

	/* Top room */
	if ((roomAux=result.links[2])) {
		memcpy(result.fore+1,slevel(fore)+30*(roomAux-1)+20,10);
		memcpy(result.back+1,slevel(back)+30*(roomAux-1)+20,10);
	} else {
		memcpy(result.fore+1,"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01",10); /* TODO: use tiles */
		memcpy(result.back+1,"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01",10); /* TODO: use tiles */
	}

	/* Bottom room */
	if ((roomAux=result.links[3])) {
		memcpy(result.fore+49,slevel(fore)+30*(roomAux-1)+0,10);
		memcpy(result.back+49,slevel(back)+30*(roomAux-1)+0,10);
	} else {
		memcpy(result.fore+49,"\0\0\0\0\0\0\0\0\0\0",10); /* TODO: use tiles */
		memcpy(result.back+49,"\0\0\0\0\0\0\0\0\0\0",10); /* TODO: use tiles */
	}

	/* Main room */
	memcpy(result.fore+13,slevel(fore)+30*(result.id-1)+0,10);
	memcpy(result.back+13,slevel(back)+30*(result.id-1)+0,10);
	memcpy(result.fore+25,slevel(fore)+30*(result.id-1)+10,10);
	memcpy(result.back+25,slevel(back)+30*(result.id-1)+10,10);
	memcpy(result.fore+37,slevel(fore)+30*(result.id-1)+20,10);
	memcpy(result.back+37,slevel(back)+30*(result.id-1)+20,10);

/* This will save a screen map into data.
 * 
 * data is a pre-allocated array with 10x3=30 bytes with the screen map
 * dumped over it in left-to-right/upper-to-lower oreder. 
 *
 * borders is another pre-allocated array with 10 upper bytes, 10 bottom
 * bytes, 3 left-side bytes, 3 right-side bytes and 4 bytes for the
 * corners. This is another 30 bytes. The order has to be determined!
 * In case the screen doesn't exist the 0x00 (free foreground) byte will
 * be returned for the bottom and right sides, the wall foreground for
 * the left side and the simple tile byte for the top side.
 * int levelGetScreenItems(unsigned char* data,unsigned char* borders) {
 * This will save a screen map modifiers into data.
 *
 * data and borders are in the same form as Walls but they contain
 * additional modifiers that must be defined in maps.h.
 * e.g. MAPS_ITEMS_DOOROPEN 0x01
 */

	return result;
}

void  mapStart(tData* map, tKid* kid, tRoomId *roomId) {
	/* kid->x,y */
	*roomId=slevel(start)[0];
	printf("mapStart: binding kid to map in room %d\n",*roomId);
	roomLoadGfx(RES_IMG_ENV_DUNGEON);
}

void  mapMove(tData* map) {
	slevel(time)++;
	if (slevel(time)==1000) slevel(time)=0;
}

























#ifdef OLD_MAP_SRC

/***************************************************************\
|                           Prototipos                          |
\***************************************************************/

	/* File management procedures */
	char mLoadPED(char* vFile,tLevel* lev);
	char mLoadDAT(char* vFile,tLevel* lev,char levelNumber); /* Tested 5 */
	char mSavePED(char* vFile,tLevel* lev); /* Tested 8 */
	char mSaveDAT(char* vFile,tLevel* lev); /* Tested 3 */

	/* char mChangeLevel(tLevel* lev,char newLevel); //0 si no hay cambios; si hay cambios en paredes devuelve la camtidad de paredes cambiadas */
	/* char mAllowedOn(char levelnumber,char wall); //1 si wall es permitido en levelnumber; 0 si no */

	/* Information methods */
	void mSetText (tLevel* lev,char* text);
	void mGetText (tLevel* lev,char* text);

	/* tLevel Set methods */
	void mSetWall (tLevel* lev,char pantalla,char p,char b,char valor);
	void mSetLink (tLevel* lev,char pantalla,char tipo,char valor);
	void mSetGuard(tLevel* lev,char pantalla,char p,char b,char vidas,char color,char sentido,char erase);
	void mSetBack (tLevel* lev,char pantalla,char p,char b,char valor);
	/* tLevel Get Methods */
	char mGetWall (tLevel* lev,char pantalla,char p,char b);
	char mGetLink (tLevel* lev,char pantalla,char tipo);
	void mGetGuard(tLevel* lev,char pantalla,char* p,char* b,char* vidas,char* color,char* sentido,char* erase);
	char mGetBack (tLevel* lev,char pantalla,char p,char b);

	/* Gate handling Methods */
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


	/* Screen Links Handling Methods */
	char mGetScreen    (tLevel* lev, char i,char j, char* error);
	void mGetMainScreen(tLevel* lev, char* i,char* j);
	void mRemScreen    (tLevel* lev, char i,char j, char* error);
	void mAddScreen    (tLevel* lev, char i,char j, char* error);
	void mGetSize      (tLevel* lev, char* i,char* j);
	char mGetScrCount  (tLevel* lev);
		/* Nota: si mGetScrCount(lev) es 24, entonces el nivel esta lleno */
	void mCpyScreen    (tLevel* lev, char fromi,char fromj,char toi,char toj, char* error);

	/* Start position handling */
	void mGetStartPosition(tLevel* lev,char* pantalla, char* p, char *b,char *sentido,char *sentido2);
	void mSetStartPosition(tLevel* lev,char pantalla, char p, char b,char sentido,char sentido2);
	void mGetDebugPosition(tLevel* lev,char* pantalla, char* p, char *b,char *sentido);
	void mSetDebugPosition(tLevel* lev,char pantalla, char p, char b,char sentido);
		/* Nota: DebugPosition puede no existir en lev, en ese caso la convension es no existe si pantalla igual a 0, de lo contrario existe y la pantalla inicial es pantalla */

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/*
	Se utilizan partes del archivo como validadores, texto aparte,
	etc. Aparentemente es texto desperdiciado, no se detectaron
	cambios en el comportamiento de los niveles.
*/

/***************************************************************\
|                Private memory stream handling                 |
\***************************************************************/

void mSetArray(tLevel* lev, char *pos,int from,unsigned int length,int validator) {
	char validate=0;
	while (length--) {
		validate+=((*lev).levelArray[from]-pos[length]);
		(*lev).levelArray[from++]=pos[length];
	}
	(*lev).levelArray[validator]+=validate;
}

void mGetArray(tLevel* lev, char *pos,int from,int length) {
	while (length--) pos[length]=(*lev).levelArray[from++];
}

/***************************************************************\
|                         Gate handling                         |
\***************************************************************/

/* Privados */
void getRawEvent(tLevel* lev,char id,tGateEvent* event) { /* private */
	unsigned char x1,x2,valor;
	(*event).pos[0]  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id];
	(*event).pos[1]  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id];
}

void setRawEvent(tLevel* lev,char id,tGateEvent* event) { /* private */
	unsigned char x1,x2,validate;
	x1=(*event).pos[0];
	x2=(*event).pos[1];
	validate  =((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]-x1)+((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]-x2);

	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]=x1;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]=x2;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=validate;
}

/* Privado, pero publico dentro de maps.c */
void mLoadEventList(tLevel* lev) {
	/* Esta funcion carga el TDA con los datos que lee del rigido */
	char a=0;
	char b=0;

	/* Cargar gateList con las asociaciones del mapa */
	int j=0;
	for (int i=0;i<MAPS_BLOCK_SIZEOF_WALL;i++) {
		switch ((*lev).levelArray[MAPS_BLOCK_OFFSET_WALL+i]) {
			case MAPS_OBJ_BAL_D:
			case MAPS_OBJ_BAL_U:
				(*lev).gateList.item[j].door=i;
				(*lev).gateList.item[j++].id=(*lev).levelArray[MAPS_BLOCK_OFFSET_BACK+i];
		}
	}
	(*lev).gateList.size=j;

	/* Cargar gateTDA con datos de la tabla */
	j=0;
	for (tGateEvent evento;j<MAPS_BLOCK_SIZEOF_GATE;j++) {
		getRawEvent(lev,j,&evento);
		if (evento.pos[0]>=(0x80)) {
			(*lev).gateTDA.row[a].size=b;
			b=0;
			a++;
		} else {
			b++;
		}
		(*lev).gateTDA.row[a].event[b]=evento;
	}
	(*lev).gateTDA.size=a;
}

/* mSevEventList private functions */

void anularEvento(tGateEvent *e) {
	(*e).pos[0]=0;
	(*e).pos[1]=0;
}

char obtenerFila(tGateRow *fila,unsigned char c,int k,tLevel* lev) {
	char aux=((*lev).gateTDA.row[k].size==c);
	if (aux) {
		*fila=(*lev).gateTDA.row[k];
	}
	return aux;
}

char sameEvent(tGateEvent event1,tGateEvent event2) {
	return ((event1.pos[1]==event2.pos[1]) && ((event1.pos[0]%0x80)==(event1.pos[0]%0x80)));
}

tGateEvent setFlag(tGateEvent evento,char flag) {
	evento.pos[0]=(evento.pos[0]%0x80)+(0x80*flag);
	return evento;
}

#define MAPS_GATE_DIFFERS 0
#define MAPS_GATE_EQUALS  1
#define MAPS_GATE_BELONGS 2

char belongsToDiskList(tLevel* lev, tGateRow *fila,int i) {
	tGateEvent evento;

	char total=0;
	char pertenece;
	/* recorrer la lista de eventos */
	for (int k=i;((*lev).gateEvents.event[k].pos[0]<(0x80));k++) {
		total++;
		pertenece=0;
		for (char j=0;(j<(*fila).size) && (!pertenece);j++) {
			pertenece=(sameEvent((*fila).event[j],(*lev).gateEvents.event[k]));
		}
		if (!pertenece) {
			return MAPS_GATE_DIFFERS;
		}
	}
	if (total==(*fila).size) {
		return MAPS_GATE_EQUALS;
	} else {
		return MAPS_GATE_BELONGS;
	}
}

void apuntar(tLevel* lev,unsigned char i) {
	(*lev).gateList.item[(*lev).gateList.size].id=i;
	(*lev).gateList.size++;
}

#define MAPS_GATE_SINGLE_EVENT    0
#define MAPS_GATE_MULTIPLE_EVENT  1
char intertLeft(tLevel* lev,tGateRow fila,int inicioFila,int numeroFila,char mode) {
	/* Esta funcion inserta en gateEvents el o los eventos de fila que no estan insertados */
	/* Por cada insersion deben ser modificados los ids correspondientes en la gateList */
	/* devuelve 1 si entro */
	/* devuelve 0 en caso de superarse lso 256 bytes */

	/*
	 1) En caso de MULTIPLE EVENT los elementos de la fila
	    que ya estan enliastados en gateEvents deben ser
	    eliminados de la fila. Se coloca el evento nulo en
	    reemplazo de estos.
	*/

	char j,pertenece;
	if (mode==MAPS_GATE_MULTIPLE_EVENT) {
		for (int k=inicioFila;((*lev).gateEvents.event[k].pos[0]<(0x80));k++) {
			pertenece=0;
			for (j=0;(j<fila.size) && (!pertenece);j++) {
				pertenece=(sameEvent(fila.event[j],(*lev).gateEvents.event[k]));
			}
			if (pertenece) {
				anularEvento(&(fila.event[j]));
			}
		}
	}

	/*
	 2) En caso de MAPS_GATE_SINGLE_EVENT el ultimo debe
	    marcar fin de la lista. A tal fin, cambiaremos ese
	    flag del evento. Como la inserion se hara de adelante
	    hacia atras, simplemente debemos setear el flag al
	    principio
	*/

	int c=(mode==MAPS_GATE_SINGLE_EVENT);

	/*
	 3) Se debera iterar para toda la fila e insertar
	    evento por evento en la gateEvents en la posicion inicioFila.
			Cada insersion implica un corrimiento de ids en la gateList
	*/

	for (j=0;(j<fila.size);j++) {
		if (fila.event[j].pos[0]) { /* Para cada evento nonulo de la fila */
			/* Adelantar todos los eventos posteriores a inicioFila */
			for (int k=(*lev).gateEvents.size;k>inicioFila;k--) {
				(*lev).gateEvents.event[k+1]=(*lev).gateEvents.event[k];
			}

			/* Insertar (*fila).event[j] en la gateEvents en la posicion generada en inicioFila. */
			/* Se tendra en cuenta el flag de de fin de lista */
			(*lev).gateEvents.event[inicioFila]=setFlag(fila.event[j],c);

			/* El flag del fin de lista se cancela */
			c=0;

			/* Incrementar la cantidad de eventos de la gateEvent, en caso de no poder abortar */
			if ((*lev).gateEvents.size==255) {
				return 0;
			} else {
				(*lev).gateEvents.size++;
			}

			/* Finalmente se debe recorrer la gate list e incrementar el contenido que supere a inicioFila */
			for (int k=0;k<(*lev).gateList.size;k++) {
				if ((*lev).gateList.item[k].id>=inicioFila)
					((*lev).gateList.item[k].id)++;
			}
		}
	}
	return 1;
}

/* Main function */
char mSaveEventList(tLevel* lev) {
	/* Lee el TDA, optimiza el espacio que ocupan los datos y lo guardaen disco */
	/* devuelve 1 en caso de que entre en disco */
	/* devuelve 0 en caso de que no hayan entrado los datos en disco */

	/* Inicializar variables */
	unsigned char n=(*lev).gateTDA.size;
	unsigned char c=1;
	char x;
	tGateRow fila;
	(*lev).gateEvents.size=0;
	(*lev).gateList.size=0;

	/* Generar a partir de gateTDA la lista de eventos gateEventList y de asociaciones gateList */
	while (n) { /* Para todos los elementos */
		for (int k=0;k<(*lev).gateTDA.size;k++) { /* Recorrer filas de gateTDA */
			if (obtenerFila(&fila,c,k,lev)) {  /* mietras haya elementos con tamagno=c sin procesar fila=(*lev).gateTDA.row[n] where fila=(*lev).gateTDA.row[n].size=c; */
				/* entra con fila seteada en la fila k con c elementos adentro. */
				n--;
				x=0;
				for (int i=0;((i<(*lev).gateEvents.size) && (!x));i++) { /* recorrer lo ya creado de gateEvents */
					x=belongsToDiskList(lev,&fila,i);
					switch (x) {
						case MAPS_GATE_BELONGS:
							/*
							  Pertenece, pero puede que este no sea el principio de la lista
							  en ese caso no puedo insertar porque corromperia la puerta
							  anterior, por eso voy a verificar que sea el primero de la lista
							  o bien que el anterior tenga el flag c.
							*/
							if ((i=0) || ((*lev).gateEvents.event[i-1].pos[0]>0x80)) {
								if (!intertLeft(lev,fila,i,k,MAPS_GATE_MULTIPLE_EVENT)) {
									return 0;
								}
								apuntar(lev,i);
							}
							break;
						case MAPS_GATE_EQUALS:
							apuntar(lev,i);
					}
				}
				if (!x) {
					if (!intertLeft(lev,fila,(*lev).gateEvents.size,k,MAPS_GATE_SINGLE_EVENT)) {
						return 0;
					}
				}
			}
		}
		c++;
	}

	/* Guardar gateEvents y gateList en el formato. */
	/* gateList */
	for (int i=0;i<(*lev).gateEvents.size;i++) {
		setRawEvent(lev,i,&((*lev).gateEvents.event[i]));
	}
	int location;
	unsigned char validate=0;
	for (int i=0;i<(*lev).gateList.size;i++) {
		location=MAPS_BLOCK_OFFSET_BACK+(*lev).gateList.item[i].door;
		validate+=(*lev).levelArray[location]-(*lev).gateList.item[i].id;
		(*lev).levelArray[location]=(*lev).gateList.item[i].id;
	}
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=validate;

	return 1;
}

void getGateAsociation(tLevel* lev,char id,char* scr,char* p,char* b, char *termino) {
	unsigned char x1,x2,valor;
	x1        =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id];
	x2        =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id];

	*scr      =(x2/8)+(x1%(0x80))/(0x20);
	valor     =(x1%(0x20));
	*b        =(valor%10);
	*p        =(valor/10);
	*termino  =(x1/(0x80)); /* c=(Se pasa a la siguiente)?0:1 */
}

void setGateAsociation(tLevel* lev,char id,char scr,char p,char b, char termino) {
	unsigned char x1,x2,validate;

	x1        =((scr%4)*0x20)+(p*10+b)+(termino?0x80:0);
	x2        =((scr/4)*0x20);
	validate  =((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]-x1)+((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]-x2);

	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]=x1;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]=x2;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=validate;
}

void shiftEventId(tLevel* lev,char from,char delta) { /* private */
	char validate=0;
	char aux;
	for (int i=0;i<MAPS_BLOCK_SIZEOF_WALL;i++) {
		switch ((*lev).levelArray[MAPS_BLOCK_OFFSET_WALL+i]) {
			case MAPS_OBJ_BAL_D:
			case MAPS_OBJ_BAL_U:
				if ((*lev).levelArray[MAPS_BLOCK_OFFSET_BACK+i]>from) {
					(*lev).levelArray[MAPS_BLOCK_OFFSET_BACK+i]+=delta;
					validate-=delta;
				}
		}
	}
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=validate;
}

void insertEvent(tLevel* lev,char id,char scr,char p,char b) {
	char x1,x2,x3,x4;

	shiftEventId(lev,id,+1);
	for (int i=20;i>id;i--) {
		getGateAsociation(lev,i-1,&x1,&x2,&x3,&x4);
		setGateAsociation(lev,i,x1,x2,x3,x4);
	}
	setGateAsociation(lev,id,scr,p,b,0);
}

/***************************************************************\
|                         Text handling                         |
\***************************************************************/

#define MAPS_BLOCK_OFFSET_START_POSITION_SIZE3 3
/* Actualmente guarda 1+MAPS_BLOCK_SIZEOF_UNKNOWN_1+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_4+unk_5+unk_6+unk_7 = 564 caracteres. *text debe medir 565 incluyendo el caracter nulo. */
void mSetText (tLevel* lev,char* text) {
	mSetArray(lev,text,MAPS_BLOCK_OFFSET_START_POSITION+MAPS_BLOCK_OFFSET_START_POSITION_SIZE3,1,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[1]),MAPS_BLOCK_OFFSET_UNKNOWN_1,MAPS_BLOCK_SIZEOF_UNKNOWN_1,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_2,MAPS_BLOCK_SIZEOF_UNKNOWN_2,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_4,MAPS_BLOCK_SIZEOF_UNKNOWN_4,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_5,MAPS_BLOCK_SIZEOF_UNKNOWN_5,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_5+MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_6,MAPS_BLOCK_SIZEOF_UNKNOWN_6,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_6+MAPS_BLOCK_SIZEOF_UNKNOWN_5+MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_7,MAPS_BLOCK_SIZEOF_UNKNOWN_7,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
}

void mGetText (tLevel* lev,char* text) {
	mGetArray(lev,text,MAPS_BLOCK_OFFSET_START_POSITION+MAPS_BLOCK_OFFSET_START_POSITION_SIZE3,1);
	mGetArray(lev,&(text[1]),MAPS_BLOCK_OFFSET_UNKNOWN_1,MAPS_BLOCK_SIZEOF_UNKNOWN_1);
	mGetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_2,MAPS_BLOCK_SIZEOF_UNKNOWN_2);
	mGetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_4,MAPS_BLOCK_SIZEOF_UNKNOWN_4);
	mGetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_5,MAPS_BLOCK_SIZEOF_UNKNOWN_5);
	mGetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_5+MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_6,MAPS_BLOCK_SIZEOF_UNKNOWN_6);
	mGetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_6+MAPS_BLOCK_SIZEOF_UNKNOWN_5+MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_7,MAPS_BLOCK_SIZEOF_UNKNOWN_7);
	text[MAPS_BLOCK_SIZEOF_UNKNOWN_7+MAPS_BLOCK_SIZEOF_UNKNOWN_6+MAPS_BLOCK_SIZEOF_UNKNOWN_5+MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]=0;
}

/***************************************************************\
|                     Start Position handling                   |
\***************************************************************/

void mGetDebugPosition(tLevel* lev,char* pantalla, char* p, char *b,char *sentido) {
	unsigned char dp[2];
	unsigned char valor;
	mGetArray(lev,dp,MAPS_BLOCK_OFFSET_START_POSITION+4,2);
	*pantalla =(dp[0]);
	*sentido  =(dp[1]%2);
	valor     =(dp[1]/2);
	*b        =(valor%10);
	*p        =(valor/10);
}

void mSetDebugPosition(tLevel* lev,char pantalla, char p, char b,char sentido) {
	unsigned char dp[2];
	dp[0]     =pantalla;
	dp[1]     =pantalla?(((p*10+b)*2)+(sentido?1:0)):0;
	mSetArray(lev,dp,MAPS_BLOCK_OFFSET_START_POSITION+4,2,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
}

void mSetStartPosition(tLevel* lev,char pantalla, char p, char b,char sentido,char sentido2) {
	unsigned char valor=pantalla;
	int location=MAPS_BLOCK_OFFSET_START_POSITION;
	unsigned char offsetval=0;

	/* pantalla */
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location++]=valor;

	/* Posicion */
	valor     =p*10+b;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location++]=valor;

	/* sentidos */
	valor     =sentido?MAPS_DIRECTION_RIGHT:MAPS_DIRECTION_LEFT;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;

	location +=4;
	valor     =sentido2?MAPS_DIRECTION_RIGHT:MAPS_DIRECTION_LEFT;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;

	/* Validacion */
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_LINK]+=offsetval;
}

/***************************************************************\
|                      S E T S  &  G E T S                      |
\***************************************************************/


void mSetWall  (tLevel* lev,char pantalla,char p,char b,char valor) {
	int location=MAPS_BLOCK_OFFSET_WALL+30*(pantalla-1)+10*p+b;

	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=(*lev).levelArray[location]-valor;
	(*lev).levelArray[location]=valor;
}

void mSetBack (tLevel* lev,char pantalla,char p,char b,char valor) {
	int location=MAPS_BLOCK_OFFSET_BACK+30*(pantalla-1)+10*p+b;

	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=(*lev).levelArray[location]-valor;
	(*lev).levelArray[location]=valor;
}


/***************************************************************\
|                   Abstract Guard Handling                     |
\***************************************************************/

void mSetGuard(tLevel* lev,char pantalla,char p,char b,char vidas,char color,char sentido,char erase) {

	/* Posicion */
	unsigned char valor=erase?30:p*10+b;
	int location=MAPS_BLOCK_OFFSET_GUARD_POSITION-1+pantalla;
	unsigned char offsetval=((*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_LINK]+(256-valor)+(*lev).levelArray[location]);
	(*lev).levelArray[location]=valor;
	/* sentido */
	valor     =sentido?MAPS_DIRECTION_RIGHT:MAPS_DIRECTION_LEFT;
	location  =MAPS_BLOCK_OFFSET_GUARD_DIRECTION-1+pantalla;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;
	/* skill */
	valor     =erase?1:vidas;
	location  =MAPS_BLOCK_OFFSET_GUARD_SKILL-1+pantalla;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;
	/* Color */
	valor     =erase?0:color;
	location  =MAPS_BLOCK_OFFSET_GUARD_COLOR-1+pantalla;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;

	/* Validar */
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_LINK]=offsetval/*+(256-valor)+(*lev).levelArray[location]*/;
}

void mGetGuard(tLevel* lev,char pantalla,char* p,char* b,char* vidas,char* color,char* sentido,char* exists) {
	/* Posicion */
	unsigned char valor=((*lev).levelArray[(MAPS_BLOCK_OFFSET_GUARD_POSITION-1+pantalla)]);
	*exists = (valor<30);
	*b      = (valor%10);
	*p      = (valor/10);
	/* sentido */
	*sentido=!(*lev).levelArray[MAPS_BLOCK_OFFSET_GUARD_DIRECTION-1+pantalla];
	/* skill */
	*vidas  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GUARD_SKILL-1+pantalla];
	/* Color */
	*color  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GUARD_COLOR-1+pantalla];
}

/***************************************************************\
|                     Screen Link Handling                      |
\***************************************************************/

/* Recursive mapping sub procedures */
void mRecLink(char i, char j, tLevel* lev); /* prototype */
void mRecIndirect(char i, char j, tLevel* lev, char scr, char pos) {
	char aux;
	char auxb;
	if ((*lev).handledLinks.linkMap[i][j]==255) {
		aux=mGetLink (lev,scr,pos);
		if (!(auxb=(!aux))) auxb=(!(*lev).handledLinks.linkList[aux]);
		if (auxb) {
			(*lev).handledLinks.linkMap[i][j]=aux;
			if (aux) {
				(*lev).handledLinks.linkList[aux]=1;
				mRecLink(i,j,lev);
			}
		} else {
			(*lev).handledLinks.linkMap[i][j]=0;
		}
	}
}

void mRecLink(char i, char j, tLevel* lev) {
	char scr=(*lev).handledLinks.linkMap[i][j];

	mRecIndirect(i,j-1,lev,scr,MAPS_sLeft);
	mRecIndirect(i,j+1,lev,scr,MAPS_sRight);
	mRecIndirect(i-1,j,lev,scr,MAPS_sUp);
	mRecIndirect(i+1,j,lev,scr,MAPS_sDown);
}

void mCalcularLimUp (tLevel* lev) {
	char i=0;
	char j=0;
	while (((*lev).handledLinks.linkMap[i][j]==255) && (j<MAPS_MAX_LENGTH)) {
		if (++i==MAPS_MAX_LENGTH) {
			j++;
			i=0;
		}
	}
	(*lev).handledLinks.limUp=j;
}

void mCalcularLimLeft (tLevel* lev) {
	char i=0;
	char j=0;
	while (((*lev).handledLinks.linkMap[j][i]==255) && (j<MAPS_MAX_LENGTH)) {
		if (++i==MAPS_MAX_LENGTH) {
			j++;
			i=0;
		}
	}
	(*lev).handledLinks.limLeft=j;
}

void mCalcularLimDown (tLevel* lev) {
	char i=0;
	char j=MAPS_MAX_LENGTH-1;
	while (((*lev).handledLinks.linkMap[i][j]==255) && j) {
		if (++i==MAPS_MAX_LENGTH) {
			j--;
			i=0;
		}
	}
	(*lev).handledLinks.limDown=j;
}

void mCalcularLimRight (tLevel* lev) {
	char i=0;
	char j=MAPS_MAX_LENGTH-1;
	while (((*lev).handledLinks.linkMap[j][i]==255) && j) {
		if (++i==MAPS_MAX_LENGTH) {
			j--;
			i=0;
		}
	}
	(*lev).handledLinks.limRight=j;
}


/* 	Main mCreateLinkMap */
void mCreateLinkMap(tLevel* lev) { /* private */
	char	i,j,start;

	/* Initialize arrays */
	for (j=0;j<MAPS_MAX_LENGTH;j++)
		for (i=0;i<MAPS_MAX_LENGTH;(*lev).handledLinks.linkMap[i++][j]=255);
	for (j=1;j<=MAPS_MAX_SCREENS;(*lev).handledLinks.linkList[j++]=0);

	/* Inicializar start en la pantalla de inicio */
	mGetStartPosition(lev,&start,&i,&i,&i,&i);
	(*lev).handledLinks.linkMap[MAPS_LINKMAP_CENTER][MAPS_LINKMAP_CENTER]=start;
	(*lev).handledLinks.linkList[start]=1;

	/* Start Recursive Mapper */
	mRecLink(MAPS_LINKMAP_CENTER,MAPS_LINKMAP_CENTER,lev);

	/* Buscar limites del nivel */
	mCalcularLimUp   (lev);
	mCalcularLimDown (lev);
	mCalcularLimLeft (lev);
	mCalcularLimRight(lev);

}

void mGetSize      (tLevel* lev, char* i,char* j) {
	*i=(*lev).handledLinks.limRight - (*lev).handledLinks.limLeft;
	*j=(*lev).handledLinks.limDown  - (*lev).handledLinks.limUp;
}

char mGetScrCount  (tLevel* lev) {
	char l,aux;

	aux=0;
	for (l=1;l<=MAPS_MAX_SCREENS;aux+=((*lev).handledLinks.linkList[l++]?1:0));
	return aux;
}

char mGetScreen    (tLevel* lev, char i,char j, char* error) {
	char aux;

	aux=(*lev).handledLinks.linkMap[(*lev).handledLinks.limLeft+i][(*lev).handledLinks.limUp+j];
	*error=(aux==255);
	return aux;
}

void mGetMainScreen(tLevel* lev, char* i,char* j) {
	*i=(MAPS_LINKMAP_CENTER) - (*lev).handledLinks.limLeft;
	*j=(MAPS_LINKMAP_CENTER) - (*lev).handledLinks.limUp;
}

void mAddScreen    (tLevel* lev, char i,char j, char* error) {
	char scr=mGetScreen(lev,i,j,error);
	*error=(*error || scr);
	if (!*error) {
		int l=1;
		while ((*lev).handledLinks.linkList[l] && (l++<MAPS_MAX_SCREENS));
		if ((*lev).handledLinks.linkList[l]) {
			*error=1;
		} else {
			/* Pedir pantalla */
			(*lev).handledLinks.linkList[l]=1;

			i+=(*lev).handledLinks.limLeft;
			j+=(*lev).handledLinks.limUp;

			/* En caso de agregar una pantalla en alguno de los bordes, estos se amplian */
			if ((*lev).handledLinks.limLeft=i) (*lev).handledLinks.limLeft--;
			if ((*lev).handledLinks.limUp=j) (*lev).handledLinks.limUp--;
			if ((*lev).handledLinks.limRight=i) (*lev).handledLinks.limRight++;
			if ((*lev).handledLinks.limDown=j) (*lev).handledLinks.limDown++;


			/* Poner 0 alrededor de l */
			(*lev).handledLinks.linkMap[i][j-1]%=255;
			(*lev).handledLinks.linkMap[i][j+1]%=255;
			(*lev).handledLinks.linkMap[i+1][j]%=255;
			(*lev).handledLinks.linkMap[i-1][j]%=255;

			/* Poner l en l */
			(*lev).handledLinks.linkMap[i][j]=l;

			/* Setear links desde l a las 4 pantallas adyacentes */
			mSetLink (lev,l,MAPS_sUp,(*lev).handledLinks.linkMap[i][j-1]);
			mSetLink (lev,l,MAPS_sDown,(*lev).handledLinks.linkMap[i][j+1]);
			mSetLink (lev,l,MAPS_sRight,(*lev).handledLinks.linkMap[i+1][j]);
			mSetLink (lev,l,MAPS_sLeft,(*lev).handledLinks.linkMap[i-1][j]);

			/* Setear links desde las 4 pantallas adyacentes a l */
			mSetLink (lev,(*lev).handledLinks.linkMap[i][j+1],MAPS_sUp,l);
			mSetLink (lev,(*lev).handledLinks.linkMap[i][j-1],MAPS_sDown,l);
			mSetLink (lev,(*lev).handledLinks.linkMap[i-1][j],MAPS_sRight,l);
			mSetLink (lev,(*lev).handledLinks.linkMap[i+1][j],MAPS_sLeft,l);

			/* Limpiar contenido de la pantalla l */
			for (i=0;i<3;i++) {
				for (j=0;j<10;j++) {
					mSetWall(lev,l,i,j,0);
					mSetBack(lev,l,i,j,0);
				}
			}
			mSetGuard(lev,l,1,1,1,1,1,1);
			/* Listo, ya cree la pantalla y la linkie */
		}
	}
}

void mRemScreen    (tLevel* lev, char i,char j, char* error) {
	/*{Q&D:
		1) Si parten a la mitad un nivel se pierde una de las mitades
		2) no se refresca la LinkMap en ese caso
		3) tampoco une las pantallas que estaban a los costados}*/
	unsigned char l=mGetScreen(lev,i,j,error);
	*error=*error || (!l);
	if (!*error) {
		/* Liberar pantalla */
		(*lev).handledLinks.linkList[l]=0;

		/* Seteo posiciones absolutas */
		i+=(*lev).handledLinks.limLeft;
		j+=(*lev).handledLinks.limUp;

		/* Poner 0 en l */
		(*lev).handledLinks.linkMap[i][j]=0;

		/* En caso de remover una pantalla en alguno de los bordes, estos se recalculan */
		if ((*lev).handledLinks.limLeft=i) 	mCalcularLimLeft(lev);
		if ((*lev).handledLinks.limUp=j)    mCalcularLimUp(lev);
		if ((*lev).handledLinks.limRight=i) mCalcularLimRight(lev);
		if ((*lev).handledLinks.limDown=j)  mCalcularLimDown(lev);

		/* Borrar links desds las 4 pantallas adyacentes a l */
		mSetLink (lev,(*lev).handledLinks.linkMap[i][j+1],MAPS_sLeft,0);
		mSetLink (lev,(*lev).handledLinks.linkMap[i][j-1],MAPS_sRight,0);
		mSetLink (lev,(*lev).handledLinks.linkMap[i-1][j],MAPS_sDown,0);
		mSetLink (lev,(*lev).handledLinks.linkMap[i+1][j],MAPS_sUp,0);
	}
}

void mCpyScreen    (tLevel* lev, char fromi,char fromj,char toi,char toj, char* error) {
	char fromscr,toscr,i,j,k,l;
	char sentido,existe;

	/* Verificar que la pantalla source y la pantalla target existan */
	fromscr=mGetScreen(lev,fromi,fromj,error); /* Verifico que existe la pantalla from */
	*error=(*error || fromscr);
	if (!*error) { /* Verifico que existe la pantalla to */
		toscr=mGetScreen(lev,toi,toj,error);
		*error=(*error || toscr);
	}
	if (!*error) { /* Si existen ambas pantallas */
		/* Copiar contenido de la pantalla from a la pantalla to: */

		/* Copiar wall y back */
		char contenido[30];
		mGetArray(lev,contenido,MAPS_BLOCK_OFFSET_WALL-30+fromscr*30,30);
		mSetArray(lev,contenido,MAPS_BLOCK_OFFSET_WALL-30+toscr*30,30,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);
		mGetArray(lev,contenido,MAPS_BLOCK_OFFSET_BACK-30+fromscr*30,30);
		mSetArray(lev,contenido,MAPS_BLOCK_OFFSET_BACK-30+toscr*30,30,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);

		/* Copiar Guards */
		mGetGuard(lev,fromscr,&i,&j,&k,&l,&sentido,&existe);
		mSetGuard(lev,toscr,i,j,k,l,sentido,existe);
		/* Listo, ya se copio el contenido y el guardia */
	}
}

/***************************************************************\
|                         File handling                         |
\***************************************************************/

char mLoadPED(char* vFile,tLevel* lev) {
	FILE *fp;
	char aux;

	if ((fp=fopen(vFile,"rb"))==NULL) {
		return 0;
	} else {
		aux=fread (lev,sizeof(*lev),1,fp);
		fclose(fp);
		return aux;
	}
}

char mLoadDAT(char* vFile,tLevel* lev,char levelNumber) {
	FILE *fp;
	char aux;

	if ((fp=fopen(vFile,"rb"))==NULL) {
		return 0;
	} else {
		(*lev).level=levelNumber;
		fpos_t position=levelNumber*MAPS_BLOCK_SIZEOF_LEVEL+MAPS_BLOCK_OFFSET_LEVELS;
		fsetpos (fp, &position);
		aux=fread (&(*lev).levelArray,MAPS_BLOCK_SIZEOF_LEVEL,1,fp);
		mCreateLinkMap(lev);
		fclose(fp);
		return aux;
	}
}

char mSavePED(char* vFile,tLevel* lev) {
  FILE * pFile;
  if ((pFile = fopen (vFile , "wb"))==NULL) {
		return 0;
	}
  fwrite (lev, 1, sizeof(*lev), pFile);
  fclose (pFile);
  return 1;
}

char mSaveDAT(char* vFile,tLevel* lev) {
  FILE * pFile;
  if ((pFile = fopen (vFile , "r+b"))==NULL) {
		return 0;
	}
	fpos_t position=(*lev).level*MAPS_BLOCK_SIZEOF_LEVEL+MAPS_BLOCK_OFFSET_LEVELS;
  fsetpos (pFile, &position);
  fwrite ((*lev).levelArray, 1, MAPS_BLOCK_SIZEOF_LEVEL, pFile);
  fclose (pFile);
  return 1;
}

#endif
