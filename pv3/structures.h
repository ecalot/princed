/***************************************************************\
|                       Estructuras de Datos                    |
\***************************************************************/

//Link Handler
typedef struct {
	unsigned char linkMap[MAPS_MAX_LENGTH][MAPS_MAX_LENGTH];
	unsigned char linkList[MAPS_MAX_SCREENS];
	char limLeft,limRight,limUp,limDown;
}tLevelHandleLinks;

//Gate Handler
typedef struct {
	unsigned char pos[2];
}tGateEvent;

typedef struct {
	tGateEvent event[256];
	unsigned char size;
}tGateEvents;

typedef struct {
	unsigned char size;
	tGateEvent event[50];
}tGateRow;

typedef struct {
	unsigned char size;
	tGateRow row[50];
}tGateTDA;

typedef struct {
	unsigned char id;
	unsigned char door;
}tGateListItem;

typedef struct {
	int size;
	tGateListItem item[720];
}tGateList;

//Main level structure
typedef struct {
	char level;
	tLevelHandleLinks handledLinks;
	unsigned char levelArray[MAPS_BLOCK_SIZEOF_LEVEL];
	char desc[200];
	char name[50];
	tGateTDA gateTDA;
	tGateList gateList;
	tGateEvents gateEvents;
}tLevel;
