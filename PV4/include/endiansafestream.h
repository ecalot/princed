#ifndef _ENDIANSAFESTREAM_H_
#define _ENDIANSAFESTREAM_H_

#include <iostream>
#include <fstream>

using namespace std;

class iesstream: public ifstream  {
public:
	iesstream(const char* file);
	void read(unsigned char &c);
	void read(char &c);
	void read(long &l);
	void read(unsigned long &l);
	void read(short &s);
	void read(unsigned short &s);
	void read(unsigned char* c,int size);
	void read(char* c,int size);
};


#endif
