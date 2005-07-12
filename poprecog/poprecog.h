/*  Poprecog - Prince Of Persia Screenshots Recognizer
		Copyright (C) 2005 Princed Development Team

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
poprecog.c: Prince of Persia Screenshots Recognizer
¯¯¯¯¯¯¯¯¯¯

 Author: peter_k <peter@princed.com.ar>

*/

/* Defines */
#ifndef WIN32
#define UNIX
#endif

#include <sys/types.h>
#include <sys/stat.h>

#ifndef WIN32
	#define defmkdir(a) mkdir (a,(mode_t)0755)
	#define osIndepGetCharacter() getchar()
	#define SEPS "/"
	#define SEPC '/'
#else
	#include <direct.h> /* mkdir */ 
	#define defmkdir(a) mkdir (a)
	#define osIndepGetCharacter() getche()
	#define SEPS "\\"
	#define SEPC '\\'
#endif

/* Text Defines */
#define MAX_DIRS 9999
#define MAX_SCREENSHOTS 99999
#define MAX_IMAGES 999999
#define MAX_RECOGNIZED_ITEMS 999999
#define NUMBER_OF_CONTROL_PIXELS 5

/* about */
#define POPRECOG_URL "http://www.princed.com.ar"
#define POPRECOG_ABOUT "\
Prince of Persia Screenshots Recognizer\n\
(c) Copyright 2005 Princed Development Team\n\
Programmed by peter_k\n" POPRECOG_URL "\n\n"

/* steps */
#define POPRECOG_STEP1 "\
Step 1. Type dir where screenshots are stored.\nThis should be 320x200 bmp files with 256 colour palette.\n"

#define POPRECOG_STEP2 "\n\
Step 2. Type dirs where are stored bitmaps to recognize.\n\
Please type in this format: [dirname] [max images on screenshot][ENTER].\n\
When you'll finish type END[ENTER].\n"

#define POPRECOG_STEP3 "\n\
Step 3. Type number of maximum layers\n\
HINT:\n\
If you'll type 0, poprecog will automatically detect the number of layers,\n\
but it is not recommended for tiles, walls, cinematic etc.\n"

#define POPRECOG_STEP4 "\n\
Step 4. Type the number of bitmap debug method\n\
 0 - don't create bitmap debug files\n\
 1 - colour recognized things (expext not important) + additional info\n\
 2 - show recognized things (expect not important) + additional info\n\
 4 - pink EVERY recognized thing (320x200)\n\
 8 - only show recognized (expect not important; 320x200)\n\
15 - create every bitmap file from top list\n"

#define POPRECOG_STEP5 "\n\
Step 5. Type output dir, in which you'll see recognized things\n"

/* other texts */
#define POPRECOG_RECOGNIZING         "\nRecognizing %s (shot %d of %d)\n"
#define POPRECOG_RECOGNIZING2        "Recognizing %s\n"
#define POPRECOG_CHECKING_LAYER      "Checking layer nr. %d"
#define POPRECOG_DONE                "Done"
#define POPRECOG_FOUND               "Found %s d %c l %d x %d y %d w %d h %d fpx %d fpy %d\n"
#define POPRECOG_SUMMARY             "%d recognized images in about %d seconds\n"
#define POPRECOG_ERROR_CANNOT_OPEN   "ERROR: Cannot open %s\nProgram terminated\n"
#define POPRECOG_LOADING             "Loading bitmaps from dir %s\n"
#define POPRECOG_END_SUMMARY         "\nRecognized %d frames in about %ld seconds\n"	
#define POPRECOG_RELEASING_MEMORY    "Releasing memory\n"
#define POPRECOG_TYPE_STH            "Type something and press ENTER key...\n"
#define POPRECOG_DEBUG_HEADER        "%s (%d/%d)"
#define POPRECOG_DEBUG_RECOGNIZED_NR "Recognized number: %d"
#define POPRECOG_DEBUG_FIRST         "P(%d;%d) D%c L%d"
#define POPRECOG_DEBUG_SECOND        "D(%d;%d) Per:%d%%"

/* debug methods */
#define colour_flag 0x0001
#define show_flag   0x0002
#define trans_flag  0x0004
#define movie_flag  0x0008
#define hasFlag(a) (optDebugMethod&(a))
#define setFlag(a) optDebugMethod|=(a)

/* for match function */
#define equalsIgnoreCase(a,b) (((a)==(b))||((('A'<=((a)&0xdf)&&((a)&0xdf)<='Z'))&& !((a)^(b))&0xdf))
