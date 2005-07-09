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
dfsdfs.c: Princed Resources : dfsgsdgsdfgdfgs
��������
 Copyright 2005 Princed Development Team
  Created: 4 Jul 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2005-Jul-04)

 Note:
  DO NOT remove this copyright notice
*/


static int i;
static char buffer[200];

int emit(char c) {
	if (i==200) return 0;
	buffer[i++]=c;
	return 1;
}

#define readk(k) (k=*(format++))
#define isNumber(k) ('0'<=k && k<='9')

const char* parseformat(const char* format,long value,const char* index, const char* type, const char* extension, long numberOfThisType, int order, const char* desc) {
	/* This function parses format in this way:
	 * %v - the value
	 * %i - the index name
	 * %t - the item type (image, wave, etc)
	 * %e - the extension (bmp, wav, etc)
	 * %n - the number of the item typed in %t
	 * %o - the order number (in case the index and values are the same)
	 * %d - the description
	 * %% - a % sign
	 * 
	 * modifiers:
	 * %5i  - the index truncated to 5 bytes or the index size, whatever comes first
	 * %05i - the same, but in case the index is sized less than 5 it's filled with spaces
	 * %5v  - the value tabulated to 5 digits (filled with spaces), in case the number is bigger, more digits will be used
	 * %05v - the same, but in case the value is sized less than 5 it's filled with zeros
	 *
	 * Examples:
	 * "%i%05v.%e" for shap25001.bmo
	 * "%t%02n.%e" for image01.bmp
	 * 
	 * Limitations:
	 * The generated string size has a limit, in case it is reached, a NULL pointer will be returned
	 */

	char k;
	int zeroflag;
	int size;
				
	i=0; /* initialize buffer */

	while (readk(k)) { /* for each byte in format as k */
		if (k=='%') {
			zeroflag=0;
			size=0;
			
			readk(k); /* read next char */
			if (!k) return NULL; /* just in case the string is finished in the wrong place */
			
			if (k=='0') { /* it's %0... */
				zeroflag=1;
				readk(k); /* read next char */
				if (!k) return NULL; /* just in case the string is finished in the wrong place */
			}
			while (isNumber(k)) { /* it's %0[0-9]... */
				size=size*10+k;
				readk(k); /* read next char */
				if (!k) return NULL; /* just in case the string is finished in the wrong place */
			}
			switch (k) {
			case 'v': /* value */
				emitNumber(value,size,zeroflag);
				break;
			case 'i': /* index name */
				emitString(index,size,zeroflag);
				break;
			case 't': /* item type (image, wave, etc) */
				emitString(type,size,zeroflag);
				break;
			case 'e': /* extension (bmp, wav, etc) */
				emitString(extension,size,zeroflag);
				break;
			case 'n': /* number of the item typed in %t */
				emitNumber(numberOfThisType,size,zeroflag);
				break;
			case 'o': /* order number (in case the index and values are the same) */
				emitNumber(order,size,zeroflag);
				break;
			case 'd': /* description */
				emitString(desc,size,zeroflag);
				break;
			case '%': /* the % symbol */
				emit(k); /* emit it */
				break;
			default:
				return NULL;
			}
		} else { /* normal characters are emitted */
			emit(k);
		}
	}
	
	return emit(0)?buffer:NULL; /* close the string and return it (return NULL in case of error)*/
}
