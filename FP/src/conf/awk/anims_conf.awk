#   Princed V3 - Prince of Persia Level Editor for PC Version
#   Copyright (C) 2003 Princed Development Team
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#   The authors of this program may be contacted at http://forum.princed.com.ar

# anims_conf.awk: FreePrince : walls.conf parser
# ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
# Copyright 2005 Princed Development Team
#  Created: 5 Jan 2005
#
#  Author: Enrique Calot <ecalot.cod@princed.com.ar>
#
# Note:
#  DO NOT remove this copyright notice
#

BEGIN {
}

###########################
# PART 1                  #
# Parse input into memory #
###########################

#comments
/^[[:space:]]*#/ {
	halt
}

#animation cathegories
/^[[:space:]]*ANIMATION[[:space:]]/ {
	$2=toupper($2)
	animcount++
	animation["name" animcount]=$2
	animation["size" animcount]=$3
	animation["startf" animcount]=totalfixedimg
	animation["startt" animcount]=totalstate
	animation["starto" animcount]=totalsound
	halt
}

#tables
#fixed images
/^[[:space:]]*[[:digit:]]+[[:space:]]+FIXEDIMG[[:space:]]/ {
	totalfixedimg++
	f["frame" totalfixedimg]=$1/1
	f["res" totalfixedimg]=toupper($3)
	if (toupper($4)=="INFINITE") $4=0
	f["duration" totalfixedimg]=$4/1
	f["layer" totalfixedimg]=toupper($5)
	if (tolower($6)=="right") $6=320 #here will be the screen width size
	if (tolower($6)=="left") $6=0
	f["x" totalfixedimg]=$6/1
	if (tolower($7)=="bottom") $7=200 #here will be the screen height size
	if (tolower($7)=="top") $7=0
	f["y" totalfixedimg]=$7/1
	animation["sizef" animcount]++
	halt
}

#states
/^[[:space:]]*[[:digit:]]+[[:space:]]+STATE[[:space:]]/ {
	totalstate++
	$6=tolower($6)
	$7=tolower($7)
	
	t["frame" totalstate]=$1/1
	t["res" totalstate]=toupper($3)
	t["state" totalstate]=toupper($4)
	t["location" totalstate]=$5/1
	if ($6=="up") $6=1
	if ($6=="middle") $6=2
	if ($6=="center") $6=2
	if ($6=="centre") $6=2
	if ($6=="down") $6=3
	t["floor" totalstate]=$6/1
	if ($7=="no") $7=0
	if ($7=="yes") $7=1
	if ($7=="mirror") $7=1
	t["cacheMirror" totalstate]=$7/1
	animation["sizet" animcount]++
	halt
}

#sounds
/^[[:space:]]*[[:digit:]]+[[:space:]]+(MIDI|WAV|SPEAKER)[[:space:]]/ {
	totalsound++
	o["frame" totalsound]=$1/1
	o["res" totalsound]=toupper($3)
	o["type" totalsound]=tolower($2)
	animation["sizeo" animcount]++
	halt
}

##########################
# PART 2                 #
# Generate output        #
##########################

END {
	#avoid empty records
	if (!totalfixedimg) {
		printf("Semantic error in line at anims.conf: At least one FIXEDIMG is needed.\n")>"/dev/stderr"
		exit 30
	}
	if (!totalstate) {
		printf("Semantic error in anims.conf: At least one STATE is needed.\n")>"/dev/stderr"
		exit 31
	}
	if (!totalsound) {
		printf("Semantic error in anims.conf: At least one SOUND is needed.\n")>"/dev/stderr"
		exit 32
	}

	#static defines
	printf("#define ANIMS_LAYERTYPE_TOP     0\n")
	printf("#define ANIMS_LAYERTYPE_BOTTOM  1\n")
	printf("\n")
	
	#output fixedimg table
	coma=""
	printf("#define ANIMS_FIXEDIMG {")
	for (i=1;i<=totalfixedimg;i++) {
		printf("%s\\\n\t{/*frame*/ (unsigned short)%d,/*res*/ (unsigned long)RES_%s, /*duration*/ %d, /*layer*/ (unsigned char)ANIMS_LAYERTYPE_%s, /*x,y*/ (unsigned short)%d,(unsigned short)%d}",coma,f["frame" i],f["res" i],f["duration" i],f["layer" i],f["x" i],f["y" i])
		coma=","
	}
	printf("\\\n}\n\n")

	#output state mark table
	coma=""
	printf("#define ANIMS_STATE {")
	for (i=1;i<=totalstate;i++) {
		printf("%s\\\n\t{/*frame*/ %d, /*res*/ (unsigned long)RES_%s, /*state*/ STATE_MARKS_%s, /*loc*/ %d, /*floor*/%d, /*mirror*/ %d}",coma,t["frame" i],t["res" i],t["state" i],t["location" i],t["floor" i],t["cacheMirror" i])
		coma=","
	}
	printf("\\\n}\n\n")
	
	#output sound table
	coma=""
	printf("#define ANIMS_SOUND {")
	for (i=1;i<=totalsound;i++) {
		printf("%s\\\n\t{/*frame*/ %d,/*res*/ (unsigned long)RES_%s, /*type*/ anims_enum_%s}",coma,o["frame" i],o["res" i],o["type" i])
		coma=","
	}
	printf("\\\n}\n\n")
	
	#the index table
	coma=""
	printf("#define ANIMS_TABLE {")
	for (i=1;i<=animcount;i++) {
		printf("%s\\\n\t{/*fixedimg*/ %d,%d, /*state*/ %d,%d, /*sound*/ %d,%d, /*animsize*/ %d}",coma,animation["startf" i],animation["sizef" i],animation["startt" i],animation["sizet" i],animation["starto" i],animation["sizeo" i],animation["size" i])
		coma=","
	}
	printf("\\\n}\n\n")

	#finally write the animation ids
	for (i=1;i<=animcount;i++) {
		printf("#define ANIMS_ID_%s %d\n",animation["name" i],i-1)
	}
}

