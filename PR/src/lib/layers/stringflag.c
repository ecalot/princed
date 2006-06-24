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
stringflag.c: Princed Resources : Flag string parsing feature routines
¯¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 22 Jun 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2006-Jun-22)

 Note:
  DO NOT remove this copyright notice
*/

/*#include "stringflag.h"*/

unsigned long parseflag(const char* stringflag);
const char* generateflag(unsigned long integerflag);

unsigned long parseflag(const char* stringflag) {

	unsigned long result=0;
	int val=0;

	if (!stringflag) return 0;

	while (*stringflag) {
		if (*stringflag==',' || *stringflag==' ') {
			while (*stringflag==' ') stringflag++;
			if (val) result|=1<<(val-1);
			val=0;
		} else if (*stringflag>='0' && *stringflag<='9') {
			val*=10;
			val+=*stringflag-'0';
		}
		stringflag++;
	}
	if (val) result|=1<<(val-1);
	return result;
}

/*#define DEBUG_STRINGS*/

#ifdef DEBUG_STRINGS
int main(int a,char** b) {
	printf("%x\n", parseflag(
		b[1]
	));
	return 0;
}
#endif

