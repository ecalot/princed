#include "states.h"
#include <stdlib.h>
#include "tiles.h" /* isInGroup & groups */
#include <stdio.h> /* For debug purposes */
#include "kid.h" /* DIR_LEFT DIR_RIGHT */

void debugShowFlag(short optionflag) {
	if (optionflag&STATES_FLAG_U) printf("ScreenUp ");
	if (optionflag&STATES_FLAG_H) printf("ScreenLeft ");
	if (optionflag&STATES_FLAG_J) printf("ScreenRight ");
	if (optionflag&STATES_FLAG_N) printf("ScreenDown ");
	if (optionflag&STATES_FLAG_P) printf("PressFloor ");
	if (optionflag&STATES_FLAG_C) printf("PressCeiling ");
	if (optionflag&STATES_FLAG_S) printf("Sound");
	printf("\n");
}

#define STATES_STEPS_PER_TILE 32 
/* Private static state graph */
static tsAction statesActionList[]=STATES_ACTIONS;
static short statesAnimationList[]=STATES_ANIMATIONS;
static tsCondition statesConditionList[]=STATES_CONDITIONS;

void stateGetAnimation(int action,tState *state/*short *frames,short** flags,float* offsets*/) {
	tsAction* a=statesActionList+action;
	short i=a->animSize;
	short* j=statesAnimationList+(a->animStart*2);
	short totaloffset=a->moveOffset;
	/* TODO: depending on relative and absolute crop the middle frames */
	state->frame=i;
	state->animation=(short*)malloc(sizeof(short)*i);
	state->flags=(short*)malloc(sizeof(short)*i);
	printf("* Animsize=%d Animstart=%d. (new animation allocated) Next:\n",i,a->animStart);
	state->step=(float)(totaloffset)/(float)(i); /* the first short is the frame */
	while (i--) {
		(state->animation)[i]=*(j++);
		((state->flags)[i])=*(j++); /* the second short is the flag */
	}
}

/* public functions interface */
tState createState(int level) {
	tState start;
	static short statesLevelList[]=STATES_LEVELS;
	stateGetAnimation(statesLevelList[level],&start);
	start.currentState=statesActionList[statesLevelList[level]].nextStateId;
	return start;
}

/* private functions */

/* Evaluates a condition indexed in the condition table */
int evaluateCondition(int condition,tKey* key, tKid* kid, tRoom* room) {
	tsCondition c=statesConditionList[condition];
	int thisTile=kid->location/STATES_STEPS_PER_TILE+1+12*kid->floor;
	int whereInTile=(kid->direction==DIR_LEFT)?(kid->location%STATES_STEPS_PER_TILE):STATES_STEPS_PER_TILE-(kid->location%STATES_STEPS_PER_TILE);
	switch(c.type) {
	case esKeyUp:
		return inputGetUp(key->status)? /* TODO: argument notPressed isn't supported */
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyDown:
		return inputGetDown(key->status)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyForward:
		return ((kid->direction==DIR_LEFT)?inputGetLeft(key->status):inputGetRight(key->status))?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyBack:
		return ((kid->direction==DIR_LEFT)?inputGetRight(key->status):inputGetLeft(key->status))?
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
		return isInGroup(room->fore[thisTile+((kid->direction==DIR_LEFT)?-1:1)],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapBack:
		return isInGroup(room->fore[thisTile+((kid->direction==DIR_LEFT)?1:-1)],c.argument)?
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
		return ((kid->direction==DIR_LEFT)&&(thisTile==1))
			||((kid->direction==DIR_RIGHT)&&(thisTile==10))?
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
short stateUpdate(tKey* key, tKid* kid,tRoom* room) {
	tState* current=&(kid->action);
	/*static float step;
	static float acumLocation;*/
	short flags;
	
	current->frame--;
	
	current->image=current->animation[current->frame];
	flags         =current->flags    [current->frame];
	
	/* BEGIN DEBUG */
	printf("stateUpdate: animation=%d ",current->image);
	debugShowFlag(flags);
	/* END DEBUG */
	
	if (!current->frame) {
		int action;
		free(current->animation);
		free(current->flags);
		/* Find matching action */
		action=evaluateState(current->currentState,key,kid,room);

		/* Performs the events called by this action */
			/* Remember the animation and flags for the next current->frame frames */
		stateGetAnimation(action,current);
			/* Remember the state where we are now */
		current->currentState=statesActionList[action].nextStateId;
			/* Move the kid (turn+traslate) */
		if (kid->direction==DIR_LEFT) {
			current->step=-current->step;
			switch(statesActionList[action].moveType) {
			case STATES_MOVETYPES_RELATIVE:
				/*kid->location-=statesActionList[action].moveOffset;*/
				break;
			case STATES_MOVETYPES_ABSOLUTEFORWARD:
				kid->location=kid->location-(kid->location%STATES_STEPS_PER_TILE);
				break;
			case STATES_MOVETYPES_RELATIVETURN:
				/*kid->location-=statesActionList[action].moveOffset;*/
				kid->direction=DIR_RIGHT;
				break;
			}
		} else {
			switch(statesActionList[action].moveType) {
			case STATES_MOVETYPES_RELATIVE:
				/*kid->location+=statesActionList[action].moveOffset;*/
				break;
			case STATES_MOVETYPES_ABSOLUTEFORWARD:
				kid->location=STATES_STEPS_PER_TILE+kid->location-(kid->location%STATES_STEPS_PER_TILE);
				break;
			case STATES_MOVETYPES_RELATIVETURN:
				/*kid->location+=statesActionList[action].moveOffset;*/
				kid->direction=DIR_LEFT;
				break;
			}
		}
		current->acumLocation=kid->location;
	}
	current->acumLocation+=current->step;
	kid->location=current->acumLocation;
	return flags;
}


