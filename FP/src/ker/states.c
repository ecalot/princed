/*  Princed V3 - Prince of Persia Level Editor for PC Version
    Copyright (C) 2003 Princed Development Team

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
states.c: FreePrince : State object
��������
 Copyright 2004 Princed Development Team
  Created: 16 Oct 2004

  Author:  Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "states.h"
#include <stdlib.h>
#include "tiles.h" /* isIn & groups */
#include <stdio.h> /* For debug purposes */
#include "object.h" /* DIR_LEFT DIR_RIGHT */
#include "room.h" /* getTile */

#ifdef DEBUGSTATES
void debugShowFlag(short optionflag) {
	if (optionflag&STATES_FLAG_F) printf("Falling ");
	if (optionflag&STATES_FLAG_P) printf("PressFloor ");
	if (optionflag&STATES_FLAG_C) printf("PressCeiling ");
	if (optionflag&STATES_FLAG_S) printf("Sound");
	printf("\n");
}
#endif

#define STATES_STEPS_PER_TILE TILE_W 
/* Private static state graph */
static tsAction statesActionList[]=STATES_ACTIONS;
static short statesAnimationList[]=STATES_ANIMATIONS;
static tsCondition statesConditionList[]=STATES_CONDITIONS;

void stateGetAnimation(int action,tState *state/*short *frames,short** flags,float* offsets*/) {
	tsAction* a=statesActionList+action;
	short i=a->animSize;
	short* j=statesAnimationList+(a->animStart*4);
	short totaloffset=a->moveOffset;

	state->frame=i;
	state->animation=(short*)malloc(sizeof(short)*i);
	state->steps=(short*)malloc(sizeof(short)*i);
	state->flags=(short*)malloc(sizeof(short)*i);
	state->offsx=(short*)malloc(sizeof(short)*i);
#ifdef DEBUGSTATES
	printf("* Animsize=%d Animstart=%d. (new animation allocated) Next:\n",i,a->animStart);
#endif
	state->step=(float)(totaloffset)/(float)(i); /* this is the full step to be used in case a non step walk is set TODO: delete this system */
	while (i--) {
		(state->animation)[i]=*(j++); /* the first short is the frame */
		((state->flags)[i])=*(j++); /* the second short is the flag */
		((state->steps)[i])=*(j++); /* the third short is the frame step */
		((state->offsx)[i])=*(j++); /* the fourth short is the height */
	}
}

/* public functions interface */
int stateKidInLevel(int level) {
	static short statesLevelList[]=STATES_LEVELS;
	return statesLevelList[level];
}

tState createState(short stateId) {
	tState start;
	stateGetAnimation(stateId,&start);
	start.currentState=statesActionList[stateId].nextStateId;
	return start;
}

/* private functions */

/* Evaluates a condition indexed in the condition table */
#define DefaultTrue(pointer)  if (!pointer) return STATES_CONDRESULT_TRUE
#define DefaultFalse(pointer) if (!pointer) return STATES_CONDRESULT_FALSE

#define statesKidLeft (kid->direction==DIR_LEFT)

#define whereInTile (statesKidLeft? \
	(kid->location%STATES_STEPS_PER_TILE): \
	STATES_STEPS_PER_TILE-(kid->location%STATES_STEPS_PER_TILE))

#define kidX (kid->location/STATES_STEPS_PER_TILE+1)
#define kidY (kid->floor+1)
#define statesTile(x,y) roomGetTile(room,kidX+x,kidY+y)
#define kidDirection (statesKidLeft?-1:1)

#define statesCondRet(a) return (a)?STATES_CONDRESULT_TRUE:STATES_CONDRESULT_FALSE

/* Memory interpreter */
int evaluateCondition(int condition,tKey* key, tObject* kid, tRoom* room) {
	tsCondition c=statesConditionList[condition];
	switch(c.type) {
	case esKeyUp:
		DefaultFalse(key);
		statesCondRet(inputGetUp(key->status));
		/* TODO: argument notPressed isn't supported */
	case esKeyDown:
		DefaultFalse(key);
		statesCondRet(inputGetDown(key->status));
	case esKeyForward:
		DefaultFalse(key);
		statesCondRet((statesKidLeft?inputGetLeft(key->status):inputGetRight(key->status)));
	case esKeyBack:
		DefaultFalse(key);
		statesCondRet((statesKidLeft?inputGetRight(key->status):inputGetLeft(key->status)));
	case esKeyShift:
		DefaultFalse(key);
		statesCondRet(inputGetShift(key->status));
	case esMapUpForward:
		DefaultFalse(room);
		statesCondRet(isIn(statesTile(kidDirection,-1),c.argument));
	case esMapUp:
		DefaultFalse(room);
		statesCondRet(isIn(statesTile(0,-1),c.argument));
	case esMapDown:
		DefaultFalse(room);
		statesCondRet(isIn(statesTile(0,1),c.argument));
	case esMapForward:
		DefaultFalse(room);
		statesCondRet(isIn(statesTile(kidDirection,0),c.argument));
	case esMapBack:
		DefaultFalse(room);
		statesCondRet(isIn(statesTile(-kidDirection,0),c.argument));
	case esMapOn:
		DefaultFalse(room);
		statesCondRet(isIn(statesTile(0,0),c.argument));
	case esForwardTileNearerThan:
		DefaultFalse(kid);
		statesCondRet((whereInTile<c.argument));
	case esForwardTileFartherThan:
		DefaultFalse(kid);
		statesCondRet((whereInTile>c.argument));
	case esInScreen:
		DefaultFalse(room);
		statesCondRet((room->id==c.argument));
	case esInLevel:
		DefaultFalse(room);
		statesCondRet((room->level->levelNumber==c.argument));
	case esForwardChangeToScreen:
		DefaultFalse(kid);
		statesCondRet(((statesKidLeft)&&(kidX==1))||((!statesKidLeft)&&(kidX==10)));
	case esLast:
		return STATES_CONDRESULT_END;
	default:
		return STATES_CONDRESULT_FALSE;
	}
}

/* Evaluates all conditions needed for an action and returns true or false */
int evaluateAction(int currentAction,tKey* key, tObject* kid,tRoom* room) {
	int i=statesActionList[currentAction].conditionId;
	int result;
	while ((result=evaluateCondition(i,key,kid,room))==STATES_CONDRESULT_TRUE) i++;
	if (result==STATES_CONDRESULT_FALSE) return 0;
	return 1; /* Only returns true if STATES_CONDRESULT_END is reached */
}

/* Evaluates a state: all conditions for all actions until a true is found and returns the next state id*/
int evaluateState(int state, tKey* key, tObject* kid, tRoom* room) {
	int i=state;
	while (!evaluateAction(i,key,kid,room)) i++;
	return i;
}

#define alternate(i,n) (((i)%2)?(n-(((i)+1)>>1)):((i)>>1))

/* This function should return the image frame and actions to be performed by this call
 * returns the animation number corresponding to this frame */
short stateUpdate(tKey* key, tObject* kid,tRoom* room) {
	tState* current=&(kid->action);
	/*static float step;
	static float acumLocation;*/
	short flags;
	short steps;
	
	current->frame--;
	
	current->image=current->animation[current->frame];
	flags         =current->flags    [current->frame];
	steps         =current->steps    [current->frame];
	current->imgoffx=current->offsx    [current->frame];
	current->mirror=flags&STATES_FLAG_M?1:0;
	
	/* BEGIN DEBUGSTATES */
#ifdef DEBUGSTATES
	printf("stateUpdate: animation=%d steps=%d ",current->image,steps);
	debugShowFlag(flags);
#endif
	/* END DEBUGSTATES */
	
	if (!current->frame) {
		int action;
		free(current->animation);
		free(current->flags);
		free(current->steps);
		free(current->offsx);
		/* Find matching action */
		action=evaluateState(current->currentState,key,kid,room);

		/* Performs the events called by this action */
			/* Remember the animation and flags for the next current->frame frames */
		stateGetAnimation(action,current);
			/* Remember the state where we are now */
		current->currentState=statesActionList[action].nextStateId;
#ifdef DEBUGSTATES
		printf("NEW STATE: action=%d next=%d\n",action,current->currentState);
#endif
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
			 * Deletes frames (in the middle) to make sure that, at the end of
			 * the animation, the kid had moved only x step units from the first
			 * forward tile change
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
#ifdef DEBUGSTATES
					printf("from=%d to=%d ok\n",from,to);
#endif
					current->animation[to]=current->animation[from];
					current->flags[to]=current->flags[from];
					current->steps[to]=current->steps[from];
				}
			}
#ifdef DEBUGSTATES
			printf("total frames=%d number of frames to be used=%d. wanted movement=%d movement to be performed=%d\n",current->frame,i,statesActionList[action].moveOffset,accumulate);
#endif
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

	if (current->frame==1&&current->currentState<0) return current->currentState; /* if last frame of the last state, return exit code */
	return flags;
}


