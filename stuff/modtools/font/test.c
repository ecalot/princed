
#include <SDL/SDL.h>
#include "../include/output.h"

/* Test outputDrawText
 *
 * Compililar con : 
 * gcc -o test_text output.c test.c `sdl-config --libs --cflags` -I../include
 */
int main (int argc, char *argv[])
{
	initText ();

	outputInit ();
	outputDrawText (10, 0, "Feliz Cumple %d, Gazer", 26);
	
	outputDrawText (55, 20, "ABCD 123456 &/$#    000", 26);

	outputUpdateScreen();

	SDL_Delay (5000);

	outputStop();
	return 0;
}

