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
resources.h: Free Prince : Output Devices Handler
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: 23 Mar 2004

  Author: vhgh <fhdht.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _OUTPUT_H_
#define _OUTPUT_H_

/* Text Primitives*/
drawText(const char* text);
drawMessage(const char* text);

/* Sound */
playWav(tWave sound);
playMid(tMidi music); /* Starts the music reproduction and returns */

/* Graph */
drawBitmap(tImage image);
clearScreen();

/* Initialization */
startGraphicMode();
stopGraphicMode();

#endif
