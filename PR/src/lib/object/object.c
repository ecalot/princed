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

#include "common.h"
#include "object.h"
#include "reslist.h"

#include "image.h"
#include "other.h"
#include "palette.h"
#include "sound.h"

/* Object polimorphism support layer */

tObject getObject(tResource* r, int* error) {
	tObject o;
	if (!r) {
		*error=PR_RESULT_SUCCESS;
		o.type=eResTypeNone;
		o.obj=NULL;
		return o;
	}

	o.type=r->type;
	switch (o.type) {
	case eResTypeLevel:
		/*o.obj=objLevelCreate(r->content,r->number,vDatFileName,r->name,r->desc,r->datAuthor,error); */
		break;
	case eResTypeBinary: /* Binary files */
	case eResTypeText: /* Text files */
	case eResTypeRaw: /* Raw files */
		o.obj=objBinaryCreate(r->content,error); 
		break;
	case eResTypePop1Palette4bits: /* save and remember palette file */
		o.obj=objPalette_pop1_4bitsCreate(r->content,error);
		break;
	case eResTypePcspeaker: /* save pcs file */
		o.obj=objPcspeakerCreate(r->content,error);
		break;
	case eResTypeMidi:	/* save midi file */
		o.obj=objMidiCreate(r->content,error);
		break;
	case eResTypeWave: /* save wav file */
		o.obj=objWaveCreate(r->content,error);
		break;
	case eResTypeImage: /* save image */
		o.obj=objImageCreate(r->content,r->palette,error);
		break;
	default:
		break;
	}
	
	return o;
}

int writeObject(tObject o, const char* file, int optionflag, const char* backupExtension) {
	int error;
	switch (o.type) {
	case eResTypeLevel:
		break;
	case eResTypeBinary: /* Binary files */
	case eResTypeText: /* Text files */
	case eResTypeRaw: /* Raw files */
		break;
	case eResTypePop1Palette4bits: /* save and remember palette file */
		error=objPalette_pop1_4bitsWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypePcspeaker: /* save pcs file */
		error=objPcspeakerWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeMidi:	/* save midi file */
		error=objMidiWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeWave: /* save wav file */
		error=objWaveWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeImage: /* save image */
		error=objImageWrite(o.obj,file,optionflag,backupExtension);
		break;
	default:
		break;
	}

	return error;
}	


int paletteGetBits(tObject pal) { return 4; }
int paletteGetColors(tObject pal) { return 16; }
tColor* paletteGetColorArray(tObject pal) {
	switch (pal.type) {
	case eResTypePop1Palette4bits: /* save and remember palette file */
		return objPalette_pop1_4bitsGetColors(pal.obj);
		break;
	default:
		return NULL;
	}
}

/***************************************************************\
|                    Dat compiling primitive                    |
\***************************************************************/

void setObject(tObject o,int *result) {
}

/* Format detection function (private function, not in header file) */
tObject readObject(const char* file,tResource* res,int *result) {
	/* return true if ok, false if error */
	tObject o;
	switch (res->type) {
		case eResTypeLevel:
			/*o.obj=objLevelRead(file,res.content,result);*/
			break;
		case eResTypeImage:
			/*o.obj=objImageRead(file,res.content,res.palette,result);*/
			break;
		case eResTypeWave:
			o.obj=objWaveRead(file,result);
			break;
		case eResTypeMidi:
			/*o.obj=objMidiRead(file,res.content,result);*/
			break;
		case eResTypePcspeaker:
			/*o.obj=objPcspeakerRead(file,res.content,result);*/
			break;
		case eResTypePop1Palette4bits:
			/*o.obj=objPop1Palette4bitsRead(file,res.content,result);*/
			break;
		case eResTypeBinary:
		default:
			o.obj=objBinaryRead(file,result);
			break;
	}
	o.type=res->type;

	return o;
}

