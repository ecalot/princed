#include "states.h"


short* stateGetAnimation(int action,short* frames);

static tsAction statesActionList[]=STATES_ACTIONS;
static short statesAnimationList[]=STATES_ANIMATIONS;
static tsCondition statesConditionList[]=STATES_CONDITIONS;

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
int evaluateCondition(int condition,tKey* key,tMap* map) {
	tsCondition c=statesConditionList[condition];
	switch(c.type) {
	case esKeyUp:
	case esKeyDown:
	case esKeyForward:
	case esKeyBack:
	case esKeyShift:
	case esMapUp:
	case esMapDown:
	case esMapForward:
	case esMapBack:
	case esMapOn:
	case esForwardTileNearerThan:
	case esForwardTileFartherThan:
	case esInScreen:
	case esInLevel:
	case esForwardChangeToScreen:
		return STATES_CONDRESULT_TRUE;
	case esLast:
		return STATES_CONDRESULT_END;
	default:
		return STATES_CONDRESULT_FALSE;
	}
}

/* Evaluates all conditions needed for an action and returns true or false */
int evaluateAction(int currentAction,tKey* key,tMap* map) {
	int i=statesActionList[currentAction].conditionId;
	int result;
	while ((result=evaluateCondition(i,key,map))==STATES_CONDRESULT_TRUE) i++;
	if (result==STATES_CONDRESULT_FALSE) return 0;
	return 1; /* Only returns true if STATES_CONDRESULT_END is reached */
}

/* Evaluates a state: all conditions for all actions until a true is found and returns the next state id*/
int evaluateState(int state, tKey* key, tMap* map) {
	int i=state;
	while (!evaluateAction(i,key,map)) i++;
	return i;
}

/* This function should return the image frame and actions to be performed by this call
 * returns the animation number corresponding to this frame */
int stateUpdate(tState* current,tKey* key,tMap* map) {
	int imageFrame=statesAnimationList[current->animation[current->frame]];
	if (current->frame) {
		current->frame--;
	} else {
		int action;
		/* free(current->animation) */
		action=evaluateState(current->currentState,key,map);
		current->animation=stateGetAnimation(action,&(current->frame));
		current->currentState=statesActionList[action].nextStateId;
	}
	return imageFrame;
}


