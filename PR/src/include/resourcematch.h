
#include "reslist.h" /* tResourceId*/

typedef struct resourceMatch {
	int flag;
	int value;
	int order;
	char* index;
	char* path;
} tResourceMatch;

void freeRM(tResourceMatch *r);
int initRM(const char* text, tResourceMatch *r);
int runRM(const tResourceMatch *r, const char* path, const tResourceId *id);
