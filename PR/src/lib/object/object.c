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
main.c: Princed Resources : Main item class implementation
¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 16 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "object.h"
#include "reslist.h"
#include "other.h"

tObject getObject(tResource* r, int* error) {
	tObject o;

	o.type=r->type;
	switch (o.type) {
	case eResTypeLevel:
		/*o.obj=objLevelCreate(r->data,r->size,r->number,vDatFileName,r->name,r->desc,r->datAuthor,error); */
		break;
	case eResTypeBinary: /* Binary files */
	case eResTypeText: /* Text files */
	case eResTypeRaw: /* Raw files */
		o.obj=objBinaryCreate(r->data,r->size,error); 
		break;
	case eResTypePop1Palette4bits: /* save and remember palette file */
		/*o.obj=objPalette_pop1_4bitsCreate(r->data,r->size,error); */
		break;
	case eResTypePcspeaker: /* save pcs file */
		/*o.obj=objPcspeakerCreate(r->data,r->size,error); */
		break;
	case eResTypeMidi:	/* save midi file */
		/*o.obj=objMidiCreate(r->data,r->size,error); */
		break;
	case eResTypeWave: /* save wav file */
		/*o.obj=objWaveCreate(r->data,r->size,error); */
		break;
	case eResTypeImage: /* save image */
		/*o.obj=objImageCreate(r->data,r->size,r->pal,error); */
		break;
	default:
		break;
	}
	
	return o;
}



