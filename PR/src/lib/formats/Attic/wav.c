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
wav.c: Princed Resources : WAV files support
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

//Includes
#include "wav.h"
#include "compile.h"
#include "disk.h"

int mFormatExtractWav(unsigned char* data, char *vFileext,unsigned long int size) {
	FILE*         target;
	int ok;
	unsigned char wav[]=WAVE_HEADER;

	size-=2;
	ok=writeOpen(vFileext,&target);

	wav[4]=(unsigned char)((size+36)&0xFF);
	wav[5]=(unsigned char)(((size+36)>>8)&0xFF);
	wav[6]=(unsigned char)(((size+36)>>16)&0xFF);
	wav[7]=(unsigned char)(((size+36)>>24)&0xFF);

	wav[40]=(unsigned char)((size)&0xFF);
	wav[41]=(unsigned char)(((size)>>8)&0xFF);
	wav[42]=(unsigned char)(((size)>>16)&0xFF);
	wav[43]=(unsigned char)(((size)>>24)&0xFF);

	ok=ok&&fwrite(wav,sizeof(wav),1,target);
	ok=ok&&fwrite(data+2,size-2,1,target);
	ok=ok&&(!fclose(target));

	return ok;
}

int mFormatCompileWav(unsigned char* data, FILE* fp, tResource *res) {
	unsigned char wav[]=WAVE_HEADER;
	int i=sizeof(wav);

	if ((*res).size<=i) return 0;
	res->size-=(--i);
	while ((i==4||i==5||i==6||i==7||i==40||i==41||i==42||i==43||(data[i]==wav[i]))&&(i--));
	data[sizeof(wav)-1]=1; //First character must be a 0x01 (wav type in dat)
	if (i==-1) mAddFileToDatFile(fp,data+sizeof(wav)-1,(*res).size);
	return 1;
}
