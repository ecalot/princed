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
reslist.c: Princed Resources : Ordered Read-Only list implementarion
¯¯¯¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 04 Jun 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2005-Jun-04)

 Note:
  DO NOT remove this copyright notice
*/

/* Defines */
#include "list.h"

/* app test */

int dataCmp(void* a,void* b) {
	int ap,bp;
	ap=*((int*)a);
	bp=*((int*)b);

	if (ap>bp) return GT;
	if (ap<bp) return LT;
	return EQ;	
}


int main(int a, char** b) {
	tList lista;
	const int* value;
	int k;
	int i;

	lista=list_create(sizeof(int),dataCmp);

	for (i=1;i<a;i++) {
		k=atoi(b[i]);
		list_insert(&lista,&k);
	}

	list_firstCursor(&lista);
	while (value=list_getCursor(&lista)) {
		printf("value: %d\n",*value);
		list_nextCursor(&lista);
	}

	list_firstCursor(&lista);
	k=100;
	i=list_moveCursor(&lista,&k);
	if (value=list_getCursor(&lista)) 
		printf("value prior 100: %d %d\n",*value,i);
	

	printf("listo\n");
	list_drop(&lista);
	return 0;
}


