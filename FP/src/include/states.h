/*

tsCondition stateGetCondition(tsState state, int number);
* Returns a condition number "number" or NULL if there are no more conditions *

tsConditionType stateGetConditionType(tsCondition condition);
* returns condition type *

int stateEvaluateKeyCondition(tKey* keyState);
int stateEvaluateMapCondition(tRoom* room,int x, int y);
 returns 0 if the condition if false for the current position or keys 

tsAction stateGetAction(tsCondition condition);
 get the action related to the given condition to be performed in case that it is true */

/* Start here */

/* public object */
struct tState { /* The state object: only struct that is not static to the state class */
	int frame; /* when zero, animation is released and the next state is taken */
	void** animation;
	tsAction* currentState;
}

/* public functions interface */
tState* createState(tKid* kid,tMap* map);
void updateState(tState* current,tKid* kid,tMap* map,tKey* key); /* This function should return the image frame
																													and actions to be performed by this call */

/* privates */

#include "states_conf.h"

tsAction createInitialState(tKid* kid,tMap* map); 
tsAction evaluateState(tState* current,tKid* kid,tMap* map); 

typedef enum {esKeyUp,esKeyDown,esKeyForward,esKeyBack,esShift,esMapUp,esMapDown,esMapForward,esMapBack,esMapOn,esForwardTileNearerThan,esForwardTileFartherThan,esInScreen,esInLevel,esForwardChangeToScreen,esLast=0}tsConditionType;


typedef enum {esDangerous,esNone,esWalk,esPotion,esSword,esMirror,esNotApplicable=0}tsTileType;
typedef enum {esRelative,esForwardTile}tsMoveType; /*  */

struct tsCondition {
	tsConditionType type;
	union {
		tsTileType tile;
		char units; 
		char screen; 
		char level; 
	}
}

struct tsAction {
	short        tsConditionId; /* Id of the first condition to be evaluated */
	tsMoveType   moveType; /* absolute or relative */
	char         moveOffset; /* amount of moving units the kid will move depending on the offset type */
	short        tsNextStateId; /* Id of the first action of the next state */
}


