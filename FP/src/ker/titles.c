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
titles.c: FreePrince : Titles, animation and presentation
¯¯¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 18 Jul 2004

  Authores: Diego Essaya <dessaya.cod@princed.com.ar>
	          Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "output.h"
#include "titles.h"
#include "input.h"
#include <stdio.h> /* NULL */
#include <stdlib.h> /* malloc/free */

#include "anims.h"
#include "kid.h"

typedef struct {
	tData*        img;
	unsigned char layer;
	unsigned short duration;
	unsigned short x;
	unsigned short y;
} titleFixedimg;

/* New source */
tMenuOption playAnimation(int id) {
	int qf,qt,qo,i;
	animFixedimg* f;
	animState* t;
	animSound* o;
	titleFixedimg* fa;
	tObject* ta;
	animSound* oa;
	tKey key=inputCreateKey();
	tKey nullKey=inputCreateKey();
	int activef=0;
	int activet=0;
	/*int activeo=0;*/
	int totalf,totalt,totalo;
	tObject* object;
	
	animStart(id,&totalf,&totalt,&totalo);
	fa=(titleFixedimg*)malloc(totalf*sizeof(titleFixedimg));
	ta=(tObject*)malloc(totalt*sizeof(tObject));
	oa=(animSound*)malloc(totalo*sizeof(animSound));
	
	while (animGetFrame(&qf,&qt,&qo,f,t,o)) {
		if (inputGetEvent(&key)) {
			/* key pressed */
			printf("key pressed\n");
		} else {
			/* create new images/objects/sounds */
			for (i=0;i<qf;i++) { /*images*/
				fa[activef].img=resLoad(f[i].res);
				fa[activef].y=f[i].y;
				fa[activef].x=f[i].x;
				fa[activef].layer=f[i].layer;
				fa[activef].duration=f[i].duration;
				activef++;
			}
			for (i=0;i<qt;i++) { /*objects*/
				ta[activet]=objectCreate(t[i].location,t[i].floor,DIR_LEFT,t[i].state,t[i].res,t[i].cacheMirror);
				activet++;
			}
/*		TODO: code sounds	
 *		for (i=0;i<qo;i++) {
				fa[activeo]=o[i];
				activeo++;
			}*/

			outputClearScreen();
			/* The bottom layer */
			for (i=0;i<activef;i++) {
				if (fa[i].layer==ANIMS_LAYERTYPE_BOTTOM)
					outputDrawBitmap(fa[i].img->pFrames[0], fa[i].x, fa[i].y);
			}
			/* move objects */
			for (i=0;i<activet;i++) {
				/*TODO: detect exits */
	  		objectMove(ta+i,nullKey,NULL);
	  		/*objectDraw(ta+i);*/
			}
			/* The top layer */
			for (i=0;i<activef;i++) {
				if (fa[i].layer==ANIMS_LAYERTYPE_TOP)
					outputDrawBitmap(fa[i].img->pFrames[0], fa[i].x, fa[i].y);
			}
			outputUpdateScreen();

			/* exited states and caducied backgrounds destruction */
			for (i=0;i<activef;i++) {
				if (fa[i].duration) { /* if not 0 (infinite) */
					fa[i].duration--;
					if (!fa[i].duration) { /* time is over for this images */
						activef--;
						resFree(fa[i].img);
						fa[i]=fa[activef];
					}
				}
			}	
		}
	}
	/*void objectDraw(tObject kid);*/
	for (i=0;i<activef;i++) objectFree(object[i]);
	free(object);
	return menuQuit;
}

/* Old source */

tMenuOption getAction(tKey key) {
	switch(key.actionPerformed) {
	case quit:
		return menuQuit;
	case load:
		return menuLoad;
	default:
		return menuStart;
	}
}

tMenuOption sleep(int ticks) {
	/* Wait ticks or a key is pressed if an action is thrown process it */
	tKey key=inputCreateKey();
	while (ticks) {
		ticks=inputDelay(&key,ticks);
		if (
			/* there are ticks remaining (a non-temporal action was thrown) */
			ticks &&
	 		/* there is an action */
			key.actionPerformed!=none &&
			/* the action wasn't control key */
			!(inputGetCtrl(key.status) && key.actionPerformed==other)
			) 
			return getAction(key); /* return the results of this action */ 
	}
	return menuNone;
}

#define goodBye {resFree(main_title);return result;}
	
tMenuOption showTitles() {
/* Show the titles animation
 * returns 0 if the user has finished the animations with quit
 * returns 1 if the user has selected to load a saved game 
 * returns 2 if the user has selected to start the game
 */

	tData *main_title;
	/*tData *main_text;*/
	tMenuOption result;

	main_title = resLoad(RES_IMG_MAIN_BACKGROUND);
	if (! main_title) {
		printf("The resource couldn't be loaded!\n");
		return menuQuit;
	}
	outputClearScreen();
	/* The main background */
	outputDrawBitmap(main_title->pFrames[0], 0, 200);
	outputUpdateScreen();
	result=sleep(12);
	if (result!=menuNone) goodBye;
	/* The presents */
	outputDrawBitmap(main_title->pFrames[1], 100, 100);
	outputUpdateScreen();
	result=sleep(12);
	if (result!=menuNone) goodBye;
	/* The author*/
	outputClearScreen();
	outputDrawBitmap(main_title->pFrames[0], 0, 200);
	outputDrawBitmap(main_title->pFrames[2], 100, 100);
	outputUpdateScreen();
	result=sleep(12);
	if (result!=menuNone) goodBye;
	/* The game name*/
	outputClearScreen();
	outputDrawBitmap(main_title->pFrames[0], 0, 200);
	outputDrawBitmap(main_title->pFrames[3], 30, 70);
	outputDrawBitmap(main_title->pFrames[4], 30, 190);
	outputUpdateScreen();
	/*SDL_Delay(1050);*/
	result=sleep(12);
	if (result!=menuNone) goodBye;

	/*
	 * The outputDraw doesn't support the text files yet
	 * 
	main_text = resLoad(RES_IMG_TEXT_BACKGROUND);
	if (! main_text) {
		printf("The resource couldn't be loaded!\n");
		return menuQuit;
	}
	*/
	/* The presents */
	/*
	outputDrawBitmap(main_text->pFrames[0], 0, 0);
	outputDrawBitmap(main_text->pFrames[1], 0, 0);
	outputUpdateScreen();
	*/
	do {
		result=sleep(1000);
	} while (result==menuNone);

	goodBye;
}	

