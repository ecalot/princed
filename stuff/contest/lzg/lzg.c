/* 
 * LZG compressor/extractor
 * 
 * This sample code is to test the LZG algorithm
 *
 * Usage:
 *  ./lzg [-x] (input file) (output file)
 *
 * Use -x to uncompress.
 * 
 * ---------------------------------------------------------------------------- 
 * 
 * Authors: 
 *   Enrique Calot <ecalot.cod@princed.com.ar>
 *   Diego Essaya <dessaya@fi.uba.ar>
 * 
 * Research: Tammo Jan Dijkemma, Anke Balderer, Enrique Calot
 *
 * ----------------------------------------------------------------------------
 *
 * Copyright (C) 2004, 2005 the Princed Team
 * 
 * This file is part of the Princed project.
 * 
 * Princed is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Princed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <string.h>
#include "lzg_compress.h"
#include "lzg_uncompress.h"

typedef enum {COMPRESS, EXPAND, HELP, ERROR} action_t;

struct t_options {
	char *input_file;
	char *output_file;
};

/* A big number (the input/output must be less that that) */
#define LZG_MAX_MEMSIZE    65536

void print_syntax()
{
	fprintf(stderr, "Syntax: lzg [-x] input [output]\n");
}

action_t process_args(int argc, char *argv[], struct t_options *opts)
{
	action_t action = COMPRESS;
	int i;

	opts->input_file = opts->output_file = NULL;

	for(i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-x")) {
			action = EXPAND;
		}
		else { /* filename */
			if (!opts->input_file || !strcmp(opts->input_file, "-"))
				opts->input_file = argv[i];
			else if (!opts->output_file || !strcmp(opts->output_file, "-"))
				opts->output_file = argv[i];
			else 
			{
				fprintf(stderr, "Invalid argument: %s\n", argv[i]);
				return ERROR;
			}
		}
	}

	return action;
}

int main(int argc, char *argv[])
{
	action_t         action;
	struct t_options opts;
	unsigned char    input[LZG_MAX_MEMSIZE];
	unsigned char    output[LZG_MAX_MEMSIZE];
	int              inputSize, outputSize;
	FILE*            fp;
	int              result;

	action = process_args(argc, argv, &opts);
	if (action == ERROR)
	{
		print_syntax();
		return 1;
	}
	if (action == HELP)
	{
		print_syntax();
		return 0;
	}
	
	if (!opts.input_file)
	{
		fprintf(stderr, "warning: Reading from stdin.\n");
		fp = stdin;
	}
	else
	{
		fp = fopen(opts.input_file, "rb");
		if (!fp) {
			fprintf(stderr, "Error opening file '%s' for reading.\n", opts.input_file);
			return 1;
		}
	}

	inputSize = fread(input, 1, sizeof(input), fp);
	if (inputSize <= 0)
	{
		fprintf(stderr, "Error reading input file.\n");
		return 1;
	}

	fclose(fp);

	if (!opts.output_file)
	{
		fprintf(stderr, "warning: Writing to stdout.\n");
		fp = stdout;
	}
	else
	{
		fp = fopen(opts.output_file, "wb");
		if (!fp) {
			fprintf(stderr, "Error opening file '%s' for writing.\n", opts.output_file);
			return 1;
		}
	}

	if (action == EXPAND)
	{
		result=expandLzg(input, inputSize, output, &outputSize);
		if (result)
			fprintf(stderr,"Maskbyte not clean, possible data corruption\n");
	}
	else
	{
		compressLzg(input, inputSize, output, &outputSize);
	}
	
	if (fwrite(output, outputSize, 1, fp) <= 0)
	{
		fprintf(stderr, "Error writing output.\n");
		return 1;
	}

	fclose(fp);

	/* show results on screen */
	printf("Results:\n Input file: %s (%d)\n Output file: %s (%d)\n",
	       opts.input_file ? opts.input_file : "stdin", inputSize,
	       opts.output_file ? opts.output_file : "stdout", outputSize);

	return 0;
}

