
#include "conf_ids.h"

void configSetShort(int id,short int value);
void configSetLong(int id, long int value);
void configSetString(int id, char* value);

short int configGetShort(int id);
long int configGetLong(int id);
char* configGetString(int id);

void configSync();
