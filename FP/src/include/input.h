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
input.h: Free Prince : Input devices handling
��������
 Copyright 2004 Princed Development Team
  Created: 24 Aug 2003

  Author: name <name.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _INPUT_H_
#define _INPUT_H_

typedef enum {none=0,quit,load,reload,passLevel,showUp,showLeft,showRight,showDown,moreTime,lessTime,addLive,addHitPoint,gotoTitles,kill,resurrect,other}tAction;

/*#define inputIgnoreCtrl(a)  (a&( ~(1<<1) ))*/
/* TODO: use inputIgnore* for each Set* */
#define inputSetShift(a,b) a=(a&( ~(1<<0) )) | ((b)<<0)
#define inputSetCtrl(a,b)  a=(a&( ~(1<<1) )) | ((b)<<1)
#define inputSetUp(a,b)    a=(a&( ~(1<<2) )) | ((b)<<2)
#define inputSetDown(a,b)  a=(a&( ~(1<<3) )) | ((b)<<3)
#define inputSetLeft(a,b)  a=(a&( ~(1<<4) )) | ((b)<<4)
#define inputSetRight(a,b) a=(a&( ~(1<<5) )) | ((b)<<5)


#define inputGetShift(a) ((a)&( (1<<0) )) 
#define inputGetCtrl(a)  ((a)&( (1<<1) )) 
#define inputGetUp(a)    ((a)&( (1<<2) ))
#define inputGetDown(a)  ((a)&( (1<<3) ))
#define inputGetLeft(a)  ((a)&( (1<<4) ))
#define inputGetRight(a) ((a)&( (1<<5) ))

typedef struct {
/*	int shiftPressed;
	int controlPressed;
	int upPressed;
	int leftPressed;
	int rightPressed;
	int downPressed;*/
	unsigned char status;
	tAction actionPerformed;
} tKey;

tKey inputCreateKey();
int inputGetEvent(tKey* key);
/* Saves the key status un key (either from the keyboard or joystick or whatever)
 * returns 1 if a time event is activated or 0 if an action was performed
 */

int inputDelay(tKey* key,int ticks);
void inputStopTimer();
void inputInitTimer();

#endif
