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
mid.c: Princed Resources : MIDI files support
�����
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

//Includes
#include "mid.h"
#include "extract.h"
#include "compile.h"
#include "memory.h"
#include <string.h>

char mFormatExtractMid(unsigned char* data, char *vFileext,unsigned long int size) {
	//Mid files are saved as raw except you must ignore checksum & sound type
	return writeData(data,2,vFileext,size);
}

char mFormatCompileMid(unsigned char* data, FILE* fp, tResource *res) {
	unsigned char* file;

	file=getMemory((*res).size);
	file[0]=(*res).type-2; //Now should be 0x02 //First character must be a 0x01 (wav type in dat)
	memcpy(file+1,data,(*res).size);
	(*res).size++;
	mAddFileToDatFile(fp,file,(*res).size);
	free(file);
	return 1;
}
