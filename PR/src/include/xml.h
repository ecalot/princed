
#ifndef _XML_H_
#define _XML_H_

int equalsIgnoreCase(const char s1[],const char s2[]);

typedef struct tTag {
 struct tTag* child;
 struct tTag* next;
 char* tag;
 char* desc;
 char* external;
 char* file;
 char* itemtype;
 char* name;
 char* palette;
 char* type;
 char* value;
 char* version;
 char* number;
}tTag;

//Constants
#define MAX_VALUE_SIZE 1000
#define MAX_SIZE_OF_FILE 30000

//tTag* getTagStructure(); //initializes
void freeTagStructure(tTag* structure);
//int parseNext(char** pString, tTag* tag);
tTag* parseXmlFile(const char* vFile,int* error);
#endif
