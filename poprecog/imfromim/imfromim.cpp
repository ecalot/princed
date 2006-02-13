#include<allegro.h>
#include<stdio.h>
#include<vector.h>

#define IMFROMIM_VISUAL_MODE

int match(char *pat, char *str)


class imFromIm {
public:
	int setImageSize(int newWidth, int newHeight);

	void loadDir(char dirName[500], int openSubDirs);
	int width;
	int height;
	vector<BITMAP*> images;
};

int imFromIm::setImageSize(int newWidth, int newHeight) {
	if ((newWidth>0)&&(newHeight>0)) {
		width = newWidth;
		height = newHeight;
		return 1;
	} else
		return 0;
}

void imFromIm::loadDir(char dirName[500], int openSubDirs);

int main() {
	imFromIm imFromIm;
	
	#ifdef IMFROMIM_VISUAL_MODE
		allegro_init();
		set_color_depth(desktop_color_depth()?desktop_color_depth():32);
		if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1280, 800, 0, 0)) {
			allegro_message(allegro_error);
			exit(-1);
		}
		install_timer();
		install_keyboard();
		install_mouse();
	#endif
	
//	printf("%d\n", imFromIm.images.size());
//	system("pause");

	#ifdef IMFROMIM_VISUAL_MODE
		textprintf_ex(screen, font, 0, 0, makecol(255,0,0), makecol(0,0,0), "1/5 Loading bitmaps... %5d now...", 123);
		while (!key[KEY_ESC]);
		clear_keybuf();
	#endif
	
	return 0;
}
END_OF_MAIN()
