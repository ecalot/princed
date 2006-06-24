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
object.c: Princed Resources : Main item class implementation
¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 16 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#include "common.h"
#include "object.h"
#include "types.h"

#include "image.h"
#include "other.h"
#include "palette.h"
#include "sound.h"

/* Object polimorphism hook layer */

tObject objectCreate(tResource* r, int* error) {
	tObject o;
	if (!r) {
		*error=PR_RESULT_SUCCESS;
		o.type=eResTypeNone;
		o.obj=NULL;
		return o;
	}

	o.type=r->type;
	switch (o.type) {
	case eResTypeLevelPop1:
		o.obj=objectLevelPop1Create(r->content,r->number,r->datfile,r->name,r->desc,r->datAuthor,error);
		break;
	case eResTypeLevelPop2:
		o.obj=objectLevelPop2Create(r->content,r->number,r->datfile,r->name,r->desc,r->datAuthor,error);
		break;
	case eResTypeOtherBinary: /* Binary files */
	case eResTypeOtherText: /* Text files */
	case eResTypeOtherRaw: /* Raw files */
		o.obj=objectBinaryCreate(r->content,error);
		break;
	case eResTypePalettePop1_16: /* save and remember palette file */
		o.obj=objectPalettePop1_16Create(r->content,error);
		break;
	case eResTypePalettePop2_NColors: /* save and remember palette file */
		o.obj=objectPalettePop2_NColorsCreate(r->content,error);
		break;
	case eResTypeSoundPcspeaker: /* save pcs file */
		o.obj=objectSoundPcspeakerCreate(r->content,error);
		break;
	case eResTypeSoundMidi:	/* save midi file */
		o.obj=objectSoundMidiCreate(r->content,error);
		break;
	case eResTypeSoundWave: /* save wav file */
		o.obj=objectSoundWaveCreate(r->content,error);
		break;
	case eResTypeImage2: /* save image */
		o.obj=objectImage2Create(r->content,error);
		break;
	case eResTypeImage16: /* save image */
		o.obj=objectImage16Create(r->content,error);
		break;
	case eResTypeImage256: /* save image */
		o.obj=objectImage256Create(r->content,error);
		break;
	default:
printf("Exception: Unhooked type %d\n",o.type);
		*error=PR_RESULT_SUCCESS; /* NOTE: change to 1 to detect unhooked types */
		o.obj=NULL;
		break;
	}

	return o;
}

int objectWrite(tObject o, const char* file, int optionflag, const char* backupExtension) {
	int error;
	switch (o.type) {
	case eResTypeLevelPop1:
		error=objectLevelPop1Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeLevelPop2:
		error=objectLevelPop2Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeOtherBinary: /* Binary files */
	case eResTypeOtherText: /* Text files */
	case eResTypeOtherRaw: /* Raw/autodetect files */
		error=objectBinaryWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypePalettePop1_16: /* save and remember palette file */
		error=objectPalettePop1_16Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypePalettePop2_NColors: /* save and remember palette file */
		error=objectPalettePop2_NColorsWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeSoundPcspeaker: /* save pcs file */
		error=objectSoundPcspeakerWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeSoundMidi:	/* save midi file */
		error=objMidiWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeSoundWave: /* save wav file */
		error=objWaveWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeImage2: /* save image */
		error=objectImage2Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeImage16: /* save image */
		error=objectImage16Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeImage256: /* save image */
		error=objectImage256Write(o.obj,file,optionflag,backupExtension);
		break;
	default:
printf("Warning: Couldn't write unhooked type %d\n",o.type);
		break;
	}

	return error;
}

/* Palette class methods */

int objectPaletteGetBits(tObject pal) {
	switch (pal.type) {
	case eResTypePalettePop2_NColors:
		return 8;
	case eResTypePalettePop1_16:
		return 4;
	case eResTypePalettePop1_Mono:
		return 1;
	default:
		return 0;
	}
}

int objectGetColors(tObject object) {
	switch (object.type) {
	case eResTypePalettePop2_NColors:
		return PAL_COLORS_eResTypePalettePop2_NColors;
	case eResTypePalettePop1_16:
		return PAL_COLORS_eResTypePalettePop1_16;
	case eResTypePalettePop1_Mono:
		return  PAL_COLORS_eResTypePalettePop1_Mono;
	case eResTypeNone:
		return 256;
	case eResTypeImage16:
	case eResTypeImage2:
	case eResTypeImage256:
		return objectImageGetColorCount(object.obj);
	default:
		return 0;
	}
}

tColor* objectGetColorsArray(tObject pal) {
	switch (pal.type) {
	case eResTypePalettePop1_16: /* save and remember palette file */
		return objectPalettePop1_16GetColors(pal.obj);
	case eResTypePalettePop2_NColors:
		return objectPalettePop2_NColorsGetColors(pal.obj);
	default:
		return NULL;
	}
}

/***************************************************************\
|                    Dat compiling primitive                    |
\***************************************************************/

void objectSet(tObject o,int *result,tResource* res) {
	switch (o.type) {
		case eResTypeLevelPop1:
			/*result=objLevelSet(o.obj,res); TODO */
			break;
		case eResTypeImage16:
			*result=objectImage16Set(o.obj,res);
			break;
		case eResTypeSoundWave:
			*result=objWaveSet(o.obj,res);
			break;
		case eResTypeSoundMidi:
			*result=objMidiSet(o.obj,res);
			break;
		case eResTypeSoundPcspeaker:
			*result=objPcspeakerSet(o.obj,res);
			break;
		case eResTypePalettePop1_16:
			*result=objectPalettePop1_16Set(o.obj,res);
			break;
		case eResTypeOtherBinary:
		default:
			*result=objBinarySet(o.obj,res);
			break;
	}

}

/* Format detection function (private function, not in header file) */
tObject objectRead(const char* file,tResource* res,int *result) {
	/* return true if ok, false if error */
	tObject o;
	switch (res->type) {
		case eResTypeLevelPop1:
			/*o.obj=objLevelRead(file,res.content,result); TODO */
			break;
		case eResTypeImage16:
			o.obj=objectImage16Read(file,*res->palette,result);
			break;
		case eResTypeSoundWave:
			o.obj=objWaveRead(file,result);
			break;
		case eResTypeSoundMidi:
			o.obj=objMidiRead(file,result);
			break;
		case eResTypeSoundPcspeaker:
			o.obj=objPcspeakerRead(file,result);
			break;
		case eResTypePalettePop1_16:
			o.obj=objectPalettePop1_16Read(file,result);
			break;
		case eResTypeOtherBinary:
		default:
			o.obj=objBinaryRead(file,result);
			break;
	}
	o.type=res->type;

	return o;
}

