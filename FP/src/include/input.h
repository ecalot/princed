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

typedef enum {none=0,quit,reload,passLevel,showUp,showLeft,showRight,showDown,moreTime,lessTime,addLive,addHitPoint,gotoTitles,kill,resurrect}tAction;


typedef struct {
	int shiftPressed;
	int controlPressed;
	int upPressed;
	int leftPressed;
	int rightPressed;
	tAction actionPerformed;
} tKey

void inputInitKey(tKey* key);
int inputGetEvent(tKey* key);
/* Saves the key status un key (either from the keyboard or joystick or whatever)
 * returns 1 if a time event is activated or 0 if an action was performed
 */

void inputStopTimer();
void inputInitTimer();

#endif
