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
main.c: FreePrince : Main function - parsing
������
 Copyright 2004, 2003 Princed Development Team
  Created: 24 Mar 2004

  Author: Endfhgfhgfhg <efghgfdht.cod@princed.com.ar>

 Note:
  DO NOT remove this copyright notice
*/

#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>
#include "kernel.h"
#include "main.h"
#include <stdio.h>

/***************************************************************\
|      Standard executable command line parsing function        |
\***************************************************************/

int main (int argc, char **argv) {
	/* declare variables */
	int c;
	int optionflag=0;
	int junk = 0;

	int level=-1;

	/* Parse command line options */
	do {
		static struct option long_options[] = PARSING_OPTIONS;

		c = getopt_long(argc,argv,PARSING_CHARS,long_options,&junk);
		switch (c) {
				case 'm':
					setFlag(menu_flag);
					break;
				case 'l':
					level=0;
					if (optarg) level=atoi(optarg);
					break;
				case 't':
					setFlag(megahit_flag);
				case -1:
					break;
				case 1:
					setFlag(version_flag);
					break;
				default:
					setFlag(help_flag);
					break;
		}
	} while (c!=-1);


	/* Check syntax, help and version screens */
	
	if (hasFlag(help_flag)) {
		fprintf(stderr,"This is a help screen\n");
		exit(1);
	}

	if (hasFlag(version_flag)) {
		fprintf(stderr,"This is a version screen\n");
		exit(1);
	}

	/* Start kernel program */
	return kernel(optionflag,level);
}
