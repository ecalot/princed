#include "states.h"
#include <stdlib.h>
#include "tiles.h" /* isInGroup & groups */

/* Private static state graph */
static tsAction statesActionList[]=STATES_ACTIONS;
static short statesAnimationList[]=STATES_ANIMATIONS;
static tsCondition statesConditionList[]=STATES_CONDITIONS;

short* stateGetAnimation(int action,short *frames) {
	short* result;
	tsAction* a=statesActionList+action;
	short i=a->animSize;
	short* j=statesAnimationList+a->animStart;
	/* TODO: depending on relative and absolute crop the middle frames */
	*frames=i;
	result=(short*)malloc(sizeof(short)*i);
	while (i--) result[i]=*(j++);
	return result;
}

/* public functions interface */
tState createState(int level) {
	tState start;
	static short statesLevelList[]=STATES_LEVELS;
	start.animation=stateGetAnimation(statesLevelList[level],&(start.frame));
	start.currentState=statesActionList[statesLevelList[level]].nextStateId;
	return start;
}

/* private functions */

/* Evaluates a condition indexed in the condition table */
int evaluateCondition(int condition,tKey* key, tKid* kid, tRoom* room) {
	tsCondition c=statesConditionList[condition];
	int thisTile=kid->location/32+1+12*kid->floor;
	int whereInTile=(kid->direction==eLeft)?(kid->location%32):32-(kid->location%32);
	switch(c.type) {
	case esKeyUp:
		return inputGetUp(key->status)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyDown:
		return inputGetDown(key->status)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyForward:
		return ((kid->direction==eLeft)?inputGetLeft(key->status):inputGetRight(key->status))?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyBack:
		return ((kid->direction==eLeft)?inputGetRight(key->status):inputGetLeft(key->status))?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyShift:
		return inputGetShift(key->status)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapUp:
		return isInGroup(room->fore[thisTile-12],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapDown:
		return isInGroup(room->fore[thisTile+12],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapForward:
		return isInGroup(room->fore[thisTile+((kid->direction==eLeft)?-1:1)],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapBack:
		return isInGroup(room->fore[thisTile+((kid->direction==eLeft)?1:-1)],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapOn:
		return isInGroup(room->fore[thisTile],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esForwardTileNearerThan:
		return (whereInTile<c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esForwardTileFartherThan:
		return (whereInTile>c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esInScreen:
		return (room->id==c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esInLevel:
		return (room->level->levelNumber==c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esForwardChangeToScreen:
		return ((kid->direction==eLeft)&&(thisTile==1))
			||((kid->direction==eRight)&&(thisTile==10))?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esLast:
		return STATES_CONDRESULT_END;
	default:
		return STATES_CONDRESULT_FALSE;
	}
}

/* Evaluates all conditions needed for an action and returns true or false */
int evaluateAction(int currentAction,tKey* key, tKid* kid,tRoom* room) {
	int i=statesActionList[currentAction].conditionId;
	int result;
	while ((result=evaluateCondition(i,key,kid,room))==STATES_CONDRESULT_TRUE) i++;
	if (result==STATES_CONDRESULT_FALSE) return 0;
	return 1; /* Only returns true if STATES_CONDRESULT_END is reached */
}

/* Evaluates a state: all conditions for all actions until a true is found and returns the next state id*/
int evaluateState(int state, tKey* key, tKid* kid, tRoom* room) {
	int i=state;
	while (!evaluateAction(i,key,kid,room)) i++;
	return i;
}

/* This function should return the image frame and actions to be performed by this call
 * returns the animation number corresponding to this frame */
int stateUpdate(tState* current,tKey* key, tKid* kid,tRoom* room) {
	int imageFrame=statesAnimationList[current->animation[current->frame]];
	if (current->frame) {
		current->frame--;
	} else {
		int action;
		/* free(current->animation) */
		action=evaluateState(current->currentState,key,kid,room);
		current->animation=stateGetAnimation(action,&(current->frame));
		current->currentState=statesActionList[action].nextStateId;
	}
	return imageFrame;
}


