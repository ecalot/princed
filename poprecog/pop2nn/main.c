/*
**  PRINCE OF PERSIA 2 : No Name
**  New, tiny Prince Of Persia 2 engine.
**  (c) Copyright 2005 Princed Development Team
**  Programmed by peter_k
*/

/*
  Changelog:
  - 2005.06.22 about 22:00 - I've start coding this ;)
*/

/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <dirent.h>
#include <allegro.h>

#include "keyboard.h"

int main(int argc, char *argv[])
{
    int result;
  
    allegro_init();
    install_timer();
    install_keyboard();    
    set_color_depth(16);
    set_color_conversion(COLORCONV_TOTAL);

   	result = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 320, 200, 0, 0);
   	if (result != 0)
    {
    		  allegro_message(allegro_error);
    		  exit(-1);
   	}
   	
   	for (;;)
   	{
        refreshKeys();

        if (key[KEY_ESC]) break;
        textprintf(screen, font, 50, 50, makecol16(255, 255, 255), "RIGHT ARROW: %d    %d    ", key[KEY_RIGHT], keyStatus[KEY_RIGHT]);
        
        rest(200);  
    }
    return 0;
}
END_OF_MAIN();  
