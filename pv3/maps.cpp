// maps.cpp : implementation of the cMaps class
//

#include "stdafx.h"
#include "Princed V3.h"
#include "cSelectLevelWindow.h"

#include <sys/types.h>
#include <sys/stat.h>
#include "maps.h"

cMaps::cMaps(char* fileName) {
	struct _stat buf;
	if (_stat( "stat.c", &buf )) {
		return;
	}
	if (buf.st_size==MAPS_BLOCK_SIZEOF_FILE) {
		char levelNumber=1;
		//!ToDo!: que levelnumber se lea de un msgbox
		cSelectLevelWindow levelWindow;
		levelWindow.DoModal();
		mLoadDAT(fileName,lev,levelNumber);
	} else {
		mLoadPED(fileName,lev);
	}
}

bool cMaps::exists() {
	return (lev!=NULL);
}


cMaps::~cMaps() {
	free(lev);
}

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

void cMaps::mSetArray(tLevel* lev, const char *pos,int from,unsigned int length,int validator) {
	char validate=0;
	while (length--) {
		validate+=((*lev).levelArray[from]-pos[length]);
		(*lev).levelArray[from++]=pos[length];
	}
	(*lev).levelArray[validator]+=validate;
}

void cMaps::mGetArray(tLevel* lev, char* pos,int from,int length) {
	while (length--) pos[length]=(*lev).levelArray[from++];
}

/***************************************************************\
|                         Gate handling                         |
\***************************************************************/

//Privados
void cMaps::getRawEvent(tLevel* lev,char id,tGateEvent* event) { //private
	(*event).pos[0]  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id];
	(*event).pos[1]  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id];
}

void cMaps::setRawEvent(tLevel* lev,char id,tGateEvent* event) { //private
	unsigned char x1,x2,validate;
	x1=(*event).pos[0];
	x2=(*event).pos[1];
	validate  =((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]-x1)+((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]-x2);

	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]=x1;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]=x2;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=validate;
}

//Privado, pero publico dentro de maps.c
void cMaps::mLoadEventList(tLevel* lev) {
	//Esta funcion carga el TDA con los datos que lee del rigido
	char a=0;
	char b=0;

	//Cargar gateList con las asociaciones del mapa
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

	//Cargar gateTDA con datos de la tabla
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

//mSevEventList private functions

void cMaps::anularEvento(tGateEvent *e) {
	(*e).pos[0]=0;
	(*e).pos[1]=0;
}

char cMaps::obtenerFila(tGateRow *fila,unsigned char c,int k,tLevel* lev) {
	char aux=((*lev).gateTDA.row[k].size==c);
	if (aux) {
		*fila=(*lev).gateTDA.row[k];
	}
	return aux;
}

char cMaps::sameEvent(tGateEvent event1,tGateEvent event2) {
	return ((event1.pos[1]==event2.pos[1]) && ((event1.pos[0]%0x80)==(event1.pos[0]%0x80)));
}

tGateEvent cMaps::setFlag(tGateEvent evento,char flag) {
	evento.pos[0]=(evento.pos[0]%0x80)+(0x80*flag);
	return evento;
}

#define MAPS_GATE_DIFFERS 0
#define MAPS_GATE_EQUALS  1
#define MAPS_GATE_BELONGS 2

char cMaps::belongsToDiskList(tLevel* lev, tGateRow *fila,int i) {

	char total=0;
	char pertenece;
	//recorrer la lista de eventos
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

void cMaps::apuntar(tLevel* lev,unsigned char i) {
	(*lev).gateList.item[(*lev).gateList.size].id=i;
	(*lev).gateList.size++;
}

#define MAPS_GATE_SINGLE_EVENT    0
#define MAPS_GATE_MULTIPLE_EVENT  1
char cMaps::intertLeft(tLevel* lev,tGateRow fila,int inicioFila,int numeroFila,char mode) {
	//Esta funcion inserta en gateEvents el o los eventos de fila que no estan insertados
	//Por cada insersion deben ser modificados los ids correspondientes en la gateList
	//devuelve 1 si entro
	//devuelve 0 en caso de superarse lso 256 bytes

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
		if (fila.event[j].pos[0]) { //Para cada evento nonulo de la fila
			//Adelantar todos los eventos posteriores a inicioFila
			for (int k=(*lev).gateEvents.size;k>inicioFila;k--) {
				(*lev).gateEvents.event[k+1]=(*lev).gateEvents.event[k];
			}

			//Insertar (*fila).event[j] en la gateEvents en la posicion generada en inicioFila.
			//Se tendra en cuenta el flag de de fin de lista
			(*lev).gateEvents.event[inicioFila]=setFlag(fila.event[j],c);

			//El flag del fin de lista se cancela
			c=0;

			//Incrementar la cantidad de eventos de la gateEvent, en caso de no poder abortar
			if ((*lev).gateEvents.size==255) {
				return 0;
			} else {
				(*lev).gateEvents.size++;
			}

			//Finalmente se debe recorrer la gate list e incrementar el contenido que supere a inicioFila
			for (int k2=0;k2<(*lev).gateList.size;k2++) {
				if ((*lev).gateList.item[k2].id>=inicioFila)
					((*lev).gateList.item[k2].id)++;
			}
		}
	}
	return 1;
}

//Main function
char cMaps::mSaveEventList(tLevel* lev) {
	//Lee el TDA, optimiza el espacio que ocupan los datos y lo guardaen disco
	//devuelve 1 en caso de que entre en disco
	//devuelve 0 en caso de que no hayan entrado los datos en disco

	//Inicializar variables
	unsigned char n=(*lev).gateTDA.size;
	unsigned char c=1;
	char x;
	tGateRow fila;
	(*lev).gateEvents.size=0;
	(*lev).gateList.size=0;

	//Generar a partir de gateTDA la lista de eventos gateEventList y de asociaciones gateList
	while (n) { //Para todos los elementos
		for (int k=0;k<(*lev).gateTDA.size;k++) { //Recorrer filas de gateTDA
			if (obtenerFila(&fila,c,k,lev)) {  //mietras haya elementos con tamagno=c sin procesar fila=(*lev).gateTDA.row[n] where fila=(*lev).gateTDA.row[n].size=c;
				//entra con fila seteada en la fila k con c elementos adentro.
				n--;
				x=0;
				for (int i=0;((i<(*lev).gateEvents.size) && (!x));i++) { //recorrer lo ya creado de gateEvents
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

	//Guardar gateEvents y gateList en el formato.
	//gateList
	for (int i=0;i<(*lev).gateEvents.size;i++) {
		setRawEvent(lev,i,&((*lev).gateEvents.event[i]));
	}
	int location;
	unsigned char validate=0;
	for (int i2=0;i2<(*lev).gateList.size;i2++) {
		location=MAPS_BLOCK_OFFSET_BACK+(*lev).gateList.item[i2].door;
		validate+=(*lev).levelArray[location]-(*lev).gateList.item[i2].id;
		(*lev).levelArray[location]=(*lev).gateList.item[i2].id;
	}
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=validate;

	return 1;
}

void cMaps::getGateAsociation(tLevel* lev,char id,char* scr,char* p,char* b, char *termino) {
	unsigned char x1,x2,valor;
	x1        =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id];
	x2        =(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id];

	*scr      =(x2/8)+(x1%(0x80))/(0x20);
	valor     =(x1%(0x20));
	*b        =(valor%10);
	*p        =(valor/10);
	*termino  =(x1/(0x80)); //c=(Se pasa a la siguiente)?0:1
}

void cMaps::setGateAsociation(tLevel* lev,char id,char scr,char p,char b, char termino) {
	unsigned char x1,x2,validate;

	x1        =((scr%4)*0x20)+(p*10+b)+(termino?0x80:0);
	x2        =((scr/4)*0x20);
	validate  =((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]-x1)+((*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]-x2);

	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_1+id]=x1;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_GATE_2+id]=x2;
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=validate;
}

void cMaps::shiftEventId(tLevel* lev,char from,char delta) { //private
	char validate=0;
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

void cMaps::insertEvent(tLevel* lev,char id,char scr,char p,char b) {
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
//Actualmente guarda 1+MAPS_BLOCK_SIZEOF_UNKNOWN_1+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_4+unk_5+unk_6+unk_7 = 564 caracteres. *text debe medir 565 incluyendo el caracter nulo.
void cMaps::mSetText (tLevel* lev,const char* text) {
	mSetArray(lev,text,MAPS_BLOCK_OFFSET_START_POSITION+MAPS_BLOCK_OFFSET_START_POSITION_SIZE3,1,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[1]),MAPS_BLOCK_OFFSET_UNKNOWN_1,MAPS_BLOCK_SIZEOF_UNKNOWN_1,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_2,MAPS_BLOCK_SIZEOF_UNKNOWN_2,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_4,MAPS_BLOCK_SIZEOF_UNKNOWN_4,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_5,MAPS_BLOCK_SIZEOF_UNKNOWN_5,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_5+MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_6,MAPS_BLOCK_SIZEOF_UNKNOWN_6,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
	mSetArray(lev,&(text[MAPS_BLOCK_SIZEOF_UNKNOWN_6+MAPS_BLOCK_SIZEOF_UNKNOWN_5+MAPS_BLOCK_SIZEOF_UNKNOWN_4+MAPS_BLOCK_SIZEOF_UNKNOWN_2+MAPS_BLOCK_SIZEOF_UNKNOWN_1+1]),MAPS_BLOCK_OFFSET_UNKNOWN_7,MAPS_BLOCK_SIZEOF_UNKNOWN_7,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
}

void cMaps::mGetText (tLevel* lev,char* text) {
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

void cMaps::mGetDebugPosition(tLevel* lev,char* pantalla, char* p, char *b,char *sentido) {
	char dp[2];
	unsigned char valor;
	mGetArray(lev,dp,MAPS_BLOCK_OFFSET_START_POSITION+4,2);
	*pantalla =(dp[0]);
	*sentido  =(dp[1]%2);
	valor     =(dp[1]/2);
	*b        =(valor%10);
	*p        =(valor/10);
}

void cMaps::mSetDebugPosition(tLevel* lev,char pantalla, char p, char b,char sentido) {
	char dp[2];
	dp[0]     =pantalla;
	dp[1]     =pantalla?(((p*10+b)*2)+(sentido?1:0)):0;
	mSetArray(lev,dp,MAPS_BLOCK_OFFSET_START_POSITION+4,2,MAPS_BLOCK_OFFSET_VALIDATOR_LINK);
}

void cMaps::mGetStartPosition(tLevel* lev,char* pantalla, char* p, char *b,char *sentido,char *sentido2) {
	*pantalla =(*lev).levelArray[MAPS_BLOCK_OFFSET_START_POSITION];
	char valor=(*lev).levelArray[MAPS_BLOCK_OFFSET_START_POSITION+1];
	*b        =(valor%10);
	*p        =(valor/10);
	*sentido  =((*lev).levelArray[MAPS_BLOCK_OFFSET_START_POSITION+2])?1:0;
	*sentido2 =((*lev).levelArray[MAPS_BLOCK_OFFSET_START_POSITION+6])?1:0;
}

void cMaps::mSetStartPosition(tLevel* lev,char pantalla, char p, char b,char sentido,char sentido2) {
	unsigned char valor=pantalla;
	int location=MAPS_BLOCK_OFFSET_START_POSITION;
	unsigned char offsetval=0;

	//pantalla
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location++]=valor;

	//Posicion
	valor     =p*10+b;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location++]=valor;

	//sentidos
	valor     =sentido?MAPS_DIRECTION_RIGHT:MAPS_DIRECTION_LEFT;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;

	location +=4;
	valor     =sentido2?MAPS_DIRECTION_RIGHT:MAPS_DIRECTION_LEFT;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;

	//Validacion
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_LINK]+=offsetval;
}

/***************************************************************\
|                      S E T S  &  G E T S                      |
\***************************************************************/


void cMaps::mSetWall  (tLevel* lev,char pantalla,char p,char b,char valor) {
	int location=MAPS_BLOCK_OFFSET_WALL+30*(pantalla-1)+10*p+b;

	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=(*lev).levelArray[location]-valor;
	(*lev).levelArray[location]=valor;
}

char cMaps::mGetWall (tLevel* lev,char pantalla,char p,char b) const {
	return (*lev).levelArray[MAPS_BLOCK_OFFSET_WALL+30*(pantalla-1)+10*p+b];
}

void cMaps::mSetBack (tLevel* lev,char pantalla,char p,char b,char valor) {
	int location=MAPS_BLOCK_OFFSET_BACK+30*(pantalla-1)+10*p+b;

	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_WALL]+=(*lev).levelArray[location]-valor;
	(*lev).levelArray[location]=valor;
}

char cMaps::mGetBack (tLevel* lev,char pantalla,char p,char b) const {
	return (*lev).levelArray[MAPS_BLOCK_OFFSET_BACK+30*(pantalla-1)+10*p+b];
}

void cMaps::mSetLink (tLevel* lev,char pantalla,char tipo,char valor) {
	if ((pantalla<25) && (pantalla) && (valor<25)) {
		int location=(MAPS_BLOCK_OFFSET_LINK+((pantalla-1)*4)+tipo);
		(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_LINK]+=(*lev).levelArray[location]-valor;
		(*lev).levelArray[location]=valor;
	}
}

char cMaps::mGetLink (tLevel* lev,char pantalla,char tipo) {
	return (*lev).levelArray[MAPS_BLOCK_OFFSET_LINK+((pantalla-1)*4)+tipo];
}

/***************************************************************\
|                   Abstract Guard Handling                     |
\***************************************************************/

void cMaps::mSetGuard(tLevel* lev,char pantalla,char p,char b,char vidas,char color,char sentido,char erase) {

	//Posicion
	unsigned char valor=erase?30:p*10+b;
	int location=MAPS_BLOCK_OFFSET_GUARD_POSITION-1+pantalla;
	unsigned char offsetval=((*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_LINK]+(256-valor)+(*lev).levelArray[location]);
	(*lev).levelArray[location]=valor;
	//sentido
	valor     =sentido?MAPS_DIRECTION_RIGHT:MAPS_DIRECTION_LEFT;
	location  =MAPS_BLOCK_OFFSET_GUARD_DIRECTION-1+pantalla;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;
	//skill
	valor     =erase?1:vidas;
	location  =MAPS_BLOCK_OFFSET_GUARD_SKILL-1+pantalla;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;
	//Color
	valor     =erase?0:color;
	location  =MAPS_BLOCK_OFFSET_GUARD_COLOR-1+pantalla;
	offsetval+=(256-valor)+(*lev).levelArray[location];
	(*lev).levelArray[location]=valor;

	//Validar
	(*lev).levelArray[MAPS_BLOCK_OFFSET_VALIDATOR_LINK]=offsetval/*+(256-valor)+(*lev).levelArray[location]*/;
}

void cMaps::mGetGuard(tLevel* lev,char pantalla,char* p,char* b,char* vidas,char* color,char* sentido,char* exists)  const {
	//Posicion
	unsigned char valor=((*lev).levelArray[(MAPS_BLOCK_OFFSET_GUARD_POSITION-1+pantalla)]);
	*exists = (valor<30);
	*b      = (valor%10);
	*p      = (valor/10);
	//sentido
	*sentido=!(*lev).levelArray[MAPS_BLOCK_OFFSET_GUARD_DIRECTION-1+pantalla];
	//skill
	*vidas  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GUARD_SKILL-1+pantalla];
	//Color
	*color  =(*lev).levelArray[MAPS_BLOCK_OFFSET_GUARD_COLOR-1+pantalla];
}

/***************************************************************\
|                     Screen Link Handling                      |
\***************************************************************/

//Recursive mapping sub procedures
void cMaps::mRecIndirect(char i, char j, tLevel* lev, char scr, char pos) {
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

void cMaps::mRecLink(char i, char j, tLevel* lev) {
	char scr=(*lev).handledLinks.linkMap[i][j];

	mRecIndirect(i,j-1,lev,scr,MAPS_sLeft);
	mRecIndirect(i,j+1,lev,scr,MAPS_sRight);
	mRecIndirect(i-1,j,lev,scr,MAPS_sUp);
	mRecIndirect(i+1,j,lev,scr,MAPS_sDown);
}

void cMaps::mCalcularLimUp (tLevel* lev) {
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

void cMaps::mCalcularLimLeft (tLevel* lev) {
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

void cMaps::mCalcularLimDown (tLevel* lev) {
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

void cMaps::mCalcularLimRight (tLevel* lev) {
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


//	Main mCreateLinkMap
void cMaps::mCreateLinkMap(tLevel* lev) { //private
	char	i,j,start;

	//Initialize arrays
	for (j=0;j<MAPS_MAX_LENGTH;j++)
		for (i=0;i<MAPS_MAX_LENGTH;(*lev).handledLinks.linkMap[i++][j]=255);
	for (j=1;j<=MAPS_MAX_SCREENS;(*lev).handledLinks.linkList[j++]=0);

	//Inicializar start en la pantalla de inicio
	mGetStartPosition(lev,&start,&i,&i,&i,&i);
	(*lev).handledLinks.linkMap[MAPS_LINKMAP_CENTER][MAPS_LINKMAP_CENTER]=start;
	(*lev).handledLinks.linkList[start]=1;

	//Start Recursive Mapper
	mRecLink(MAPS_LINKMAP_CENTER,MAPS_LINKMAP_CENTER,lev);

	//Buscar limites del nivel
	mCalcularLimUp   (lev);
	mCalcularLimDown (lev);
	mCalcularLimLeft (lev);
	mCalcularLimRight(lev);

}

void cMaps::mGetSize      (tLevel* lev, char* i,char* j)  const {
	*i=(*lev).handledLinks.limRight - (*lev).handledLinks.limLeft;
	*j=(*lev).handledLinks.limDown  - (*lev).handledLinks.limUp;
}

char cMaps::mGetScrCount  (tLevel* lev)  {
	char l,aux;

	aux=0;
	for (l=1;l<=MAPS_MAX_SCREENS;aux+=((*lev).handledLinks.linkList[l++]?1:0));
	return aux;
}

char cMaps::mGetScreen    (tLevel* lev, char i,char j, char* error)  const  {
	char aux;

	aux=(*lev).handledLinks.linkMap[(*lev).handledLinks.limLeft+i][(*lev).handledLinks.limUp+j];
	*error=(aux==255);
	return aux;
}

void cMaps::mGetMainScreen(tLevel* lev, char* i,char* j) const {
	*i=(MAPS_LINKMAP_CENTER) - (*lev).handledLinks.limLeft;
	*j=(MAPS_LINKMAP_CENTER) - (*lev).handledLinks.limUp;
}

void cMaps::mAddScreen    (tLevel* lev, char i,char j, char* error) {
	char scr=mGetScreen(lev,i,j,error);
	*error=(*error || scr);
	if (!*error) {
		int l=1;
		while ((*lev).handledLinks.linkList[l] && (l++<MAPS_MAX_SCREENS));
		if ((*lev).handledLinks.linkList[l]) {
			*error=1;
		} else {
			//Pedir pantalla
			(*lev).handledLinks.linkList[l]=1;

			i+=(*lev).handledLinks.limLeft;
			j+=(*lev).handledLinks.limUp;

			//En caso de agregar una pantalla en alguno de los bordes, estos se amplian
			if ((*lev).handledLinks.limLeft=i) (*lev).handledLinks.limLeft--;
			if ((*lev).handledLinks.limUp=j) (*lev).handledLinks.limUp--;
			if ((*lev).handledLinks.limRight=i) (*lev).handledLinks.limRight++;
			if ((*lev).handledLinks.limDown=j) (*lev).handledLinks.limDown++;


			//Poner 0 alrededor de l
			(*lev).handledLinks.linkMap[i][j-1]%=255;
			(*lev).handledLinks.linkMap[i][j+1]%=255;
			(*lev).handledLinks.linkMap[i+1][j]%=255;
			(*lev).handledLinks.linkMap[i-1][j]%=255;

			//Poner l en l
			(*lev).handledLinks.linkMap[i][j]=l;

			//Setear links desde l a las 4 pantallas adyacentes
			mSetLink (lev,l,MAPS_sUp,(*lev).handledLinks.linkMap[i][j-1]);
			mSetLink (lev,l,MAPS_sDown,(*lev).handledLinks.linkMap[i][j+1]);
			mSetLink (lev,l,MAPS_sRight,(*lev).handledLinks.linkMap[i+1][j]);
			mSetLink (lev,l,MAPS_sLeft,(*lev).handledLinks.linkMap[i-1][j]);

			//Setear links desde las 4 pantallas adyacentes a l
			mSetLink (lev,(*lev).handledLinks.linkMap[i][j+1],MAPS_sUp,l);
			mSetLink (lev,(*lev).handledLinks.linkMap[i][j-1],MAPS_sDown,l);
			mSetLink (lev,(*lev).handledLinks.linkMap[i-1][j],MAPS_sRight,l);
			mSetLink (lev,(*lev).handledLinks.linkMap[i+1][j],MAPS_sLeft,l);

			//Limpiar contenido de la pantalla l
			for (i=0;i<3;i++) {
				for (j=0;j<10;j++) {
					mSetWall(lev,l,i,j,0);
					mSetBack(lev,l,i,j,0);
				}
			}
			mSetGuard(lev,l,1,1,1,1,1,1);
			//Listo, ya cree la pantalla y la linkie
		}
	}
}

void cMaps::mRemScreen    (tLevel* lev, char i,char j, char* error) {
	/*{Q&D:
		1) Si parten a la mitad un nivel se pierde una de las mitades
		2) no se refresca la LinkMap en ese caso
		3) tampoco une las pantallas que estaban a los costados}*/
	unsigned char l=mGetScreen(lev,i,j,error);
	*error=*error || (!l);
	if (!*error) {
		//Liberar pantalla
		(*lev).handledLinks.linkList[l]=0;

		//Seteo posiciones absolutas
		i+=(*lev).handledLinks.limLeft;
		j+=(*lev).handledLinks.limUp;

		//Poner 0 en l
		(*lev).handledLinks.linkMap[i][j]=0;

		//En caso de remover una pantalla en alguno de los bordes, estos se recalculan
		if ((*lev).handledLinks.limLeft=i) 	mCalcularLimLeft(lev);
		if ((*lev).handledLinks.limUp=j)    mCalcularLimUp(lev);
		if ((*lev).handledLinks.limRight=i) mCalcularLimRight(lev);
		if ((*lev).handledLinks.limDown=j)  mCalcularLimDown(lev);

		//Borrar links desds las 4 pantallas adyacentes a l
		mSetLink (lev,(*lev).handledLinks.linkMap[i][j+1],MAPS_sLeft,0);
		mSetLink (lev,(*lev).handledLinks.linkMap[i][j-1],MAPS_sRight,0);
		mSetLink (lev,(*lev).handledLinks.linkMap[i-1][j],MAPS_sDown,0);
		mSetLink (lev,(*lev).handledLinks.linkMap[i+1][j],MAPS_sUp,0);
	}
}

void cMaps::mCpyScreen    (tLevel* lev, char fromi,char fromj,char toi,char toj, char* error) {
	char fromscr,toscr,i,j,k,l;
	char sentido,existe;

	//Verificar que la pantalla source y la pantalla target existan
	fromscr=mGetScreen(lev,fromi,fromj,error); //Verifico que existe la pantalla from
	*error=(*error || fromscr);
	if (!*error) { //Verifico que existe la pantalla to
		toscr=mGetScreen(lev,toi,toj,error);
		*error=(*error || toscr);
	}
	if (!*error) { //Si existen ambas pantallas
		//Copiar contenido de la pantalla from a la pantalla to:

		//Copiar wall y back
		char contenido[30];
		mGetArray(lev,contenido,MAPS_BLOCK_OFFSET_WALL-30+fromscr*30,30);
		mSetArray(lev,contenido,MAPS_BLOCK_OFFSET_WALL-30+toscr*30,30,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);
		mGetArray(lev,contenido,MAPS_BLOCK_OFFSET_BACK-30+fromscr*30,30);
		mSetArray(lev,contenido,MAPS_BLOCK_OFFSET_BACK-30+toscr*30,30,MAPS_BLOCK_OFFSET_VALIDATOR_WALL);

		//Copiar Guards
		mGetGuard(lev,fromscr,&i,&j,&k,&l,&sentido,&existe);
		mSetGuard(lev,toscr,i,j,k,l,sentido,existe);
		//Listo, ya se copio el contenido y el guardia
	}
}

/***************************************************************\
|                         File handling                         |
\***************************************************************/

char cMaps::mLoadPED(const char* vFile,tLevel* lev) {
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

char cMaps::mLoadDAT(const char* vFile,tLevel* lev,char levelNumber) {
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

char cMaps::mSavePED(const char* vFile,tLevel* lev) {
  FILE * pFile;
  if ((pFile = fopen (vFile , "wb"))==NULL) {
		return 0;
	}
  fwrite (lev, 1, sizeof(*lev), pFile);
  fclose (pFile);
  return 1;
}

char cMaps::mSaveDAT(const char* vFile,tLevel* lev) {
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
;