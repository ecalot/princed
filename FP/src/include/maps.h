


int levelUse(void* level);
/* Sets the level for future use using the resource given
 * Initializes the position in the beginning of the level
 *
 * Return 0 on success
 */

int levelGetScreenWalls(unsigned char* data,unsigned char* borders);
int levelGetScreenItems(unsigned char* data,unsigned char* borders);

int levelMoveScreen(tDirection direction);

int levelGetGuards(/* TODO: add modifiers */);
int levelGetDoorMap(/* TODO: idem */);


