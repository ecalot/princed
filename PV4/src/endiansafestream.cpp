#include "endiansafestream.h"

#include <iostream>
#include <fstream>

using namespace std;

iesstream::iesstream(const char* file) {
   this->open(file,ios::in|ios::binary);
   if (!this->is_open()) throw -2;
}

void iesstream::read(unsigned char& c) {
         this->read((char*)&c,1);
}

void iesstream::read(char &c){
	this->read(&c,1);
         }

void iesstream::read(long &l){
         this->read((char*)&l,4);
         }

void iesstream::read(unsigned long &l){
	this->read((char*)&l,4);
	}

 void iesstream::read(short &s){
	 this->read((char*)&s,2);
	 }

  void iesstream::read(unsigned short &s){
		this->read((char*)&s,2);
		}

  void iesstream::read(unsigned char* c,int size){
		this->read((char*)c,size);
		}

  void iesstream::read(char* c,int size){
		this->read(c,(streamsize)size);
		}



