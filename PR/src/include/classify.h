
#ifndef _TASKS_H_
#define _TASKS_H_

/*
	Return values may be:
	 -2 Memory error
	 -1 File not found or no access error

	 00 Not a valid POP1 DAT file

	 01 Levels file
	 02 Graphic file with an image in the first valid entry (not common)
	 03 Waves/Digital sound file
	 04 Midis file
	 05 Valid DAT file with Undefined content
	 06 Graphic file with a palette in the first valid entry (common)
	 07 PC Speaker dat file

	 11 Pop2 dat files
*/

int prVerifyDatType(char* vFiledat);

#endif
