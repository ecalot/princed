#include "states.h"
#include <stdlib.h>
#include "tiles.h" /* isInGroup & groups */
#include <stdio.h> /* For debug purposes */
#include "kid.h" /* DIR_LEFT DIR_RIGHT */

void debugShowFlag(short optionflag) {
	if (optionflag&STATES_FLAG_F) printf("Falling ");
	if (optionflag&STATES_FLAG_P) printf("PressFloor ");
	if (optionflag&STATES_FLAG_C) printf("PressCeiling ");
	if (optionflag&STATES_FLAG_S) printf("Sound");
	printf("\n");
}

#define STATES_STEPS_PER_TILE 10 
/* Private static state graph */
static tsAction statesActionList[]=STATES_ACTIONS;
static short statesAnimationList[]=STATES_ANIMATIONS;
static tsCondition statesConditionList[]=STATES_CONDITIONS;

void stateGetAnimation(int action,tState *state/*short *frames,short** flags,float* offsets*/) {
	tsAction* a=statesActionList+action;
	short i=a->animSize;
	short* j=statesAnimationList+(a->animStart*3);
	short totaloffset=a->moveOffset;
	/* TODO: depending on relative and absolute crop the middle frames */
	state->frame=i;
	state->animation=(short*)malloc(sizeof(short)*i);
	state->steps=(short*)malloc(sizeof(short)*i);
	state->flags=(short*)malloc(sizeof(short)*i);
	printf("* Animsize=%d Animstart=%d. (new animation allocated) Next:\n",i,a->animStart);
	state->step=(float)(totaloffset)/(float)(i); /* this is the full step to be used in case a non step walk is set TODO: delete this system */
	while (i--) {
		(state->animation)[i]=*(j++); /* the first short is the frame */
		((state->flags)[i])=*(j++); /* the second short is the flag */
		((state->steps)[i])=*(j++); /* the third short is the frame step */
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
#define DefaultTrue(pointer) if (!pointer) return STATES_CONDRESULT_TRUE
#define DefaultFalse(pointer) if (!pointer) return STATES_CONDRESULT_TRUE
int evaluateCondition(int condition,tKey* key, tKid* kid, tRoom* room) {
	tsCondition c=statesConditionList[condition];
#define thisTile (kid->location/STATES_STEPS_PER_TILE+13+12*kid->floor)
#define whereInTile ((kid->direction==DIR_LEFT)?(kid->location%STATES_STEPS_PER_TILE):STATES_STEPS_PER_TILE-(kid->location%STATES_STEPS_PER_TILE))
	switch(c.type) {
	case esKeyUp:
		DefaultFalse(key);
		return inputGetUp(key->status)? /* TODO: argument notPressed isn't supported */
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyDown:
		DefaultFalse(key);
		return inputGetDown(key->status)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyForward:
		DefaultFalse(key);
		return ((kid->direction==DIR_LEFT)?inputGetLeft(key->status):inputGetRight(key->status))?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyBack:
		DefaultFalse(key);
		return ((kid->direction==DIR_LEFT)?inputGetRight(key->status):inputGetLeft(key->status))?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esKeyShift:
		DefaultFalse(key);
		return inputGetShift(key->status)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapUpForward:
		DefaultFalse(room);
		{
			int t=thisTile-12+((kid->direction==DIR_LEFT)?-1:1);
		return isInGroup(room->fore[t],room->back[t],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
		}
	case esMapUp:
		DefaultFalse(room);
		return isInGroup(room->fore[thisTile-12],room->back[thisTile-12],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapDown:
		DefaultFalse(room);
		return isInGroup(room->fore[thisTile+12],room->back[thisTile+12],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapForward:
		DefaultFalse(room);
		return isInGroup(room->fore[thisTile+((kid->direction==DIR_LEFT)?-1:1)],room->back[thisTile+((kid->direction==DIR_LEFT)?-1:1)],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapBack:
		DefaultFalse(room);
		return isInGroup(room->fore[thisTile+((kid->direction==DIR_LEFT)?1:-1)],room->back[thisTile+((kid->direction==DIR_LEFT)?1:-1)],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esMapOn:
		DefaultFalse(room);
		return isInGroup(room->fore[thisTile],room->back[thisTile],c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esForwardTileNearerThan:
		DefaultFalse(kid);
		return (whereInTile<c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esForwardTileFartherThan:
		DefaultFalse(kid);
		return (whereInTile>c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esInScreen:
		DefaultFalse(room);
		return (room->id==c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esInLevel:
		DefaultFalse(room);
		return (room->level->levelNumber==c.argument)?
			STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE;
	case esForwardChangeToScreen:
		DefaultFalse(kid);
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

#define alternate(i,n) (((i)%2)?(n-(((i)+1)>>1)):((i)>>1))

/* This function should return the image frame and actions to be performed by this call
 * returns the animation number corresponding to this frame */
short stateUpdate(tKey* key, tKid* kid,tRoom* room) {
	tState* current=&(kid->action);
	/*static float step;
	static float acumLocation;*/
	short flags;
	short steps;
	
	current->frame--;
	
	current->image=current->animation[current->frame];
	flags         =current->flags    [current->frame];
	steps         =current->steps    [current->frame];
	
	/* BEGIN DEBUG */
	printf("stateUpdate: animation=%d steps=%d ",current->image,steps);
	debugShowFlag(flags);
	/* END DEBUG */
	
	if (!current->frame) {
		int action;
		free(current->animation);
		free(current->flags);
		free(current->steps);
		/* Find matching action */
		action=evaluateState(current->currentState,key,kid,room);

		/* Performs the events called by this action */
			/* Remember the animation and flags for the next current->frame frames */
		stateGetAnimation(action,current);
			/* Remember the state where we are now */
		current->currentState=statesActionList[action].nextStateId;
		printf("NEW STATE: action=%d next=%d\n",action,current->currentState);
			/* Move the kid (turn+traslate) */
		switch(statesActionList[action].moveType) {
		case STATES_MOVETYPES_ABSOLUTEONSTART:
			/* AbsoluteOnStart (x)
			 * Moves the kid x step units from the first forward tile change
			 * and starts the animation there
			 */

			/* 1) move current location to the left tileChange */
			kid->location-=(kid->location%STATES_STEPS_PER_TILE);
			/* 2) if looking right add one tile to reach the right tileChange
			 * 3) if looking right add x, if looking left substract x */
			if (kid->direction!=DIR_LEFT)
				kid->location+=STATES_STEPS_PER_TILE+statesActionList[action].moveOffset;
			else
				kid->location-=statesActionList[action].moveOffset;
			break;
		case STATES_MOVETYPES_ABSOLUTEONSTOP: {
			/* AbsoluteOnStop (x)
			 * Deletes frames (in the middle) to make sure that, at the end of the animation,
			 * the kid had moved only x step units from the first forward tile change
			 * if there is a lack of movements by frame it stops before reaching it.
			 */
			/* 1) calculate the number of frames the guy will move */
			int accumulate=0;
			int i,j;
			int from,to;
			/* First iteration: determine i=number of frames not cropped */
			for (i=0;(i<current->frame)&&(accumulate<statesActionList[action].moveOffset);i++) {
				accumulate+=current->steps[alternate(i,current->frame)];
			}
			for (j=0;j<i;j++) {
				from=alternate(j,current->frame);
				to=alternate(j,i);
				if (j%2) {
					/* the first frames are ok, so I'll fix the last frames */
					printf("from=%d to=%d ok\n",from,to);
					current->animation[to]=current->animation[from];
					current->flags[to]=current->flags[from];
					current->steps[to]=current->steps[from];
				}
			}
			printf("total frames=%d number of frames to be used=%d. wanted movement=%d movement to be performed=%d\n",current->frame,i,statesActionList[action].moveOffset,accumulate);
			current->frame=i; /* now the last frames are moved, crop the animation */
			break;
			}
		case STATES_MOVETYPES_RELATIVETURN:
			/* relative but turning */
			kid->direction=(kid->direction==DIR_LEFT)?DIR_RIGHT:DIR_LEFT;
			kid->location+=(kid->direction==DIR_LEFT)?
				-statesActionList[action].moveOffset:
				statesActionList[action].moveOffset;
			break;
		case STATES_MOVETYPES_RELATIVE:
				kid->location+=(kid->direction==DIR_LEFT)?
				-statesActionList[action].moveOffset:
				statesActionList[action].moveOffset;
			break;
		}
	}
	
	kid->location+=(kid->direction==DIR_LEFT)?-steps:steps;

/*
	{
		int jj;
		for (jj=0;jj<5;jj++) {

			printf("%d=>",alternate(jj,10));
			printf("%d ",alternate(jj,5));
		}
		printf("\n");
	}
*/

	
	if (current->currentState<0) return current->currentState; /* if last state return exit code */
	return flags;
}


