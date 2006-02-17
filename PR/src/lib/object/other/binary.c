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
.c: Princed Resources : 
¯¯¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 17 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2006-Feb-09)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include "binary.h" /* tBinary */
#include "common.h"
#include <stdlib.h>

/***************************************************************\
|                         Binary Object                         |
\***************************************************************/

void* objBinaryCreate(unsigned char* data, int size, int *error) { /* use get like main.c */
	tBinary* r;
	*error=PR_RESULT_SUCCESS;
	
	r=(tBinary*)malloc(sizeof(tBinary));
	r->data=data;
	r->size=size;
	return (void*)r;
}



