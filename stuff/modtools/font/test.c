
#include <SDL/SDL.h>
#include "../include/output.h"

/* Test outputDrawText
 *
 * Compililar con : 
 * gcc -o test_text output.c test.c `sdl-config --libs --cflags` -I../include
 */
int main (int argc, char *argv[])
{
	outputInit ();
	outputDrawText (10, 0, "Texto \"Hola\" tiene %d pixels de largo", outputGetTextWidth("Hola"));
	
	outputDrawText (55, 20, "ABCD 123456 &/$#    000", 26);
	
	outputDrawMessage ("hola, que %s.", "tal");

	outputUpdateScreen();

	SDL_Delay (5000);

	outputStop();
	return 0;
}

