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
} titleImage;

typedef struct {
	tObject        obj;
	int            active;
	unsigned short duration;
} titleObject;

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

tMenuOption playAnimation(int id) {

	/* Declare variables */
	int imgCount,         objCount,              sndCount,               i;
	animImage*  img;      animObject* obj;       animSound* snd;
	titleImage* imgArray; titleObject* objArray; /*animSound* sndArray;*/
	int imgsActive=0;     int objsActive=0;      /*int sndsActive=0;*/
	int imgTotal,         objTotal,              sndTotal;

	tKey key=inputCreateKey();
	tKey nullKey=inputCreateKey();

	/* Initialize animation and allocate memory */
	animStart(id,&imgTotal,&objTotal,&sndTotal);
	imgArray=(titleImage*)malloc(imgTotal*sizeof(titleImage));
	objArray=(titleObject*)malloc(objTotal*sizeof(titleObject));
	/*sndArray=(animSound*)malloc(sndTotal*sizeof(animSound));*/

	/* main animation kernel loop */
	while (animGetFrame(&imgCount,&objCount,&sndCount,&img,&obj,&snd)) {
		int reprocessInput=1;

		while(reprocessInput) {
		if (!inputGetEvent(&key)) {
			/* key pressed */
			printf("key pressed\n");
		 	/*  if there is an action      and  the action wasn't control key */
			if (key.actionPerformed!=none  &&   !(inputGetCtrl(key.status)&&key.actionPerformed==other))
				return getAction(key);
		} else {
			reprocessInput=0;
			/* create new images/objects/sounds */
			for (i=0;i<imgCount;i++) { /*images*/
				imgArray[imgsActive].img=resLoad(img[i].res);
				if (!imgArray[imgsActive].img) {
					fprintf(stderr,"resource coudn't be loaded.");
					return menuQuit;
				}
				imgArray[imgsActive].y=img[i].y;
				imgArray[imgsActive].x=img[i].x;
				imgArray[imgsActive].layer=img[i].layer;
				imgArray[imgsActive].duration=img[i].duration;
				imgsActive++;
			}
			for (i=0;i<objCount;i++) { /*objects*/
				objArray[objsActive].obj=objectCreate(obj[i].location,obj[i].floor,DIR_LEFT,obj[i].state,obj[i].res,obj[i].cacheMirror,oGeneric);
				objArray[objsActive].active=1;
				objArray[objsActive].duration=obj[i].duration;
				objsActive++;
			}
/*		TODO: code sounds	
 *		for (i=0;i<sndCount;i++) {
				sndArray[sndsActive]=snd[i];
				sndsActive++;
			}*/

			outputClearScreen();

			/* The bottom layer */
			for (i=0;i<imgsActive;i++) {
				if (imgArray[i].layer==ANIMS_LAYERTYPE_BOTTOM)
					outputDrawBitmap(imgArray[i].img->pFrames[0], imgArray[i].x, imgArray[i].y);
			}
			
			/* move objects */
			for (i=0;i<objsActive;i++) {
				/*TODO: detect exits */
				if (objArray[i].active) {
					int exitCode;
		  		exitCode=objectMove(&(objArray[i].obj),nullKey,NULL);
					if (objArray[i].duration) objArray[i].duration--;

					/* detect exited states and destroy them */

					/* if the time is over or exit code detected */
					if ((objArray[i].duration==1)||(exitCode<0)) {
						printf("exit Code detected: i=%d exit=%d \n",i,exitCode);
						objectFree(objArray[i].obj);
						objArray[i].active=0; /* remember it is destroyed */
					} else {
		  			objectDraw(objArray[i].obj);
					}
				}
			}
			
			/* The top layer */
			for (i=0;i<imgsActive;i++) {
				if (imgArray[i].layer==ANIMS_LAYERTYPE_TOP) {
					outputDrawBitmap(imgArray[i].img->pFrames[0], imgArray[i].x, imgArray[i].y);
				}
			}
			outputUpdateScreen();

			/* caducied backgrounds destruction */
			i=imgsActive;
			while(i) {
				i--;
				if (imgArray[i].duration) { /* if not 0 (infinite) */
					imgArray[i].duration--;
					if (!imgArray[i].duration) { /* time is over for this images */
						imgsActive--;
						resFree(imgArray[i].img);
						imgArray[i]=imgArray[imgsActive];
					}
				}
			}	}
		}
	}

	for (i=0;i<objsActive;i++) if (objArray[i].active) objectFree(objArray[i].obj);
	for (i=0;i<imgsActive;i++) resFree(imgArray[i].img);
	free(imgArray);
	free(objArray);
	/*free(sndArray);*/
	return menuQuit;
}

/* Old source */

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
#define testing
#ifdef testing
	printf("Starting animation testing\n");
				
	return playAnimation(ANIMS_ID_PRESENTATION);
#else
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
#endif
}	

