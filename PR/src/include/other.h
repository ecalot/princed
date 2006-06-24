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
other.h: Princed Resources : Other objects headers
¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 17 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_OTHER_H_
#define _PR_OTHER_H_

#include "binary.h"

void* objectBinaryCreate(tBinary c, int *error);
int objectBinaryWrite(void* o, const char* file, int optionflag, const char* backupExtension);
void* objBinaryRead(const char* file,int *result);
int objBinarySet(void* o,tResource* res);

/* TODO: move to level.h */

#define objectLevelPop1Create(a,b,c,d,e,f,g) objectLevelCreate(a,b,c,d,e,f,g)
#define objectLevelPop2Create(a,b,c,d,e,f,g) objectLevelCreate(a,b,c,d,e,f,g)
#define objectLevelPop1Write(a,b,c,d) objectLevelWrite(a,b,c,d,1)
#define objectLevelPop2Write(a,b,c,d) objectLevelWrite(a,b,c,d,2)

void* objectLevelCreate(tBinary content,int number,const char* datfile,const char* name,const char* desc,const char* datAuthor,int *error);
int objectLevelWrite(void* o, const char* file, int optionflag, const char* backupExtension,int popVersion);

#endif
