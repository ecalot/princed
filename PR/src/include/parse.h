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
xmlparse.h: Princed Resources : xml handling functions headers
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 23 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-25)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _XMLPARSE_H_
#define _XMLPARSE_H_

int equalsIgnoreCase(const char s1[],const char s2[]);

typedef struct tTag {
	struct tTag* child;
	struct tTag* next;
	char* tag;
	char* desc;
	char* path;
	char* file;
	char* itemtype;
	char* name;
	char* palette;
	char* type;
	char* value;
	char* version;
	char* number;
}tTag;

/* Constants */
#define MAX_VALUE_SIZE 3000

/* Prototypes */
void  freeTagStructure (tTag* structure);
tTag* parseXmlFile     (const char* vFile,int* error);

#endif
